

#include <iostream>
#include <limits>
#include <vector>
using namespace std;


template <typename Comparable>
class AVLTree
{
public:
    AVLTree();
    ~AVLTree();

    void makeEmpty();
    const Comparable & findMin() const;
    const Comparable & findMax() const;

    bool contains(const Comparable & x) const;
    void insert(const Comparable & x);
    void remove(const Comparable & x);
    int treeSize() const;
    int computeHeight() const;
    int readRootHeight() const;
    bool isBalanced() const;
    bool isBST() const;

    double averageDepth() const;
    void removeByRank(int rank);

    // the next line follows textbook Figure 4.42, Line 19
    static const int ALLOWED_IMBALANCE = 1;
private:
    struct AVLNode  // refer to textbook, Figure 4.40
    {
        Comparable element;
        AVLNode *left;
        AVLNode *right;
        int height;

        AVLNode( const Comparable & theElement, AVLNode *lt, AVLNode *rt ): element(theElement), left(lt), right(rt) {}
        AVLNode( Comparable && theElement, AVLNode *lt, AVLNode *rt ): element(move(theElement)), left(lt), right(rt) {}
    };

    AVLNode *root;

    AVLNode * findMin( AVLNode * t ) const;
    AVLNode * findMax( AVLNode * t ) const;
    void makeEmpty( AVLNode * & t );

    void balance(AVLNode * & t);
    void rotateWithLeftChild( AVLNode * & t );
    void rotateWithRightChild( AVLNode * & t );
    void doubleWithLeftChild( AVLNode * & t);
    void doubleWithRightChild( AVLNode * & t);

    void insert(const Comparable & x, AVLNode * & t);  // Inserts a new element
    void remove(const Comparable & x, AVLNode * & t);  // Removes an element

    // Search operations
    bool contains(const Comparable & x, AVLNode * t) const; // Checks if the tree contains x

    // Size and depth calculations
    int treeSize(AVLNode * t) const;                  // Returns the size of the tree
    void computeAverageDepth(AVLNode *t, int depth, int &totalDepth, int &numNodes) const; // Computes average depth
    bool isBST(AVLNode *t, const Comparable & min, const Comparable & max) const; // Checks if the tree is a BST
    bool isBalanced(AVLNode *t) const;                  // Checks if the tree is balanced
    int computeHeight(AVLNode * t) const;              // Computes the height of the tree
    int readRootHeight(AVLNode * t) const;             // Returns the height of the root

    // Other functions
    double averageDepth(AVLNode *t, int depth, int &numNodes) const; // Calculates average depth
    void removeByRank(int rank, AVLNode * & t);  
};

// constructor
template <class Comparable>
AVLTree<Comparable>::AVLTree() : root(NULL) {}

// destructor
template <class Comparable>
AVLTree<Comparable>::~AVLTree()
{
    makeEmpty();
}

// public makeEmpty: follow the makeEmpty in BST, referring to textbook, Figure 4.27
template <typename Comparable>
void AVLTree<Comparable>::makeEmpty() {
    makeEmpty(root);
}

// private recursive makeEmpty: follow the makeEmpty in BST, referring to textbook, Figure 4.27
template <typename Comparable>
void AVLTree<Comparable>::makeEmpty(AVLNode * & t) {
    if ( t != NULL ) {
        makeEmpty(t->left);
        makeEmpty(t->right);
        delete t;
        t = NULL;
    }
}

// public findMin: follow the findMin in BST, referring to textbook, Figure 4.20
template <typename Comparable>
const Comparable & AVLTree<Comparable>::findMin() const {
    if (root == NULL) {
        throw underflow_error("Tree is empty");
    }
    return findMin(root)->element;
}

// private findMin: follow the findMin in BST, referring to textbook, Figure 4.20
template <typename Comparable>
typename AVLTree<Comparable>::AVLNode* AVLTree<Comparable>::findMin(AVLNode * t) const {
    if ( t == NULL ) {
        return NULL;
    } else if (t->left == NULL) {
        return t;
    } else {
        return findMin(t->left);
    }
}

// public findMax: follow the findMax in BST, referring to textbook, Figure 4.21
template <typename Comparable>
const Comparable & AVLTree<Comparable>::findMax() const {
    if (root == NULL) {
        throw underflow_error("Tree is empty");
    }
    return findMax(root)->element;
}

// private findMax: follow the findMax in BST, referring to textbook, Figure 4.21
template <typename Comparable>
typename AVLTree<Comparable>::AVLNode* AVLTree<Comparable>::findMax(AVLNode * t) const {
    if ( t == NULL ) {
        return NULL;
    } else if (t->right == NULL) {
        return t;
    } else {
        return findMax(t->right);
    }
}

// start our implementation:
// public contains: follow the contains in BST, referring to textbook, Figure 4.17 and Figure 4.18
// contains: Check if the tree contains a specific element.
template<typename Comparable>
bool AVLTree<Comparable>::contains(const Comparable & x) const {
    AVLNode *current = root;
    while (current != NULL) {
        if (x < current->element)
            current = current->left;
        else if (x > current->element)
            current = current->right;
        else
            return true; // Found the element
    }
    return false; // Element not found
}

// insert: Add a new element to the AVL tree, ensuring balance.
template<typename Comparable>
void AVLTree<Comparable>::insert(const Comparable & x) {
    insert(x, root);
}

// Helper function for insert
template<typename Comparable>
void AVLTree<Comparable>::insert(const Comparable & x, AVLNode * & t) {
    if (t == NULL) {
        t = new AVLNode(x, NULL, NULL);
    } else if (x < t->element) {
        insert(x, t->left);
    } else if (x > t->element) {
        insert(x, t->right);
    }
    balance(t); // Balance the tree
}

// remove: Remove an element and rebalance if needed.
template<typename Comparable>
void AVLTree<Comparable>::remove(const Comparable & x) {
    remove(x, root);
}

// Helper function for remove
template<typename Comparable>
void AVLTree<Comparable>::remove(const Comparable & x, AVLNode * & t) {
    if (t == NULL)
        return; // Item not found; do nothing

    if (x < t->element) {
        remove(x, t->left);
    } else if (x > t->element) {
        remove(x, t->right);
    } else if (t->left != NULL && t->right != NULL) { // Two children
        t->element = findMin(t->right)->element;
        remove(t->element, t->right);
    } else { // One or no children
        AVLNode *oldNode = t;
        t = (t->left != NULL) ? t->left : t->right;
        delete oldNode;
    }

    balance(t); // Balance the tree
}

// balance: Balance the AVL tree if necessary.
template<typename Comparable>
void AVLTree<Comparable>::balance(AVLNode * & t) {
    if (t == NULL)
        return;

    if (computeHeight(t->left) - computeHeight(t->right) > ALLOWED_IMBALANCE) {
        if (computeHeight(t->left->left) >= computeHeight(t->left->right))
            rotateWithLeftChild(t);
        else
            doubleWithLeftChild(t);
    } else if (computeHeight(t->right) - computeHeight(t->left) > ALLOWED_IMBALANCE) {
        if (computeHeight(t->right->right) >= computeHeight(t->right->left))
            rotateWithRightChild(t);
        else
            doubleWithRightChild(t);
    }

    t->height = max(computeHeight(t->left), computeHeight(t->right)) + 1;
}

// rotateWithLeftChild: Perform a single rotation for the left-left case.
template<typename Comparable>
void AVLTree<Comparable>::rotateWithLeftChild(AVLNode * & k2) {
    AVLNode *k1 = k2->left;
    k2->left = k1->right;
    k1->right = k2;
    k2->height = max(computeHeight(k2->left), computeHeight(k2->right)) + 1;
    k1->height = max(computeHeight(k1->left), k2->height) + 1;
    k2 = k1;
}

// rotateWithRightChild: Perform a single rotation for the right-right case.
template<typename Comparable>
void AVLTree<Comparable>::rotateWithRightChild(AVLNode * & k2) {
    AVLNode *k1 = k2->right;
    k2->right = k1->left;
    k1->left = k2;
    k2->height = max(computeHeight(k2->left), computeHeight(k2->right)) + 1;
    k1->height = max(computeHeight(k1->right), k2->height) + 1;
    k2 = k1;
}

// doubleWithLeftChild: Perform a double rotation for the left-right case.
template<typename Comparable>
void AVLTree<Comparable>::doubleWithLeftChild(AVLNode * & k3) {
    rotateWithRightChild(k3->left);
    rotateWithLeftChild(k3);
}

// doubleWithRightChild: Perform a double rotation for the right-left case.
template<typename Comparable>
void AVLTree<Comparable>::doubleWithRightChild(AVLNode * & k3) {
    rotateWithLeftChild(k3->right);
    rotateWithRightChild(k3);
}

// computeHeight: Calculate and return the height of a node.
template<typename Comparable>
int AVLTree<Comparable>::computeHeight(AVLNode *t) const {
    return t == NULL ? -1 : t->height;
}

// isBalanced: Check if the AVL tree is balanced.
template <class Comparable>
bool AVLTree<Comparable>::isBalanced() const {
    return isBalanced(root);
}

// Helper function for isBalanced
template<typename Comparable>
bool AVLTree<Comparable>::isBalanced(AVLNode *t) const {
    if (t == NULL) return true;
    int heightDiff = abs(computeHeight(t->left) - computeHeight(t->right));
    return heightDiff <= ALLOWED_IMBALANCE && isBalanced(t->left) && isBalanced(t->right);
}

// isBST: Verify if the tree maintains BST properties.
template <class Comparable>
bool AVLTree<Comparable>::isBST() const {
    return isBST(root, std::numeric_limits<Comparable>::min(), std::numeric_limits<Comparable>::max());
}

// Helper function for isBST
template<typename Comparable>
bool AVLTree<Comparable>::isBST(AVLNode *t, const Comparable & min, const Comparable & max) const {
    if (t == NULL) return true;
    if (t->element < min || t->element > max) return false;
    return isBST(t->left, min, t->element) && isBST(t->right, t->element, max);
}

// treeSize: Calculate and return the number of nodes in the tree.
template <typename Comparable>
int AVLTree<Comparable>::treeSize() const {
    return treeSize(root);
}

// Helper function for treeSize
template <typename Comparable>
int AVLTree<Comparable>::treeSize(AVLNode * t) const {
    if (t == NULL) return 0;
    return 1 + treeSize(t->left) + treeSize(t->right);
}

// readRootHeight: Return the height of the root node.
template <typename Comparable>
int AVLTree<Comparable>::readRootHeight() const {
    return computeHeight(root);
}

// averageDepth: Calculate the average depth of all nodes in the tree.
template <typename Comparable>
double AVLTree<Comparable>::averageDepth() const {
    int totalDepth = 0;
    int numNodes = 0;
    computeAverageDepth(root, 0, totalDepth, numNodes);
    return numNodes == 0 ? 0.0 : static_cast<double>(totalDepth) / numNodes;
}

// Helper function for averageDepth
template<typename Comparable>
void AVLTree<Comparable>::computeAverageDepth(AVLNode *t, int depth, int &totalDepth, int &numNodes) const {
    if (t == NULL) return;
    totalDepth += depth;
    numNodes++;
    computeAverageDepth(t->left, depth + 1, totalDepth, numNodes);
    computeAverageDepth(t->right, depth + 1, totalDepth, numNodes);
}

template <typename Comparable>
int AVLTree<Comparable>::computeHeight() const {
    return computeHeight(root); // Call the private function with the root node
}

// removeByRank: Remove the node at a given rank.
// Public removeByRank
template <typename Comparable>
void AVLTree<Comparable>::removeByRank(int rank) {
    if (rank < 0 || rank >= treeSize()) {
        throw out_of_range("Rank is out of range");
    }
    removeByRank(rank, root);
}

// Private recursive removeByRank
template <typename Comparable>
void AVLTree<Comparable>::removeByRank(int rank, AVLNode *&t) {
    if (t == NULL) {
        return; // not happen if rank is valid
    }

    // Calculate the rank of the current node
    int leftSubtreeSize = treeSize(t->left);

    if (rank < leftSubtreeSize) {
        // the left subtree
        removeByRank(rank, t->left);
    } else if (rank > leftSubtreeSize) {
        // right subtree
        removeByRank(rank - leftSubtreeSize - 1, t->right);
    } else {
        
        remove(t->element, t); // Use the existing remove function
    }

    // Balance the tree after removal
    balance(t);
}

