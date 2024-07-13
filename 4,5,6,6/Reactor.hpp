
#pragma once

#include <functional>

#include <iostream>
#include <sys/epoll.h>
#include <unordered_map>
// #include <sys/socket.h>
#include <unistd.h>

using ReactorFunc = std::function<void(int sockfd)>;

class Reactor
{
    private:
        int epollFd;
        bool active;
        std::unordered_map<int, ReactorFunc> fdMap;

    public:
        Reactor();
        ~Reactor();

        int start(); //
        
        // returns 0 on success, adds fd to poll list
        int addFd(int fd, ReactorFunc func);

        int removeFd(int fd);

        int stop();
};