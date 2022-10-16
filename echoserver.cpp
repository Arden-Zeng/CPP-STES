/*
  MIT License

  Copyright (c) 2022 Arden Zeng

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/socketvar.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <iostream>

const int MIN_PORT_VAL = 1;
const int MAX_PORT_VAL = 65535;

const int ADDRESS_FAMILY = AF_INET;
const int TYPE = SOCK_STREAM;
const int PROTOCOL = 0;

bool isNum(char* str) {
  try {
    std::stoi(str);
    return true;
  }
  catch (...) {
    return false;
  }
}

int main(int argc, char* argv[]) {
  int PORT = 3000;

  if (argc > 1) {
    if (isNum(argv[1])) {
      int user_supplied_port = std::stoi(argv[1]);
      if (user_supplied_port >= MIN_PORT_VAL && user_supplied_port <= MAX_PORT_VAL) {
        PORT = user_supplied_port;
      }
    }
    else {
      std::cerr << "[ERR]: PROVIDED PORT IS INVALID. DEFAULTING TO PORT 3000" << std::endl;
    }
  }
  else {
    std::cerr << "[INFO]: NO PORT PROVIDED. DEFAULTING TO PORT 3000" << std::endl;
  }

  // Create socket
  int sockfd = socket(ADDRESS_FAMILY, TYPE, PROTOCOL);

  // Check if socket was created successfully
  if (sockfd < 0) {
    std::cerr << "[ERR]: COULD NOT CREATE SOCKET" << std::endl;
    close(sockfd);
    return -1;
  }
  std::cout << "[INFO]: SOCKET CREATED" << std::endl;

  // Create addresss
  sockaddr_in s_addr = {0};
  s_addr.sin_family = AF_INET;
  s_addr.sin_port = htons(PORT);
  s_addr.sin_addr = (in_addr) {inet_addr("127.0.0.1")};

  while (bind(sockfd, (sockaddr *) &s_addr, sizeof(s_addr)) < 0) {
    std::cerr << "[ERR]: COULD NOT BIND SOCKET TO PORT: " << PORT << std::endl;
    close(sockfd);
    return -1;
  }
  std::cout << "[INFO]: SOCKET BOUND TO PORT: " << PORT << std::endl;

  // Start listening
  if (listen(sockfd, SOMAXCONN) < 0) {
    std::cerr << "[ERR]: COULD NOT LISTEN TO PORT:" << PORT << std::endl;
    close(sockfd);
    return -1;
  }
  std::cout << "[INFO]: SOCKET LISTENING - READY" << std::endl;

  // Accept incomming connection
  sockaddr_in c_addr = {0};
  int c_len = sizeof(c_addr);
  int ic_sock = accept(sockfd, (sockaddr *) &c_addr, (socklen_t *) &c_len);
  if (ic_sock < 0) {
    std::cerr << "[ERR]: COULD NOT ACCEPT CONNECTION" << std::endl;
    close(sockfd);
    return -1;
  }
  std::cout << "[INFO]: SOCKET CONNECTED" << std::endl;

  // Echo server logic
  while (true) {
    char buffer[8192] = {0};
    int buffer_size = 8192;
    int bytes_read = recv(ic_sock, &buffer, buffer_size, 0);
    if (bytes_read == 0) {
      std::cout << "[INFO]: CLIENT TERMINATED CONNECTION" << std::endl;
      break;
    }
    else if (bytes_read < 0) {
      std::cout << "[ERR]: ERROR RECEIVING DATA" << std::endl;
      break;
    }
    if (send(ic_sock, &buffer, bytes_read, 0) < 0) {
      std::cout << "[ERR]: ERROR SENDING DATA" << std::endl;
      break;
    }
    std::cout << "[INFO]: DATA RECEIVED" << "\n" << buffer << std::endl;
  }
  close(ic_sock);
  close(sockfd);
 }
