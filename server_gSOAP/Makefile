GSOAP_ROOT = ../gsoap-2.8/gsoap 
CC = gcc -g -DWITH_NONAMESPACES  -DSOAP_DEBUG -DDEBUG
INCLUDE = -I$(GSOAP_ROOT)
SERVER_OBJS = soapC.o stdsoap2.o soapClient.o  onvif_server_interface.o soapServer.o duration.o onvif_server.o
 
 
all: server 
server: $(SERVER_OBJS)
	$(CC) $(INCLUDE) -o deviceserver -lpthread $(SERVER_OBJS)
 
clean: 
	rm -f *.o deviceprobe  deviceserver onvif
