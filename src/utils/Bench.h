/*
 * Bench.h
 *
 *  Created on: Oct 27, 2014
 *      Author: andy
 */

#ifndef BENCH_H_
#define BENCH_H_

#include <iostream>
#include <string>
#include <stdio.h>
#include <sys/time.h>

using namespace std;

namespace Salamon {
class Bench {
public:
	Bench();
	virtual ~Bench();
	void setStart();
	void setStop();
	string getDiff();
	double time();
protected:
	double start;
	double stop;


private:
	static const double DIVIDER;
	static const double ZERO_TIME;
};

} /* namespace Salamon */

#endif /* BENCH_H_ */
