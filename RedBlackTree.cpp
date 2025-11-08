#include "RedBlackTree.h"
#include <algorithm>
#include <stdexcept>
#include <iomanip>
#include <iostream>
using namespace std;

RedBlackTree::RedBlackTree(SortMode m) : root(nullptr), mode(m) {}

RedBlackTree::~RedBlackTree() {
    destroyTree(root);
}

void RedBlackTree::destroyTree(RBNode* node) {
    if (node) {
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }
}

double RedBlackTree::getKey(const Song& s) const {
    switch (mode) {
    case SortMode::POPULARITY: return s.popularity;
    case SortMode::DANCEABILITY: return s.danceability;
    case SortMode::ENERGY: return s.energy;
    case SortMode::DURATION: return s.duration_ms;
    default: return s.popularity;
    }
}


void RedBlackTree::rotateLeft(RBNode* x) {
    RBNode* y = x->right;
    x->right = y->left;
    if (y->left) y->left->parent = x;
    y->parent = x->parent;
    if (!x->parent)
        root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    y->left = x;
    x->parent = y;
}

void RedBlackTree::rotateRight(RBNode* y) {
    RBNode* x = y->left;
    y->left = x->right;
    if (x->right) x->right->parent = y;
    x->parent = y->parent;
    if (!y->parent)
        root = x;
    else if (y == y->parent->left)
        y->parent->left = x;
    else
        y->parent->right = x;
    x->right = y;
    y->parent = x;
}


void RedBlackTree::insert(const Song& s) {
    RBNode* Node = new RBNode(s);
    RBNode* y = nullptr;
    RBNode* x = root;

    double key = getKey(s);

    while (x) {
        y = x;
        if (key < getKey(x->song))
            x = x->left;
        else
            x = x->right;
    }

    Node->parent = y;
    if (!y)
        root = Node;
    else if (key < getKey(y->song))
        y->left = Node;
    else
        y->right = Node;

    while (Node->parent && Node->parent->color == RED) {
        RBNode* grand = Node->parent->parent;
        if (Node->parent == grand->left) {
            RBNode* uncle = grand->right;
            if (uncle && uncle->color == RED) {
                // case 1: uncle is red
                Node->parent->color = BLACK;
                uncle->color = BLACK;
                grand->color = RED;
                Node = grand;
            } else {
                if (Node == Node->parent->right) {
                    // case 2: left-right
                    Node = Node->parent;
                    rotateLeft(Node);
                }
                // case 3: left-left
                Node->parent->color = BLACK;
                grand->color = RED;
                rotateRight(grand);
            }
        } else {
            RBNode* uncle = grand->left;
            if (uncle && uncle->color == RED) {
                Node->parent->color = BLACK;
                uncle->color = BLACK;
                grand->color = RED;
                Node = grand;
            } else {
                if (Node == Node->parent->left) {
                    Node = Node->parent;
                    rotateRight(Node);
                }
                Node->parent->color = BLACK;
                grand->color = RED;
                rotateLeft(grand);
            }
        }
    }
    root->color = BLACK;
}

void RedBlackTree::inorderHelper(RBNode* node, vector<Song>& out) const {
    if (!node) return;
    inorderHelper(node->left, out);
    out.push_back(node->song);
    inorderHelper(node->right, out);
}

vector<Song> RedBlackTree::getTopN(int n) const {
    vector<Song> sorted;
    inorderHelper(root, sorted);
    // Sort by key descending
    sort(sorted.begin(), sorted.end(), [this](const Song& a, const Song& b) {
        return getKey(a) > getKey(b);
    });
    if ((int)sorted.size() > n)
        sorted.resize(n);
    return sorted;
}

void RedBlackTree::printTopN(int n) const {
    auto top = getTopN(n);
    cout << left;
    cout << "---------------------------------------------------------------------------------------------------------\n";
    cout << setw(5)  << "Rank"
         << setw(25) << "Song Name"
         << setw(25) << "Artist"
         << setw(12) << "Key"
         << setw(12) << "Popularity"
         << setw(12) << "Duration"
         << endl;
    cout << "---------------------------------------------------------------------------------------------------------\n";

    for (int i = 0; i < (int)top.size(); ++i) {
        const Song& s = top[i];
        cout << setw(5) << (i + 1)
             << setw(25) << (s.name.size() > 24 ? s.name.substr(0,21) + "..." : s.name)
             << setw(25) << (s.artist.size() > 24 ? s.artist.substr(0,21) + "..." : s.artist)
             << setw(12) << fixed << setprecision(3) << getKey(s)
             << setw(12) << s.popularity
             << setw(12) << s.duration_ms
             << endl;
    }
    cout << "---------------------------------------------------------------------------------------------------------\n";
}

void RedBlackTree::printHelper(RBNode* node, int indent) const {
    if (node) {
        printHelper(node->right, indent + 4);
        if (indent) cout << setw(indent) << ' ';
        cout << (node->color == RED ? "R: " : "B: ") << node->song.name << endl;
        printHelper(node->left, indent + 4);
    }
}

void RedBlackTree::printTree() const {
    printHelper(root);
}
