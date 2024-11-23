#include <iostream>
#include "headers/radix.hpp"

using std::cout, std::endl, std::string, Radix::Node, Radix::Tree;

int main() {
  // Testing creation of sample nodes
  Tree rootNode;
  Node n1;
  Node n2;
  //  Node n3;
  rootNode.add(&n1, "romance");
  rootNode.add(&n2, "romanus");
  //  rootNode.add(&n3, "romulus");
  //  cout << sizeof(rootNode) << endl;
  //  cout << rootNode.totalChildNodes() << endl;
  for (const auto &n : rootNode.childrens["rom"]->childrens) {
    cout << n.first << endl;
  }
  return 0;
}