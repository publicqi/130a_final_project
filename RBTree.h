#include <string>
#include <vector>
#define RED 0
#define BLACK 1

using namespace std;

class RBTree{
private:
    class Node{
    public:
        Node(string data);
        string data;
        Node *left = nullptr;
        Node *right = nullptr;
        Node *parent = nullptr;
        int color = RED;
    };

    Node* root = nullptr;

    void rotateLeft(Node *&, Node *&);
    void rotateRight(Node *&, Node *&);
    void fixViolation(Node *&, Node *&);
    Node* insertBST(Node* root, Node* ptr);

public:
    ~RBTree();
    void insertValue(string value);
    Node* searchValue(string value);
    vector<Node*> rangeSearch(string value1, string value2);
};
