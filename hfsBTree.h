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

	//void dumpHeaderRec();

protected:
	NodeFactory* _nodeFactory;
	// HfsDriveInfo* _driveInfo;
	// std::ifstream* _backingStore;
	// HFSPlusForkData* _fork;

	// u_int16_t _nodeSize;
	// u_int8_t* _headerNode;

	// BTHeaderRec* _header;

	// u_int64_t calcHeaderNodeOffset();
	// u_int64_t calcOffsetFromNodeNumber(u_int32_t nodeNumber);

	// void* search(BTreeKey* key, size_t dataLength);

private:
	// void init();
	
	// u_int16_t readNodeSize();
	// std::streamsize readNode(u_int32_t nodeNumber, u_int8_t* buffer);

	// void initNode(u_int8_t* buffer);
	// void initHeaderNode(u_int8_t* buffer);
};

#endif