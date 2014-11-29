#include <cstdlib>
#include <iostream>
#include "utils/udp.hpp"
#include "drone/AtCmd.hpp"
#include "drone/AtCmd.hpp"

using namespace std;

void testCallback(uint8_t* data, int length);

UDP udp("192.168.1.1", 5555);

int main() {
    AtCmd::sendLanding();
    AtCmd::sendLanding();
    udp.receive(testCallback);
//    udp.send("test");
    return 0;
}

void testCallback(uint8_t* data, int length){
    cout << "receive (" << length << ") : ";
    printf("%.*s", length, data);
    cout << endl;
    udp.send(data, length);
}