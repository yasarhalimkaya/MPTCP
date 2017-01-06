# README #

How to article to build and run the app

### What is this repository for? ###

* Project for graduate course Computer Networks

### How do I get set up? ###

* Build

	make

* Run

	./client <server_ip>:<port> <server_ip>:<port>

	It should also support more than two interfaces, altough not tested

* Dependencies
  
	Requires a *unix host

	Requires a gcc with a c++11 support to be installed on the host

	Most recent Linux based devices should have it by default

	It was mostly developed on MacOS Sierra 10.12.1, so it should be built successfully on Mac as well

### Followed approach ###

* Not okays

	Hardcoded timeout (100ms), it could have been adaptive

	Do not favour faster connections, each connections are waited to be done, slower ones blocks the others to proceed further

	Followed a rigorous approach while checking the recevied data's validity, retransmit whole chunk if it's not valid

* Okays

	Applied a window size per connection, with minimum and starting from 1K bytes, doubling if the latest transmission is successfull setting in half if not

	Reliable, altough it may request too many redundant chunks due to retransmissions

	Have been implemented in C++ from scratch, it should be faster than its Java counterparts

	Pretty output during transmission

### Who do I talk to? ###

* Repo owner or admin

	ysrhlmky (yasarhalimkaya@gmail.com)