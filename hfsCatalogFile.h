//hfsCatalogFile.h

#ifndef HFS_CATALOG_FILE
#define HFS_CATALOG_FILE

#include <fstream>
#include <hfs/hfs_format.h>

class HfsCatalogFile {
public: 
	HfsCatalogFile(std::ifstream* backingStore, unsigned long int offset, HFSPlusForkData* fork);

private:
	std::ifstream* _backingStore;
	unsigned long int _offset;
	HFSPlusForkData* _fork;
};

#endif