#include <iostream>
#include <string>
#include <sstream>
#include "../include/node.h"

const std::multimap<int, std::vector<int>> Node::s_combo_lookup { 
  {2, {1, 1}},
  {2, {2, 0}},

  {3, {2, 1}}, 
  {3, {3, 0}}, 

  {4, {3, 1}}, 
  {4, {2, 2}}, 
  {4, {4, 0}}, 

  {5, {4, 1}}, 
  {5, {3, 2}}, 

  {6, {4, 2}}, 
  {6, {3, 3}}, 
};

static std::vector<int> win_stats {
    0,
    0
};


Node::Node()
: hands_ {1, 1, 1, 1 }
, player_(1)
, kill_num_(0)
{
    parent_ = nullptr;
    dump(std::cout);
    genScenarios();
    //branches_.push_front(new Node(p1r_, p1l_, p2r_ + p1r_, p2l_, 2, this, 1)); 

             
}


// Node::Node(const Node& node) 
// : p1r_(node.p1r_)
// , p1l_(node.p1l_)
// , p2r_(node.p2r_)
// , p2l_(node.p2l_)
// , player_(node.player_)
// , parent_(node.parent_)
// , kill_num_(node.kill_num_) {
//     std::cout << "copy construct" << std::endl;
// }


Node::Node(const std::vector<int>& hands, int player, Node* parent, int kill_num) 
: hands_(hands)
, player_(player)
, parent_(parent)
, kill_num_(kill_num) {


    // for (std::vector<int>::iterator it = hands_.begin(); it != hands_.end(); ++it)
    //     *it = *it % 5;
    
    for (auto& hand : hands_) {
        hand = hand % 5;
    }

    dump(std::cout);

    if (kill_num_ < 10) {
        genScenarios();
    } else {
        // exit(0);
    }
}


// Node::~Node() {
//     std::list<Node*>::iterator iterator = branches_.begin();
//     for (size_t i; i < branches_.size(); i++) {
//         delete *iterator;
//         advance(iterator, 1);
//     }
// }

void Node::dump(std::ostream& os) const {
    // os << "This: " << this;
    // os << "\nplayer_: " << player_ << "\tlvl: "<< kill_num_ << "\nP2 Right P2 Left\n   " << hands_[2] << "         " << hands_[3] << "\n\n   " << hands_[0] << "         " << hands_[1] << "\nP1 Right P1 Left\n";
    // os << std::endl;

    if (hands_[2] == 0 && hands_[3] == 0) {
        win_stats[0] = win_stats[0] + 1;
    } else if (hands_[0] == 0 && hands_[1] == 0) {
        win_stats[1] = win_stats[1] + 1;
    }

    for(const auto& branch : branches_) {
        branch.dump(std::cout);
    }
}

void Node::printWinStats() {
    std::cout << "\nPlayer 1 Wins: " << win_stats[0] << std::endl;
    std::cout << "\nPlayer 2 Wins: " << win_stats[1] << std::endl;

}

void Node::tester() {

    // auto pair_iter = s_combo_lookup.equal_range(3);
    // for (auto i = pair_iter.first; i != pair_iter.second; ++i) {
    //     std::cout << i->first << ": {"  << i->second[0] << ", " << i->second[1] << "}" << std::endl;
    // }

    // pair_iter = s_combo_lookup.equal_range(4);
    // for (auto i = pair_iter.first; i != pair_iter.second; ++i) {
    //     std::cout << i->first << ": {"  << i->second[0] << ", " << i->second[1] << "}" << std::endl;
    // }

}

void Node::combineHands() {
    int ix = (1 == player_) ? 0 : 2;
    std::vector<int> hands = hands_;


    // combine
    if (hands_[ix+0] + hands_[ix+1] < 7) {
        int sum  = hands_[ix+0] + hands_[ix+1];

        auto pair_iter = s_combo_lookup.equal_range(sum);
        for (auto i = pair_iter.first; i != pair_iter.second; ++i) {

            // std::cout << i->first << ": {"  << i->second[0] << ", " << i->second[1] << "}" << std::endl;
            // std::cout << "Parent: " << parent_ << "\n" << std::endl;
            if (i->second[0] != hands_[ix+0] && i->second[1] != hands_[ix+1]) {
                hands[ix+0] = i->second[0];
                hands[ix+1] = i->second[1];

                branches_.push_back(std::move(Node(hands, (player_+1) % 2, this, kill_num_+1)));
            }
        }
    }
}

void Node::addOpponent() {
    int ix = (1 == player_) ? 0 : 2;
    // Opponent hands finder
    int opp = (1 == player_) ? 2 : 0;
    std::vector<int> hands = hands_;

    // Do right hand always if possible
    if (0 != hands_[ix+0]) {
        if (0 != hands_[opp+0]) {
            hands[opp+0] = hands_[ix+0] + hands_[opp+0];
            branches_.push_back(std::move(Node(hands, (player_+1) % 2, this, kill_num_+1)));
        }
        if (0 != hands_[opp+1]) {
            hands[opp+1] = hands_[ix+0] + hands_[opp+1];
            branches_.push_back(std::move(Node(hands, (player_+1) % 2, this, kill_num_+1)));
        }
    }

    if (0 != hands_[ix+1] && hands_[ix+0] != hands_[ix+1]) {
        // Do left hand only if it isn't 0 and isn't equal to the right hand
        if (0 != hands_[opp+0]) {
            hands[opp+0] = hands_[ix+1] + hands_[opp+0];
            branches_.push_back(std::move(Node(hands, (player_+1) % 2, this, kill_num_+1)));
        }
        if (0 != hands_[opp+1]) {
            hands[opp+1] = hands_[ix+1] + hands_[opp+1];
            branches_.push_back(std::move(Node(hands, (player_+1) % 2, this, kill_num_+1)));
        }
    }
}

void Node::genScenarios() {
    combineHands();
    addOpponent();

}

int Node::winner() {
    if (hands_[2] == 0 && hands_[3] == 0) {
        std::cout << "\nPlayer 1 Wins" << std::endl;
        return 0;
    } else if (hands_[0] == 0 && hands_[1] == 0) {
        std::cout << "\nPlayer 2 Wins" << std::endl;
        return 1;
    } else {
        // No winner yet
        return -1;
    }
}

/*

bool Node::checkMatching(Node& node) {
    if (node.p1r_ == p1r_ &&
        node.p1l_ == p1l_ &&
        node.p2r_ == p2r_ &&
        node.p2l_ == p2l_ &&
        node.player_ == player_) {
        
        return true;
    }
    return false;
}

Node* Node::searchFromTop(Node* node, Node& checkNode) {
    if (node->checkMatching(checkNode)) {
        // If node exists, return pointer to node

        return node;
    }

    // Recursively search from top down in normal search
    std::list<Node*>::iterator iterator = node->branches_.begin();
    for (size_t i = 0; i < node->branches_.size(); i++) {
        searchFromTop(*iterator, checkNode);
        advance(iterator, 1);
    }

    // Existing copy of node doesn't exist, so return nullptr
    return nullptr;
}

Node* Node::searchForCopy(Node* node, Node& checkNode) {
    Node* returnNode = nullptr;

    if (nullptr == node->parent_) {
        // At the top so search from top down for node
        returnNode = searchFromTop(node, checkNode);
    } else {
        // Not at the top yet, keep climbing to main 1 1 1 1 p1 root
        return searchForCopy(parent_, checkNode);
    }

    if (nullptr == returnNode) {
        return &checkNode;
    } else {
        return returnNode;
    }
}
*/

/*
// Use checkbeforeAdding instead
void Node::autoAdd(int p1r_, int p1l_, int p2r_, int p2l_, int player_, Node* parent_, int kill_num_) {
    Node attemptInsertNode = *(new Node(p1r_, p1l_, p2r_, p2l_, player_, parent_, kill_num_));

    branches_.push_front(searchForCopy(this, attemptInsertNode));

    return;
}

void Node::fillBranches() {

    // Node* mightExist = searchForCopy(this, *this);
    // if (nullptr != mightExist) {
    //     std::cout << "\n\ncopy exists\n" << std::endl;
        // // This node already exists in the tree, so we can just point to that
        // std::list<Node*>::iterator iterator = parent_->branches_.begin();
        // for (size_t i = 0; i < parent_->branches_.size(); i++) {
        //     if (this == *iterator) {
        //         *iterator = mightExist;

        //         // Idk if this is allowed
        //         // delete this;

        //         return;
        //     }
        //     advance(iterator, 1);
        // }
    // }

    if (player_ == 1) {
        if (p1r_ == p1l_) {
            // Each hand is the same and can only add to 1 or the other

            // Add possibility of p1 right hand hits p2 right hand
            if (p2r_ != 0) {
                autoAdd((int)p1r_, (int)p1l_, p2r_ + p1r_, (int)p2l_, 2, this, kill_num_ + 1);
            }

            if (p2r_ != p2l_ && p2l_ != 0) {
                // Add possibility of p1 right hand hist p2 left hand
                autoAdd((int)p1r_, (int)p1l_, (int)p2r_, p2l_ + p1r_, 2, this, kill_num_ + 1);
            }

            // Add possibility of combining
            if (p1r_ == 1) {
                autoAdd(2, 0, (int)p2r_, (int)p2l_, 2, this, kill_num_ + 1);
            } else if (p1r_ == 2) {
                autoAdd(4, 0, (int)p2r_, (int)p2l_, 2, this, kill_num_ + 1);
                autoAdd(3, 1, (int)p2r_, (int)p2l_, 2, this, kill_num_ + 1);
            } else if (p1r_ == 3) {
                autoAdd(4, 2, (int)p2r_, (int)p2l_, 2, this, kill_num_ + 1);
            } else {
                autoAdd(p1r_ + p1r_, 0, (int)p2r_, (int)p2l_, 2, this, kill_num_ + 1);
            }
        } else {

            // Add possibility of p1 right hand hits p2 right hand
            autoAdd((int)p1r_, (int)p1l_, p2r_ + p1r_, (int)p2l_, 2, this, kill_num_ + 1);
            autoAdd((int)p1r_, (int)p1l_, p2r_ + p1l_, (int)p2l_, 2, this, kill_num_ + 1);
            if (p2r_ != p2l_) {
                // Add possibility of p1 right hand hist p2 left hand
                autoAdd((int)p1r_, (int)p1l_, (int)p2r_, p2l_ + p1r_, 2, this, kill_num_ + 1);
                autoAdd((int)p1r_, (int)p1l_, (int)p2r_, p2l_ + p1l_, 2, this, kill_num_ + 1);
            }

            // Add possibility of combining
            autoAdd(p1r_ + p1l_, 0, (int)p2r_, (int)p2l_, 2, this, kill_num_ + 1);
            if (p1r_ + 1 != p1l_ && p1l_ + 1 != p1r_) {
                // Can add both then subtract 1 without doing something I consider stupid
                autoAdd((p1r_ + p1l_) - 1, 1, (int)p2r_, (int)p2l_, 2, this, kill_num_ + 1);
            }

        }
    } else {
        if (p2r_ == p2l_) {
            // Each hand is the same and can only add to 1 or the other

            // Add possibility of p1 right hand hits p2 right hand
            if (p1r_ != 0) {
                autoAdd((int)p1r_, (int)p1l_, p2r_ + p1r_, (int)p2l_, 1, this, kill_num_ + 1);
            }

            if (p1r_ != p1l_ && p1l_ != 0) {
                // Add possibility of p1 right hand hist p2 left hand
                autoAdd((int)p1r_, (int)p1l_, (int)p2r_, p2l_ + p1r_, 1, this, kill_num_ + 1);
            }

            // Add possibility of combining
            if (p1r_ == 1) {
                autoAdd(2, 0, (int)p2r_, (int)p2l_, 1, this, kill_num_ + 1);
            } else if (p1r_ == 2) {
                autoAdd(4, 0, (int)p2r_, (int)p2l_, 1, this, kill_num_ + 1);
                autoAdd(3, 1, (int)p2r_, (int)p2l_, 1, this, kill_num_ + 1);
            } else if (p1r_ == 3) {
                autoAdd(4, 2, (int)p2r_, (int)p2l_, 1, this, kill_num_ + 1);
            } else {
                autoAdd(p1r_ + p1r_, 0, (int)p2r_, (int)p2l_, 1, this, kill_num_ + 1);
            }
        } else {
            // Add possibility of p2 hand hits p1 right hand
            if (p1r_ != 0) {
                autoAdd((int)p2r_ + p1r_, (int)p1l_, (int)p2r_, (int)p2l_, 1, this, kill_num_ + 1);
                autoAdd((int)p2l_ + p1r_, (int)p1l_, (int)p2r_, (int)p2l_, 1, this, kill_num_ + 1);
            }

            if (p1r_ != p1l_ && p1r_ != 0) {
                // Add possibility of p2 hand hist p1 left hand
                autoAdd((int)p1r_, (int)p2r_ + p1l_, (int)p2r_, (int)p2l_, 1, this, kill_num_ + 1);
                autoAdd((int)p1r_, (int)p2l_ + p1l_, (int)p2r_, (int)p2l_, 1, this, kill_num_ + 1);
            }

            // Add possibility of combining
            autoAdd(p1r_, p1l_, p2r_ + p2l_, 0, 2, this, kill_num_ + 1);
            if (p1r_ + 1 != p1l_ && p1l_ + 1 != p1r_) {
                // Can add both then subtract 1 without doing something I consider stupid
                autoAdd((int)p1r_, (int)p1l_, (p2r_ + p2l_) - 1, 1, 1, this, kill_num_ + 1);
            }
        }
    }
}
*/
//             if (p2r_ != p2l_ && p2l_ != 0) {
//                 // Add possibility of p1 right hand hist p2 left hand
//                 branches_.push_front(new Node((int)p1r_, (int)p1l_, (int)p2r_, p2l_ + p1r_, 2, this, kill_num_ + 1));
//             }

//             // Add possibility of combining
//             if (p1r_ == 1) {
//                 branches_.push_front(new Node(2, 0, (int)p2r_, (int)p2l_, 2, this));
//             } else if (p1r_ == 2) {
//                 branches_.push_front(new Node(4, 0, (int)p2r_, (int)p2l_, 2, this));
//                 branches_.push_front(new Node(3, 1, (int)p2r_, (int)p2l_, 2, this));
//             } else if (p1r_ == 3) {
//                 branches_.push_front(new Node(4, 2, (int)p2r_, (int)p2l_, 2, this));
//                 branches_.push_front(new Node(1, 0, (int)p2r_, (int)p2l_, 2, this));
//             } else {
//                 branches_.push_front(new Node(p1r_ + p1r_, 0, (int)p2r_, (int)p2l_, 2, this));
//             }
            
//         } else {
//             // Add possibility of p1 right hand hits p2 right hand
//             branches_.push_front(new Node((int)p1r_, (int)p1l_, p2r_ + p1r_, (int)p2l_, 2, this));
//             branches_.push_front(new Node((int)p1r_, (int)p1l_, p2r_ + p1l_, (int)p2l_, 2, this));
//             if (p2r_ != p2l_) {
//                 // Add possibility of p1 right hand hist p2 left hand
//                 branches_.push_front(new Node((int)p1r_, (int)p1l_, (int)p2r_, p2l_ + p1r_, 2, this));
//                 branches_.push_front(new Node((int)p1r_, (int)p1l_, (int)p2r_, p2l_ + p1l_, 2, this));
//             }

//             // Add possibility of combining
//             branches_.push_front(new Node(p1r_ + p1l_, 0, (int)p2r_, (int)p2l_, 2, this));
//             if (p1r_ + 1 != p1l_ && p1l_ + 1 != p1r_) {
//                 // Can add both then subtract 1 without doing something I consider stupid
//                 branches_.push_front(new Node((p1r_ + p1l_) - 1, 1, (int)p2r_, (int)p2l_, 2, this));
//             }
//         }
//     }

// }
