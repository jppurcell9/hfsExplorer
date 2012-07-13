// hfsDrive.cc

#include <iostream>
#include <fstream>
#include <hfs/hfs_format.h>

#include "hfsDrive.h"
#include "hfsUtils.h"

using namespace std;

HfsDrive::HfsDrive() {
	this->_backingStore = NULL;
	this->_primaryHeader = NULL;
	this->_secondaryHeader = NULL;
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
}

void HfsDrive::dumpPrimaryHeader() {
	this->dumpVolumeHeader("primary", this->_primaryHeader);
}

void HfsDrive::dumpSecondaryHeader() {
	this->dumpVolumeHeader("secondary", this->_secondaryHeader);
}

void HfsDrive::dumpVolumeHeader(const char* name, HFSPlusVolumeHeader* header) {
	cout << "Dumping volume header: " << name << endl
		 << "signature: " << header->signature << endl
		 << "version: " << header-> version << endl
		 << "attributes: " << header->attributes << endl
		 << "lastMountedVersion: " << header->lastMountedVersion << endl
		 << "journalInfoBlock: " << header->journalInfoBlock << endl
		 << endl
		 << "create date: " << header->createDate << endl
		 << "modify date: " << header->modifyDate << endl
		 << "backup date: " << header->backupDate << endl
		 << "checked date: " << header->checkedDate << endl
		 << endl;
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