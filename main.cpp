#include <iostream>
#include <sys/types.h>

#ifdef __WIN32__
#include <winsock.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#else
# include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#endif

#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include "data_types.h"
#include "packer.h"

#include <signal.h>
#include "thread"

#define SERVER_PORT htons(7000)
#define bzero(b,len) (memset((b), '\0', (len)), (void) 0)
#define bcopy(b1,b2,len) (memmove((b2), (b1), (len)), (void) 0)

bool stop = false;

float randf(float min, float max)
{
    float random = ((float) rand()) / (float) RAND_MAX;

    // generate (in your case) a float between 0 and (4.5-.78)
    // then add .78, giving you a float between .78 and 4.5
    float range = max - min;
    return (random*range) + min;
}

/*
int exit_flag = 0;

int main() {
    char filename[] = "/home/step305/SLAM_FIFO.tmp";

    int s_fifo = mkfifo(filename, S_IRWXU);
    if (s_fifo != 0)
    {
        std::cout  << "mkfifo() error: " << s_fifo << std::endl;
        exit_flag = 1;
    }
    FILE * wfd = fopen(filename, "w");
    if (wfd < 0)
    {
        std::cout << "open() error: " << wfd << std::endl;
        exit_flag = 1;
    }

    for(int ii = 0; ii < 1000; ii++) {
        T_report_packet pack;
        pack.adc = randf(-100.0f, 100.0f);
        std::string json;
        pack_data(pack, json);
        char const *out_buffer = json.c_str();

        int s_write = fprintf(wfd, "%s\n", out_buffer);
        fflush(wfd);
    }

    fclose(wfd);
    unlink(filename);
}
*/

void exit_catch(int sig) {
    std::cout << "User stop requested!" << std::endl;
    stop = true;
}

int main() {
    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = exit_catch;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntHandler, NULL);

#ifdef __WIN32__
    WORD versionWanted = MAKEWORD(1, 1);
    WSADATA wsaData;
    WSAStartup(versionWanted, &wsaData);
#endif

    char buffer[1000];
    int n;

    int serverSock=socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = SERVER_PORT;
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    bind(serverSock, (struct sockaddr*)&serverAddr, sizeof(struct sockaddr));

    // wait for a client
    listen(serverSock,1);

    sockaddr_in clientAddr;
    socklen_t sin_size=sizeof(struct sockaddr_in);
    int clientSock=accept(serverSock,(struct sockaddr*)&clientAddr, &sin_size);
    std::cout << "Ready" << std::endl;

    while (!stop) {
        bzero(buffer, 1000);

        //receive a message from a client
#ifdef __WIN32__
        n = recv(clientSock, buffer, 500, 0);
#else
        n = read(clientSock, buffer, 500);
#endif
        if (n > 0) {
//            std::cout << "Confirmation code  " << n << std::endl;
            if (strcmp(buffer, "quit") == 0) {
                stop = true;
            }
        }
        T_report_packet pack;
        pack.adc = randf(-100.0f, 100.0f);
        pack.euler[0] = randf(-180.0f, 180.0f);
        pack.euler[1] = randf(-180.0f, 180.0f);
        pack.euler[2] = randf(-180.0f, 180.0f);
        std::string json;
        pack_data(pack, json);
        char const *out_buffer = json.c_str();
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
//        std::cout << json << std::endl;
//        std::cout << out_buffer << std::endl;

        //strcpy(buffer, "test");
#ifdef __WIN32__

        n = send(clientSock, out_buffer, strlen(out_buffer), 0);
#else
        n = write(clientSock, out_buffer, strlen(out_buffer));
#endif
        if (n > 0) {
//            std::cout << "Confirmation code  " << n << std::endl;
        }
    }
#ifdef __WIN32__
    closesocket(clientSock);
#else
    close(clientSock);
#endif

#ifdef __WIN32__
    WSACleanup();
#endif

    return 0;
}
