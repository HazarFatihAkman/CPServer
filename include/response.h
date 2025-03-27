#ifndef RESPONSE_H
#define RESPONSE_H

#include "utilities.h"
#include <stdbool.h>

typedef enum http_code {
    CONTINUE = 100,
    SWITCHING_PROTOCOLS = 101,
    PROCCESING = 102,
    EARLY_HINTS = 103,
    OK = 200,
    CREATED = 201,
    NON_AUTHORITATIVE_INFORMATION = 203,
    NO_CONTENT = 204,
    RESET_CONTENT = 205,
    PARTIAL_CONTENT = 206,
    MULTI_STATUS = 207,
    ALREADY_REPORTED = 208,
    IM_USED = 226,
    MULTIPLE_CHOICES = 300,
    MOVED_PERMANENTLY = 301,
    FOUND = 302,
    SEE_OTHER = 303,
    NOT_MODIFIED = 304,
    UNUSED = 306,
    TEMPORARY_REDIRECT = 307,
    PERMANENT_REDIRECT = 308,
    BAD_REQUEST = 400,
    UNAUTHORIZED = 401,
    PAYMENT_REQUIRED = 402,
    NOT_FOUND = 404,
    METHOD_NOT_ALLOWED = 405,
    NOT_ACCEPTABLE = 406,
    PROXY_AUTHENTICATION_REQUIRED = 407,
    REQUEST_TIMEOUT = 408,
    CONFLICT = 409,
    GONE = 410,
    LENGTH_REQUIRED = 411,
    PRECONDITION_FAILED = 412,
    CONTENT_TOO_LARGE = 413,
    URI_TOO_LONG = 414,
    UNSUPPORTED_MEDIA_TYPE = 415,
    RANGE_NOT_SATISFIABLE = 416,
    EXPECTATION_FAILED = 417,
    MISDIRECTED_REQUEST = 421,
    UNPROCCESSABLE_CONTENT = 422,
    LOCKED = 423,
    FAILED_DEPENDENCY = 424,
    TOO_EARLY = 425,
    UPGRADE_REQUIRED = 426,
    PRECONDITION_REQUIRED = 428,
    TOO_MANY_REQUESTS = 429,
    REQUEST_HEADER_FIELDS_TOO_LARGE = 431,
    UNAVAILABLE_FOR_LEGAL_REASONS = 451,
    NOT_IMPLEMENTED = 501,
    BAD_GATEWAY = 502,
    SERVICE_UNAVAILABLE = 503,
    GATEWAY_TIMEOUT = 504,
    HTTP_VERSION_NOT_SUPPORTED = 505,
    VARIANT_ALSO_NEGOTIATES = 506,
    INSUFFICIENT_STORAGE = 507,
    LOOP_DETECTED = 508,
    NOT_EXTENDED = 510,
    NETWORK_AUTHENTICATION_REQUIRED = 511
} http_code_t;

typedef enum content_type {
    TEXT_HTML,
    TEXT_PLAIN,
    TEXT_CSS,
    TEXT_JAVASCRIPT,
    APPLICATION_JSON,
    APPLICATION_XML,
    APPLICATION_JAVASCRIPT,
    APPLICATION_PDF,
    APPLICATION_ZIP,
    APPLICATION_FORM_URLENCODED,
    MULTIPART_FORM_DATA,
    IMAGE_JPEG,
    IMAGE_PNG,
    IMAGE_GIF,
    IMAGE_BMP,
    IMAGE_WEBP,
    AUDIO_MPEG,
    AUDIO_OGG,
    VIDEO_MP4,
    VIDEO_WEBM,
    VIDEO_OGG,
    FONT_TTF,
    FONT_WOFF,
    FONT_WOFF2,
    FONT_OTF,
    APPLICATION_XWWW_FORM_URLENCODED,
    APPLICATION_OCTET_STREAM,
    CONTENT_TYPE_UNKNOWN
} content_type_t;

typedef enum http_request {
    GET,
    HEAD,
    OPTIONS,
    TRACE,
    PUT,
    DELETE,
    POST,
    PATCH,
    CONNECT
} http_request_t;

typedef enum connection_type {
    CLOSE,
    KEEP_ALIVE,
    UPGRADE,
    CONNECTION_UNKNOWN
} connection_type_t;

typedef struct http_content {
    http_request_t request_type;
    content_type_t content_type;
    connection_type_t connection_type;

    int content_len;
    char *content;

    char *user_agent;
    char *accept;
    char *host;
    char *accept_encoding;

    char **custom_data; // Like tokens, api keys...
} http_content_t;

#define RESPONSE "HTTP/1.1 %s\r\n"\
                 "Content-Type: %s\r\n"\
                 "Connection: keep-alive\r\n"\
                 "Content-Length: %d\r\n"\
                 "\r\n"\
                 "%s"

const char *create_response(http_code_t, content_type_t, char*);
http_content_t *http_content(char*, char**, int);

const char* http_code_name(http_code_t);
const char* content_type_name(content_type_t);
content_type_t content_type_from_str(const char*);
const char* http_request_name(http_request_t);
http_request_t http_request_from_str(char*);
const char* connection_type_name(connection_type_t);
connection_type_t connection_type_from_str(const char*);

#endif