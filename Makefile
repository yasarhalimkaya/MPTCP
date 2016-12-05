all: client server

client:
	g++ Runner.cpp Connection.cpp -o client
	
server:
	g++ DummyServer.cpp -o server
	
clean:
	rm -rvf client server