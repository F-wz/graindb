//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/execution/index/art/node48.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once
#include "graindb/execution/index/art/node.hpp"

namespace graindb {

class Node48 : public Node {
public:
	Node48(ART &art, size_t compressionLength);

	uint8_t childIndex[256];
	unique_ptr<Node> child[48];

public:
	//! Get position of a byte, returns -1 if not exists
	idx_t GetChildPos(uint8_t k) override;
	//! Get the position of the first child that is greater or equal to the specific byte, or INVALID_INDEX if there are
	//! no children matching the criteria
	idx_t GetChildGreaterEqual(uint8_t k, bool &equal) override;
	//! Get the next position in the node, or INVALID_INDEX if there is no next position
	idx_t GetNextPos(idx_t pos) override;
	//! Get Node48 Child
	unique_ptr<Node> *GetChild(idx_t pos) override;

	idx_t GetMin() override;

	//! Insert node in Node48
	static void insert(ART &art, unique_ptr<Node> &node, uint8_t keyByte, unique_ptr<Node> &child);

	//! Shrink to node 16
	static void erase(ART &art, unique_ptr<Node> &node, int pos);
};
} // namespace graindb
