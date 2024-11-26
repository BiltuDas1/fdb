#include <iostream>
#include <memory>
#include "headers/radix.hpp"

using std::cout, std::endl, std::string, Radix::Node, Radix::Tree, std::unique_ptr, std::make_unique;

int main() {
  // Testing creation of sample nodes
  Tree rootNode;
  unique_ptr<Node> n1 = make_unique<Node>(10);
  unique_ptr<Node> n2 = make_unique<Node>(20);
  unique_ptr<Node> n3 = make_unique<Node>(30);
  unique_ptr<Node> n4 = make_unique<Node>(40);
  unique_ptr<Node> n5 = make_unique<Node>(50);
  unique_ptr<Node> n6 = make_unique<Node>(60);
  unique_ptr<Node> n7 = make_unique<Node>(70);
  rootNode.add(std::move(n1), "romane");
  rootNode.add(std::move(n2), "romanus");
  rootNode.add(std::move(n3), "romulus");
  rootNode.add(std::move(n4), "rubens");
  // rootNode.add(std::move(n5), "ruber");
  //  rootNode.add(std::move(n6), "rubicon");
  //  rootNode.add(std::move(n7), "rubicundus");

  cout << rootNode.totalChildNodes() << endl;
  for (const auto &n : *rootNode.getChildrens()) {
    cout << "Node: " << n.first << ", Childrens: " << n.second->totalChildNodes() << endl;
    for (const auto &p : *n.second->getChildrens()) {
      cout << p.first << endl;
    }
    cout << endl;
  }
  return 0;
}