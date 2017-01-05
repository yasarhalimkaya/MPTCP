#include "DeltaTimer.hpp"

DeltaTimer::DeltaTimer() {

}

void DeltaTimer::start() {
	clock_gettime(CLOCK_MONOTONIC_RAW, &startTs);
}

void DeltaTimer::stop() {
	clock_gettime(CLOCK_MONOTONIC_RAW, &endTs);
}

uint64_t DeltaTimer::getDeltaMSec() {
	return (endTs.tv_sec - startTs.tv_sec) * 1000 + (endTs.tv_nsec - startTs.tv_nsec) / 1000000;
}

uint64_t DeltaTimer::getDeltaUSec() {
	return (endTs.tv_sec - startTs.tv_sec) * 1000000 + (endTs.tv_nsec - startTs.tv_nsec) / 1000;
}

DeltaTimer::~DeltaTimer() {

}
