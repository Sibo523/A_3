#include "Reactor.hpp"
#include <sys/socket.h>
#include "kosaraju.hpp"
#include <netinet/in.h>
#include <string.h>
#include <fcntl.h>

vector<pair<int, int>> edges;

int handle_client_msg(int sockfd)
{
    char buffer[1024];
    int read_bytes = recv(sockfd, buffer, sizeof(buffer), 0);

    buffer[read_bytes] = '\0';
    cout << buffer << endl;
    std::string msg = buffer;
    if(msg.find("NewGraph") != std::string::npos)
    {
        cout << "graph" <<std::endl;
        msg = msg.substr(msg.find(" ") + 1);
        int n = std::stoi(msg.substr(0, msg.find(",")));
        int m = std::stoi(msg.substr(msg.find(",") + 1));
            
        if(dup2(sockfd, STDIN_FILENO) == -1)
        {
            perror("could not duplicate file descriptor");
            exit(EXIT_FAILURE);
        }
        NewGraph(n, m, edges);
        return 1;
    }
    else if(msg.find("Kosaraju") != std::string::npos)
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
        return 2;
    }
    else if(msg.find("NewEdge") != std::string::npos)
    {
        msg = msg.substr(msg.find(" ") + 1);
        int i = std::stoi(msg.substr(0, msg.find(",")));
        int j = std::stoi(msg.substr(msg.find(",") + 1));
        Newedge(i, j, edges);
        return 3;
    }
    else if(msg.find("RemoveEdge") != std::string::npos)
    {
        msg = msg.substr(msg.find(" ") + 1);
        int i = std::stoi(msg.substr(0, msg.find(",")));
        int j = std::stoi(msg.substr(msg.find(",") + 1));
        cout << i << j << "edges" << endl;
        RemoveEdge(i, j, edges);
        return 4;
    }
    else
    {
        char msg[] = "Invalid operation\nValid operations are:\n1. NewGraph n,m\n2. Kosaraju\n3. NewEdge i,j\n4. RemoveEdge i,j\n";
        send(sockfd, msg, sizeof(msg), 0);
    }

    return 0;
}

int main()
{
    Reactor reactor;
    
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
    reactor.addFd(server_sockfd, [&](int sockfd)
    {
        client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, (socklen_t *)&client_address);
        if(client_sockfd == -1)
        {
            perror("could not accept connection");
            exit(EXIT_FAILURE);
        }
        reactor.addFd(client_sockfd, handle_client_msg);
        cout << "new connection" << client_address.sin_addr.s_addr << endl;
    });
    reactor.start();

    close(server_sockfd);
    return 0;
}