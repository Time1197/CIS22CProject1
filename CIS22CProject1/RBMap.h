#pragma once

#include <stdexcept>
#include <iostream>
#include <string> // For print() debugging

template <typename K, typename V>
class RBMap {
private:
    // --- 1. Private Enums and Structs ---
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

    // --- 2. Private Member Variables ---
    Node* root;
    Node* NIL; // A single "sentinel" node to represent all leaves
    int count;

public:
    // --- 3. Public Constructor, Destructor, "Rule of 3" ---

    /**
     * Constructor
     */
    RBMap() {
        // Create the sentinel node
        NIL = new Node(K(), V()); // Use default constructor for K and V
        NIL->color = BLACK;
        NIL->left = NIL;
        NIL->right = NIL;
        NIL->parent = NIL;
        root = NIL;
        count = 0;
    }

    /**
     * Destructor
     */
    ~RBMap() {
        clearRecursive(root); // Calls private helper
        delete NIL; // Don't forget to delete the sentinel
    }

    /**
     * Copy Constructor
     */
    RBMap(const RBMap<K, V>& other) {
        // 1. Set up this map's new NIL node
        NIL = new Node(K(), V());
        NIL->color = BLACK;
        NIL->left = NIL;
        NIL->right = NIL;
        NIL->parent = NIL;

        // 2. Call the recursive copy function (private helper)
        root = copyRecursive(other.root, NIL, other.NIL); // Pass our NIL as parent of root

        // 3. Copy the size
        count = other.count;
    }

    /**
     * Copy Assignment Operator
     */
    RBMap<K, V>& operator=(const RBMap<K, V>& other) {
        if (this == &other) {
            return *this; // Handle self-assignment
        }

        // 1. Clear this map's old data
        clearRecursive(root);
        // Note: We keep our existing NIL node

        // 2. Call the recursive copy function (private helper)
        root = copyRecursive(other.root, NIL, other.NIL); // Pass our NIL as parent of root

        // 3. Copy the size
        count = other.count;

        return *this;
    }


    // --- 4. Public Interface ---

    /**
     * Inserts a key-value pair. Fixes the tree if properties are violated.
     */
    void insert(const K& key, const V& value) {
        Node* z = new Node(key, value);
        z->left = NIL;
        z->right = NIL;
        z->parent = NIL;

        Node* y = NIL;
        Node* x = this->root;

        // Standard BST insert
        while (x != NIL) {
            y = x;
            if (z->key < x->key) {
                x = x->left;
            }
            else if (z->key > x->key) {
                x = x->right;
            }
            else {
                // z->key == x->key
                // Key already exists, update value and clean up
                x->value = value;
                delete z; // Don't need the new node
                return; // Exit function, no rebalance needed
            }
        }

        // Link new node z into the tree
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

        count++; // Increment count
        fixInsert(z); // Calls private helper
    }

    /**
     * Searches for a key and returns its value via out-parameter.
     */
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

    /**
     * Returns the number of items in the map.
     */
    int size() const {
        return count;
    }

    /**
     * Checks if the map is empty.
     */
    bool isEmpty() const {
        return root == NIL;
    }

    /**
     * Prints the tree contents for debugging.
     */
    void print() const {
        std::cout << "RBMap (" << size() << " items):\n";
        if (isEmpty()) {
            std::cout << " (empty)\n";
        }
        else {
            printInOrder(root); // Calls private helper
        }
    }

private:
    // --- 5. Private Helper Functions ---
    // (All definitions are placed *after* the public functions
    // that call them, so no forward declarations are needed.)

    /**
     * Performs a left rotation on node x.
     */
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

    /**
     * Performs a right rotation on node y.
     */
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

    /**
     * Restores Red-Black properties after insertion.
     */
    void fixInsert(Node* z) {
        while (z->parent->color == RED) {
            if (z->parent == z->parent->parent->left) {
                Node* y = z->parent->parent->right; // Uncle
                if (y->color == RED) {
                    // Case 1: Uncle is RED
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                }
                else {
                    if (z == z->parent->right) {
                        // Case 2: Uncle is BLACK, z is right child (Left-Right)
                        z = z->parent;
                        leftRotate(z);
                    }
                    // Case 3: Uncle is BLACK, z is left child (Left-Left)
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    rightRotate(z->parent->parent);
                }
            }
            else {
                // Symmetric to above, with "right" and "left" exchanged
                Node* y = z->parent->parent->left; // Uncle
                if (y->color == RED) {
                    // Case 4: Uncle is RED
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                }
                else {
                    if (z == z->parent->left) {
                        // Case 5: Uncle is BLACK, z is left child (Right-Left)
                        z = z->parent;
                        rightRotate(z);
                    }
                    // Case 6: Uncle is BLACK, z is right child (Right-Right)
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    leftRotate(z->parent->parent);
                }
            }
        }
        root->color = BLACK;
    }

    /**
     * Recursively deletes all nodes in a post-order traversal.
     */
    void clearRecursive(Node* node) {
        if (node != NIL) {
            clearRecursive(node->left);
            clearRecursive(node->right);
            delete node;
        }
    }

    /**
     * Recursively copies the tree.
     */
    Node* copyRecursive(Node* nodeToCopy, Node* newParent, Node* otherNIL) {
        if (nodeToCopy == otherNIL) {
            return NIL; // Base case: hit a leaf
        }

        // 1. Create the new node and copy data
        Node* newNode = new Node(nodeToCopy->key, nodeToCopy->value);
        newNode->color = nodeToCopy->color;
        newNode->parent = newParent; // Link to new parent

        // 2. Recursively copy children, passing *this new node* as their parent
        newNode->left = copyRecursive(nodeToCopy->left, newNode, otherNIL);
        newNode->right = copyRecursive(nodeToCopy->right, newNode, otherNIL);

        return newNode;
    }

    /**
     * Recursively prints the tree in-order for debugging.
     */
    void printInOrder(Node* node) const {
        if (node == NIL) return;
        printInOrder(node->left);
        std::cout << " (" << node->key << ": " << node->value
            << ", c=" << (node->color == RED ? "R" : "B")
            // Use key (a std::string) directly, not to_string()
            << ", p=" << (node->parent == NIL ? "NIL" : node->parent->key)
            << ")\n";
        printInOrder(node->right);
    }
};

