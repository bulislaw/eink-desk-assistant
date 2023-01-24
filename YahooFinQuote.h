#pragma once

#include <SSLClient.h>
#include <ArduinoHttpClient.h>
#include <ArduinoJson.h>

#include "HTTPQuote.h"

class YahooFinQuote: public HTTPQuote {
  private:
    static constexpr const char *_server = "query2.finance.yahoo.com";
    static constexpr const uint16_t _port = 443;

    static constexpr const char *_yahoo_fin_path = "/v10/finance/quoteSummary/";
    static constexpr const char *_yahoo_fin_module = "?modules=price";

    // Filter the response so we limit the size of temporary JSON document
    static StaticJsonDocument<128> _filter; // Got 128 from the assistant https://arduinojson.org/v6/assistant/
    static StaticJsonDocument<256> _json; // Got 256 from the assistant https://arduinojson.org/v6/assistant/

  public:
    YahooFinQuote(SSLClient &client);
    QuoteError fetchQuote(const char *symbol, Quote &quote);
};