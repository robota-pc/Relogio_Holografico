#ifndef WEB_SERVER_HANDLERS_H
#define WEB_SERVER_HANDLERS_H

#include "esp_http_server.h"

void start_webserver();
void handle_led_on(httpd_req_t *req);
void handle_led_off(httpd_req_t *req);
void handle_system_data(httpd_req_t *req);

#endif // WEB_SERVER_HANDLERS_H
