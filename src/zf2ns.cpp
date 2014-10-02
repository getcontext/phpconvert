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

//#include "utils/Regexer.h"
//#include "utils/DirectoryReader.h"
//#include "plugin/ZendParser.h"

#include <boost/regex.hpp>

using namespace std;
using namespace Salamon;

int main(int argc, char* argv[]) {
	AppParams p;
	AppParams* appParams = p.createFromRequest(argc, argv);
	AppManager* app = new AppManager();

	try {
		app->run(appParams);
	} catch (SystemException& e) {
		cout << e.what();
	} catch (exception& e) { //ooopss ;)
		cout << e.what();
	}


//	DirectoryReader r;
//	string content = r.readTextFile("D:\\workspace\\Zend\\Acl.php");
//	string content = r.readTextFile("D:\\workspace\\Zend\\Auth.php");
//	string content = r.readTextFile("D:\\workspace\\Zend\\Exception.php");
//
//	Regexer reg;
//	reg.test(content, ZendParser::RGX_MAIN_TYPE);


	return 0;
}
