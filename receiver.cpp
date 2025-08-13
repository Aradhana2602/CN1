#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

using namespace std;

int detectError(string codeword) {
    int code[12];
    for (int i = 0; i < 12; ++i)
        code[i] = codeword[i] - '0';

    int errorPos = 0;
    for (int i = 0; i < 4; ++i) {
        int pos = (1 << i);
        int parity = 0;
        for (int k = 0; k < 12; ++k) {
            if ((k+1) & pos)
                parity ^= code[k];
        }
        if (parity != 0)
            errorPos += pos;
    }

    return errorPos;
}

int main() {
    int serverSock, clientSock;
    sockaddr_in serverAddr{}, clientAddr{};
    socklen_t addrSize = sizeof(clientAddr);

    serverSock = socket(AF_INET, SOCK_STREAM, 0);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(9999);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    bind(serverSock, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(serverSock, 5);

    cout << "Waiting for connection...\n";
    clientSock = accept(serverSock, (struct sockaddr*)&clientAddr, &addrSize);
    cout << "Connected.\n";

    char buffer[1024] = {0};
    recv(clientSock, buffer, 1024, 0);
    string codeword(buffer);

    cout << "Received codeword: " << codeword << endl;

    int errorBit = detectError(codeword);

    cout << "\nMenu:\n";
    if (errorBit == 0)
        cout << "1. Data received without error\n";
    else
        cout << "2. Data received with error at bit position: " << errorBit << endl;

    close(clientSock);
    close(serverSock);

    return 0;
}
