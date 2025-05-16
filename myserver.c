#include <asm-generic/socket.h>
#include <netdb.h>  // getnameinfo
#include <stdio.h>  // console input/output error
#include <stdlib.h> // exit
#include <string.h> // string manipulation

#include <netinet/in.h> // sockaddr_in
#include <sys/socket.h> // socket APIs
#include <sys/types.h>
#include <unistd.h> // open, close

#include <signal.h> // signal handling
#include <time.h>   // time

#define PORT 3000 // port number
#define BACKLOG 3 // num of pending connections for queue
#define BUFF_SIZE 1024

struct http_req {
  char method[10];
  char filename[BUFF_SIZE];
};

struct http_res {
  int code;
  char *reason_phrase;
  char *content; // file content
};

int get_mime_type(const char *filename, char *mime);

int extract_request(struct http_req *out, char *msg);

int main() {
  struct sockaddr_in server_addr;
  int server_socket; //, new_socket;
  char buffer[BUFF_SIZE] = {0};

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT); // hostshort to network byte
  server_addr.sin_addr.s_addr =
      htonl(INADDR_LOOPBACK); // hostlong to network byte
                              // refers to localhost
  // FD for TCP socket
  server_socket = socket(AF_INET, SOCK_STREAM, 0);

  if (!(server_socket < 0)) {
    // sets SO_REUSEADDR to true ({1})
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int));
  } else {
    printf("Error: socket failed");
    exit(EXIT_FAILURE);
  }

  if (bind(server_socket, (struct sockaddr *)&server_addr,
           sizeof(server_addr))) {
    printf("Error: server not bound to addr");
    exit(EXIT_FAILURE);
  }

  if (listen(server_socket, BACKLOG) < 0) {
    printf("Error: server not listening");
    exit(EXIT_FAILURE);
  }

  printf("Server running on port %d...\n", PORT);

  while (1) {
    int client_fd = accept(server_socket, NULL, NULL);
    recv(client_fd, buffer, BUFF_SIZE, 0);

    struct http_req req;
    extract_request(&req, buffer);

    if (strcmp(req.filename, "./") == 0) {
      strcpy(req.filename, "./index.html");
    }

    printf("%s\n\n", buffer);

    // TODO(daria): add headers
    struct http_res res = {.code = 200, .reason_phrase = "OK", .content = NULL};

    FILE *fp = fopen(req.filename, "rb");
    unsigned long f_size = 0;
    char mime_type[BUFF_SIZE];
    char headers[BUFF_SIZE];

    char *response;

    if (fp) {
      get_mime_type(req.filename, mime_type);

      fseek(fp, 0, SEEK_END);
      f_size = ftell(fp);
      fseek(fp, 0, SEEK_SET);

      res.content = malloc(f_size);
      fread(res.content, 1, f_size, fp);
      fclose(fp);

      int headers_len =
          snprintf(headers, sizeof(headers),
                   "HTTP/1.1 %d %s \r\nContent-Type: %s\r\nContent-Length: "
                   "%lu\r\n\r\n",
                   res.code, res.reason_phrase, mime_type, f_size);

      send(client_fd, headers, headers_len, 0);
      send(client_fd, res.content, f_size, 0);

      free(res.content);
    } else {
      int headers_len = snprintf(headers, sizeof(headers),
                                 "HTTP/1.1 404 Not Found \r\nContent-Type: "
                                 "text/plain\r\n\r\n404 Not Found");
      send(client_fd, headers, headers_len, 0);
    }

    close(client_fd);
  }

  close(server_socket); // do this atexit

  return 0;
}

int get_mime_type(const char *filename, char *mime) {
  // get file ext
  const char *dot = strrchr(filename, '.');

  if (dot == NULL) {
    strcpy(mime, "text/html; charset=utf-8");
  } else if (strcmp(dot, ".html") == 0 || strcmp(dot, ".htm") == 0) {
    strcpy(mime, "text/html; charset=utf-8");
  } else if (strcmp(dot, ".css") == 0) {
    strcpy(mime, "text/css");
  } else if (strcmp(dot, ".js") == 0) {
    strcpy(mime, "text/javascript");
  } else if (strcmp(dot, ".jpeg") == 0 || strcmp(dot, ".jpg") == 0) {
    strcpy(mime, "image/jpeg");
  } else if (strcmp(dot, ".png") == 0) {
    strcpy(mime, "image/png");
  } else if (strcmp(dot, ".json") == 0) {
    strcpy(mime, "application/json");
  }

  return 0;
}

int extract_request(struct http_req *out, char *msg) {
  if (msg == NULL) {
    return -1;
  }
  char *path;
  char *end;

  *(out->filename) = '.';
  path = strchr(msg, '/');
  strncpy(out->filename + 1, path, BUFF_SIZE - 1);
  end = strchr(out->filename, ' ');
  *end = '\0';

  strncpy(out->method, msg, 10);
  end = strchr(out->method, ' ');
  *end = '\0';

  return 0;
}
