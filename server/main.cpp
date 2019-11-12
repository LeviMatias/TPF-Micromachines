#include <string>
#include <iostream>
#include "Server.h"

// ./Server port
int main (int argc, char** argv) {
    try {
        if (argc != 2) return 1;
        Server server;
        std::string port = argv[1];
        return server.play(port);
    } catch (...) {
        std::cout << "Exception unknown occurred";
        return 1;
    }
}