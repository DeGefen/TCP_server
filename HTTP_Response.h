#pragma once
#include "config.h"
#include "HTTP_Request.h"

class HTTP_Status: public exception {
public:
    enum StatusCode {
        // Informational responses
        CONTINUE = 100, SWITCHING_PROTOCOLS = 101,

        // Successful responses
        OK = 200, CREATED = 201, ACCEPTED = 202, NON_AUTHORITATIVE_INFORMATION = 203,
        NO_CONTENT = 204, RESET_CONTENT = 205, PARTIAL_CONTENT = 206,

        // Redirection messages
        MULTIPLE_CHOICES = 300, MOVED_PERMANENTLY = 301, FOUND = 302, SEE_OTHER = 303,
        NOT_MODIFIED = 304, TEMPORARY_REDIRECT = 307, PERMANENT_REDIRECT = 308,

        // Client error responses
        BAD_REQUEST = 400, UNAUTHORIZED = 401, PAYMENT_REQUIRED = 402, FORBIDDEN = 403,
        NOT_FOUND = 404, METHOD_NOT_ALLOWED = 405, NOT_ACCEPTABLE = 406, PROXY_AUTHENTICATION_REQUIRED = 407,
        REQUEST_TIMEOUT = 408, CONFLICT = 409,

        // Server error responses
        INTERNAL_SERVER_ERROR = 500, NOT_IMPLEMENTED = 501, BAD_GATEWAY = 502,
        SERVICE_UNAVAILABLE = 503, GATEWAY_TIMEOUT = 504, HTTP_VERSION_NOT_SUPPORTED = 505
    };

    const map<StatusCode, string> StatusText =
            {   {StatusCode::CONTINUE, "Continue"},
                {StatusCode::SWITCHING_PROTOCOLS, "Switching Protocols"},
                {StatusCode::OK, "OK"},
                {StatusCode::CREATED, "Created"},
                {StatusCode::ACCEPTED, "Accepted"},
                {StatusCode::NON_AUTHORITATIVE_INFORMATION, "Non-Authoritative Information"},
                {StatusCode::NO_CONTENT, "No Content"},
                {StatusCode::RESET_CONTENT, "Reset Content"},
                {StatusCode::PARTIAL_CONTENT, "Partial Content"},
                {StatusCode::MULTIPLE_CHOICES, "Multiple Choices"},
                {StatusCode::MOVED_PERMANENTLY, "Moved Permanently"},
                {StatusCode::FOUND, "Found"},
                {StatusCode::SEE_OTHER, "See Other"},
                {StatusCode::NOT_MODIFIED, "Not Modified"},
                {StatusCode::TEMPORARY_REDIRECT, "Temporary Redirect"},
                {StatusCode::PERMANENT_REDIRECT, "Permanent Redirect"},
                {StatusCode::BAD_REQUEST, "Bad Request"},
                {StatusCode::UNAUTHORIZED, "Unauthorized"},
                {StatusCode::PAYMENT_REQUIRED, "Payment Required"},
                {StatusCode::FORBIDDEN, "Forbidden"},
                {StatusCode::NOT_FOUND, "Not Found"},
                {StatusCode::METHOD_NOT_ALLOWED, "Method Not Allowed"},
                {StatusCode::NOT_ACCEPTABLE, "Not Acceptable"},
                {StatusCode::PROXY_AUTHENTICATION_REQUIRED, "Proxy Authentication Required"},
                {StatusCode::REQUEST_TIMEOUT, "Request Timeout"},
                {StatusCode::CONFLICT, "Conflict"},
                {StatusCode::INTERNAL_SERVER_ERROR, "Internal Server Error"},
                {StatusCode::NOT_IMPLEMENTED, "Not Implemented"},
                {StatusCode::BAD_GATEWAY, "Bad Gateway"},
                {StatusCode::SERVICE_UNAVAILABLE, "Service Unavailable"},
                {StatusCode::GATEWAY_TIMEOUT, "Gateway Timeout"},
                {StatusCode::HTTP_VERSION_NOT_SUPPORTED, "HTTP Version Not Supported"} };

    explicit HTTP_Status(StatusCode status = OK): statusCode(status) {};

private:
    StatusCode statusCode;

    const char* what() const noexcept override {
        return StatusText.at(statusCode).c_str();
    }

    [[nodiscard]] string extract() const noexcept {
        return to_string(statusCode) + " " + StatusText.at(statusCode);
    }
};

class HTTP_Response {
public:

    void recive(const HTTP_Request& request);
    char* extract();

private:
    string version;
    HTTP_Status status;
    map<string, string> headers;
    string body;
};
