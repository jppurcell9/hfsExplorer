//hfsDriveInfo.h

#ifndef HFS_DRIVE_INFO
#define HFS_DRIVE_INFO

struct HfsDriveInfo {
	u_int64_t startOffset;
	u_int32_t blockSize;
};

#endif