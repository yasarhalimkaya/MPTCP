all: client server

CLIENT_SRC = Runner.cpp \
			 Connection.cpp
			 
SERVER_SRC = DummyServer.cpp

client: $(CLIENT_SRC)
	g++ $(CLIENT_SRC) -o client
	
server: $(SERVER_SRC)
	g++ $(SERVER_SRC) -o server
	
clean:
	rm -rvf client server