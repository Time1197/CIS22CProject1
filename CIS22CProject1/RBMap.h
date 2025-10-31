#pragma once

#include <stdexcept>
#include <iostream>
#include <string> // for print() debugging

template <typename K, typename V>
class RBMap {
private:
    // private enums and stacks
    enum Color { RED, BLACK };

    struct Node {
        K key;
        V value;
        Node* left;
        Node* right;
        Node* parent;
        Color color;

        Node(const K& k, const V& v) : key(k), value(v), left(nullptr), right(nullptr), parent(nullptr), color(RED) {}
    };

    // private member variables
    Node* root;
    Node* NIL; //single "sentinel" node to represent all leaves
    int count;

public:
    // public constructor, destructor

    //constructor
    RBMap() {
        //create sentinel node
        NIL = new Node(K(), V()); //use default constructor for K and V
        NIL->color = BLACK;
        NIL->left = NIL;
        NIL->right = NIL;
        NIL->parent = NIL;
        root = NIL;
        count = 0;
    }


     //destructor
    ~RBMap() {
        clearRecursive(root); //private helper call
        delete NIL; //delete the sentinel
    }

    //copy constructor
    RBMap(const RBMap<K, V>& other) {
        // 1.set new NIL node
        NIL = new Node(K(), V());
        NIL->color = BLACK;
        NIL->left = NIL;
        NIL->right = NIL;
        NIL->parent = NIL;

        // 2. call recursive copy function (private helper)
        root = copyRecursive(other.root, NIL, other.NIL); //pass NIL parent of root

        // 3. copy size
        count = other.count;
    }

    
    //copy assignment operator
    RBMap<K, V>& operator=(const RBMap<K, V>& other) {
        if (this == &other) {
            return *this; //handle self-assignment
        }

        // 1. clear this map's old data
        clearRecursive(root);
        //keep our existing NIL node

        // 2. call recursive copy function (private helper)
        root = copyRecursive(other.root, NIL, other.NIL); // Pass NIL as parent of root

        // 3. copy size
        count = other.count;

        return *this;
    }


    //public interface


     //inserts a key-value pair. Fixes tree if not balanced
    void insert(const K& key, const V& value) {
        Node* z = new Node(key, value);
        z->left = NIL;
        z->right = NIL;
        z->parent = NIL;

        Node* y = NIL;
        Node* x = this->root;

        //standard BST insert
        while (x != NIL) {
            y = x;
            if (z->key < x->key) {
                x = x->left;
            }
            else if (z->key > x->key) {
                x = x->right;
            }
            else {
                //z->key == x->key
                //key already exists, update value and clean up
                x->value = value;
                delete z; //don't need the new node
                return; //exit function, no rebalance needed
            }
        }

        //link new node z into tree
        z->parent = y;
        if (y == NIL) {
            this->root = z; //tree was empty
        }
        else if (z->key < y->key) {
            y->left = z;
        }
        else {
            y->right = z;
        }

        count++; //increment count
        fixInsert(z); //calls private helper
    }

    
    //searches for a key and returns its value via out-parameter.
    bool search(const K& key, V& outValue) const {
        Node* node = root;
        while (node != NIL) {
            if (key == node->key) {
                outValue = node->value;
                return true;
            }
            if (key < node->key) {
                node = node->left;
            }
            else {
                node = node->right;
            }
        }
        return false;
    }

    //returns the number of items in the map.
    int size() const {
        return count;
    }

    //checks if map is empty.
    bool isEmpty() const {
        return root == NIL;
    }

     //prints tree contents for debugging.
    void print() const {
        std::cout << "RBMap (" << size() << " items):\n";
        if (isEmpty()) {
            std::cout << " (empty)\n";
        }
        else {
            printInOrder(root); //calls private helper
        }
    }

private:
    //private helper functions
    //(all definitions are placed *after* the public functions
    //that call them, so no forward declarations are needed.)

    //performs a left rotation on node x.
    void leftRotate(Node* x) {
        Node* y = x->right;
        x->right = y->left;
        if (y->left != NIL) {
            y->left->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == NIL) {
            this->root = y;
        }
        else if (x == x->parent->left) {
            x->parent->left = y;
        }
        else {
            x->parent->right = y;
        }
        y->left = x;
        x->parent = y;
    }


    //performs a right rotation on node y.
    void rightRotate(Node* y) {
        Node* x = y->left;
        y->left = x->right;
        if (x->right != NIL) {
            x->right->parent = y;
        }
        x->parent = y->parent;
        if (y->parent == NIL) {
            this->root = x;
        }
        else if (y == y->parent->right) {
            y->parent->right = x;
        }
        else {
            y->parent->left = x;
        }
        x->right = y;
        y->parent = x;
    }

    //restores Red-Black properties after insertion.
    void fixInsert(Node* z) {
        while (z->parent->color == RED) {
            if (z->parent == z->parent->parent->left) {
                Node* y = z->parent->parent->right; //uncle
                if (y->color == RED) {
                    //case 1: uncle is RED
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                }
                else {
                    if (z == z->parent->right) {
                        //case 2: uncle is BLACK, z is right child (Left-Right)
                        z = z->parent;
                        leftRotate(z);
                    }
                    //case 3: uncle is BLACK, z is left child (Left-Left)
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    rightRotate(z->parent->parent);
                }
            }
            else {
                //symmetric to above, with "right" and "left" exchanged
                Node* y = z->parent->parent->left; //uncle
                if (y->color == RED) {
                    //case 4: uncle is RED
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                }
                else {
                    if (z == z->parent->left) {
                        //case 5: uncle is BLACK, z is left child (Right-Left)
                        z = z->parent;
                        rightRotate(z);
                    }
                    //case 6: uncle is BLACK, z is right child (Right-Right)
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    leftRotate(z->parent->parent);
                }
            }
        }
        root->color = BLACK;
    }

    
    //recursively deletes all nodes in a post-order traversal.
    void clearRecursive(Node* node) {
        if (node != NIL) {
            clearRecursive(node->left);
            clearRecursive(node->right);
            delete node;
        }
    }

    //recursively copies tree.
    Node* copyRecursive(Node* nodeToCopy, Node* newParent, Node* otherNIL) {
        if (nodeToCopy == otherNIL) {
            return NIL; //base case: hit a leaf
        }

        //1. create new node and copy data
        Node* newNode = new Node(nodeToCopy->key, nodeToCopy->value);
        newNode->color = nodeToCopy->color;
        newNode->parent = newParent; //link to new parent

        //2. recursively copy children, passing *this new node* as their parent
        newNode->left = copyRecursive(nodeToCopy->left, newNode, otherNIL);
        newNode->right = copyRecursive(nodeToCopy->right, newNode, otherNIL);

        return newNode;
    }

    //recursively prints the tree in-order for debugging
    void printInOrder(Node* node) const {
        if (node == NIL) return;
        printInOrder(node->left);
        std::cout << " (" << node->key << ": " << node->value
            << ", c=" << (node->color == RED ? "R" : "B")
            //use key (a string) directly, not to_string()
            << ", p=" << (node->parent == NIL ? "NIL" : node->parent->key)
            << ")\n";
        printInOrder(node->right);
    }
};

