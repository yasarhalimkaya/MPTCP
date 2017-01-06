#ifndef DELTATIMER_HPP_
#define DELTATIMER_HPP_

#include <cstdint>
#include <ctime>

class DeltaTimer {
public:
	DeltaTimer();

	void start();
	void stop();

	uint64_t getDeltaMSec();
	uint64_t getDeltaUSec();

	virtual ~DeltaTimer();

private:
	struct timespec startTs, endTs;
};

#endif /* DELTATIMER_HPP_ */
