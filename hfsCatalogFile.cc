//hfsCatalogFile.cc

#include "hfsCatalogFile.h"

using namespace std;

HfsCatalogFile::HfsCatalogFile(HfsDriveInfo* driveInfo, ifstream* backingStore, HFSPlusForkData* fork) 
	: HfsBTree(driveInfo, backingStore, fork)
{
}

void HfsCatalogFile::directoryList() {
	
}