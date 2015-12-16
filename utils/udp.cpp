#include "udp.hpp"
#include <cstring>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

UDP::UDP(string host, uint16_t port): port(port), ip_addr(host) {
    cout << "host : " << host << " ; port " << port << endl;
    if((this->sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("socket");
		exit(errno);
	}
     
    if(this->sock == -1) {
        perror("sock");
        exit(errno);
    }
    if((this->addr = (struct sockaddr_in*) calloc(1, sizeof(struct sockaddr_in))) == NULL) {
        perror("malloc");
        exit(errno);
    }

    this->addr->sin_family = AF_INET;
    this->addr->sin_port = htons(this->port);                                                                
    
    if(inet_pton(AF_INET, host.c_str(), &this->addr->sin_addr) == 0) {
        perror("inet_pton");
        exit(-1);
    }
} 
void UDP::send(uint8_t *data, int length) {
    //cout << "send (" << length << ") : ";
    //printf("%.*s", length, data);
    //cout << endl;
    
    if(sendto(this->sock, data, length, 0, (struct sockaddr*) this->addr, sizeof(*this->addr)) == -1) {
        perror("send");
        exit(errno);
    }
}

void UDP::send(string data) {
    this->send((uint8_t*)data.c_str(), (int)data.length());
}

void UDP::receive(receiveCallback func) {
    char buffer[1024];
    socklen_t length = sizeof(this->addr);
    
    if(recvfrom(this->sock, buffer, sizeof(buffer), 0, (struct sockaddr*) this->addr, &length) == -1) {
        perror("send");
        exit(errno);
    } 
    else {
        func((uint8_t *) buffer, sizeof(buffer));
    }
};

UDP::~UDP() {
    delete this->addr;
    close(this->sock);
}

