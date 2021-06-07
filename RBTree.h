#include <string>
#include <vector>
#define RED 0
#define BLACK 1

using namespace std;

#ifndef RBTREE_H
#define RBTREE_H

class RBTree{
public:
    class Node{
    public:
        Node(string data);
        string data;
        Node *left = nullptr;
        Node *right = nullptr;
        Node *parent = nullptr;
        int color = RED;
        int index = -1;
    };
public:
    Node* root = nullptr;
    int maxIndex = 0;
private:
    void rotateLeft(Node *&, Node *&);
    void rotateRight(Node *&, Node *&);
    void fixViolation(Node *&, Node *&);
    Node* insertBST(Node* root, Node* ptr);
    void deleteTree(Node* ptr);
    Node* inOrderSuccessor(Node* ptr);
    Node* minValue(Node* ptr);

public:
    ~RBTree();
    void insertValue(string value);
    Node* searchValue(string value);
    vector<Node*>* rangeSearch(string lower, string upper);
};

#endif
