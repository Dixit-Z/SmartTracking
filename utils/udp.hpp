#ifndef UDP_H
#define UDP_H

#include <iostream>
#include <cstdlib>

using namespace std;

typedef void(*receiveCallback)(uint8_t*, int);

class UDP {
public:
    UDP(string host, uint16_t port);
    ~UDP() {};

    void send(uint8_t* data, int length);
    void send(string data);

    void receive(receiveCallback func);
};

#endif