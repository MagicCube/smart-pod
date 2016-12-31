#ifndef HTTP_CLIENT_2_H
#define HTTP_CLIENT_2_H

#include <ESP8266HTTPClient.h>

class HTTPClient2 : public HTTPClient
{
public:
    transferEncoding_t getTransferEncoding();
};

#endif
