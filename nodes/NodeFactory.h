//NodeFactory.h

#ifndef NODE_FACTORY_H
#define NODE_FACTORY_H

#include <fstream>
#include <sys/types.h>
#include <hfs/hfs_format.h>

#include "../hfsDriveInfo.h"
#include "Node.h"

class NodeFactory {
public: 
	NodeFactory(HfsDriveInfo* drive, std::ifstream* backingStore, HFSPlusForkData* fork);

	Node* getNode(u_int32_t nodeNumber);

private:
	HfsDriveInfo* _driveInfo;
	std::ifstream* _backingStore;
	HFSPlusForkData* _fork;

	u_int16_t _nodeSize;

	u_int16_t readNodeSize();
	u_int64_t calcHeaderNodeOffset();
	u_int64_t calcOffsetFromNodeNumber(u_int32_t nodeNumber);

	std::streamsize readNode(u_int32_t nodeNumber, u_int8_t* buffer);
	Node* createNode(u_int8_t* buffer);
};

#endif