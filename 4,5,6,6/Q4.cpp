#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <fcntl.h>
#include <string>
#include <iostream>
#include <sstream>

#include <kosaraju.hpp>

int handle_client_msg(int sockfd, fd_set &main_set, std::string &op)
{
    char buffer[1024];
    int read_bytes = recv(sockfd, buffer, sizeof(buffer), 0);
    if(read_bytes == 0)
    {
        std::cout << "Socket" << sockfd << "closed by cleint" << std::endl;
        close(sockfd);
        FD_CLR(sockfd, &main_set);
        return -2;
    }
    else if(read_bytes < 0)
    {
        perror("could not read from socket");
        close(sockfd);
        FD_CLR(sockfd, &main_set);
        return -1;
    }

    buffer[read_bytes] = '\0';
    op = buffer;
    if(op.find("NewGraph") != std::string::npos)
    {
        cout << "graph" <<std::endl;
        return 1;
    }
    else if(op.find("Kosaraju") != std::string::npos)
    {
        return 2;
    }
    else if(op.find("NewEdge") != std::string::npos)
    {
        return 3;
    }
    else if(op.find("RemoveEdge") != std::string::npos)
    {
        return 4;
    }
    else
    {
        char msg[] = "Invalid operation\nValid operations are:\n1. NewGraph n,m\n2. Kosaraju\n3. NewEdge i,j\n4. RemoveEdge i,j\n";
        send(sockfd, msg, sizeof(msg), 0);
    }

    return 0;
}

int recvInt(int sockfd)
{
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    recv(sockfd, buffer, sizeof(buffer), 0);
    // memset((buffer+100), 0, sizeof(buffer-100));
    return atoi(buffer);
}

int main(void)
{
    vector<pair<int, int>> edges;
    int server_sockfd, client_sockfd;
    sockaddr_in server_address, client_address;

    memset(&server_address, 0, sizeof(server_address));
    memset(&client_address, 0, sizeof(client_address));

    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if(server_sockfd == -1)
    {
        perror("could not create socket");
        exit(EXIT_FAILURE);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(9034);

    int opt = 1;
    // make the socket port reusable
    if(setsockopt(server_sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)) == -1)
    {
        perror("could not set socket option");
        exit(EXIT_FAILURE);
    }

    fcntl(server_sockfd, F_SETFL, O_NONBLOCK);

    if(bind(server_sockfd, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
    {
        perror("could not bind socket");
        exit(EXIT_FAILURE);
    }

    if(listen(server_sockfd, 10) == -1)
    {
        perror("could not listen on socket");
        exit(EXIT_FAILURE);
    }

    fd_set main_set, rfds;
    int max_fd = server_sockfd;

    FD_ZERO(&main_set);
    FD_ZERO(&rfds);
    FD_SET(server_sockfd, &main_set);
    std::string msg;
    while(true)
    {
        rfds = main_set;

        if(select(max_fd + 1, &rfds, NULL, NULL, NULL) == -1)
        {
            perror("select failed");
            exit(EXIT_FAILURE);
        }

        for(int i = 0; i <= max_fd; ++i)
        {
            if(FD_ISSET(i, &rfds))
            {
                if(i == server_sockfd)
                {
                    socklen_t client_len = sizeof(client_address);
                    if((client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, &client_len)) == -1)
                    {
                        perror("could not accept connection");
                        exit(EXIT_FAILURE);
                    }
                    
                    std::cout << "New connection established from " << client_address.sin_addr.s_addr << std::endl;

                    FD_SET(client_sockfd, &main_set);
                    max_fd = (client_sockfd > max_fd) ? client_sockfd : max_fd;
                }
                else
                {
                    int op = handle_client_msg(i, main_set, msg);
                    switch(op)
                    {
                    case 1:
                        {
                        msg = msg.substr(msg.find(" ") + 1);
                        int n = std::stoi(msg.substr(0, msg.find(",")));
                        int m = std::stoi(msg.substr(msg.find(",") + 1));
                            
                        if(dup2(i, STDIN_FILENO) == -1)
                        {
                            perror("could not duplicate file descriptor");
                            exit(EXIT_FAILURE);
                        }
                        NewGraph(n, m, edges);
                        
                        break;
                        }
                    case 2:
                        {
                        vector<vector<int>> ans = kosaraju_list(edges.size(), edges);
                        for(auto &component : ans)
                        {
                            for(auto &node : component)
                            {
                                std::cout << node << " ";
                            }
                            std::cout << std::endl;
                        }
                        }
                        break;
                    case 3:
                        {
                        msg = msg.substr(msg.find(" ") + 1);
                        int i = std::stoi(msg.substr(0, msg.find(",")));
                        int j = std::stoi(msg.substr(msg.find(",") + 1));
                        Newedge(i, j, edges);
                        break;
                        }
                    case 4:
                        {
                        msg = msg.substr(msg.find(" ") + 1);
                        int i = std::stoi(msg.substr(0, msg.find(",")));
                        int j = std::stoi(msg.substr(msg.find(",") + 1));
                        cout << i << j << "edges" << endl;
                        RemoveEdge(i, j, edges);
                        break;
                        }
                    default:
                        break;
                    }
                }
            }
        }
        for(auto &edge : edges)
        {
            std::cout << edge.first << " " << edge.second << std::endl;
        }
    }

    close(server_sockfd);
   exit(EXIT_SUCCESS);
}