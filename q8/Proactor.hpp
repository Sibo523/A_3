#ifndef PROACTOR_HPP
#define PROACTOR_HPP

#include <functional>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <future>

// Forward declaration of Proactor
class Proactor;

// Completion handler type
using CompletionHandler = std::function<void(Proactor *, int)>;

// Proactor class
class Proactor
{
public:
    Proactor();
    ~Proactor();

    // Function to start the Proactor event loop
    void run();

    // Function to stop the Proactor event loop
    void stop();

    // Function to post a new completion handler
    void post(int clientSocket, CompletionHandler handler);

private:
    void eventLoop();

    std::queue<std::pair<int, CompletionHandler>> handlers;
    std::mutex handlersMutex;
    std::condition_variable handlersCondition;
    bool running;
    std::thread eventLoopThread;
};

#endif // PROACTOR_HPP
