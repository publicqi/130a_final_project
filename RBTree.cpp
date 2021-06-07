#include "RBTree.h"
#include <iostream>

using namespace std;

RBTree::Node::Node(string value){
    this->data = value;
}

void RBTree::insertValue(string value){

    Node* n = new Node(value);
    n->index = maxIndex;
    this->maxIndex++;
    root = insertBST(root, n);
    fixViolation(root, n);
}

RBTree::Node* RBTree::insertBST(Node* root, Node* ptr){
    if(root == nullptr){
        return ptr;
    }

    if (ptr->data < root->data) {
        root->left = insertBST(root->left, ptr);
        root->left->parent = root;
    } else if (ptr->data > root->data) {
        root->right = insertBST(root->right, ptr);
        root->right->parent = root;
    }
    else{
        int errno;
        errno = 1;
        perror("Replicated name");
        abort();
    }

    return root;
}

void RBTree::fixViolation(Node *&root, Node *&ptr){
    Node *parent_ptr = NULL;
    Node *grand_parent_ptr = NULL;
  
    while ((ptr != root) && (ptr->color != BLACK) &&
           (ptr->parent->color == RED))
    {
  
        parent_ptr = ptr->parent;
        grand_parent_ptr = ptr->parent->parent;
  
        /*  Case : A
            Parent of ptr is left child 
            of Grand-parent of ptr */
        if (parent_ptr == grand_parent_ptr->left)
        {
  
            Node *uncle_ptr = grand_parent_ptr->right;
  
            /* Case : 1
               The uncle of ptr is also red
               Only Recoloring required */
            if (uncle_ptr != NULL && uncle_ptr->color == 
                                                   RED)
            {
                grand_parent_ptr->color = RED;
                parent_ptr->color = BLACK;
                uncle_ptr->color = BLACK;
                ptr = grand_parent_ptr;
            }
  
            else
            {
                /* Case : 2
                   ptr is right child of its parent
                   Left-rotation required */
                if (ptr == parent_ptr->right)
                {
                    rotateLeft(root, parent_ptr);
                    ptr = parent_ptr;
                    parent_ptr = ptr->parent;
                }
  
                /* Case : 3
                   ptr is left child of its parent
                   Right-rotation required */
                rotateRight(root, grand_parent_ptr);
                swap(parent_ptr->color, 
                           grand_parent_ptr->color);
                ptr = parent_ptr;
            }
        }
  
        /* Case : B
           Parent of ptr is right child 
           of Grand-parent of ptr */
        else
        {
            Node *uncle_ptr = grand_parent_ptr->left;
  
            /*  Case : 1
                The uncle of ptr is also red
                Only Recoloring required */
            if ((uncle_ptr != NULL) && (uncle_ptr->color == 
                                                    RED))
            {
                grand_parent_ptr->color = RED;
                parent_ptr->color = BLACK;
                uncle_ptr->color = BLACK;
                ptr = grand_parent_ptr;
            }
            else
            {
                /* Case : 2
                   ptr is left child of its parent
                   Right-rotation required */
                if (ptr == parent_ptr->left)
                {
                    rotateRight(root, parent_ptr);
                    ptr = parent_ptr;
                    parent_ptr = ptr->parent;
                }
  
                /* Case : 3
                   ptr is right child of its parent
                   Left-rotation required */
                rotateLeft(root, grand_parent_ptr);
                swap(parent_ptr->color, 
                         grand_parent_ptr->color);
                ptr = parent_ptr;
            }
        }
    }
  
    root->color = BLACK;
}

void RBTree::rotateLeft(Node *&root, Node *&ptr)
{
    Node *ptr_right = ptr->right;
  
    ptr->right = ptr_right->left;
  
    if (ptr->right != NULL)
        ptr->right->parent = ptr;
  
    ptr_right->parent = ptr->parent;
  
    if (ptr->parent == NULL)
        root = ptr_right;
  
    else if (ptr == ptr->parent->left)
        ptr->parent->left = ptr_right;
  
    else
        ptr->parent->right = ptr_right;
  
    ptr_right->left = ptr;
    ptr->parent = ptr_right;
}

void RBTree::rotateRight(Node *&root, Node *&ptr)
{
    Node *ptr_left = ptr->left;
  
    ptr->left = ptr_left->right;
  
    if (ptr->left != NULL)
        ptr->left->parent = ptr;
  
    ptr_left->parent = ptr->parent;
  
    if (ptr->parent == NULL)
        root = ptr_left;
  
    else if (ptr == ptr->parent->left)
        ptr->parent->left = ptr_left;
  
    else
        ptr->parent->right = ptr_left;
  
    ptr_left->right = ptr;
    ptr->parent = ptr_left;
}

RBTree::Node* RBTree::searchValue(string value){
    Node* curr = root;
    while(curr){
        if(value == curr->data){
            return curr;
        }
        else if (value < curr->data){
            curr = curr->left;
        }
        else{
            curr = curr->right;
        }
    }
    return curr;
}

RBTree::~RBTree(){
    deleteTree(root);
}

void RBTree::deleteTree(Node* ptr){
    if(!ptr){
        return;
    }

    if(ptr->left){
        deleteTree(ptr->left);
    }
    if(ptr->right){
        deleteTree(ptr->right);
    }

    delete ptr;
}


vector<RBTree::Node*>* RBTree::rangeSearch(string lower, string upper){
    // Yes, name1 < name2.
    vector<Node*>* result = new vector<Node*>();
    Node* lower_found = searchValue(lower);
    Node* parent;
    if(!lower_found){
        cout << "Lower not found!\n";
        return result;
    }
    Node* curr = lower_found;

    while(curr && curr->data <= upper){
        result->push_back(curr);
        curr = inOrderSuccessor(curr);
    }
    return result;
}

RBTree::Node* RBTree::inOrderSuccessor(RBTree::Node* ptr){
    if (ptr->right != nullptr)
        return minValue(ptr->right);
 
    Node* p = ptr->parent;
    while (p != nullptr && ptr == p->right) {
        ptr = p;
        p = p->parent;
    }
    return p;
}

RBTree::Node* RBTree::minValue(RBTree::Node* ptr){
    Node* curr = ptr;
    while (curr->left != NULL) {
        curr = curr->left;
    }
    return curr;
}
