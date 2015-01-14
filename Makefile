all: mailsvr mailclient
clean: 
	rm -f mailclient mail.pb.h mail.pb.cc
protobuf: mail.proto
	protoc --cpp_out=. mail.proto 
mailclient: mailclient.cpp protobuf
	g++ -g -o mailclient mailclient.cpp mail.pb.cc -I/usr/local/include/google/protobuf -L/usr/local/lib -lprotobuf 
mailsvr: mailsvr.cpp protobuf
	g++ -g -o mailsvr mailsvr.cpp mail.pb.cc -I/usr/local/include/google/protobuf -L/usr/local/lib -lprotobuf 
