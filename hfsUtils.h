//hfsUtils.h

#ifndef HFS_UTILS
#define HFS_UTILS

#include <time.h>
#include <hfs/hfs_format.h>

/*
 *	This is the straight GMT conversion constant:
 *	00:00:00 January 1, 1970 - 00:00:00 January 1, 1904
 *	(3600 * 24 * ((365 * (1970 - 1904)) + (((1970 - 1904) / 4) + 1)))
 */
#define MAC_GMT_FACTOR		2082844800UL

time_t to_bsd_time(u_int32_t hfs_time);
u_int32_t to_hfs_time(time_t bsd_time);

static inline u_int16_t bswap_16(u_int16_t x) {
  return (x>>8) | (x<<8);
}

static inline u_int32_t bswap_32(u_int32_t x) {
  return (bswap_16(x&0xffff)<<16) | (bswap_16(x>>16));
}

static inline u_int64_t bswap_64(u_int64_t x) {
	return (((u_int64_t)bswap_32(x&0xffffffff))<<32) | (bswap_32(x>>32));
}

void byteSwapVolumeHeader(HFSPlusVolumeHeader* header);
void byteSwapForkData(HFSPlusForkData* fork);
void byteSwapExtentRecord(HFSPlusExtentRecord extents);
void byteSwapExtentDescriptor(HFSPlusExtentDescriptor* descriptor);

void byteSwapNodeDescriptor(BTNodeDescriptor* descriptor);
void byteSwapHeaderRecord(BTHeaderRec* headerRecord);

#endif