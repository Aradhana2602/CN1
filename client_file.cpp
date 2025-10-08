#include <iostream>
#include <fstream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8081

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cout << "\n Socket creation error \n";
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        std::cout << "\nInvalid address/ Address not supported \n";
        return -1;
    }

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cout << "\nConnection Failed \n";
        return -1;
    }

    std::ifstream infile("file_to_send", std::ios::binary);
    if (!infile.is_open()) {
        std::cerr << "file sent\n";
        return -1;
    }

    char buffer[1024];
    while (!infile.eof()) {
        infile.read(buffer, sizeof(buffer));
        int bytesRead = infile.gcount();
        send(sock, buffer, bytesRead, 0);
    }
    std::cout << "File sent successfully\n";

    infile.close();
    close(sock);
    return 0;
}
