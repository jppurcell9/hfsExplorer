//hfsBTree.h

#ifndef HFS_B_TREE
#define HFS_B_TREE

#include <fstream>
#include <sys/types.h>
#include <hfs/hfs_format.h>

#include "hfsDriveInfo.h"
#include "nodes/NodeFactory.h"

class HfsBTree {
public: 
	HfsBTree(HfsDriveInfo* drive, std::ifstream* backingStore, HFSPlusForkData* fork);
	virtual ~HfsBTree();

protected:
	NodeFactory* _nodeFactory;
};

#endif