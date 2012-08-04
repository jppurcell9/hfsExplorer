// driver.cc

#include <iostream>
#include "hfsDrive.h"
using namespace std;

int main() {
	HfsDrive* myDrive = new HfsDrive();
	myDrive->mount("/Volumes/linuxDrive/timeMachine.img");

	//myDrive->dumpPrimaryHeader();
	//myDrive->dumpSecondaryHeader();

	myDrive->unmount();
	delete myDrive;

	return 0;
}