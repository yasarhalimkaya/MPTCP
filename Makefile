all: client server

CLIENT_SRC = Runner.cpp \
			 Connection.cpp \
			 Request.cpp \
			 FileListRequest.cpp \
			 FileSizeRequest.cpp
			 
SERVER_SRC = EchoServer.cpp

client: $(CLIENT_SRC)
	g++ $(CLIENT_SRC) -o client
	
server: $(SERVER_SRC)
	g++ $(SERVER_SRC) -o server
	
clean:
	rm -rvf client server