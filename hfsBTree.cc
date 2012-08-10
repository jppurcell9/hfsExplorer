//hfsBTree.cc

#include <iostream>
#include <iomanip>

#include "hfsBTree.h"
#include "hfsUtils.h"

using namespace std;

HfsBTree::HfsBTree(HfsDriveInfo* driveInfo, ifstream* backingStore, HFSPlusForkData* fork) 
{
	this->_nodeFactory = new NodeFactory(driveInfo, backingStore, fork);
}

HfsBTree::~HfsBTree() {
	if (this->_nodeFactory != NULL) {
		delete this->_nodeFactory;
		this->_nodeFactory = NULL;
	}
}
