#include <string>
#include <vector>
#include "RBTree.h"

using namespace std;
class Graph{
public:
    Graph();
    ~Graph();

    void addUser(string name);
    void addRelation(int index, string name);

    vector<string>* getRelation(RBTree* rbt, string name);
    vector<string>* getRelationByIndex(int index);

    int getIndex(RBTree* rbt, string name);

private:
    vector<pair<string, vector<string>*>*>* nameList;
};