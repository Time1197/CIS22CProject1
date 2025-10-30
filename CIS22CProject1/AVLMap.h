#pragma once

#include <stdexcept>
#include <iostream>
#include <algorithm> // For std::max

template <typename K, typename V>
class AVLMap {
private:
    struct Node {
        K key;
        V value;
        Node* left;
        Node* right;
        int height; // <-- ADDED: Height of this node

        Node(const K& k, const V& v) {
            key = k;
            value = v;
            left = nullptr;
            right = nullptr;
            height = 1; // New node is initially at height 1
        }
    };

    Node* root;
    int count;

    // --- AVL-specific Helper Functions ---

    /**
     * Gets the height of a node (handles nullptr).
     */
    int getHeight(Node* node) {
        if (node == nullptr) {
            return 0;
        }
        return node->height;
    }

    /**
     * Updates the height of a node based on its children's heights.
     */
    void updateHeight(Node* node) {
        if (node != nullptr) {
            node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));
        }
    }

    /**
     * Gets the balance factor of a node (left height - right height).
     */
    int getBalance(Node* node) {
        if (node == nullptr) {
            return 0;
        }
        return getHeight(node->left) - getHeight(node->right);
    }

    /**
     * Performs a right rotation on the subtree rooted with y.
     * y                           x
     * / \                         / \
     * x   T3  (Right Rotate) -->  T1  y
     * / \                             / \
     * T1  T2                          T2  T3
     */
    Node* rightRotate(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;

        // Perform rotation
        x->right = y;
        y->left = T2;

        // Update heights (order matters: update children first)
        updateHeight(y);
        updateHeight(x);

        // Return new root
        return x;
    }

    /**
     * Performs a left rotation on the subtree rooted with x.
     * x                              y
     * / \                            / \
     * T1  y   (Left Rotate) -->      x   T3
     * / \                        / \
     * T2  T3                     T1  T2
     */
    Node* leftRotate(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;

        // Perform rotation
        y->left = x;
        x->right = T2;

        // Update heights (order matters: update children first)
        updateHeight(x);
        updateHeight(y);

        // Return new root
        return y;
    }


    // --- Core Recursive Functions (Modified from BST) ---

    /**
     * Recursively inserts a key-value pair and rebalances the tree.
     * Returns the new root of the (sub)tree.
     */
    Node* insertRecursive(Node* node, const K& key, const V& value) {
        // 1. Standard BST Insert
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
            // Key already exists, update value and return
            node->value = value;
            return node;
        }

        // 2. Update height of this ancestor node
        updateHeight(node);

        // 3. Get the balance factor to check if this node became unbalanced
        int balance = getBalance(node);

        // 4. If unbalanced, perform rotations (4 cases)

        // Case 1: Left Left (LL)
        // Imbalance is on the left (balance > 1), and new key was
        // inserted into the left child's left subtree.
        if (balance > 1 && key < node->left->key) {
            return rightRotate(node);
        }

        // Case 2: Right Right (RR)
        // Imbalance is on the right (balance < -1), and new key was
        // inserted into the right child's right subtree.
        if (balance < -1 && key > node->right->key) {
            return leftRotate(node);
        }

        // Case 3: Left Right (LR)
        // Imbalance is on the left (balance > 1), but new key was
        // inserted into the left child's right subtree.
        if (balance > 1 && key > node->left->key) {
            node->left = leftRotate(node->left); // First, rotate left on child
            return rightRotate(node);            // Then, rotate right on parent
        }

        // Case 4: Right Left (RL)
        // Imbalance is on the right (balance < -1), but new key was
        // inserted into the right child's left subtree.
        if (balance < -1 && key < node->right->key) {
            node->right = rightRotate(node->right); // First, rotate right on child
            return leftRotate(node);             // Then, rotate left on parent
        }

        // Return the (possibly new) root of the subtree
        return node;
    }

    /**
     * Recursively searches for a key (identical to BSTMap).
     */
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

    /**
     * Recursively deletes all nodes (identical to BSTMap).
     */
    void clear(Node* node) {
        if (node != nullptr) {
            clear(node->left);
            clear(node->right);
            delete node;
        }
    }

    /**
     * Recursively copies the tree (identical to BSTMap, except for height).
     */
    Node* copyRecursive(Node* node) {
        if (node == nullptr) {
            return nullptr;
        }
        // Copy this node
        Node* newNode = new Node(node->key, node->value);
        newNode->height = node->height; // Copy height
        // Copy subtrees
        newNode->left = copyRecursive(node->left);
        newNode->right = copyRecursive(node->right);
        return newNode;
    }

    /**
     * Recursively prints the tree in-order (identical to BSTMap).
     */
    void printInOrder(Node* node) const {
        if (node == nullptr) return;
        printInOrder(node->left);
        std::cout << " (" << node->key << ": " << node->value << ", h=" << node->height << ")\n";
        printInOrder(node->right);
    }

public:
    // --- Constructor ---
    AVLMap() {
        root = nullptr;
        count = 0;
    }

    // --- Destructor ---
    ~AVLMap() {
        clear(root);
    }

    // --- Copy Constructor ---
    AVLMap(const AVLMap<K, V>& other) {
        root = nullptr;
        count = other.count;
        root = copyRecursive(other.root);
    }

    // --- Copy Assignment Operator ---
    AVLMap<K, V>& operator=(const AVLMap<K, V>& other) {
        if (this == &other) {
            return *this;
        }
        clear(root);
        root = copyRecursive(other.root);
        count = other.count;
        return *this;
    }

    // --- Public Interface ---

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

    // --- Debugging ---
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