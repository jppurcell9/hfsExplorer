//hfsUtils.h

#ifndef __HFS_UTILS__
#define __HFS_UTILS__

#include <hfs/hfs_format.h>

static inline unsigned short bswap_16(unsigned short x) {
  return (x>>8) | (x<<8);
}

static inline unsigned int bswap_32(unsigned int x) {
  return (bswap_16(x&0xffff)<<16) | (bswap_16(x>>16));
}

static inline unsigned long bswap_64(unsigned long int x) {
	return (((unsigned long int)bswap_32(x&0xffffffff))<<32) | (bswap_32(x>>32));
}

void byteSwapVolumeHeader(HFSPlusVolumeHeader* header);
void byteSwapForkData(HFSPlusForkData* fork);
void byteSwapExtentRecord(HFSPlusExtentRecord extents);
void byteSwapExtentDescriptor(HFSPlusExtentDescriptor* descriptor);

void byteSwapNodeDescriptor(BTNodeDescriptor* descriptor);
void byteSwapHeaderRecord(BTHeaderRec* headerRecord);


#endif