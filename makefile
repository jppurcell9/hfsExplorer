# hfs makefile

driver : driver.cc hfsDrive.cc hfsDrive.h hfsUtils.cc hfsUtils.h hfsDriveInfo.h hfsBTree.cc hfsBTree.h hfsCatalogFile.cc hfsCatalogFile.h
	gcc driver.cc hfsDrive.cc hfsUtils.cc hfsBTree.cc hfsCatalogFile.cc -lstdc++ -o driver
