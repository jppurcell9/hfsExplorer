// hfsDrive.h

#ifndef HFS_DRIVE
#define HFS_DRIVE

#include <fstream>
#include <hfs/hfs_format.h>

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

	std::ifstream* _backingStore;
	std::ifstream::pos_type _storeSize;

	HFSPlusVolumeHeader* _primaryHeader;
	HFSPlusVolumeHeader* _secondaryHeader;

	std::streamsize readPrimaryVolumeHeader();
	std::streamsize readSecondaryVolumeHeader();
	std::streamsize readVolumeHeader(HFSPlusVolumeHeader* header);

	void dumpVolumeHeader(const char* name, HFSPlusVolumeHeader* header);
	char* toASCII(unsigned short int value, char* buffer);
	char* toASCII(unsigned int value, char* buffer);
};

#endif