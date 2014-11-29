#include "udp.hpp"
#include <cstring>

UDP::UDP(string host, uint16_t port) {
    cout << "host : " << host << " ; port " << port << endl;
}

void UDP::send(uint8_t *data, int length) {
    cout << "send (" << length << ") : ";
    printf("%.*s", length, data);
    cout << endl;
    //TODO: Gestion de l'envoi des données
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
    char* rr = "hello";
    func((uint8_t*)rr , (int)strlen(rr));
};
