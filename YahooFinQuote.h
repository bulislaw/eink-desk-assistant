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

class YahooFinQuote {
  private:
    HttpClient _http;

    static constexpr const char *_yahoo_fin_server = "query2.finance.yahoo.com";
    static constexpr const char *_yahoo_fin_path = "/v10/finance/quoteSummary/";
    static constexpr const char *_yahoo_fin_module = "?modules=price";

    // Filter the response so we limit the size of temporary JSON document
    static StaticJsonDocument<128> _filter; // Got 128 from the assistant https://arduinojson.org/v6/assistant/
    static StaticJsonDocument<256> _json; // Got 256 from the assistant https://arduinojson.org/v6/assistant/

  public:
    typedef enum {
        OK = 0,
        HTTP_ERROR = -1,
        JSON_ERROR = -2
    } YahooFinQuoteError;

    YahooFinQuote(SSLClient &client);
    YahooFinQuoteError fetchQuote(const char *symbol, Quote &quote);
};