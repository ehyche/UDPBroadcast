//
//  main.c
//  UDPBroadcastInC
//
//  Created by Eric Hyche on 2/17/26.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define SERVERPORT 4352    // the port users will be connecting to

int main(int argc, const char * argv[]) {
    int sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        perror("socket() failed");
        exit(1);
    }

    // this call is what allows broadcast packets to be sent:
    int broadcast = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) == -1) {
        perror("setsockopt (SO_BROADCAST) failed");
        exit(1);
    }

    struct sockaddr_in their_addr; // connector's address information
    their_addr.sin_family = AF_UNIX;     // host byte order
    their_addr.sin_port = htons(SERVERPORT); // short, network byte order
    their_addr.sin_addr.s_addr = inet_addr("192.168.0.255");
    memset(their_addr.sin_zero, '\0', sizeof(their_addr.sin_zero));

    char sendMSG[] ="%2SRCH\r";

    long numbytes = sendto(sockfd, sendMSG, strlen(sendMSG), 0,
                           (struct sockaddr *)&their_addr, sizeof(their_addr));
    if (numbytes == -1) {
        perror("sendto() failed");
        exit(1);
    }

    printf("Sent %ld bytes successfully.\n", numbytes);

    close(sockfd);

    return 0;
}
