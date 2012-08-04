//hfsBTree.cc

#include <iostream>
#include <iomanip>

#include "hfsBTree.h"
#include "hfsUtils.h"

using namespace std;

HfsBTree::HfsBTree(HfsDriveInfo* driveInfo, ifstream* backingStore, HFSPlusForkData* fork) 
	: _driveInfo(driveInfo), _backingStore(backingStore), _fork(fork), _header(NULL), _headerNode(NULL)
{
	this->init();
	// BTNodeDescriptor headerDescriptor;

	// // read the header stuff from the first node
	// this->readNodeDescriptor(0, &headerDescriptor);
	// if (headerDescriptor.kind != kBTHeaderNode) {
	// 	cout << "Reading BTree header descriptor, expected node type = " << kBTHeaderNode 
	// 		 << " but got " << headerDescriptor.kind << endl;
	// 	exit(1);
	// }

	// this->_header = new BTHeaderRec;
	// this->readNodeHeaderRecord(0, this->_header);
}

HfsBTree::~HfsBTree() {
	if (this->_headerNode != NULL) {
		delete[] this->_headerNode;
		this->_headerNode = NULL;
	}
}

void HfsBTree::dumpHeaderRec() {
	cout << setfill('0');
	cout << "Dumping header record: " << endl
		 << "  tree depth: " << this->_header->treeDepth << endl
		 << "  root node: " << this->_header->rootNode << endl
		 << "  leaf records: " << this->_header->leafRecords << endl
		 << "  first leaf node: " << this->_header->firstLeafNode << endl
		 << "  last leaf node: " << this->_header->lastLeafNode << endl
		 << "  node size: " << this->_header->nodeSize << endl
		 << "  max key length: " << this->_header->maxKeyLength << endl
		 << "  total nodes: " << this->_header->totalNodes << endl
		 << "  free nodes: " << this->_header->freeNodes << endl
		 << hex
		 << "  reserved1: " << setw(4) << this->_header->reserved1 << endl
		 << dec
		 << "  clump size: " << this->_header->clumpSize << endl
		 << "  btree type: " << (int32_t)this->_header->btreeType << endl
		 << hex
		 << "  key compare type: " << setw(2) << (int32_t)this->_header->keyCompareType << endl
		 << "  attributes: " << setw(8) << this->_header->attributes << endl
		 << endl;
}

u_int64_t HfsBTree::calcHeaderNodeOffset() {
	return this->_driveInfo->startOffset
		+ this->_fork->extents[0].startBlock * this->_driveInfo->blockSize;
}

u_int64_t HfsBTree::calcOffsetFromNodeNumber(u_int32_t nodeNumber) {
	if (nodeNumber == 0) {
		return this->calcHeaderNodeOffset();
	}
	
	u_int16_t nodeSize = this->_header->nodeSize;
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

void HfsBTree::init() {
	this->_nodeSize = this->readNodeSize();
	this->_headerNode = new u_int8_t[this->_nodeSize];
	this->readNode(0, this->_headerNode);
	this->initNode(this->_headerNode);
}

u_int16_t HfsBTree::readNodeSize() {
	u_int64_t offset = 
		this->calcOffsetFromNodeNumber(0) // get the starting offset
		+ sizeof(BTNodeDescriptor)        // skip the node descriptor
		+ sizeof(u_int16_t)                  // skip the first field of the header record (treeDepth)
		+ (sizeof(u_int32_t) * 4);           // skip the next 4 fields to get to the node size field
	u_int16_t result;

	this->_backingStore->seekg(offset, ios::beg);
	this->_backingStore->read((char*)&result, sizeof(u_int16_t));
	return bswap_16(result);
}

streamsize HfsBTree::readNode(u_int32_t nodeNumber, u_int8_t* buffer) {
	this->_backingStore->seekg(this->calcOffsetFromNodeNumber(nodeNumber), ios::beg);
	this->_backingStore->read((char*)buffer, sizeof(u_int8_t) * this->_nodeSize);

	return this->_backingStore->gcount();
}

void HfsBTree::initNode(u_int8_t* buffer) {
	BTNodeDescriptor* descriptor = (BTNodeDescriptor*)buffer;
	byteSwapNodeDescriptor(descriptor);
	switch(descriptor->kind) {
		case kBTLeafNode:
			break;
		case kBTIndexNode:
			break;
		case kBTHeaderNode:
			this->initHeaderNode(buffer);
			break;
		case kBTMapNode:
			break;
	}
}

void HfsBTree::initHeaderNode(u_int8_t* buffer) {
	// byte-swap the offsets
	u_int16_t* intBuffer = (u_int16_t*)(buffer + this->_nodeSize);
	for (u_int16_t* address = intBuffer - 4; address < intBuffer; address ++) {
		*address = bswap_16(*address);
	}

	u_int16_t* headerOffset = intBuffer - 1;
	u_int16_t* userDataOffset = headerOffset - 1;
	u_int16_t* mapOffset = userDataOffset - 1;
	u_int16_t* freeOffset = mapOffset -1;

	this->_header = (BTHeaderRec*)(buffer + *headerOffset);
	byteSwapHeaderRecord(this->_header);
	// skip user data record and map record for now
}