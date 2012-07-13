//hfsUtils.cc

#include "hfsUtils.h"

void byteSwapVolumeHeader(HFSPlusVolumeHeader* header) {
	header->signature = bswap_16(header->signature);
	header->version = bswap_16(header->version);
	header->attributes = bswap_32(header->attributes);
	header->lastMountedVersion = bswap_32(header->lastMountedVersion);
	header->journalInfoBlock = bswap_32(header->journalInfoBlock);

	header->createDate = bswap_32(header->createDate);
	header->modifyDate = bswap_32(header->modifyDate);
	header->backupDate = bswap_32(header->backupDate);
	header->checkedDate = bswap_32(header->checkedDate);

	header->fileCount = bswap_32(header->fileCount);
	header->folderCount = bswap_32(header->folderCount);

	header->blockSize = bswap_32(header->blockSize);
	header->totalBlocks = bswap_32(header->totalBlocks);
	header->freeBlocks = bswap_32(header->freeBlocks);

	header->nextAllocation = bswap_32(header->nextAllocation);
	header->rsrcClumpSize = bswap_32(header->rsrcClumpSize);
	header->dataClumpSize = bswap_32(header->dataClumpSize);
	header->nextCatalogID = bswap_32(header->nextCatalogID);

	header->writeCount = bswap_32(header->writeCount);
	header->encodingsBitmap = bswap_64(header->encodingsBitmap);

	byteSwapForkData(&(header->allocationFile));
	byteSwapForkData(&(header->extentsFile));
	byteSwapForkData(&(header->catalogFile));
	byteSwapForkData(&(header->attributesFile));
	byteSwapForkData(&(header->startupFile));
}

void byteSwapForkData(HFSPlusForkData* fork) {
	fork->logicalSize = bswap_64(fork->logicalSize);
	fork->clumpSize = bswap_32(fork->clumpSize);
	fork->totalBlocks = bswap_32(fork->totalBlocks);

	byteSwapExtentRecord(fork->extents);
}

void byteSwapExtentRecord(HFSPlusExtentRecord extents) {
	int i;
	for (i = 0; i < 8; i++) {
		byteSwapExtentDescriptor(&(extents[i]));
	}
}

void byteSwapExtentDescriptor(HFSPlusExtentDescriptor* descriptor) {
	descriptor->startBlock = bswap_32(descriptor->startBlock);
	descriptor->blockCount = bswap_32(descriptor->blockCount);
}

void byteSwapNodeDescriptor(BTNodeDescriptor* descriptor) {
	descriptor->fLink = bswap_32(descriptor->fLink);
	descriptor->bLink = bswap_32(descriptor->bLink);
	descriptor->numRecords = bswap_16(descriptor->numRecords);
	descriptor->reserved = bswap_16(descriptor->reserved);
}

void byteSwapHeaderRecord(BTHeaderRec* headerRecord) {
	headerRecord->treeDepth = bswap_16(headerRecord->treeDepth);
	headerRecord->rootNode = bswap_32(headerRecord->rootNode);
	headerRecord->leafRecords = bswap_32(headerRecord->leafRecords);
	headerRecord->firstLeafNode = bswap_32(headerRecord->firstLeafNode);
	headerRecord->lastLeafNode = bswap_32(headerRecord->lastLeafNode);
	headerRecord->nodeSize = bswap_16(headerRecord->nodeSize);
	headerRecord->maxKeyLength = bswap_16(headerRecord->maxKeyLength);
	headerRecord->totalNodes = bswap_32(headerRecord->totalNodes);
	headerRecord->freeNodes = bswap_32(headerRecord->freeNodes);
	headerRecord->reserved1 = bswap_16(headerRecord->reserved1);
	headerRecord->clumpSize = bswap_32(headerRecord->clumpSize);
	headerRecord->attributes = bswap_32(headerRecord->attributes);
}
