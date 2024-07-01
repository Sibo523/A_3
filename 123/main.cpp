#include "kosaraju.hpp"
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

using namespace std;

void ManuForUser(int clientSocket, vector<pair<int, int>> &edges)
{
    char num[256];
    enum
    {
        NEW_GRAPH,
        ADD_EDGE,
        REMOVE_EDGE,
        RUN_KOSARAJU,
        EXIT
    };
    while (true)
    {
        const char *message = "Enter 1 to enter new graph, 2 to add edge, 3 to remove edge, 4 to run kosaraju, 5 to exit: ";
        write(clientSocket, message, strlen(message));
        recv(clientSocket, num, sizeof(num), 0);
        int k = atoi(num);

        switch (k)
        {
        case NEW_GRAPH:
            newGraph(edges);
            break;
        case ADD_EDGE:
            break;
        case REMOVE_EDGE:
            break;
        case RUN_KOSARAJU:
            break;
        case EXIT:
            return;
        default:
            break;
        }
    }
}
void handleClient(int clientSocket)
{
    char nums[256];
    // Assume client sends n and m
    const std::string message = "Enter the number of vertices and edges: \n";
    // new graph
    write(clientSocket, message.data(), message.size());
    fflush(stdout);
    recv(clientSocket, nums, sizeof(nums), 0);
    int n, m;
    // parse nums to n and m
    n = atoi(strtok(nums, " "));
    m = atoi(strtok(NULL, " "));
    std::cout << "n: " << n << " m: " << m << std::endl;
    vector<pair<int, int>> edges(m);
    const std::string message2 = "Enter the edges: ";
    write(clientSocket, message2.data(), message2.size());
    for (int i = 0; i < m; ++i)
    {
        int j, k;
        const char *message4 = "Enter the edge v,u separated by space in the same line: ";
        write(clientSocket, message4, strlen(message4));
        recv(clientSocket, nums, sizeof(nums), 0);
        j = atoi(strtok(nums, " "));
        k = atoi(strtok(NULL, " "));
        std::cout << "j: " << j << " k: " << k << std::endl; // not really relevent
        Newedge(j, k, edges);
    }
    //

    // running the thing
    const std::string message3 = "running ksaraju\n";
    write(clientSocket, message3.data(), message3.size());
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
    close(clientSocket);
    //
}

int main()
{
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addr_size;

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

    while (true)
    {
        addr_size = sizeof clientAddr;
        clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &addr_size);
        std::cout << "Connected" << std::endl;
        // fflush(stdout);
        thread(ManuForUser, clientSocket).detach();
    }

    return 0;
}
