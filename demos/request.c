#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define HTTP_IMPLEMENTATION
#include "../http.h"

void error(const char *msg) { perror(msg); exit(0); }

HTTP_Response http_make_request(HTTP_Request *req, const char *host, uint16_t port) {
	char *header = http_req_header_to_str(req);
	size_t header_len = strlen(header);

	uint8_t request[8 * 1024];
	size_t req_len = header_len + req->body_len;
	memcpy(request, header, sizeof(char) * strlen(header));
	memcpy(request + header_len, req->body, sizeof(uint8_t) * req->body_len);

	struct hostent *h = gethostbyname(host);
	if (!h) { perror("gethostbyname"); return (HTTP_Response){0}; }

	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) { perror("socket"); return (HTTP_Response){0}; }

	struct sockaddr_in addr = {0};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	memcpy(&addr.sin_addr.s_addr, h->h_addr_list[0], h->h_length);
	printf("host addr: %s\n", h->h_addr_list[0]);

	if (connect(sock, (struct sockaddr *)&addr, sizeof addr) < 0) {
		perror("connect");
		close(sock);
		return (HTTP_Response){0};
	}

	if (send(sock, request, req_len, 0) < 0) {
		perror("send");
		close(sock);
		return (HTTP_Response){0};
	}

	uint8_t response[1024 * 1024];
	uint8_t buf[4096];
	ssize_t n, ln = 0;

	while ((n = recv(sock, buf, sizeof buf - 1, 0)) > 0) {
		memcpy(response + ln, buf, sizeof(uint8_t) * n);
		ln = n;
	}
	if (n < 0) perror("recv");

	bool err = false;
	HTTP_Response resp = http_resp_parse(response, &err);

	close(sock);
	return resp;
}

int main(void) {
	HTTP_Request req = http_req_create();
	http_req_set_status_line(&req, "GET", "/");
	http_req_add_header(&req, "Host", "google.com");

	HTTP_Response resp = http_make_request(&req, "google.com", 80);

	char *header = http_resp_header_to_str(&resp);
	printf("%s\n", header);

	char html[1024 * 1024];
	memcpy(html, req.body, sizeof(uint8_t) * req.body_len);
	html[req.body_len] = '\0';

	printf("%s\n", html);

	return 0;
}
