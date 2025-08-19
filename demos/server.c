#include <signal.h>

#define HTTP_IMPLEMENTATION
#include "../http.h"

void randnum_handler(void *ctx, HTTP_Request *req, HTTP_Response *resp) {
	UNUSED(ctx);
	http_req_ensure_method(req, resp, METHOD_GET);

	http_resp_set_status_line(resp, STATUS_OK, "OK");
	http_resp_add_header(resp, "Content-Type", CONTENT_TYPE_TEXT_HTML"; charset=utf-8");
	http_resp_add_header(resp, "Connection", "close");

	char buf[256];
	sprintf(buf, "<!DOCTYPE html><body><h1>%f</h1><a href=\"/\">Back</a></body></html>", (float) rand() / RAND_MAX);
	
	http_resp_set_body(resp, (uint8_t *) strdup(buf), strlen(buf));
}

int main(void) {
	signal(SIGPIPE, SIG_IGN);
	srand(time(0));

	HTTP_Server serv = http_server_create(8080);

	http_server_handle(&serv, "/randnum", randnum_handler, NULL);

	if (http_server_serve_file(&serv, "/", CONTENT_TYPE_TEXT_HTML, "./files/index.html") != 0) {
		fprintf(stderr, "failed to register /index.html\n");
	}

	if (http_server_serve_file(&serv, "/img.jpg", CONTENT_TYPE_IMAGE_JPEG, "./files/img.jpg") != 0) {
		fprintf(stderr, "failed to register /img.jpg\n");
	}

	http_server_run(&serv);
	return 0;
}
