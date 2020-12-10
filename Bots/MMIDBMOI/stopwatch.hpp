#ifndef STOPWATCH_HPP
#define STOPWATCH_HPP

#include <limits.h>
#include <iostream>
#include <assert.h>
#include <algorithm>
#include <string>
#include <stdio.h>
#include <chrono>

using namespace std;
using namespace chrono;

#define Now() chrono::high_resolution_clock::now()

static struct Stopwatch {
	chrono::high_resolution_clock::time_point c_time, c_timeout;
	void setTimeout(int us) { c_timeout = c_time + chrono::microseconds(us); }
	void Start() { c_time = Now();}
	inline bool Timeout() { return Now() > c_timeout; }
	long long EllapsedMicroseconds() { return chrono::duration_cast<chrono::microseconds>(Now() - c_time).count(); }
	long long EllapsedMilliseconds() { return chrono::duration_cast<chrono::milliseconds>(Now() - c_time).count(); }
} stopwatch;

#endif