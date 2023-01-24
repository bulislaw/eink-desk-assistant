#pragma once

#include <SSLClient.h>
#include <ArduinoHttpClient.h>
#include <ArduinoJson.h>

class Quote {
  public:
    float price;
    float previousClose;
    float changeSincePreviousClose;
};

class HTTPQuote {
  protected:
    HttpClient _http;

  public:
    typedef enum {
        OK = 0,
        HTTP_ERROR = -1,
        JSON_ERROR = -2
    } QuoteError;

    HTTPQuote(Client &client, const char *server, uint16_t port) : _http(client, server, port) {}

    virtual QuoteError fetchQuote(const char *symbol, Quote &quote) = 0;
};