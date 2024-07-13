#include "Reactor.hpp"


Reactor::Reactor() : active(false), fdMap()
{
    epollFd = epoll_create1(0);
    if(epollFd == -1)
    {
        perror("failed to create epoll");
        exit(EXIT_FAILURE);
    }
}

Reactor::~Reactor()
{
    close(epollFd);
}

int Reactor::start()
{
    active = true;
    struct epoll_event events[10];
    while(active)
    {
        int nfds = epoll_wait(epollFd, events, 10, -1);
        if(nfds == -1)
        {
            perror("epoll_wait failed");
            return -1;
        }
        for(int i = 0; i < nfds; ++i)
        {
            int fd = events[i].data.fd;
            fdMap[fd](fd);
        }
    }

    return 0;
}

int Reactor::addFd(int fd, ReactorFunc func)
{
    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = fd;

    if(epoll_ctl(epollFd, EPOLL_CTL_ADD, fd, &event) == -1)
    {
        perror("failed to add fd to epoll");
        return -1;
    }

    fdMap[fd] = func;
    return 0;
}

int Reactor::removeFd(int fd)
{
    if(epoll_ctl(epollFd, EPOLL_CTL_DEL, fd, NULL) == -1)
    {
        perror("failed to remove fd from epoll");
        return -1;
    }

    fdMap.erase(fd);
    return 0;
}

int Reactor::stop()
{
    active = false;
    return 0;
}