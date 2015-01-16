all: mailsvr mailclient
clean: 
	rm -f mailsvr mailclient mail.pb.h mail.pb.cc
protobuf: mail.proto
	protoc --cpp_out=. mail.proto 
mailclient: mailclient.cpp mailclient.h mailclientimpl.cpp  protobuf
	g++ -g -o mailclient mailclient.cpp mailclientimpl.cpp mail.pb.cc -I/usr/local/include/google/protobuf -L/usr/local/lib -lprotobuf 
mailsvr: mailsvr.cpp mailsvrimpl.cpp mailsvrimpl.h protobuf
	g++ -g -o mailsvr mailsvr.cpp mailsvrimpl.cpp mail.pb.cc -I/usr/local/include/google/protobuf -L/usr/local/lib -lprotobuf 
