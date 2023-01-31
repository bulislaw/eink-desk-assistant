#pragma once

#include <SSLClient.h>
#include <ArduinoHttpClient.h>
#include <ArduinoJson.h>

#include "RestError.h"

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
    HTTPQuote(Client &client, const char *server, uint16_t port) : _http(client, server, port) {}

    virtual RestError fetchQuote(const char *symbol, Quote &quote) = 0;
};