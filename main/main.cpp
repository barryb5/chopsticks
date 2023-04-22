#include <iostream>
#include <string>
#include <sstream>
#include "include/node.h"

int main() {
    Node node = *(new Node());
    std::cout << node.toString() << std::endl;
    return 0;
}