#ifndef REDBLACKTREE_H
#define REDBLACKTREE_H

#include <vector>
#include "Song.h"
using namespace std;

enum Color { RED, BLACK };

struct RBNode {
    Song song;
    Color color;
    RBNode* left;
    RBNode* right;
    RBNode* parent;

    explicit RBNode(const Song& s)
        : song(s), color(RED), left(nullptr), right(nullptr), parent(nullptr) {}
};

class RedBlackTree {
private:
    RBNode* root;
    SortMode mode;

    void destroyTree(RBNode* node);

    double getKey(const Song& s) const;
    void rotateLeft(RBNode* x);
    void rotateRight(RBNode* y);
    void fixInsert(RBNode* node);

    void inorderHelper(RBNode* node, std::vector<Song>& out) const;
    void printHelper(RBNode* node, int indent = 0) const;

public:
    explicit RedBlackTree(SortMode m);
    ~RedBlackTree();

    void insert(const Song& s);
    std::vector<Song> getTopN(int n) const;
    void printTopN(int n) const;
    void printTree() const;
};



#endif // REDBLACKTREE_H
