#ifndef NODE_H

#define NODE_H

#include <iostream>
#include <list>

class Node {
public:
 Node();
 Node(int, int, int, int, int, Node*, int);
 Node(const Node&);
 
 ~Node();
 
 std::string toString();
 int winner();

private:
 int p1r;
 int p1l;
 int p2r;
 int p2l;

 int player;
 int killNum;

 Node* parent;
 std::list <Node*> branches;

 void fillBranches();
};

#endif
