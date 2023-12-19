#ifndef __HTTP_WEB_SERVER_H__
#define __HTTP_WEB_SERVER_H__

#include "esp_http_server.h"

httpd_handle_t start_webserver(void);

void stop_webserver(httpd_handle_t server);

#endif // __HTTP_WEB_SERVER_H__