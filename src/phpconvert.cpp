//============================================================================
// Name        : phpconvert
// Author      : Andrzej phpconvert <andrzej.salamon@gmail.com>
// Version     :
// Copyright   : Copyright by Andrzej phpconvert <andrzej.salamon@gmail.com>
// Description : php converter, old php 5 to new php 5 with namespaces and usages,
//               new php code is being generated after processing old one
//============================================================================

#include <iostream>
#include <string>

#include "AppParams.h"
#include "AppManager.h"
#include "SystemException.h"

#include "utils/Bench.h"

#include <boost/regex.hpp>

using namespace std;
using namespace phpconvert;

int main(int argc, char *argv[]) {
    Bench b;
    b.setStart();

    AppParams *appParams;
    AppManager *app;

    appParams = AppParams::createFromRequest(argc, argv);
    app = new AppManager();

    try {
        app->run(appParams);
    } catch (SystemException &e) {
        cout << e.what();
    } catch (exception &e) { //ooopss ;)
        cout << e.what();
    }

    delete appParams;
    delete app;

    b.setStop();
    cout << b.getDiff() << "\n";
    return 0;
}
