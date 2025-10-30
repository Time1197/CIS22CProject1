#pragma once

#include <stdexcept>
#include <iostream>

// Use an enum for color; it's clearer than a bool
enum Color { RED, BLACK };

template <typename K, typename V>
class RBMap {
private:
    struct Node {
        K key;
        V value;
        Node* left, * right, * parent;
        Color color;

        Node(const K& k, const V& v) : key(k), value(v), left(nullptr), right(nullptr), parent(nullptr), color(RED) {}
    };

    Node* root;
    Node* NIL; // A single "sentinel" node to represent all leaves
    int count;

    // --- Core Helper Functions ---

    /**
     * Performs a left rotation on the subtree rooted at x.
     * This operation must correctly update parent pointers.
     */
    void leftRotate(Node* x) {
        Node* y = x->right; // Set y
        x->right = y->left; // Turn y's left subtree into x's right
        if (y->left != NIL) {
            y->left->parent = x;
        }
        y->parent = x->parent; // Link y's parent to x's parent
        if (x->parent == NIL) {
            this->root = y;
        }
        else if (x == x->parent->left) {
            x->parent->left = y;
        }
        else {
            x->parent->right = y;
        }
        y->left = x; // Put x on y's left
        x->parent = y;
    }

    /**
     * Performs a right rotation on the subtree rooted at y.
     * This operation must correctly update parent pointers.
     */
    void rightRotate(Node* y) {
        Node* x = y->left; // Set x
        y->left = x->right; // Turn x's right subtree into y's left
        if (x->right != NIL) {
            x->right->parent = y;
        }
        x->parent = y->parent; // Link x's parent to y's parent
        if (y->parent == NIL) {
            this->root = x;
        }
        else if (y == y->parent->left) {
            y->parent->left = x;
        }
        else {
            y->parent->right = x;
        }
        x->right = y; // Put y on x's right
        y->parent = x;
    }

    /**
     * "Fixes" the Red-Black Tree properties after a standard BST insert.
     * This is the core balancing logic.
     */
    void fixInsert(Node* z) {
        Node* y; // Uncle node
        // We only need to fix things if the parent is RED (violates Rule 4)
        while (z->parent->color == RED) {
            // Case A: Parent is a LEFT child
            if (z->parent == z->parent->parent->left) {
                y = z->parent->parent->right; // y is the Uncle

                // Case 1: Uncle is RED
                // Recolor parent, uncle, and grandparent. Move z up to grandparent.
                if (y->color == RED) {
                    // 
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                }
                // Case 2 & 3: Uncle is BLACK
                else {
                    // Case 2: Triangle (z is a RIGHT child)
                    // Rotate parent left to turn this into Case 3 (a line).
                    if (z == z->parent->right) {
                        z = z->parent;
                        leftRotate(z);
                    }
                    // Case 3: Line (z is a LEFT child)
                    // Recolor parent/grandparent, rotate grandparent right.
                    // 
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    rightRotate(z->parent->parent);
                }
            }
            // Case B: Parent is a RIGHT child (symmetric to Case A)
            else {
                y = z->parent->parent->left; // y is the Uncle

                // Case 1: Uncle is RED
                if (y->color == RED) {
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                }
                // Case 2 & 3: Uncle is BLACK
                else {
                    // Case 2: Triangle (z is a LEFT child)
                    if (z == z->parent->left) {
                        z = z->parent;
                        rightRotate(z);
                    }
                    // Case 3: Line (z is a RIGHT child)
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    leftRotate(z->parent->parent);
                }
            }
            // If z is the root, the loop condition (z->parent->color == RED) will
            // be false (since root->parent is NIL which is BLACK).
        }
        // Ensure root is always Black (Rule 2)
        root->color = BLACK;
    }

    // --- Search, Clear, and Print Helpers ---

    /**
     * Recursively searches for a key (same as BST/AVL).
     */
    bool searchRecursive(Node* node, const K& key, V& outValue) const {
        if (node == NIL) {
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
     * Recursively deletes all nodes (post-order).
     */
    void clear(Node* node) {
        if (node != NIL) {
            clear(node->left);
            clear(node->right);
            delete node;
        }
    }

    /**
     * Recursively prints the tree (in-order).
     */
    void printInOrder(Node* node) const {
        if (node == NIL) {
            return;
        }
        printInOrder(node->left);
        std::cout << " (" << node->key << ": " << node->value
            << ", " << (node->color == RED ? "R" : "B")
            // --- FIX: Removed std::to_string ---
            << ", p=" << (node->parent == NIL ? "NIL" : node->parent->key) << ")\n";
        printInOrder(node->right);
    }

    // Note: A copy constructor and assignment operator for an R-B tree
    // are very complex (due to parent pointers and the NIL sentinel)
    // and are omitted here as they are likely beyond the project scope.

public:
    // --- Constructor ---
    RBMap() {
        // Create the sentinel node
        NIL = new Node(K(), V()); // Use default K, V
        NIL->color = BLACK;
        NIL->left = nullptr;   // NIL's children are not used
        NIL->right = nullptr;  //
        NIL->parent = NIL;     // NIL's parent is itself
        root = NIL;            // Root starts as NIL
        count = 0;
    }

    // --- Destructor ---
    ~RBMap() {
        clear(this->root);
        delete NIL; // Clean up the sentinel node
    }

    // --- Public Interface ---

    /**
     * Inserts a key-value pair into the tree and rebalances.
     */
    void insert(const K& key, const V& value) {
        // --- 1. Standard BST Insert (Iterative) ---
        Node* z = new Node(key, value); // New node is always RED
        z->left = NIL;
        z->right = NIL;

        Node* y = NIL;     // y will be the parent
        Node* x = this->root; // x is the "current" node for traversal

        // Find the correct spot to insert
        while (x != NIL) {
            y = x; // y "lags" behind x
            if (z->key < x->key) {
                x = x->left;
            }
            else if (z->key > x->key) {
                x = x->right;
            }
            else {
                // Key already exists: update value and exit
                x->value = value;
                delete z; // Don't need the new node
                return;
            }
        }

        // We found the spot. 'y' is the parent for our new node 'z'.
        z->parent = y;
        if (y == NIL) {
            this->root = z; // Tree was empty
        }
        else if (z->key < y->key) {
            y->left = z;
        }
        else {
            y->right = z;
        }

        count++;

        // --- 2. Fixup ---
        // Call the fixup routine to restore R-B properties
        fixInsert(z);
    }

    /**
     * Searches for a key and returns its value via out-parameter.
     */
    bool search(const K& key, V& outValue) const {
        return searchRecursive(root, key, outValue);
    }

    int size() const {
        return count;
    }

    bool isEmpty() const {
        return root == NIL;
    }

    // --- Debugging ---
    void print() const {
        std::cout << "RBMap (" << size() << " items):\n";
        if (isEmpty()) {
            std::cout << " (empty)\n";
        }
        else {
            printInOrder(root);
        }
    }

private:
    // Disable Copy Constructor and Assignment Operator
    // (They are very complex to implement correctly for R-B trees)
    RBMap(const RBMap& other) = delete;
    RBMap& operator=(const RBMap& other) = delete;
};