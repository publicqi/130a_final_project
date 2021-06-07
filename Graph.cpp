#include "Graph.h"
#include <iostream>

using namespace std;
Graph::Graph(){
    nameList = new vector<pair<string, vector<string>*>*>();
}

void Graph::addUser(string name){
    nameList->push_back(new pair<string, vector<string>*>(name, new vector<string>()));
}

void Graph::addRelation(int index, string name){
    nameList->at(index)->second->push_back(name);
}

int Graph::getIndex(RBTree* rbt, string name){
    if(rbt->searchValue(name))
        return rbt->searchValue(name)->index;

    return -1;
}


vector<string>* Graph::getRelationByIndex(int index){
    return nameList->at(index)->second;
}

vector<string>* Graph::getRelation(RBTree* rbt, string name){
    int index = getIndex(rbt, name);
    if(index == -1){
        cerr << "Name not found!";
        return nullptr;
    }
    return getRelationByIndex(index);
}

Graph::~Graph(){
    for(int i = 0; i < nameList->size(); i++){
        delete nameList->at(i)->second;
        delete nameList->at(i);
    }

    delete nameList;
}
