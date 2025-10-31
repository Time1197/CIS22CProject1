#pragma once

#include <stdexcept>
#include <iostream>
#include <algorithm> //for max

template <typename K, typename V>
class AVLMap {
private:
    struct Node {
        K key;
        V value;
        Node* left;
        Node* right;
        int height;

        Node(const K& k, const V& v) {
            key = k;
            value = v;
            left = nullptr;
            right = nullptr;
            height = 1; //new node is initially at height 1
        }
    };

    Node* root;
    int count;

    //AVL helper funcitons

    //gets height of a node (handles nullptr).
    int getHeight(Node* node) {
        if (node == nullptr) {
            return 0;
        }
        return node->height;
    }

    
    //updates height of a node based on children's heights.
    void updateHeight(Node* node) {
        if (node != nullptr) {
            node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));
        }
    }

    
     //gets the balance factor of a node (left height - right height).
    int getBalance(Node* node) {
        if (node == nullptr) {
            return 0;
        }
        return getHeight(node->left) - getHeight(node->right);
    }

    //performs a right rotation on the subtree rooted with y.
    Node* rightRotate(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;

        //perform rotation
        x->right = y;
        y->left = T2;

        //update heights (order matters: update children first)
        updateHeight(y);
        updateHeight(x);

        //return new root
        return x;
    }

    //performs a left rotation on the subtree rooted with x.
    Node* leftRotate(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;

        //perform rotation
        y->left = x;
        x->right = T2;

        //update heights (order matters: update children first)
        updateHeight(x);
        updateHeight(y);

        //return new root
        return y;
    }


    //core recursive functions (modified from BST)

    
    //recursively inserts a key-value pair and rebalances the tree.
    //returns the new root of the (sub)tree.
    Node* insertRecursive(Node* node, const K& key, const V& value) {
        // 1. standard BST insert
        if (node == nullptr) {
            count++;
            return new Node(key, value);
        }

        if (key < node->key) {
            node->left = insertRecursive(node->left, key, value);
        }
        else if (key > node->key) {
            node->right = insertRecursive(node->right, key, value);
        }
        else {
            //key already exists, update value and return
            node->value = value;
            return node;
        }

        // 2. update height of this ancestor node
        updateHeight(node);

        // 3. get the balance factor to check if this node became unbalanced
        int balance = getBalance(node);

        // 4. if unbalanced, perform rotations (4 cases)

        //case 1: left left
        //imbalance is on the left (balance > 1), and new key was
        //inserted into the left child's left subtree.
        if (balance > 1 && key < node->left->key) {
            return rightRotate(node);
        }

        //case 2: right right
        //imbalance is on the right (balance < -1), and new key was
        //inserted into the right child's right subtree.
        if (balance < -1 && key > node->right->key) {
            return leftRotate(node);
        }

        //case 3: left right
        //imbalance is on the left (balance > 1), but new key was
        //inserted into the left child's right subtree.
        if (balance > 1 && key > node->left->key) {
            node->left = leftRotate(node->left); //first, rotate left on child
            return rightRotate(node);            //then, rotate right on parent
        }

        //case 4: right left
        //imbalance is on the right (balance < -1), but new key was
        //inserted into the right child's left subtree.
        if (balance < -1 && key < node->right->key) {
            node->right = rightRotate(node->right); // First, rotate right on child
            return leftRotate(node);             // Then, rotate left on parent
        }

        //return the (possibly new) root of the subtree
        return node;
    }

    
    //recursively searches for a key (identical to BSTMap).
    bool searchRecursive(Node* node, const K& key, V& outValue) const {
        if (node == nullptr) {
            return false;
        }
        if (key == node->key) {
            outValue = node->value;
            return true;
        }
        if (key < node->key) {
            return searchRecursive(node->left, key, outValue);
        }
        else {
            return searchRecursive(node->right, key, outValue);
        }
    }

    
     //recursively deletes all nodes (identical to BSTMap).
    void clear(Node* node) {
        if (node != nullptr) {
            clear(node->left);
            clear(node->right);
            delete node;
        }
    }

     //recursively copies the tree (identical to BSTMap, except for height).
    Node* copyRecursive(Node* node) {
        if (node == nullptr) {
            return nullptr;
        }
        //copy this node
        Node* newNode = new Node(node->key, node->value);
        newNode->height = node->height; //copy height
        //copy subtrees
        newNode->left = copyRecursive(node->left);
        newNode->right = copyRecursive(node->right);
        return newNode;
    }

    //recursively prints the tree in-order (identical to BSTMap).
    void printInOrder(Node* node) const {
        if (node == nullptr) return;
        printInOrder(node->left);
        std::cout << " (" << node->key << ": " << node->value << ", h=" << node->height << ")\n";
        printInOrder(node->right);
    }

public:
    //constructor
    AVLMap() {
        root = nullptr;
        count = 0;
    }

    //destructor
    ~AVLMap() {
        clear(root);
    }

    //copy constrcutor
    AVLMap(const AVLMap<K, V>& other) {
        root = nullptr;
        count = other.count;
        root = copyRecursive(other.root);
    }

    //copy assignment operator
    AVLMap<K, V>& operator=(const AVLMap<K, V>& other) {
        if (this == &other) {
            return *this;
        }
        clear(root);
        root = copyRecursive(other.root);
        count = other.count;
        return *this;
    }

    //public interface

    void insert(const K& key, const V& value) {
        root = insertRecursive(root, key, value);
    }

    bool search(const K& key, V& outValue) const {
        return searchRecursive(root, key, outValue);
    }

    int size() const {
        return count;
    }

    bool isEmpty() const {
        return root == nullptr;
    }

    //print (for testing)
    void print() const {
        std::cout << "AVLMap (" << size() << " items):\n";
        if (isEmpty()) {
            std::cout << " (empty)\n";
        }
        else {
            printInOrder(root);
        }
    }
};