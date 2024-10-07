#ifndef __BST_H
#define __BST_H

#include <iostream>
#include <limits>
#include <vector>
#include <queue>
#include <stdexcept> // Include this for exception handling
using namespace std;

/* ----------------------------------------------------------------------------
---- Below is declaration of BST class, referring to textbook, Figure 4.16 ----
---------------------------------------------------------------------------- */

template <typename Comparable>
class BST
{
public:
    BST();
    ~BST();
    void makeEmpty();
    
    const Comparable & findMin() const;
    const Comparable & findMax() const;

    bool contains(const Comparable & x) const;
    void insert(const Comparable & x);
    void remove(const Comparable & x);
    int treeSize() const;
    int treeHeight() const;
    void printInOrder() const;
    void printLevels() const;
    void printMaxPath() const;

private:
    struct BinaryNode
    {
        Comparable element;
        BinaryNode *left;
        BinaryNode *right;

        BinaryNode(const Comparable & theElement, BinaryNode *lt = nullptr, BinaryNode *rt = nullptr)
            : element(theElement), left(lt), right(rt) {}
    };

    BinaryNode *root;

    void makeEmpty(BinaryNode *& t);
    BinaryNode * findMin(BinaryNode * t) const;
    BinaryNode * findMax(BinaryNode * t) const;
    bool contains(const Comparable & x, BinaryNode *t) const;
    void insert(const Comparable & x, BinaryNode *& t); // Helper method for insertion
    void remove(const Comparable & x, BinaryNode *& t);
    int treeSize(BinaryNode *t) const;
    int treeHeight(BinaryNode *t) const;
    void printInOrder(BinaryNode *t) const;
    void printLevels(BinaryNode *t) const;
    void printMaxPath(BinaryNode *t, Comparable path[], int currentDepth, int &maxDepth) const;
};

/* -------------------------------------------------------------- 
---- Below is implementation of public and private functions ---- 
-------------------------------------------------------------- */

// Constructor
template<typename Comparable>
BST<Comparable>::BST() : root(nullptr) {}

// Destructor
template<typename Comparable>
BST<Comparable>::~BST() {
    makeEmpty();
}

// Public makeEmpty
template <typename Comparable>
void BST<Comparable>::makeEmpty() {
    makeEmpty(root);
}

// Private recursive makeEmpty
template <typename Comparable>
void BST<Comparable>::makeEmpty(BinaryNode *& t) {
    if (t != nullptr) {
        makeEmpty(t->left);
        makeEmpty(t->right);
        delete t;
        t = nullptr;
    }
}

// Public findMin
template <typename Comparable>
const Comparable & BST<Comparable>::findMin() const {
    if (root == nullptr) {
        throw underflow_error("Tree is empty");
    }
    return findMin(root)->element;
}

// Private findMin
template <typename Comparable>
typename BST<Comparable>::BinaryNode* BST<Comparable>::findMin(BinaryNode * t) const {
    if (t == nullptr) {
        return nullptr;
    } else if (t->left == nullptr) {
        return t;
    } else {
        return findMin(t->left);
    }
}

// Public findMax
template <typename Comparable>
const Comparable & BST<Comparable>::findMax() const {
    if (root == nullptr) {
        throw underflow_error("Tree is empty");
    }
    return findMax(root)->element;
}

// Private findMax
template <typename Comparable>
typename BST<Comparable>::BinaryNode* BST<Comparable>::findMax(BinaryNode * t) const {
    if (t == nullptr) {
        return nullptr;
    } else if (t->right == nullptr) {
        return t;
    } else {
        return findMax(t->right);
    }
}

// Public contains
template<typename Comparable>
bool BST<Comparable>::contains(const Comparable & x) const {
    return contains(x, root);
}

// Private contains
template<typename Comparable>
bool BST<Comparable>::contains(const Comparable & x, BinaryNode *t) const {
    if (t == nullptr)
        return false;
    else if (x < t->element)
        return contains(x, t->left);
    else if (t->element < x)
        return contains(x, t->right);
    return true;
}

// Public insert
template<typename Comparable>
void BST<Comparable>::insert(const Comparable & x) {
    insert(x, root); 
}

// Private insert
template<typename Comparable>
void BST<Comparable>::insert(const Comparable & x, BinaryNode *& t) {
    if (t == nullptr) {
        t = new BinaryNode(x);
    } else if (x < t->element) {
        insert(x, t->left); 
    } else if (t->element < x) {
        insert(x, t->right);
    }
   
}

// Private remove helper function
template<typename Comparable>
void BST<Comparable>::remove(const Comparable & x, BinaryNode *& t) {
    if (t == nullptr) {
        return;
    }
    
    if (x < t->element) {

        remove(x, t->left);
    } else if (t->element < x) {

        remove(x, t->right);
    } else if (t->left != nullptr && t->right != nullptr) {
        t->element = findMin(t->right)->element;
  
        remove(t->element, t->right);
    } else {
        BinaryNode *oldNode = t;
        t = (t->left != nullptr) ? t->left : t->right; 
        delete oldNode; 
    }
}

// Public remove function
template<typename Comparable>
void BST<Comparable>::remove(const Comparable & x) {
    remove(x, root);
}
// Public treeSize
template <typename Comparable>
int BST<Comparable>::treeSize() const {
    return treeSize(root);
}

// Private recursive treeSize helper
template <typename Comparable>
int BST<Comparable>::treeSize(BinaryNode *t) const {
    if (t == nullptr) {
        return 0; 
    }
  
    return 1 + treeSize(t->left) + treeSize(t->right);
}

// Public treeHeight
template <typename Comparable>
int BST<Comparable>::treeHeight() const {
    return treeHeight(root); 
}

// Private recursive treeHeight helper
template <typename Comparable>
int BST<Comparable>::treeHeight(BinaryNode *t) const {
    if (t == nullptr) {
        return -1; 
    }
   
    return 1 + max(treeHeight(t->left), treeHeight(t->right));
}

// Public printInOrder
template<typename Comparable>
void BST<Comparable>::printInOrder() const {
    printInOrder(root); 
}

// Private recursive printInOrder helper
template<typename Comparable>
void BST<Comparable>::printInOrder(BinaryNode *t) const {
    if (t != nullptr) {
        printInOrder(t->left); 
        cout << t->element << " "; 
        printInOrder(t->right); 
    }
}

// Public printLevels
template <typename Comparable>
void BST<Comparable>::printLevels() const {
    if (root == nullptr) {
        cout << "Tree is empty" << endl;
        return;
    }
    printLevels(root); 
}

// Private printLevels helper using level-order traversal
template <typename Comparable>
void BST<Comparable>::printLevels(BinaryNode *t) const {
    queue<BinaryNode*> q; 
    q.push(t); 

    while (!q.empty()) {
        int levelSize = q.size(); 
        for (int i = 0; i < levelSize; ++i) {
            BinaryNode* current = q.front(); 
            q.pop(); 

            cout << current->element << " ";

          
            if (current->left != nullptr) {
                q.push(current->left);
            }
            if (current->right != nullptr) {
                q.push(current->right);
            }
        }
        cout << endl; 
    }
}
// Public printMaxPath (stub implementation)
template <typename Comparable>
void BST<Comparable>::printMaxPath() const {
    Comparable path[100]; 
    int maxDepth = 0;
    printMaxPath(root, path, 0, maxDepth);
}


template <typename Comparable>
void BST<Comparable>::printMaxPath(BinaryNode *t, Comparable path[], int currentDepth, int &maxDepth) const {
    if (t == nullptr) {
        return; 
    }


    path[currentDepth] = t->element;
    currentDepth++;


    if (t->left == nullptr && t->right == nullptr) {

        if (currentDepth > maxDepth) {
            maxDepth = currentDepth; 

            cout << "Max Path: ";
            for (int i = 0; i < currentDepth; ++i) {
                cout << path[i] << " ";
            }
            cout << endl;
        }
    } else {
        printMaxPath(t->left, path, currentDepth, maxDepth);
        printMaxPath(t->right, path, currentDepth, maxDepth);
    }
}
#endif // __BST_H
