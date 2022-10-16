# Single Threaded C++ Echo Server

Single threaded C++ TCP Echo Server. Accepts a single client and exits upon client side connection terminating.

## Installation

Clone the repository 
```bash
git clone https://github.com/Arden-Zeng/CPP-STES.git
```

## Usage
#### Starting server:
```bash
gcc -std=c++14 echoserver.cpp -o echoserver
chmod u+x echoserver
./echoserver [PORT NUMBER]
```
#### Connecting to server:
```bash
nc localhost [PORT NUMBER]
```

## License
MIT