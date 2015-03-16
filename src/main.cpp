#pragma warning(disable:4996)

#define min(a, b) a > b ? b : a
#define max(a, b) a < b ? b : a

#include <iostream>
#include <math.h>
#include <stdio.h>
//#include <tchar.h> // Windows specific

#include <sys/types.h>
#include <sys/timeb.h>
#include <list>
//#include <windows.h> // Windows specific

#include "Function.h"
#include "LunettesVideo.h"
#include "Matrix.h"

#include "uEye.h"
//#include "uEye_tools.h"
#include "ueye_deprecated.h"

//#include <conio.h> // Windows specific



int main(void) {

	LunettesVideo* glasses = new LunettesVideo();
	
	if(!glasses->isInitialized()) {
		cin.get();
		return 1;
	}

	int resX, resY;

	glasses->run();
	cout << "end!" << endl;

	delete glasses;

	return 0;
}



