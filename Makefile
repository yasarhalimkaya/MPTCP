all: client server

CLIENT_SRC = Runner.cpp \
			 Connection.cpp \
			 Request.cpp \
			 FileListRequest.cpp \
			 FileSizeRequest.cpp \
			 FileDataRequest.cpp \
			 Response.cpp \
			 FileListResponse.cpp \
			 FileSizeResponse.cpp \
			 FileDataResponse.cpp \
			 Downloader.cpp \
			 DeltaTimer.cpp
			 
SERVER_SRC = EchoServer.cpp

CFLAGS = -std=c++11

client: $(CLIENT_SRC)
	g++ $(CLIENT_SRC) $(CFLAGS) -o client
	
server: $(SERVER_SRC)
	g++ $(SERVER_SRC) $(CFLAGS) -o server
	
clean:
	rm -rvf client server