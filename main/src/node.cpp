#include <iostream>
#include <string>
#include <sstream>
#include "../include/node.h"

Node::Node() : 
    p1r { 1 },
    p1l { 1 }, 
    p2r { 1 },
    p2l { 1 },
    player { 1 } {

    parent = nullptr;

    std::cout << toString() << std::endl;

    branches.push_front(new Node((int)p1r, (int)p1l, p2r + p1r, (int)p2l, 2, this, 1));           
}

Node::Node(int p1rNum, int p1lNum, int p2rNum, int p2lNum, int playerNum, Node* parentNode, int num) :
    p1r { p1rNum % 5 },
    p1l { p1lNum % 5 },
    p2r { p2rNum % 5 },
    p2l { p2lNum % 5 },
    player { playerNum },
    parent { parentNode },
    killNum { num } {

    std::cout << toString() << std::endl;

    if (killNum < 3) {
        fillBranches();
    }
}

Node::Node(const Node& node) : 
    p1r { node.p1r },
    p1l { node.p1l },
    p2r { node.p2r },
    p2l { node.p2l },
    player { node.player },
    parent { node.parent },
    killNum { node.killNum } {

}

Node::~Node() {
    std::list<Node*>::iterator iterator = branches.begin();
    for (size_t i; i < branches.size(); i++) {
        delete *iterator;
        advance(iterator, 1);
    }
}

std::string Node::toString() {
    std::ostringstream oss;
    oss << "\nPlayer Turn: " << player << "\nP2 Right P2 Left\n   " << p2r << "         " << p2l << "\n\n   " << p1r << "         " << p1l << "\nP1 Right P1 Left\n";
    return oss.str();
}

int Node::winner() {
    if (p2r == 0 && p2l == 0) {
        return 1;
    } else if (p1r == 0 && p1l == 0) {
        return 2;
    } else {
        return 0;
    }
}

// void Node::fillBranches() {
//     if (player == 1) {
//         if (p1r == p1l) {
//             // Each hand is the same and can only add to 1 or the other

//             // Add possibility of p1 right hand hits p2 right hand
//             branches.push_front(new Node((int)p1r, (int)p1l, p2r + p1r, (int)p2l, 2, this, killNum + 1));           
//             if (p2r != p2l) {
//                 // Add possibility of p1 right hand hist p2 left hand
//                 branches.push_front(new Node((int)p1r, (int)p1l, (int)p2r, p2l + p1r, 2, this, killNum + 1));
//             }

//             // Add possibility of combining
//             if (p1r == 1) {
//                 branches.push_front(new Node(2, 0, (int)p2r, (int)p2l, 2, this));
//             } else if (p1r == 2) {
//                 branches.push_front(new Node(4, 0, (int)p2r, (int)p2l, 2, this));
//                 branches.push_front(new Node(3, 1, (int)p2r, (int)p2l, 2, this));
//             } else if (p1r == 3) {
//                 branches.push_front(new Node(4, 2, (int)p2r, (int)p2l, 2, this));
//                 branches.push_front(new Node(1, 0, (int)p2r, (int)p2l, 2, this));
//             } else {
//                 branches.push_front(new Node(p1r + p1r, 0, (int)p2r, (int)p2l, 2, this));
//             }
            
//         } else {
//             // Add possibility of p1 right hand hits p2 right hand
//             branches.push_front(new Node((int)p1r, (int)p1l, p2r + p1r, (int)p2l, 2, this));
//             branches.push_front(new Node((int)p1r, (int)p1l, p2r + p1l, (int)p2l, 2, this));
//             if (p2r != p2l) {
//                 // Add possibility of p1 right hand hist p2 left hand
//                 branches.push_front(new Node((int)p1r, (int)p1l, (int)p2r, p2l + p1r, 2, this));
//                 branches.push_front(new Node((int)p1r, (int)p1l, (int)p2r, p2l + p1l, 2, this));
//             }

//             // Add possibility of combining
//             branches.push_front(new Node(p1r + p1l, 0, (int)p2r, (int)p2l, 2, this));
//             if (p1r + 1 != p1l && p1l + 1 != p1r) {
//                 // Can add both then subtract 1 without doing something I consider stupid
//                 branches.push_front(new Node((p1r + p1l) - 1, 1, (int)p2r, (int)p2l, 2, this));
//             }
//         }
//     }

// }