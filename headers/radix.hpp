#include <string>
#include <map>
#include "../xxHash/xxhash.h"

namespace Radix {
class Node;

class Tree {
private:
  std::string matchedPrefix(std::string str1, std::string str2) const;

protected:
public:
  std::map<std::string, Node *> childrens;
  void add(Node *node, std::string text);
  void remove(std::string text);
  Node *search(std::string text) const;
  ulong totalChildNodes() const {
    return childrens.size();
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

void Tree::add(Node *node, std::string text) {
  // If there's is no child node
  // Store the text directly as the node
  if (this->childrens.size() == 0) {
    childrens[text] = node;
    return;
  }

  // If at least one child node exist
  // Check if any key's prefix matched with the text, if yes then create a new key with the matched prefix
  // and then make a child node with the unmatched part of the string, and also move existing node as child
  // node of the prefix matched node
  std::string matched;
  for (const auto &key : childrens) {
    matched = matchedPrefix(key.first, text);
    if (matched.size() != 0) {
      Node newNode;
      childrens[matched] = &newNode; // Storing matched Prefix node
      Node *oldNode = childrens[key.first];

      // Getting remaining part of the string excluding the prefix
      newNode.add(node, text.substr(matched.length()));
      newNode.add(oldNode, key.first.substr(matched.length()));
      childrens.erase(key.first);
      return;
    }
  }

  // If at least one child node exist but no prefix matched then simply add the node
  childrens[text] = node;
}
}