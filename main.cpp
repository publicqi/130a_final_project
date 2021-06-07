#include "Graph.h"
#include "RBTree.h"
#include "Record.h"
#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sstream>

#include <cassert>
#include <cstring>

/* Some creativity:
   
   * Find path using Dijkstra
   * Analyze k-top occupations
   * Age distribution graph
*/

using namespace std;

int menu(){
    puts("1. Point Query");
    puts("2. Range Query");
    puts("3. Insert User");
    puts("4. Friend Query");
    puts("5. Add Relation");
    puts("6. Exit");

    int choice = -1;
    cout << "Input your choice: ";
    scanf("%d", &choice);
    getchar();
    return choice;
}

void initializeData(const char* inputFilename, RBTree* rbt, const char* outputFilename, Graph* graph){
    int readFd = open(inputFilename, 0);
    bool readingName = false;
    bool readingAge = false;
    bool readingOccupation = false;
    bool readingFriends = false;
    bool isFirstLine = true;
    if(readFd == -1){
        int errno;
        errno = 2;
        perror("Cannot open input file");
        exit(-1);
    }
    char buffer[32] = {0};
    int i = 0;

    // Write file
    Record r = {0};

    int writeFd = open(outputFilename, O_WRONLY | O_CREAT | O_TRUNC);
    if(writeFd == -1){
        int errno;
        errno = 2;
        perror("Cannot open output file");
        exit(-1);
    }

    while(read(readFd, (buffer + i), 1)){
        buffer[++i] = 0;

        if(isFirstLine){  // CSV first line: field names
            if(buffer[i - 1] == '\n'){  // Finish first line
                isFirstLine = false;
                readingName = true;
                i = 0;
            }
        }
        else{  // Parse normal lines
            if(buffer[i - 1] == ','){  // When we met a comma
                buffer[i - 1] = 0;  // replace comma with \0
                if(readingName){
                    string name(buffer);
                    i = 0;
                    readingName = false;
                    readingAge = true;

                    rbt->insertValue(name);
                    strncpy(r.name, buffer, 20);

                    graph->addUser(name);
                }
                else if(readingAge){
                    int age = atoi(buffer);
                    // cout << "Age: " << age << endl;
                    i = 0;
                    readingAge = false;
                    readingOccupation = true;

                    strncpy(r.age, buffer, 3);
                }
                else if(readingOccupation){
                    string occupation(buffer);
                    // cout << "Occupation: " << occupation << endl;
                    i = 0;

                    readingOccupation = false;
                    readingFriends = true;

                    strncpy(r.occupation, buffer, 30);
                    write(writeFd, &r, sizeof(r));
                    memset(&r, 0, sizeof(Record));

                    read(readFd, buffer, 1); // eliminate the quote
                }
                else if(readingFriends){
                    string friends(buffer);
                    // cout << friends << ", ";
                    graph->addRelation(rbt->maxIndex - 1, friends);
                    i = 0;
                }
            }
            else if(buffer[i - 1] == '"'){
                read(readFd, buffer, 1);  // eliminate quote
                i = 0;
                readingName = true;
                readingFriends = false;
            }
        }
    }

    close(writeFd);
    close(readFd);
}

int main(){
    RBTree* userRBTree = new RBTree();
    Graph* friendsAdjList = new Graph();

    string inputFile, outputFile;
    cout << "Enter input filename: ";
    cin >> inputFile;
    cout << "Enter record filename: ";
    cin >> outputFile;
    initializeData(inputFile.c_str(), userRBTree, outputFile.c_str(), friendsAdjList);
    FILE* ProfileFP = fopen(outputFile.c_str(), "a+");

    if(!ProfileFP){
        int errno;
        errno = 2;
        perror("Cannot open profile file");
        exit(-1);
    }

    char buffer[30] = {0};

    while(true){
        switch(menu()){
            case 1:{
                string queryName;
                cout << "Enter the name you want to query: ";
                getline(cin, queryName);

                RBTree::Node* queryNameResult = userRBTree->searchValue(queryName);
                if(queryNameResult){
                    fseek(ProfileFP, sizeof(Record) * queryNameResult->index, 0);
                    fread(buffer, 20, 1, ProfileFP); // name
                    cout << "Name: " << buffer << " ";
                    fread(buffer, 3, 1, ProfileFP); // age
                    cout << "Age: " << buffer << " ";
                    fread(buffer, 30, 1, ProfileFP); // occupation
                    cout << "Occupation: " << buffer << "\n";
                }else{
                    cout << "Person not found!\n";
                }
                break;
            }
            case 2:{
                string queryName1;
                string queryName2;
                cout << "Enter the first name: ";
                getline(cin, queryName1);
                cout << "Enter the second name: ";
                getline(cin, queryName2);

                vector<RBTree::Node*>* rangeQueryResult = userRBTree->rangeSearch(queryName1, queryName2);

                for(int i = 0; i < rangeQueryResult->size(); i ++){
                    fseek(ProfileFP, sizeof(Record) * (*rangeQueryResult)[i]->index, 0);
                    fread(buffer, 20, 1, ProfileFP); // name
                    cout << "Name: " << buffer << " ";
                    fread(buffer, 3, 1, ProfileFP); // age
                    cout << "Age: " << buffer << " ";
                    fread(buffer, 30, 1, ProfileFP); // age
                    cout << "Occupation: " << buffer << "\n";
                }
                break;
            }
            case 3:{
                string name;
                string age;
                string occupation;
                string friends;
                Record r = {0};
                cout << "Enter the name: ";
                getline(cin, name);
                userRBTree->insertValue(name);
                friendsAdjList->addUser(name);
                cout << "Enter the age: ";
                getline(cin, age);
                cout << "Enter the occupation: ";
                getline(cin, occupation);

                fseek(ProfileFP, 0, SEEK_END);
                strncpy(r.name, name.c_str(), 20);
                strncpy(r.age, age.c_str(), 3);
                strncpy(r.occupation, occupation.c_str(), 30);
                fwrite(&r, sizeof(Record), 1, ProfileFP);

                cout << "Enter the friends, seperated by comma: ";
                getline(cin, friends);

                stringstream ss(friends);

                while(ss.good())
                {
                    string singleFriend;
                    getline(ss, singleFriend, ',' );
                    friendsAdjList->addRelation(userRBTree->maxIndex - 1, singleFriend);
                }
                break;
            }
            case 4:{
                string queryName;
                cout << "Enter the name you want to query: ";
                getline(cin, queryName);

                RBTree::Node* queryNameRelationResult = userRBTree->searchValue(queryName);
                if(queryNameRelationResult){

                    vector<string>* friendsResult = friendsAdjList->getRelationByIndex(queryNameRelationResult->index);

                    for(int i = 0; i < friendsResult->size(); i++){
                        RBTree::Node* queryNameResult = userRBTree->searchValue((*friendsResult)[i]);
                        if(queryNameResult){
                            fseek(ProfileFP, sizeof(Record) * queryNameResult->index, 0);
                            fread(buffer, 20, 1, ProfileFP); // name
                            cout << "Name: " << buffer << " ";
                            fread(buffer, 3, 1, ProfileFP); // age
                            cout << "Age: " << buffer << " ";
                            fread(buffer, 30, 1, ProfileFP); // occupation
                            cout << "Occupation: " << buffer << "\n";
                        }
                    }
                }else{
                    cout << "Person not found!\n";
                }
                break;
            }
            case 5:{
                string friendName1, friendName2;
                int index1, index2;
                cout << "Enter the first name: ";
                getline(cin, friendName1);
                cout << "Enter the second name: ";
                getline(cin, friendName2);

                index1 = friendsAdjList->getIndex(userRBTree, friendName1);
                index2 = friendsAdjList->getIndex(userRBTree, friendName2);

                if(index1 == -1 || index2 == -1){
                    cout << "User not found!\n";
                    break;
                }
                friendsAdjList->addRelation(index1, friendName2);
                friendsAdjList->addRelation(index2, friendName1);
                break;
            }
            case 6:{
                puts("Have a good day!");
                delete userRBTree;
                delete friendsAdjList;
                return 0;
            }
            default:{
                cout << "Wrong choice!\n";
                break;
            }
        }
    }
}
