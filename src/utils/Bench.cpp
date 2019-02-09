/*
 * Bench.cpp
 *
 *  Created on: Oct 27, 2014
 *      Author: andy
 */

#include "Bench.h"

namespace phpconvert {

Bench::Bench() {
	start = ZERO_TIME;
	stop = ZERO_TIME;
}

Bench::~Bench() {
}

const double Bench::DIVIDER = 1000000.0;
const double Bench::ZERO_TIME = 0.0;

double Bench::time() {
	struct timeval tv;
	bool rs;
	rs = gettimeofday(&tv, NULL) != 0;
	if (rs)
		return ZERO_TIME;
	return tv.tv_sec + tv.tv_usec / DIVIDER;
}

void Bench::setStart() {
	start = time();
}
void Bench::setStop() {
	stop = time();
}
string Bench::getDiff() {
	char out[25];
	sprintf(out, "\n\nTotal time : %fs\n", stop - start);
	return string(out);
}

} /* namespace phpconvert */
