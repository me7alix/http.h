#define HTTP_IMPLEMENTATION
#include "../http.h"

int main(void) {
	HTTP_Request req = http_req_create();
	http_req_set_status_line(&req, "GET", "/");
	http_req_add_header(&req, "Host", "google.com");
	http_req_add_header(&req, "Connection", "close");

	HTTP_Error err;
	HTTP_Response resp = http_make_request(&req, "icio.us", 80, &err);
	if (err != 0) {
		perror("request making error");
		return 1;
	}

	char *header = http_resp_header_to_str(&resp);
	printf("%s\n", header);

	char html[1024 * 1024];
	memcpy(html, resp.body, sizeof(uint8_t) * resp.body_len);
	html[resp.body_len] = '\0';

	printf("%s\n", html);
	return 0;
}
