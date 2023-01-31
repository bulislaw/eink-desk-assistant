#pragma once

#include <SSLClient.h>
#include <ArduinoHttpClient.h>
#include <ArduinoJson.h>

#include "HTTPQuote.h"

class ForexQuote: public HTTPQuote {
  private:
    static constexpr const char *_server = "api.apilayer.com";
    static constexpr const uint16_t _port = 443;

    static constexpr const char *_forex_path = "/exchangerates_data/convert?amount=1&from=GBP&to=";

    // Filter the response so we limit the size of temporary JSON document
    static StaticJsonDocument<16> _filter; // Got 16 from the assistant https://arduinojson.org/v6/assistant/
    static StaticJsonDocument<48> _json; // Got 48 from the assistant https://arduinojson.org/v6/assistant/

  public:
    ForexQuote(SSLClient &client);
    RestError fetchQuote(const char *symbol, Quote &quote);
};