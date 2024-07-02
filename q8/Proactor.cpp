#include "Proactor.hpp"

Proactor::Proactor() : running(true), eventLoopThread(&Proactor::eventLoop, this) {}

Proactor::~Proactor()
{
    stop();
}

void Proactor::run()
{
    std::unique_lock<std::mutex> lock(handlersMutex);
    running = true;
    lock.unlock();
    eventLoopThread = std::thread(&Proactor::eventLoop, this);
}

void Proactor::stop()
{
    {
        std::unique_lock<std::mutex> lock(handlersMutex);
        running = false;
    }
    handlersCondition.notify_all();
    if (eventLoopThread.joinable())
    {
        eventLoopThread.join();
    }
}

void Proactor::post(int clientSocket, CompletionHandler handler)
{
    {
        std::unique_lock<std::mutex> lock(handlersMutex);
        handlers.emplace(clientSocket, handler);
    }
    handlersCondition.notify_one();
}

void Proactor::eventLoop()
{
    while (running)
    {
        std::unique_lock<std::mutex> lock(handlersMutex);
        if (handlers.empty())
        {
            handlersCondition.wait(lock, [this]
                                   { return !handlers.empty() || !running; });
        }
        if (!running)
        {
            break;
        }
        auto [clientSocket, handler] = handlers.front();
        handlers.pop();
        lock.unlock();

        handler(this, clientSocket);
    }
}
