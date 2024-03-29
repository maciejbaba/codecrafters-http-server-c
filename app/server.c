#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

int main()
{
  // Disable output buffering
  setbuf(stdout, NULL);

  // You can use print statements as follows for debugging, they'll be visible when running tests.
  printf("Logs from your program will appear here!\n");

  // Uncomment this block to pass the first stage

  int server_fd,
      client_addr_len, client_fd;
  struct sockaddr_in client_addr;

  server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd == -1)
  {
    printf("Socket creation failed: %s...\n", strerror(errno));
    return 1;
  }

  // Since the tester restarts your program quite often, setting REUSE_PORT
  // ensures that we don't run into 'Address already in use' errors
  int reuse = 1;
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEPORT, &reuse, sizeof(reuse)) < 0)
  {
    printf("SO_REUSEPORT failed: %s \n", strerror(errno));
    return 1;
  }

  struct sockaddr_in serv_addr = {
      .sin_family = AF_INET,
      .sin_port = htons(4221),
      .sin_addr = {htonl(INADDR_ANY)},
  };

  if (bind(server_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) != 0)
  {
    printf("Bind failed: %s \n", strerror(errno));
    return 1;
  }

  int connection_backlog = 5;
  if (listen(server_fd, connection_backlog) != 0)
  {
    printf("Listen failed: %s \n", strerror(errno));
    return 1;
  }

  printf("Waiting for a client to connect...\n");
  client_addr_len = sizeof(client_addr);

  client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);

  char buffer[512] = {0};
  recv(client_fd, buffer, 512, 0);

  printf("Received: %s\n", buffer);
  printf("Splitting the request into tokens...\n");

  char *token = strtok(buffer, " ");
  int i = 0;
  char *resp;

  char *succ_resp = "HTTP/1.1 200 OK\r\n\r\n";
  char *fail_resp = "HTTP/1.1 404 Not Found\r\n\r\n";


  // GET / HTTP/1.1

  while (token != NULL)
  {
    if (i == 1)
    {
      if (strcmp(token, "/") == 0)
      {
        resp = succ_resp;
      }
      else
      {
        resp = fail_resp;
      }
    }

    printf("Token: %s\n", token);
    token = strtok(NULL, " ");
    i++;
  }

  write(client_fd, resp, strlen(resp));

  close(server_fd);

  return 0;
}
