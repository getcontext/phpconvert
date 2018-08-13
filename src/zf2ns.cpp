//============================================================================
// Name        : zf2ns.cpp
// Author      : Andrzej Salamon <andrzej.salamon@gmail.com>
// Version     :
// Copyright   : Copyright by Andrzej Salamon <andrzej.salamon@gmail.com>
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <string>

#include "AppParams.h"
#include "AppManager.h"
#include "SystemException.h"

#include "utils/Bench.h"

#include <boost/regex.hpp>

using namespace std;
using namespace Salamon;

int main(int argc, char* argv[]) {
	Bench b;
	b.setStart();
		
	AppParams* appParams;
	AppManager* app;
	
	appParams = (AppParams()).createFromRequest(argc, argv);
	app = new AppManager();

	try {
		app->run(appParams);
	} catch (SystemException& e) {
		cout << e.what();
	} catch (exception& e) { //ooopss ;)
		cout << e.what();
	}
	
	delete appParams;
	delete app;

	b.setStop();
	cout << b.getDiff() << "\n";
	return 0;
}
