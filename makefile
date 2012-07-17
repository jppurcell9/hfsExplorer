# hfs makefile

driver: driver.cc hfsDrive.o hfsUtils.o hfsBTree.o hfsCatalogFile.o 
	gcc driver.cc hfsDrive.o hfsUtils.o hfsBTree.o hfsCatalogFile.o -lstdc++ -o driver

hfsUtils.o: hfsUtils.cc hfsUtils.h
	gcc -c hfsUtils.cc -o hfsUtils.o

hfsBTree.o: hfsBTree.cc hfsBTree.h hfsDriveInfo.h
	gcc -c hfsBTree.cc -o hfsBTree.o

hfsCatalogFile.o: hfsCatalogFile.cc hfsCatalogFile.h
	gcc -c hfsCatalogFile.cc -o hfsCatalogFile.o

hfsDrive.o: hfsDrive.cc
	gcc -c hfsDrive.cc -o hfsDrive.o
