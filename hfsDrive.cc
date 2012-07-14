// hfsDrive.cc

#include <iostream>
#include <iomanip>
#include <time.h>

#include "hfsDrive.h"
#include "hfsUtils.h"

using namespace std;

HfsDrive::HfsDrive() 
	: _backingStore(NULL), _driveInfo(NULL), _primaryHeader(NULL), _secondaryHeader(NULL), 
	  _catalogFile(NULL)
{
}

void HfsDrive::mount(const char* filename) {
	if (this->_backingStore != NULL) {
		cout << "HfsDrive already mounted.  Use umount before calling this method again." << endl;
		return;
	}

	// use ios::ate to position file pointer at the end of the file
	this->_backingStore = new ifstream(filename, ios::in|ios::binary|ios::ate);
	this->_storeSize = this->_backingStore->tellg();

	cout << "Total file size: " << this->_storeSize << endl;

	this->readPrimaryVolumeHeader();
	this->readSecondaryVolumeHeader();

	this->_driveInfo = new HfsDriveInfo;
	this->_driveInfo->startOffset = HfsDrive::startOffset;
	this->_driveInfo->blockSize = this->_primaryHeader->blockSize;

	this->_catalogFile = new HfsCatalogFile(this->_driveInfo, this->_backingStore, &(this->_primaryHeader->catalogFile));
	cout << "Catalog file header: " << endl;
	this->_catalogFile->dumpHeaderRec();
}

void HfsDrive::unmount() {
	if (this->_backingStore == NULL) {
		cout << "HfsDrive not mounted.  Use mount before calling this method again." << endl;
		return;
	}

	this->_backingStore->close();
	delete this->_backingStore;
	this->_backingStore = NULL;

	if (this->_primaryHeader != NULL) {
		delete this->_primaryHeader;
		this->_primaryHeader = NULL;
	}

	if (this->_secondaryHeader != NULL) {
		delete this->_secondaryHeader;
		this->_secondaryHeader = NULL;
	}

	if (this->_catalogFile != NULL) {
		delete this->_catalogFile;
		this->_catalogFile = NULL;
	}
}

void HfsDrive::dumpPrimaryHeader() {
	this->dumpVolumeHeader("primary", this->_primaryHeader);
}

void HfsDrive::dumpSecondaryHeader() {
	this->dumpVolumeHeader("secondary", this->_secondaryHeader);
}

void HfsDrive::dumpVolumeHeader(const char* name, HFSPlusVolumeHeader* header) {
	char signatureBuffer[] = "  ";
	char lmvBuffer[] = "    ";

	time_t aTime;

	cout << setfill('0');
	cout << "Dumping volume header: " << name << endl
		 << hex
		 << "  signature: " << setw(4) << header->signature 
		 << " (" << this->toASCII(header->signature, signatureBuffer) << ")" << endl
		 << "  version: " << setw(4) << header-> version << endl
		 << "  attributes: " << setw(8) << header->attributes << endl
		 << "  last mounted version: " << header->lastMountedVersion 
		 << " (" << this->toASCII(header->lastMountedVersion, lmvBuffer) << ")" << endl
		 << dec
		 << "  journal info block: " << header->journalInfoBlock << endl
		 << endl;

	aTime = to_bsd_time(header->createDate);
	cout << "  create date: " << ctime(&aTime);
	aTime = to_bsd_time(header->modifyDate);
	cout << "  modify date: " << ctime(&aTime);
	aTime = to_bsd_time(header->backupDate);
	cout << "  backup date: " << ctime(&aTime);
	aTime = to_bsd_time(header->checkedDate);
	cout << "  checked date: " << ctime(&aTime) 
		 << endl;

	cout << "  file count: " << header->fileCount << endl
		 << "  folder count: " << header->folderCount << endl
		 << endl
		 << "  block size: " << header->blockSize << endl
		 << "  total blocks: " << header->totalBlocks << endl
		 << "  free blocks: " << header->freeBlocks << endl
		 << endl
		 << "  next allocation: " << header->nextAllocation << endl
		 << "  resource clump size: " << header->rsrcClumpSize << endl
		 << "  data clump size: " << header->dataClumpSize << endl
		 << "  next catalog id: " << header->nextCatalogID << endl
		 << endl
		 << "  write count: " << header->writeCount << endl
		 << hex
		 << "  encodings bitmap: " << setw(8) << header->encodingsBitmap << endl
		 << endl;
}

char* HfsDrive::toASCII(unsigned short value, char* buffer) {
	buffer[1] = (char)(value & 0x00FF);
	buffer[0] = (char)(value >> 8);
	return buffer;
}

char* HfsDrive::toASCII(unsigned int value, char* buffer) {
	this->toASCII((unsigned short)(value & 0x0000FFFF), buffer + (2 * sizeof(char)));
	this->toASCII((unsigned short)(value >> 16), buffer);
	return buffer;
}

streamsize HfsDrive::readPrimaryVolumeHeader() {
	if (this->_primaryHeader != NULL) {
		delete this->_primaryHeader;
	}

	this->_primaryHeader = new HFSPlusVolumeHeader();
	this->_backingStore->seekg(HfsDrive::startOffset + HfsDrive::sectorSize * 2, ios::beg);
	return this->readVolumeHeader(this->_primaryHeader);
}

streamsize HfsDrive::readSecondaryVolumeHeader() {
	if (this->_secondaryHeader != NULL) {
		delete this->_secondaryHeader;
	}

	this->_secondaryHeader = new HFSPlusVolumeHeader();
	this->_backingStore->seekg(-HfsDrive::sectorSize * 2, ios::end);
	return this->readVolumeHeader(this->_secondaryHeader);
}

streamsize HfsDrive::readVolumeHeader(HFSPlusVolumeHeader* header) {
	this->_backingStore->read((char*)header, sizeof(HFSPlusVolumeHeader));
	byteSwapVolumeHeader(header);

	return this->_backingStore->gcount();
}

unsigned int HfsDrive::getBlockSize() {
	if (this->_primaryHeader == NULL) {
		if (this->_secondaryHeader == NULL) {
			return 0;
		}
		else {
			return this->_secondaryHeader->blockSize;
		}
	} else {
		return this->_primaryHeader->blockSize;
	}
}