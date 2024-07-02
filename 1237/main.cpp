#include "kosaraju.hpp"
#include "Proactor.hpp"
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

using namespace std;
std::mutex mtxy;

void ManuForUser(Proactor *proactor, int clientSocket, vector<pair<int, int>> &edges, int &n)
{
    char num[256];
    enum
    {
        NEW_GRAPH = 1,
        ADD_EDGE = 2,
        REMOVE_EDGE = 3,
        RUN_KOSARAJU = 4,
        EXIT = 5
    };
    while (true)
    {
        const char *message = "Enter 1 to enter new graph, 2 to add edge, 3 to remove edge, 4 to run kosaraju, 5 to exit: ";
        write(clientSocket, message, strlen(message));
        recv(clientSocket, num, sizeof(num), 0);
        int k = atoi(num);
        std::cout << "k: " << k << std::endl;
        switch (k)
        {
        case NEW_GRAPH:
        {
            std::cout << "new graph" << std::endl;
            std::lock_guard<std::mutex> lock(mtxy);
            newGraph(edges, clientSocket, num, n);
            std::cout << n << std::endl;
        }
        break;

        case ADD_EDGE:
        {
            std::lock_guard<std::mutex> lock(mtxy);
            Newedge(edges, clientSocket);
        }
        break;
        case REMOVE_EDGE:
        {
            std::lock_guard<std::mutex> lock(mtxy);
            removeEdge(edges, clientSocket);
        }
        break;
        case RUN_KOSARAJU:
        {
            std::lock_guard<std::mutex> lock(mtxy);
            std::cout << "Edges: \n";
            for (const auto &edge : edges)
            {
                std::cout << edge.first << " " << edge.second << std::endl;
            }
            std::cout << n << std::endl;
            vector<vector<int>> scc = kosaraju(n, edges);
            for (const auto &component : scc)
            {
                for (int v : component)
                {
                    std::string vStr = std::to_string(v) + " ";       // Convert integer to string and add a space
                    send(clientSocket, vStr.c_str(), vStr.size(), 0); // Send the string representation of the integer
                }
                std::string newline = "\n";                             // Correctly define newline as a string
                send(clientSocket, newline.c_str(), newline.size(), 0); // Send the newline as a string
            }
        }
        break;
        case EXIT:
            close(clientSocket);
            return;
        default:
            break;
        }
    }
}

int main()
{
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addr_size;
    vector<pair<int, int>> edges;
    int n = 0;
    serverSocket = socket(PF_INET, SOCK_STREAM, 0);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(1235);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

    bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

    if (listen(serverSocket, 50) == 0)
        cout << "Listening" << endl;
    else
        cout << "Error" << endl;

    Proactor proactor;

    while (true)
    {
        addr_size = sizeof clientAddr;
        clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &addr_size);
        std::cout << "Connected" << std::endl;

        proactor.post(clientSocket, [&edges, &n](Proactor *proactor, int clientSocket)
                      { ManuForUser(proactor, clientSocket, edges, n); });
    }

    return 0;
}
