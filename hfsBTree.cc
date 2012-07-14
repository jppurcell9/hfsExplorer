//hfsBTree.cc

#include <iostream>
#include <iomanip>

#include "hfsBTree.h"
#include "hfsUtils.h"

using namespace std;

HfsBTree::HfsBTree(HfsDriveInfo* driveInfo, ifstream* backingStore, HFSPlusForkData* fork) 
	: _driveInfo(driveInfo), _backingStore(backingStore), _fork(fork), _header(NULL)
{
	BTNodeDescriptor headerDescriptor;

	// read the header stuff from the first node
	this->readNodeDescriptor(0, &headerDescriptor);
	if (headerDescriptor.kind != kBTHeaderNode) {
		cout << "Reading BTree header descriptor, expected node type = " << kBTHeaderNode 
			 << " but got " << headerDescriptor.kind << endl;
		exit(1);
	}

	this->_header = new BTHeaderRec;
	this->readNodeHeaderRecord(0, this->_header);
}

HfsBTree::~HfsBTree() {
	cout << "Disposing HfsBTree" << endl;
	if (this->_header != NULL) {
		delete this->_header;
		this->_header = NULL;
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
		 << "  btree type: " << (int)this->_header->btreeType << endl
		 << hex
		 << "  key compare type: " << setw(2) << (int)this->_header->keyCompareType << endl
		 << "  attributes: " << setw(8) << this->_header->attributes << endl
		 << endl;
}

unsigned long int HfsBTree::calcOffsetFromNodeNumber(unsigned int nodeNumber) {
	unsigned short nodeSize = 0;
	if (this->_header != NULL) {
		nodeSize = this->_header->nodeSize;
	}
	unsigned int blockSize = this->_driveInfo->blockSize;
	unsigned int nodeOffset = 0;
	unsigned long int result = 0;

	// only loop through the fist 8 extents for now.  We can implement extents overflow
	// logic later.
	int i;
	for (i = 0; i < 8; i++) {
		unsigned int maxNode = 1;
		if (nodeSize > 0) {
			this->_fork->extents[i].blockCount * blockSize / nodeSize;
		}
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

streamsize HfsBTree::readNodeDescriptor(unsigned int nodeNumber, BTNodeDescriptor* descriptor) {
	this->_backingStore->seekg(this->calcOffsetFromNodeNumber(nodeNumber), ios::beg);
	this->_backingStore->read((char*)descriptor, sizeof(BTNodeDescriptor));
	byteSwapNodeDescriptor(descriptor);

	return this->_backingStore->gcount();
}

streamsize HfsBTree::readNodeHeaderRecord(unsigned int nodeNumber, BTHeaderRec* header) {
	this->_backingStore->seekg(this->calcOffsetFromNodeNumber(nodeNumber) + sizeof(BTNodeDescriptor));
	this->_backingStore->read((char*)header, sizeof(BTHeaderRec));
	byteSwapHeaderRecord(header);

	return this->_backingStore->gcount();
}
