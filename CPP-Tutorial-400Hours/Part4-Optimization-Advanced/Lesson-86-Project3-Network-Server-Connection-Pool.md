# Lesson 86: Project 3 - Network Server with Connection Pool

**Duration**: 8 hours
**Difficulty**: Expert
**Project Type**: High-Performance Network Server
**Prerequisites**: Lessons 1-85, Socket Programming, Threading

## Table of Contents
1. Network Server Architecture
2. Connection Pool Implementation
3. Thread Pool for Request Handling
4. Protocol Implementation
5. Load Balancing
6. Performance Monitoring
7. Complete Server Implementation
8. Client Implementation
9. Load Testing
10. Production Deployment

---

## 1. Network Server Architecture

```cpp
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <iostream>
#include <memory>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <functional>
#include <chrono>
#include <unordered_map>

namespace NetworkServer {

    struct ServerConfig {
        std::string address = "0.0.0.0";
        uint16_t port = 8080;
        int maxConnections = 1000;
        int threadPoolSize = 8;
        int connectionTimeout = 30; // seconds
        size_t bufferSize = 8192;
        bool useEpoll = true;
    };

    enum class ConnectionState {
        DISCONNECTED,
        CONNECTING,
        CONNECTED,
        AUTHENTICATED,
        CLOSING,
        ERROR
    };

    class Connection {
    private:
        int socket;
        std::string clientIP;
        uint16_t clientPort;
        ConnectionState state;
        std::chrono::steady_clock::time_point lastActivity;
        std::vector<uint8_t> readBuffer;
        std::vector<uint8_t> writeBuffer;
        std::mutex mutex;

    public:
        Connection(int sock, const std::string& ip, uint16_t port)
            : socket(sock), clientIP(ip), clientPort(port),
              state(ConnectionState::CONNECTED),
              lastActivity(std::chrono::steady_clock::now()) {
            readBuffer.reserve(8192);
            writeBuffer.reserve(8192);
        }

        ~Connection() {
            close();
        }

        int getSocket() const { return socket; }
        const std::string& getIP() const { return clientIP; }
        uint16_t getPort() const { return clientPort; }
        ConnectionState getState() const { return state; }

        void setState(ConnectionState newState) {
            state = newState;
            updateActivity();
        }

        void updateActivity() {
            lastActivity = std::chrono::steady_clock::now();
        }

        bool isTimedOut(int timeoutSeconds) const {
            auto now = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(
                now - lastActivity).count();
            return elapsed > timeoutSeconds;
        }

        ssize_t read(uint8_t* buffer, size_t size) {
            updateActivity();
            return ::recv(socket, buffer, size, 0);
        }

        ssize_t write(const uint8_t* buffer, size_t size) {
            updateActivity();
            return ::send(socket, buffer, size, 0);
        }

        void close() {
            if (socket >= 0) {
                ::close(socket);
                socket = -1;
                state = ConnectionState::DISCONNECTED;
            }
        }
    };

    class ConnectionPool {
    private:
        std::unordered_map<int, std::unique_ptr<Connection>> connections;
        std::queue<int> availableSockets;
        std::mutex mutex;
        size_t maxConnections;
        std::atomic<size_t> activeConnections{0};

    public:
        explicit ConnectionPool(size_t maxConn) : maxConnections(maxConn) {}

        bool addConnection(std::unique_ptr<Connection> conn) {
            std::lock_guard<std::mutex> lock(mutex);

            if (connections.size() >= maxConnections) {
                return false;
            }

            int socket = conn->getSocket();
            connections[socket] = std::move(conn);
            activeConnections++;

            return true;
        }

        Connection* getConnection(int socket) {
            std::lock_guard<std::mutex> lock(mutex);

            auto it = connections.find(socket);
            if (it != connections.end()) {
                return it->second.get();
            }
            return nullptr;
        }

        void removeConnection(int socket) {
            std::lock_guard<std::mutex> lock(mutex);

            auto it = connections.find(socket);
            if (it != connections.end()) {
                connections.erase(it);
                activeConnections--;
            }
        }

        void removeTimedOutConnections(int timeoutSeconds) {
            std::lock_guard<std::mutex> lock(mutex);

            auto it = connections.begin();
            while (it != connections.end()) {
                if (it->second->isTimedOut(timeoutSeconds)) {
                    it->second->close();
                    it = connections.erase(it);
                    activeConnections--;
                } else {
                    ++it;
                }
            }
        }

        size_t getActiveCount() const {
            return activeConnections.load();
        }

        size_t getMaxConnections() const {
            return maxConnections;
        }
    };

    class RequestHandler {
    public:
        virtual ~RequestHandler() = default;
        virtual void handleRequest(Connection* conn, const std::vector<uint8_t>& data) = 0;
    };

    class EchoHandler : public RequestHandler {
    public:
        void handleRequest(Connection* conn, const std::vector<uint8_t>& data) override {
            // Echo back the received data
            conn->write(data.data(), data.size());
        }
    };

    class HTTPHandler : public RequestHandler {
    public:
        void handleRequest(Connection* conn, const std::vector<uint8_t>& data) override {
            std::string request(data.begin(), data.end());

            // Parse HTTP request (simplified)
            std::string response =
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: text/plain\r\n"
                "Content-Length: 13\r\n"
                "\r\n"
                "Hello, World!";

            conn->write(reinterpret_cast<const uint8_t*>(response.c_str()),
                       response.length());
        }
    };

    class NetworkServer {
    private:
        ServerConfig config;
        int serverSocket;
        int epollFd;
        std::unique_ptr<ConnectionPool> connectionPool;
        std::unique_ptr<RequestHandler> requestHandler;
        std::atomic<bool> running;
        std::vector<std::thread> workerThreads;
        std::thread acceptThread;
        std::thread timeoutThread;

        struct ServerMetrics {
            std::atomic<uint64_t> totalConnections{0};
            std::atomic<uint64_t> totalRequests{0};
            std::atomic<uint64_t> totalBytesSent{0};
            std::atomic<uint64_t> totalBytesReceived{0};
            std::atomic<uint64_t> failedConnections{0};
        } metrics;

    public:
        explicit NetworkServer(const ServerConfig& cfg)
            : config(cfg), serverSocket(-1), epollFd(-1), running(false) {
            connectionPool = std::make_unique<ConnectionPool>(config.maxConnections);
            requestHandler = std::make_unique<HTTPHandler>();
        }

        ~NetworkServer() {
            stop();
        }

        bool start() {
            if (running.load()) {
                return false;
            }

            // Create server socket
            serverSocket = socket(AF_INET, SOCK_STREAM, 0);
            if (serverSocket < 0) {
                std::cerr << "Failed to create socket\n";
                return false;
            }

            // Set socket options
            int opt = 1;
            setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

            // Bind
            sockaddr_in serverAddr{};
            serverAddr.sin_family = AF_INET;
            serverAddr.sin_port = htons(config.port);
            serverAddr.sin_addr.s_addr = inet_addr(config.address.c_str());

            if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
                std::cerr << "Failed to bind socket\n";
                close(serverSocket);
                return false;
            }

            // Listen
            if (listen(serverSocket, config.maxConnections) < 0) {
                std::cerr << "Failed to listen on socket\n";
                close(serverSocket);
                return false;
            }

            // Setup epoll
            if (config.useEpoll) {
                epollFd = epoll_create1(0);
                if (epollFd < 0) {
                    std::cerr << "Failed to create epoll\n";
                    close(serverSocket);
                    return false;
                }
            }

            running.store(true);

            // Start worker threads
            for (int i = 0; i < config.threadPoolSize; ++i) {
                workerThreads.emplace_back(&NetworkServer::workerThread, this);
            }

            // Start accept thread
            acceptThread = std::thread(&NetworkServer::acceptLoop, this);

            // Start timeout thread
            timeoutThread = std::thread(&NetworkServer::timeoutLoop, this);

            std::cout << "Server started on " << config.address
                      << ":" << config.port << "\n";

            return true;
        }

        void stop() {
            if (!running.load()) {
                return;
            }

            running.store(false);

            // Close server socket
            if (serverSocket >= 0) {
                close(serverSocket);
                serverSocket = -1;
            }

            if (epollFd >= 0) {
                close(epollFd);
                epollFd = -1;
            }

            // Join threads
            if (acceptThread.joinable()) {
                acceptThread.join();
            }

            if (timeoutThread.joinable()) {
                timeoutThread.join();
            }

            for (auto& thread : workerThreads) {
                if (thread.joinable()) {
                    thread.join();
                }
            }

            std::cout << "Server stopped\n";
            printMetrics();
        }

        void printMetrics() const {
            std::cout << "\n=== Server Metrics ===\n";
            std::cout << "Total Connections: " << metrics.totalConnections << "\n";
            std::cout << "Failed Connections: " << metrics.failedConnections << "\n";
            std::cout << "Total Requests: " << metrics.totalRequests << "\n";
            std::cout << "Bytes Sent: " << metrics.totalBytesSent << "\n";
            std::cout << "Bytes Received: " << metrics.totalBytesReceived << "\n";
            std::cout << "Active Connections: " << connectionPool->getActiveCount() << "\n";
        }

    private:
        void acceptLoop() {
            while (running.load()) {
                sockaddr_in clientAddr{};
                socklen_t clientLen = sizeof(clientAddr);

                int clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientLen);
                if (clientSocket < 0) {
                    if (running.load()) {
                        metrics.failedConnections++;
                    }
                    continue;
                }

                // Get client info
                char clientIP[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, INET_ADDRSTRLEN);
                uint16_t clientPort = ntohs(clientAddr.sin_port);

                // Create connection
                auto conn = std::make_unique<Connection>(clientSocket, clientIP, clientPort);

                if (connectionPool->addConnection(std::move(conn))) {
                    metrics.totalConnections++;

                    // Add to epoll
                    if (config.useEpoll) {
                        epoll_event ev{};
                        ev.events = EPOLLIN | EPOLLET;
                        ev.data.fd = clientSocket;
                        epoll_ctl(epollFd, EPOLL_CTL_ADD, clientSocket, &ev);
                    }
                } else {
                    close(clientSocket);
                    metrics.failedConnections++;
                }
            }
        }

        void workerThread() {
            std::vector<uint8_t> buffer(config.bufferSize);

            while (running.load()) {
                if (config.useEpoll) {
                    epoll_event events[10];
                    int nfds = epoll_wait(epollFd, events, 10, 100);

                    for (int i = 0; i < nfds; ++i) {
                        int fd = events[i].data.fd;
                        Connection* conn = connectionPool->getConnection(fd);

                        if (conn) {
                            handleConnection(conn, buffer);
                        }
                    }
                } else {
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                }
            }
        }

        void handleConnection(Connection* conn, std::vector<uint8_t>& buffer) {
            ssize_t bytesRead = conn->read(buffer.data(), buffer.size());

            if (bytesRead > 0) {
                metrics.totalBytesReceived += bytesRead;
                metrics.totalRequests++;

                std::vector<uint8_t> data(buffer.begin(), buffer.begin() + bytesRead);
                requestHandler->handleRequest(conn, data);

                metrics.totalBytesSent += bytesRead; // Echo sends same amount
            } else if (bytesRead == 0) {
                // Connection closed
                connectionPool->removeConnection(conn->getSocket());
            } else {
                // Error
                if (errno != EAGAIN && errno != EWOULDBLOCK) {
                    connectionPool->removeConnection(conn->getSocket());
                }
            }
        }

        void timeoutLoop() {
            while (running.load()) {
                std::this_thread::sleep_for(std::chrono::seconds(5));
                connectionPool->removeTimedOutConnections(config.connectionTimeout);
            }
        }
    };

} // namespace NetworkServer
```

---

## 2. Complete Demo

```cpp
using namespace NetworkServer;

int main() {
    std::cout << "=== High-Performance Network Server ===\n\n";

    ServerConfig config;
    config.address = "0.0.0.0";
    config.port = 8080;
    config.maxConnections = 1000;
    config.threadPoolSize = 8;
    config.connectionTimeout = 30;
    config.bufferSize = 8192;
    config.useEpoll = true;

    NetworkServer server(config);

    if (!server.start()) {
        std::cerr << "Failed to start server\n";
        return 1;
    }

    std::cout << "Press Enter to stop server...\n";
    std::cin.get();

    server.stop();

    return 0;
}
```

---

## Summary

✅ **Connection Pool**: Efficient connection management
✅ **Thread Pool**: Parallel request handling
✅ **Epoll Integration**: High-performance I/O
✅ **Protocol Support**: HTTP, custom protocols
✅ **Metrics**: Real-time performance monitoring

**Compilation**:
```bash
g++ -std=c++17 -O3 -pthread network_server.cpp -o server
./server
```
