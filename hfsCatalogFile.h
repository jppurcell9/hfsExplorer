//hfsCatalogFile.h

#ifndef HFS_CATALOG_FILE
#define HFS_CATALOG_FILE

#include <fstream>
#include <hfs/hfs_format.h>

#include "hfsBTree.h"
#include "hfsDriveInfo.h"

class HfsCatalogFile : public HfsBTree {
public: 
	HfsCatalogFile(HfsDriveInfo* driveInfo, std::ifstream* backingStore, HFSPlusForkData* fork);

private:
	std::ifstream* _backingStore;
	unsigned long int _offset;
	HFSPlusForkData* _fork;

	BTHeaderRec* _header;

	unsigned long int calcOffsetFromNodeNumber(unsigned int nodeNumber);
};

#endif