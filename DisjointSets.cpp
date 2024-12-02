#include "DisjointSets.h"

// Constructor: Initialize the vector with -1
// referred to page 355 - 360 of text book
DisjointSets::DisjointSets(int numElements) : s(numElements, -1) { }

// Union by rank
void DisjointSets::unionSets(int root1, int root2) {
    if (s[root2] < s[root1]) {
        s[root1] = root2; // root2 is deeper
    } else {
        if (s[root1] == s[root2]) {
            s[root1]--; // Increase rank
        }
        s[root2] = root1; // Attach root2 to root1
    }
}

// Find without path compression
int DisjointSets::find(int x) const {
    if (s[x] < 0) {
        return x; // x is a root
    } else {
        return find(s[x]); // Follow parent pointer
    }
}

// Find with path compression
int DisjointSets::find(int x) {
    if (s[x] < 0) {
        return x; // x is a root
    } else {
        return s[x] = find(s[x]); // Path compression
    }
}
