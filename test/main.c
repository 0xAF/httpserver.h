#define HTTPSERVER_IMPL
#include "../httpserver.h"

#define RESPONSE "Hello, World!"

int request_target_is(struct http_request_s* request, char const * target) {
  http_string_t url = http_request_target(request);
  int len = strlen(target);
  return len == url.len && memcmp(url.buf, target, url.len) == 0;
}

void handle_request(struct http_request_s* request) {
  http_request_connection(request, HTTP_AUTOMATIC);
  struct http_response_s* response = http_response_init();
  http_response_status(response, 200);
  http_response_header(response, "Content-Type", "text/plain");
  if (request_target_is(request, "/echo")) {
    http_string_t body = http_request_body(request);
    http_response_body(response, body.buf, body.len);
  } else if (request_target_is(request, "/user-agent")) {
    http_string_t ua = http_request_header(request, "User-Agent");
    http_response_body(response, ua.buf, ua.len);
  } else if (request_target_is(request, "/headers")) {
    int iter = 0, i = 0;
    http_string_t key, val;
    char buf[512];
    while (http_request_iterate_headers(request, &key, &val, &iter)) {
      i += snprintf(buf + i, 512 - i, "%.*s: %.*s\n", key.len, key.buf, val.len, val.buf);
    }
    http_response_body(response, buf, i);
  } else {
    http_response_body(response, RESPONSE, sizeof(RESPONSE) - 1);
  }
  http_respond(request, response);
}

int main() {
  struct http_server_s* server = http_server_init(8080, handle_request);
  http_server_listen(server);
}
