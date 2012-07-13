//hfsCatalogFile.cc

#include <fstream>
#include <hfs/hfs_format.h>

#include "hfsCatalogFile.h"

using namespace std;

HfsCatalogFile::HfsCatalogFile(ifstream* backingStore, unsigned long int offset, HFSPlusForkData* fork) {
	this->_backingStore = backingStore;
	this->_offset = offset;
	this->_fork = fork;
}