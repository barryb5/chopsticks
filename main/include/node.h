#pragma once 

#include <iostream>
#include <vector>
#include <list>
#include <map>

class Node {

public:
    Node();
//    Node(const Node&);
    Node(const std::vector<int>& hands, int player, Node* parent, int kill_num);
 
//    ~Node();
 
    void dump(std::ostream& os) const;

    // int winner();
    // Node* searchForCopy(Node*, Node&);
    // Node* searchFromTop(Node*, Node&);

    bool checkMatching(const Node&) const;
    void autoAdd(int, int, int, int, int, Node*, int);

    void tester();
    int winner();

    void printWinStats();


private:
    int player_;
    int kill_num_;

    Node* parent_;
    std::vector<int> hands_; 
    std::list <Node> branches_;

    // void fillBranches();
    void genScenarios();
    void combineHands();
    void addOpponent();

public:
    static const std::multimap<int, std::vector<int> > s_combo_lookup;
};