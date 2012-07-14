//hfsBTree.h

#ifndef HFS_B_TREE
#define HFS_B_TREE

#include <fstream>
#include <hfs/hfs_format.h>

#include "hfsDriveInfo.h"

class HfsBTree {
public: 
	HfsBTree(HfsDriveInfo* drive, std::ifstream* backingStore, HFSPlusForkData* fork);
	virtual ~HfsBTree();

	void dumpHeaderRec();

protected:
	HfsDriveInfo* _driveInfo;
	std::ifstream* _backingStore;
	HFSPlusForkData* _fork;

	BTHeaderRec* _header;

	unsigned long int calcOffsetFromNodeNumber(unsigned int nodeNumber);

private:
	std::streamsize readNodeDescriptor(unsigned int nodeNumber, BTNodeDescriptor* descriptor);
	std::streamsize readNodeHeaderRecord(unsigned int nodeNumber, BTHeaderRec* descriptor);
};

#endif