// hfsDrive.h

#ifndef HFS_DRIVE
#define HFS_DRIVE

#include <fstream>
#include <hfs/hfs_format.h>

#include "hfsDriveInfo.h"
#include "hfsCatalogFile.h"

class HfsDrive {
public:
	HfsDrive();

	void mount(const char* filename);
	void unmount();

	void dumpPrimaryHeader();
	void dumpSecondaryHeader();

	u_int32_t getBlockSize();

private:
	static const u_int64_t startOffset = 209735680;
	static const u_int64_t sectorSize = 512;

	std::ifstream* _backingStore;
	std::ifstream::pos_type _storeSize;

	HfsDriveInfo* _driveInfo;

	HFSPlusVolumeHeader* _primaryHeader;
	HFSPlusVolumeHeader* _secondaryHeader;

	HfsCatalogFile* _catalogFile;

	std::streamsize readPrimaryVolumeHeader();
	std::streamsize readSecondaryVolumeHeader();
	std::streamsize readVolumeHeader(HFSPlusVolumeHeader* header);

	void dumpVolumeHeader(const char* name, HFSPlusVolumeHeader* header);
	char* toASCII(u_int16_t value, char* buffer);
	char* toASCII(u_int32_t value, char* buffer);
};

#endif