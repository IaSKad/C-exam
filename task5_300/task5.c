#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>
#include <errno.h>
#include <ctype.h>
#include "ewpdef.h"

#define MAX_CLIENTS 5/*can handle 5 clients, the next gets queueed*/
#define BUFFER_SIZE 10240 /*defines temporary memory to hold network data*/

/* Convert int to ASCII string of length 4 */
void int_to_ascii4(int num, char *buf)
{
    snprintf(buf, 5, "%04d", num);
}

/* Create timestamp string */
void create_timestamp(char *buf, size_t size)
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    strftime(buf, size, "%d %b %Y, %H:%M:%S", &tm);
}

/* Send server accept message */
void send_server_accept(int sock, const char *server_name) 
{
    struct EWA_EXAM25_TASK5_PROTOCOL_SERVERACCEPT msg;
    memset(&msg, 0, sizeof(msg));

    memcpy(msg.stHead.acMagicNumber, EWA_EXAM25_TASK5_PROTOCOL_MAGIC, 3);
    int_to_ascii4(sizeof(msg), msg.stHead.acDataSize);
    msg.stHead.acDelimeter[0] = '|';

    strcpy(msg.acStatusCode, "220");
    msg.acHardSpace[0] = ' ';

    char timestamp[51];
    create_timestamp(timestamp, sizeof(timestamp));

    snprintf(msg.acFormattedString, sizeof(msg.acFormattedString), "127.0.0.1 SMTP %s %s", server_name, timestamp);
    msg.acHardZero[0] = 0;

    send(sock, &msg, sizeof(msg), 0);
}

/* Send a generic server reply */
void send_server_reply(int sock, const char *code, const char *text) 
{
    struct EWA_EXAM25_TASK5_PROTOCOL_SERVERREPLY msg;
    memset(&msg, 0, sizeof(msg));

    memcpy(msg.stHead.acMagicNumber, EWA_EXAM25_TASK5_PROTOCOL_MAGIC, 3);
    int_to_ascii4(sizeof(msg), msg.stHead.acDataSize);
    msg.stHead.acDelimeter[0] = '|';

    strncpy(msg.acStatusCode, code, 3);
    msg.acHardSpace[0] = ' ';
    strncpy(msg.acFormattedString, text, sizeof(msg.acFormattedString)-1);
    msg.acHardZero[0] = 0;

    send(sock, &msg, sizeof(msg), 0);
}

/* Send HELO response with client IP */
void send_server_helo(int sock, const char *client_info) 
{
    struct EWA_EXAM25_TASK5_PROTOCOL_SERVERHELO msg;
    memset(&msg, 0, sizeof(msg));

    memcpy(msg.stHead.acMagicNumber, EWA_EXAM25_TASK5_PROTOCOL_MAGIC, 3);
    int_to_ascii4(sizeof(msg), msg.stHead.acDataSize);
    msg.stHead.acDelimeter[0] = '|';

    strcpy(msg.acStatusCode, "250");
    msg.acHardSpace[0] = ' ';

    /* Assume format USERNAME.CLIENTIP*/
    char username[50] = {0};
    char client_ip[20] = {0};
    char *dot = strrchr(client_info, '.');
    if (dot) {
        strncpy(username, client_info, dot - client_info);
        strncpy(client_ip, dot + 1, sizeof(client_ip)-1);
    } else {
        strncpy(username, client_info, sizeof(username)-1);
        strncpy(client_ip, "unknown", sizeof(client_ip)-1);
    }

    snprintf(msg.acFormattedString, sizeof(msg.acFormattedString), "%s Hello %s", client_ip, username);
    msg.acHardZero[0] = 0;

    send(sock, &msg, sizeof(msg), 0);
}

/* check for valid filename */
int is_valid_filename(const char *filename) {
    if (!filename || strlen(filename) == 0) return 0;
    for (size_t i = 0; i < strlen(filename); i++) {
        if (isspace(filename[i])) return 0;
    }
    return 1;
}

/* handle client session */
void handle_client(int sock)
{
    char buffer[BUFFER_SIZE];
    ssize_t n;

    send_server_accept(sock, "SmtpTest");

    while (1) {
        n = recv(sock, buffer, sizeof(buffer), 0);
        if (n <= 0) break;

        char *payload = buffer + sizeof(struct EWA_EXAM25_TASK5_PROTOCOL_SIZEHEADER);

        if (strncmp(payload, "HELO", 4) == 0) {
            send_server_helo(sock, payload + 5);
        }
        else if (strncmp(payload, "MAIL FROM:", 10) == 0) {
            send_server_reply(sock, "250", "Sender address ok");
        }
        else if (strncmp(payload, "RCPT TO:", 8) == 0) {
            send_server_reply(sock, "250", "Recipient ok");
        }
        else if (strncmp(payload, "DATA", 4) == 0) {
            char *filename = payload + 5;
            if (!is_valid_filename(filename)) {
                send_server_reply(sock, "501", "Invalid filename");
                continue;
            }

            send_server_reply(sock, "354", "Ready for message");

            FILE *fp = fopen(filename, "wb");
            if (!fp) {
                send_server_reply(sock, "451", "File cannot be opened");
                continue;
            }

	/* Receive file content until ".\r\n" or ".\n" line */
            char file_buffer[BUFFER_SIZE];
            size_t buf_len = 0;

            while ((n = recv(sock, file_buffer + buf_len, sizeof(file_buffer)-buf_len-1, 0)) > 0) {
                buf_len += n;
                file_buffer[buf_len] = 0; /* null terminate for strstr */

                char *term = strstr(file_buffer, "\r\n.\r\n");
                if (!term) term = strstr(file_buffer, "\n.\n");

                if (term) {
                    fwrite(file_buffer, 1, term - file_buffer, fp);
                    break;
                } else {
                    fwrite(file_buffer, 1, buf_len, fp);
                    buf_len = 0;
                }
            }

            fclose(fp);
            send_server_reply(sock, "250", "Message accepted");
        }
        else if (strncmp(payload, "QUIT", 4) == 0) {
            send_server_reply(sock, "221", "Server closing");
            break;
        }
        else {
            send_server_reply(sock, "500", "Unknown command");
        }
    }

    close(sock);
}

int main(int argc, char *argv[]) 
{
    if (argc != 5) {
        fprintf(stderr, "Usage: %s -port <port> -id <ServerID>\n", argv[0]);
        return 1;
    }

    int port = 0;
    char server_id[50] = {0};

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-port") == 0) port = atoi(argv[++i]);
        else if (strcmp(argv[i], "-id") == 0) strncpy(server_id, argv[++i], sizeof(server_id)-1);
    }

    int listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock < 0) { perror("socket"); return 1; }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(port);

    if (bind(listen_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind"); return 1;
    }

    if (listen(listen_sock, MAX_CLIENTS) < 0) { perror("listen"); return 1; }
    printf("Server listening on 127.0.0.1:%d\n", port);

    while (1) {
        int client_sock = accept(listen_sock, NULL, NULL);
        if (client_sock < 0) { perror("accept"); continue; }

        printf("Client connected\n");
        handle_client(client_sock);
        printf("Client disconnected\n");
    }

    close(listen_sock);
    return 0;
}
