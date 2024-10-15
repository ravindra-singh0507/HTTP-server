/*#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

int main(int argc, char **argv) {
  // Flush after every std::cout / std::cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  std::cout << "Logs from your program will appear here!\n";

  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd < 0) {
    std::cerr << "Failed to create server socket\n";
    return 1;
  }

  // Setting SO_REUSEADDR to avoid 'Address already in use' errors
  int reuse = 1;
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
    std::cerr << "setsockopt failed\n";
    return 1;
  }

  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(4221);

  if (bind(server_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) != 0) {
    std::cerr << "Failed to bind to port 4221\n";
    return 1;
  }

  int connection_backlog = 5;
  if (listen(server_fd, connection_backlog) != 0) {
    std::cerr << "listen failed\n";
    return 1;
  }

  struct sockaddr_in client_addr;
  socklen_t client_addr_len = sizeof(client_addr);

  std::cout << "Waiting for a client to connect...\n";

  int client_fd = accept(server_fd, (struct sockaddr *) &client_addr, &client_addr_len);
  if (client_fd < 0) {
    std::cerr << "Failed to accept client connection\n";
    return 1;
  }

  std::cout << "Client connected\n";

  // Correct HTTP response message
  std::string message = "HTTP/1.1 200 OK\r\n\r\n";
  send(client_fd, message.c_str(), message.length(), 0);

  // Close the client socket and server socket
  close(client_fd);
  close(server_fd);

  return 0;
}
*/
#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

int main(int argc, char **argv) {
  // Flush after every std::cout / std::cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  std::cout << "Logs from your program will appear here!\n";

  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd < 0) {
    std::cerr << "Failed to create server socket\n";
    return 1;
  }

  int reuse = 1;
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
    std::cerr << "setsockopt failed\n";
    return 1;
  }

  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(4221);

  if (bind(server_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) != 0) {
    std::cerr << "Failed to bind to port 4221\n";
    return 1;
  }

  int connection_backlog = 5;
  if (listen(server_fd, connection_backlog) != 0) {
    std::cerr << "listen failed\n";
    return 1;
  }

  struct sockaddr_in client_addr;
  socklen_t client_addr_len = sizeof(client_addr);

  std::cout << "Waiting for a client to connect...\n";

  int client_fd = accept(server_fd, (struct sockaddr *) &client_addr, &client_addr_len);
  if (client_fd < 0) {
    std::cerr << "Failed to accept client connection\n";
    return 1;
  }

  std::cout << "Client connected\n";

  // Buffer to store the request
  char buffer[4096];
  memset(buffer, 0, sizeof(buffer));

  // Read the request from the client
  ssize_t bytes_received = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
  if (bytes_received < 0) {
    std::cerr << "Failed to read client request\n";
    close(client_fd);
    return 1;
  }

  // Parse the request line (e.g., "GET /path HTTP/1.1")
  std::string request(buffer);
  std::string request_line = request.substr(0, request.find("\r\n"));
  std::cout << "Received request: " << request_line << "\n";

  // Extract the path from the request line
  size_t method_end = request_line.find(" ");
  size_t path_end = request_line.find(" ", method_end + 1);
  std::string path = request_line.substr(method_end + 1, path_end - method_end - 1);

  // Check if the path is "/"
  std::string response;
  if (path == "/") {
    // Send 200 OK response
    response = "HTTP/1.1 200 OK\r\n\r\n";
  } else {
    // Send 404 Not Found response
    response = "HTTP/1.1 404 Not Found\r\n\r\n";
  }

  // Send the response
  send(client_fd, response.c_str(), response.length(), 0);

  // Close the client socket and server socket
  close(client_fd);
  close(server_fd);

  return 0;
}

