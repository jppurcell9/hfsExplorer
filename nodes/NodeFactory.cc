//hfsBTree.cc

#include <iostream>

#include "NodeFactory.h"
#include "HeaderNode.h"
#include "IndexNode.h"
#include "../hfsUtils.h"

using namespace std;

NodeFactory::NodeFactory(HfsDriveInfo* driveInfo, ifstream* backingStore, HFSPlusForkData* fork) 
	: _driveInfo(driveInfo), _backingStore(backingStore), _fork(fork)
{
	this->_nodeSize = this->readNodeSize();
}

Node* NodeFactory::getNode(u_int32_t nodeNumber) {
	u_int8_t* buffer = new u_int8_t[this->_nodeSize];

	this->_backingStore->seekg(this->calcOffsetFromNodeNumber(nodeNumber), ios::beg);
	this->_backingStore->read((char*)buffer, sizeof(u_int8_t) * this->_nodeSize);

	Node* result = this->createNode(buffer);
	if (result == NULL) {
		delete[] buffer;
	}
	return result;
}

u_int16_t NodeFactory::readNodeSize() {
	u_int64_t offset = 
		this->calcHeaderNodeOffset()   // get the starting offset
		+ sizeof(BTNodeDescriptor)     // skip the node descriptor
		+ sizeof(u_int16_t)            // skip the first field of the header record (treeDepth)
		+ (sizeof(u_int32_t) * 4);     // skip the next 4 fields to get to the node size field
	u_int16_t result;

	this->_backingStore->seekg(offset, ios::beg);
	this->_backingStore->read((char*)&result, sizeof(u_int16_t));
	return bswap_16(result);
}

u_int64_t NodeFactory::calcHeaderNodeOffset() {
	return this->_driveInfo->startOffset
		+ this->_fork->extents[0].startBlock * this->_driveInfo->blockSize;
}

u_int64_t NodeFactory::calcOffsetFromNodeNumber(u_int32_t nodeNumber) {
	if (nodeNumber == 0) {
		return this->calcHeaderNodeOffset();
	}
	
	u_int16_t nodeSize = this->_nodeSize;
	u_int32_t blockSize = this->_driveInfo->blockSize;
	u_int32_t nodeOffset = 0;
	u_int64_t result = 0;

	// only loop through the fist 8 extents for now.  We can implement extents overflow
	// logic later.
	int32_t i;
	for (i = 0; i < 8; i++) {
		u_int32_t maxNode = this->_fork->extents[i].blockCount * blockSize / nodeSize;
		if (nodeNumber < nodeOffset + maxNode) {
			// found the right extent, figure out the offset;
			result = this->_driveInfo->startOffset
				+ this->_fork->extents[i].startBlock * blockSize 
				+ (nodeNumber - nodeOffset) * nodeSize;
			break;
		}
		nodeOffset += maxNode;
	}

	if (result == 0) {
		cout << "We didn't find the node we were looking for." << endl;
		exit(1);
	}

	return result;
}

Node* NodeFactory::createNode(u_int8_t* buffer) {
	BTNodeDescriptor* descriptor = (BTNodeDescriptor*)buffer;
	byteSwapNodeDescriptor(descriptor);

	switch(descriptor->kind) {
		case kBTLeafNode:
			//this->initLeafNode(buffer);
			break;
		case kBTIndexNode:
			return new IndexNode(buffer, this->_nodeSize);
		case kBTHeaderNode:
			return new HeaderNode(buffer, this->_nodeSize);
		case kBTMapNode:
			break;
		default:
			cout << "Unrecognized node type: " << descriptor->kind << endl;
			break;
	}

	return NULL;
}