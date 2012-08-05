# hfs makefile

driver: driver.cc hfsDrive.o hfsUtils.o hfsBTree.o hfsCatalogFile.o NodeFactory.o Node.o HeaderNode.o IndexNode.o
	gcc driver.cc hfsDrive.o hfsUtils.o hfsBTree.o hfsCatalogFile.o NodeFactory.o Node.o HeaderNode.o IndexNode.o -lstdc++ -o driver

hfsUtils.o: hfsUtils.cc hfsUtils.h
	gcc -c hfsUtils.cc -o hfsUtils.o

hfsBTree.o: hfsBTree.cc hfsBTree.h hfsDriveInfo.h
	gcc -c hfsBTree.cc -o hfsBTree.o

hfsCatalogFile.o: hfsCatalogFile.cc hfsCatalogFile.h
	gcc -c hfsCatalogFile.cc -o hfsCatalogFile.o

hfsDrive.o: hfsDrive.cc
	gcc -c hfsDrive.cc -o hfsDrive.o

NodeFactory.o: nodes/NodeFactory.cc nodes/NodeFactory.h
	gcc -c nodes/NodeFactory.cc -o NodeFactory.o

Node.o: nodes/Node.cc nodes/Node.h
	gcc -c nodes/Node.cc -o Node.o

HeaderNode.o: nodes/HeaderNode.cc nodes/HeaderNode.h
	gcc -c nodes/HeaderNode.cc -o HeaderNode.o

IndexNode.o: nodes/IndexNode.cc nodes/IndexNode.h
	gcc -c nodes/IndexNode.cc -o IndexNode.o