#include <memory>
#include <string>
#include <map>
#include <iostream>
#include "../xxHash/xxhash.h"

namespace Radix {
class Node;

class Tree {
private:
  std::string matchedPrefix(std::string str1, std::string str2) const;

protected:
  std::map<std::string, std::unique_ptr<Node>> childrens;

public:
  void add(std::unique_ptr<Node> node, std::string text);
  void remove(std::string text);
  Node *search(std::string text) const;
  // Returns the reference of the children map
  // This returns reference cause std::unique_ptr can't be copied
  // Warning: don't try to modify the returned data
  std::map<std::string, std::unique_ptr<Node>> *getChildrens() {
    return &this->childrens;
  }
  ulong totalChildNodes() const {
    return this->childrens.size();
  }
};

class Node : public Tree {
private:
  ulong addressInRocksDB;

public:
  Node() {}
  Node(ulong address) : addressInRocksDB(address) {}

  void addAddress(ulong address) {
    this->addressInRocksDB = address;
  }

  // Returns the address of the data in RocksDB
  ulong get() const {
    return this->addressInRocksDB;
  }
};

std::string Tree::matchedPrefix(std::string str1, std::string str2) const {
  uint i, smallerLength;
  std::string matchedPart;

  // Finding for smaller string length
  if (str1.length() < str2.length()) {
    smallerLength = str1.length();
  } else {
    smallerLength = str2.length();
  }

  for (i = 0; i < smallerLength; ++i) {
    if (str1[i] != str2[i])
      break;
    matchedPart += str1[i];
  }

  return matchedPart;
}

void Tree::add(std::unique_ptr<Node> node, std::string text) {
  // If there's is no child node
  // Store the text directly as the node
  if (this->childrens.empty()) {
    this->childrens[text] = std::move(node);
    return;
  }

  // If at least one child node exist
  // Check if any key's prefix matched with the text, if yes then create a new key with the matched prefix
  // and then make a child node with the unmatched part of the string, and also move existing node as child
  // node of the prefix matched node
  //  for (auto it = this->childrens.begin(); it != this->childrens.end(); ++it) {
  for (auto &key : this->childrens) {
    std::string matched = matchedPrefix(key.first, text);
    if (!matched.empty()) {
      auto newNode = std::make_unique<Node>();
      auto oldNode = std::move(key.second);

      // Getting remaining part of the string excluding the prefix
      newNode->add(std::move(node), text.substr(matched.length()));
      newNode->add(std::move(oldNode), key.first.substr(matched.length()));
      this->childrens.erase(key.first);
      this->childrens[matched] = std::move(newNode); // Storing matched Prefix node
      return;
    }
  }

  // If at least one child node exist but no prefix matched then simply add the node
  this->childrens[text] = std::move(node);
}
}