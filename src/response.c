#include "../include/response.h"

const char* http_code_name(http_code_t http_code) {
    switch (http_code) {
        case CONTINUE: return "100 CONTINUE";
        case SWITCHING_PROTOCOLS: return "101 SWITCHING_PROTOCOLS";
        case PROCCESING: return "102 PROCCESING";
        case EARLY_HINTS: return "103 EARLY_HINTS";
        case OK: return "200 OK";
        case CREATED: return "201 CREATED";
        case NON_AUTHORITATIVE_INFORMATION: return "203 NON_AUTHORITATIVE_INFORMATION";
        case NO_CONTENT: return "204 NO_CONTENT";
        case RESET_CONTENT: return "205 RESET_CONTENT";
        case PARTIAL_CONTENT: return "206 PARTIAL_CONTENT";
        case MULTI_STATUS: return "207 MULTI_STATUS";
        case ALREADY_REPORTED: return "208 ALREADY_REPORTED";
        case IM_USED: return "226 IM_USED";
        case MULTIPLE_CHOICES: return "300 MULTIPLE_CHOICES";
        case MOVED_PERMANENTLY: return "301 MOVED_PERMANENTLY";
        case FOUND: return "302 FOUND";
        case SEE_OTHER: return "303 SEE_OTHER";
        case NOT_MODIFIED: return "304 NOT_MODIFIED";
        case UNUSED: return "306 UNUSED";
        case TEMPORARY_REDIRECT: return "307 TEMPORARY_REDIRECT";
        case PERMANENT_REDIRECT: return "308 PERMANENT_REDIRECT";
        case BAD_REQUEST: return "400 BAD_REQUEST";
        case UNAUTHORIZED: return "401 UNAUTHORIZED";
        case PAYMENT_REQUIRED: return "402 PAYMENT_REQUIRED";
        case NOT_FOUND: return "404 NOT_FOUND";
        case METHOD_NOT_ALLOWED: return "405 METHOD_NOT_ALLOWED";
        case NOT_ACCEPTABLE: return "406 NOT_ACCEPTABLE";
        case PROXY_AUTHENTICATION_REQUIRED: return "407 PROXY_AUTHENTICATION_REQUIRED";
        case REQUEST_TIMEOUT: return "408 REQUEST_TIMEOUT";
        case CONFLICT: return "409 CONFLICT";
        case GONE: return "410 GONE";
        case LENGTH_REQUIRED: return "411 LENGTH_REQUIRED";
        case PRECONDITION_FAILED: return "412 PRECONDITION_FAILED";
        case CONTENT_TOO_LARGE: return "413 CONTENT_TOO_LARGE";
        case URI_TOO_LONG: return "414 URI_TOO_LONG";
        case UNSUPPORTED_MEDIA_TYPE: return "415 UNSUPPORTED_MEDIA_TYPE";
        case RANGE_NOT_SATISFIABLE: return "416 RANGE_NOT_SATISFIABLE";
        case EXPECTATION_FAILED: return "417 EXPECTATION_FAILED";
        case MISDIRECTED_REQUEST: return "421 MISDIRECTED_REQUEST";
        case UNPROCCESSABLE_CONTENT: return "422 UNPROCCESSABLE_CONTENT";
        case LOCKED: return "423 LOCKED";
        case FAILED_DEPENDENCY: return "424 FAILED_DEPENDENCY";
        case TOO_EARLY: return "425 TOO_EARLY";
        case UPGRADE_REQUIRED: return "426 UPGRADE_REQUIRED";
        case PRECONDITION_REQUIRED: return "428 PRECONDITION_REQUIRED";
        case TOO_MANY_REQUESTS: return "429 TOO_MANY_REQUESTS";
        case REQUEST_HEADER_FIELDS_TOO_LARGE: return "431 REQUEST_HEADER_FIELDS_TOO_LARGE";
        case UNAVAILABLE_FOR_LEGAL_REASONS: return "451 UNAVAILABLE_FOR_LEGAL_REASONS";
        case NOT_IMPLEMENTED: return "501 NOT_IMPLEMENTED";
        case BAD_GATEWAY: return "502 BAD_GATEWAY";
        case SERVICE_UNAVAILABLE: return "503 SERVICE_UNAVAILABLE";
        case GATEWAY_TIMEOUT: return "504 GATEWAY_TIMEOUT";
        case HTTP_VERSION_NOT_SUPPORTED: return "505 HTTP_VERSION_NOT_SUPPORTED";
        case VARIANT_ALSO_NEGOTIATES: return "506 VARIANT_ALSO_NEGOTIATES";
        case INSUFFICIENT_STORAGE: return "507 INSUFFICIENT_STORAGE";
        case LOOP_DETECTED: return "508 LOOP_DETECTED";
        case NOT_EXTENDED: return "510 NOT_EXTENDED";
        case NETWORK_AUTHENTICATION_REQUIRED: return "511 NETWORK_AUTHENTICATION_REQUIRED";
        default: return "UNKNOWN CODE";
    }
}

const char* content_type_name(content_type_t content_type) {
    switch (content_type) {
        case TEXT_HTML: return "text/html";
        case TEXT_PLAIN: return "text/plain";
        case TEXT_CSS: return "text/css";
        case TEXT_JAVASCRIPT: return "text/javascript";
        case APPLICATION_JSON: return "application/json";
        case APPLICATION_XML: return "application/xml";
        case APPLICATION_JAVASCRIPT: return "application/javascript";
        case APPLICATION_PDF: return "application/pdf";
        case APPLICATION_ZIP: return "application/zip";
        case APPLICATION_FORM_URLENCODED: return "application/x-www-form-urlencoded";
        case MULTIPART_FORM_DATA: return "multipart/form-data";
        case IMAGE_JPEG: return "image/jpeg";
        case IMAGE_PNG: return "image/png";
        case IMAGE_GIF: return "image/gif";
        case IMAGE_BMP: return "image/bmp";
        case IMAGE_WEBP: return "image/webp";
        case AUDIO_MPEG: return "audio/mpeg";
        case AUDIO_OGG: return "audio/ogg";
        case VIDEO_MP4: return "video/mp4";
        case VIDEO_WEBM: return "video/webm";
        case VIDEO_OGG: return "video/ogg";
        case FONT_TTF: return "font/ttf";
        case FONT_WOFF: return "font/woff";
        case FONT_WOFF2: return "font/woff2";
        case FONT_OTF: return "font/otf";
        case APPLICATION_XWWW_FORM_URLENCODED: return "application/x-www-form-urlencoded";
        case APPLICATION_OCTET_STREAM: return "application/octet-stream";
        default: return "unknown";
    }
}

content_type_t content_type_from_str(const char *str) {
    if (strcmp(str, "text/html") == 0) {
        return TEXT_HTML;
    }
    else if (strcmp(str, "text/plain") == 0) {
        return TEXT_PLAIN;
    }
    else if (strcmp(str, "text/css") == 0) {
        return TEXT_CSS;
    }
    else if (strcmp(str, "text/javascript") == 0) {
        return TEXT_JAVASCRIPT;
    }
    else if (strcmp(str, "application/json") == 0) {
        return APPLICATION_JSON;
    }
    else if (strcmp(str, "application/xml") == 0) {
        return APPLICATION_XML;
    }
    else if (strcmp(str, "application/javascript") == 0) {
        return APPLICATION_JAVASCRIPT;
    }
    else if (strcmp(str, "application/pdf") == 0) {
        return APPLICATION_PDF;
    }
    else if (strcmp(str, "application/zip") == 0) {
        return APPLICATION_ZIP;
    }
    else if (strcmp(str, "application/x-www-form-urlencoded") == 0) {
        return APPLICATION_FORM_URLENCODED;
    }
    else if (strcmp(str, "multipart/form-data") == 0) {
        return MULTIPART_FORM_DATA;
    }
    else if (strcmp(str, "image/jpeg") == 0) {
        return IMAGE_JPEG;
    }
    else if (strcmp(str, "image/png") == 0) {
        return IMAGE_PNG;
    }
    else if (strcmp(str, "image/gif") == 0) {
        return IMAGE_GIF;
    }
    else if (strcmp(str, "image/bmp") == 0) {
        return IMAGE_BMP;
    }
    else if (strcmp(str, "image/webp") == 0) {
        return IMAGE_WEBP;
    }
    else if (strcmp(str, "audio/mpeg") == 0) {
        return AUDIO_MPEG;
    }
    else if (strcmp(str, "audio/ogg") == 0) {
        return AUDIO_OGG;
    }
    else if (strcmp(str, "video/mp4") == 0) {
        return VIDEO_MP4;
    }
    else if (strcmp(str, "video/webm") == 0) {
        return VIDEO_WEBM;
    }
    else if (strcmp(str, "video/ogg") == 0) {
        return VIDEO_OGG;
    }
    else if (strcmp(str, "font/ttf") == 0) {
        return FONT_TTF;
    }
    else if (strcmp(str, "font/woff") == 0) {
        return FONT_WOFF;
    }
    else if (strcmp(str, "font/woff2") == 0) {
        return FONT_WOFF2;
    }
    else if (strcmp(str, "font/otf") == 0) {
        return FONT_OTF;
    }
    else if (strcmp(str, "application/octet-stream") == 0) {
        return APPLICATION_OCTET_STREAM;
    }
    else {
        return CONTENT_TYPE_UNKNOWN;
    }
}

const char* http_request_name(http_request_t http_request) {
    switch (http_request)
    {
        case GET: return "GET";
        case HEAD: return "HEAD";
        case OPTIONS: return "OPTIONS";
        case TRACE: return "TRACE";
        case PUT: return "PUT";
        case DELETE: return "DELETE";
        case POST: return "POST";
        case PATCH: return "PATCH";
        case CONNECT: return "CONNECT";
        default: return "unknown";
    }
}

http_request_t http_request_from_str(char *str) {
    if (strcmp(str, "GET") == 0) {
        return GET;
    }
    else if (strcmp(str, "HEAD") == 0) {
        return HEAD;
    }
    else if (strcmp(str, "OPTIONS") == 0) {
        return OPTIONS;
    }
    else if (strcmp(str, "TRACE") == 0) {
        return TRACE;
    }
    else if (strcmp(str, "PUT") == 0) {
        return PUT;
    }
    else if (strcmp(str, "DELETE") == 0) {
        return DELETE;
    }
    else if (strcmp(str, "POST") == 0) {
        return POST;
    }
    else if (strcmp(str, "PATCH") == 0) {
        return PATCH;
    }
    else if (strcmp(str, "CONNECT") == 0) {
        return CONNECT;
    }
    return -1;
}

const char* connection_type_name(connection_type_t conn_type) {
    switch (conn_type) {
        case CLOSE: return "close";
        case KEEP_ALIVE: return "keep-alive";
        case UPGRADE: return "upgrade";
        case CONNECTION_UNKNOWN: return "unknown";
        default: return "unknown";
    }
}

connection_type_t connection_type_from_str(const char* str) {
    if (strcmp(str, "close") == 0) {
        return CLOSE;
    }
    else if (strcmp(str, "keep-alive") == 0) {
        return KEEP_ALIVE;
    }
    else if (strcmp(str, "upgrade") == 0) {
        return UPGRADE;
    }
    else {
        return CONNECTION_UNKNOWN;
    }
}

const char *create_response(http_code_t http_code, content_type_t content_type, char *content) {
    char *response = (char*)default_allocator.allocate(SIZE_1024 * sizeof(char));
    sprintf(response, RESPONSE, http_code_name(http_code), content_type_name(content_type), strlen(content), content);
    return response;
}

static http_request_t get_request_type(char *raw_data) {
    char *temp = (char*)default_allocator.allocate(strlen(raw_data) * sizeof(char));
    strcpy(temp, raw_data);

    char *token = strtok(temp, " ");
    http_request_t response = -1;
    while (token != NULL && token[0] != '\0') {
        for_loop(i, 9) {
            if (strstr(token, http_request_name((http_request_t)i))) {
                response = (http_request_t)i;
                break;
            }
        }
        token = strtok(NULL, " ");
    }
 
    token = NULL;
    temp = NULL;
    return response;
}

static void tkncpy(char *dst, char *token, char *prefix) {
    char *pos = strstr(token, prefix);
    size_t prefix_len = strlen(prefix);
    char *temp = (char*)default_allocator.allocate(SIZE_1024 * sizeof(char));

    memcpy(temp, token, strlen(token) * sizeof(char));
    memmove(temp, pos + prefix_len, strlen(pos + prefix_len) + 1);
    strcpy(dst, temp);
    free(temp);
    temp = NULL;
    pos = NULL;
    prefix_len = 0;
}

http_content_t *http_content(char *raw_data, char **keys, int keys_count) {
    http_content_t *http_content = (http_content_t*)default_allocator.allocate(sizeof(http_content_t));
    char *token = strtok(raw_data, "\n");
    char *pos;
    size_t prefix_len;

    http_content->custom_data = (char**)default_allocator.allocate(keys_count * sizeof(char));
    bool content_len_filled = false;
    int content_index = 0;

    while (token != NULL && token[0] != '\0') {
        if (content_len_filled && strcmp(token, "\r") != 0) {
            remove_char((char)13, token);
            remove_char((char)10, token);

            if (token[0] != '\0') {
                for_loop(i, strlen(token)) {
                    http_content->content[content_index++] = token[i];
                }
            }
        }
        else {
            if (strstr(token, "HTTP")) {
                for_loop(i, 9) {
                    if (strstr(token, http_request_name((http_request_t)i))) {
                        http_content->request_type = (http_request_t)i;
                        break;
                    }
                }
            }
            else if (strstr(token, "Connection: ")) {
                for_loop(i, 4) {
                    if (strstr(token, connection_type_name((connection_type_t)i))) {
                        http_content->connection_type = (connection_type_t)i;
                        break;
                    }
                }
            }
            else if (strstr(token, "Content-Type: ")) {
                for_loop(i, 27) {
                    if (strstr(token, content_type_name((content_type_t)i))) {
                        http_content->content_type = (content_type_t)i;
                        break;
                    }
                }
            }
            else if (strstr(token, "Content-Length: ")) {
                char *temp = (char*)default_allocator.allocate(SIZE_1024 * sizeof(char));
                tkncpy(temp, token, "Content-Length: ");
                int len = atoi(temp);
                http_content->content_len = len;
                free(temp);
                temp = NULL;
                content_len_filled = true;
                http_content->content = (char*)default_allocator.allocate(len * sizeof(char));
                len = 0;
            }
            else if(strstr(token, "User-Agent: ")) {
                http_content->user_agent = (char*)default_allocator.allocate(SIZE_1024 * sizeof(char));
                tkncpy(http_content->user_agent, token, "User-Agent: ");
            }
            else if (strstr(token, "Accept: ")) {
                http_content->accept = (char*)default_allocator.allocate(SIZE_1024 * sizeof(char));
                tkncpy(http_content->accept, token, "Accept: ");
            }
            else if (strstr(token, "Host: ")) {
                http_content->host = (char*)default_allocator.allocate(SIZE_1024 * sizeof(char));
                tkncpy(http_content->host, token, "Host: ");
            }
            else if (strstr(token, "Accept-Encoding: ")) {
                http_content->accept_encoding = (char*)default_allocator.allocate(SIZE_1024 * sizeof(char));
                tkncpy(http_content->accept_encoding, token, "Accept-Encoding: ");
            }
            else {
                for_loop(i, keys_count) {
                    if (strstr(token, keys[i])) {
                        http_content->custom_data[i] = (char*)default_allocator.allocate(SIZE_1024 * sizeof(char));
                        tkncpy(http_content->custom_data[i], token, keys[i]);
                    }
                }
            }
        }

        token = strtok(NULL, "\n");
    }

    http_content->content[content_index] = '\0';
    return http_content;
}