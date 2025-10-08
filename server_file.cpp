#include <iostream>
#include <fstream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 8081

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        return -1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        return -1;
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        return -1;
    }

    std::cout << "Waiting for file transfer connection...\n";

    if ((new_socket = accept(server_fd, (struct sockaddr*)&address,
                             (socklen_t*)&addrlen)) < 0) {
        perror("accept");
        return -1;
    }

    std::ofstream outfile("received_file", std::ios::binary);
    if (!outfile.is_open()) {
        std::cerr << "Failed to open file\n";
        return -1;
    }

    int bytesRead;
    while ((bytesRead = read(new_socket, buffer, sizeof(buffer))) > 0) {
        outfile.write(buffer, bytesRead);
    }
    std::cout << "File received successfully\n";

    outfile.close();
    close(new_socket);
    close(server_fd);
    return 0;
}
