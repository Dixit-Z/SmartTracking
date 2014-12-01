#include "udp.hpp"
#include <cstring>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>


UDP::UDP(string host, uint16_t port): port(port), ip_addr(host) {
    cout << "host : " << host << " ; port " << port << endl;
    this->sock = assign_local_socket();
    assign_sockaddr(host, port, (struct sockaddr_in*) this->addrsender);
    
    if(connect(this->sock, this->addrsender, sizeof(addrsender)) == -1) {
        perror("connect");
        exit(errno);
    }
}

void UDP::send(uint8_t *data, int length) {
    cout << "send (" << length << ") : ";
    printf("%.*s", length, data);
    cout << endl;
    //TODO: Gestion de l'envoi des données
    thread thsend(&sendto, this->sock, data, sizeof(data), 0, this->addrsender, sizeof(this->addrsender));
}

void UDP::send(string data) {
    this->send((uint8_t*)data.c_str(), (int)data.length());
}

void UDP::receive(receiveCallback func) {
    //TODO: Gestion de la reception des données
    // La fonction doit etre non bloquante
    // > Creation d'un thread (si non créé)
    // >> qui va read le socket pour attendre une reponse
    // >> puis appeler func lors de la reception d'une donnée
    // Boucle sur le thread tant que le desctructeur de la classe ne lui informe pas d'arreter
    char buffer[1024];
    thread thrcv(&recv, this->sock, buffer, sizeof(buffer), 0); 
    func((uint8_t *) buffer, strlen(buffer));
    char* rr = "hello";
    func((uint8_t*)rr , (int)strlen(rr));
};

int UDP::assign_local_socket() {
    uint16_t myport = 0;
    struct sockaddr* addr;
    assign_sockaddr(string("me"), myport, (struct sockaddr_in*) addr);
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    
    if(sock == -1) {
        perror("sock");
        exit(errno);
    }

    if(bind(sock, addr, sizeof(addr)) == -1) {
        perror("bind");
        exit(errno);
    }
    
    return sock;
}

void UDP::assign_sockaddr(string host, uint16_t& port, struct sockaddr_in* addr) {
    if(host.compare("me")) {
        addr->sin_addr.s_addr = htonl(INADDR_ANY);
    }
    else {
        if(inet_aton((const char*)host.c_str(), &addr->sin_addr) == 0) {
            perror("inet_aton");
            exit(-1);
        }
    }
    memset((char *) addr, 0, sizeof(addr));
    addr->sin_family = AF_INET;
    addr->sin_port = htons(port);
}


