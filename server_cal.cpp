#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sstream>

#define PORT 8082

double calculate(double a, double b, char op) {
    switch(op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return b != 0 ? a / b : 0;
        default: return 0;
    }
}

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
    std::cout << "Calculator Server started...\n";

    if ((new_socket = accept(server_fd, (struct sockaddr*)&address,
                            (socklen_t*)&addrlen)) < 0) {
        perror("accept");
        return -1;
    }

    read(new_socket, buffer, 1024);
    std::string expr(buffer);

    std::istringstream iss(expr);
    double a, b;
    char op;
    iss >> a >> op >> b;

    double result = calculate(a, b, op);

    std::string res_str = "Result: " + std::to_string(result);
    send(new_socket, res_str.c_str(), res_str.size(), 0);

    close(new_socket);
    close(server_fd);
    return 0;
}
