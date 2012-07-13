# hfs makefile

driver : driver.cc hfsDrive.cc hfsDrive.h hfsUtils.cc hfsUtils.h
	gcc driver.cc hfsDrive.cc hfsUtils.cc -lstdc++ -o driver
