// hfsDrive.h

#ifndef __HFS_DRIVE__
#define __HFS_DRIVE__

#include <fstream>
#include <hfs/hfs_format.h>

using namespace std;

class HfsDrive {
public:
	HfsDrive();

	void mount(const char* filename);
	void unmount();

	void dumpPrimaryHeader();
	void dumpSecondaryHeader();

private:
	static const unsigned long int startOffset = 209735680;
	static const unsigned long int sectorSize = 512;

	ifstream* _backingStore;
	ifstream::pos_type _storeSize;

	HFSPlusVolumeHeader* _primaryHeader;
	HFSPlusVolumeHeader* _secondaryHeader;

	streamsize readPrimaryVolumeHeader();
	streamsize readSecondaryVolumeHeader();
	streamsize readVolumeHeader(HFSPlusVolumeHeader* header);

	void dumpVolumeHeader(const char* name, HFSPlusVolumeHeader* header);
};

#endif