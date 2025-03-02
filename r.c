#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
// #include <arpa/inet.h>
#include <netdb.h>

#define BUFFER_SIZE 8192

void remove_html_tags(const char *str) {
    int in_tag = 0;  // Flag to track if we are inside an HTML tag
    while (*str) {
        if (*str == '<') {
            in_tag = 1;  // Entering an HTML tag
        } else if (*str == '>') {
            in_tag = 0;  // Exiting an HTML tag
        } else if (!in_tag) {
            putchar(*str);  // Print the character if it's not inside an HTML tag
        }
        str++;
    }
}

void get_man_page(const char *command) {
    int sockfd;
    struct sockaddr_in server_addr;
    struct hostent *server;
    char request[512];
    char response[BUFFER_SIZE];
    char *host = "man.he.net";
    int port = 80;

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error opening socket");
        exit(1);
    }

    // Get server by name
    server = gethostbyname(host);
    if (server == NULL) {
        fprintf(stderr, "No such host\n");
        exit(1);
    }

    // Setup server address structure
    memset((char *) &server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    memcpy((char *)&server_addr.sin_addr.s_addr, (char *)server->h_addr, server->h_length);
    server_addr.sin_port = htons(port);

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        perror("Error connecting");
        exit(1);
    }

    // Create the GET request with the new URL format
    snprintf(request, sizeof(request),
             "GET /?topic=%s&section=all HTTP/1.1\r\n"
             "Host: %s\r\n"
             "Connection: close\r\n\r\n",
             command, host);

    // Send the GET request
    if (write(sockfd, request, strlen(request)) < 0) {
        perror("Error writing to socket");
        exit(1);
    }

    // Read the response and process it
    int bytes_read;
    int header_ended = 0;
    int flag=0;
    while ((bytes_read = read(sockfd, response, sizeof(response) - 1)) > 0) {
        response[bytes_read] = '\0';
        remove_html_tags(response);
    }

    if (bytes_read < 0) {
        perror("Error reading from socket");
    }

    // Close the socket
    close(sockfd);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <command_name>\n", argv[0]);
        exit(1);
    }

    get_man_page(argv[1]);

    return 0;
}
