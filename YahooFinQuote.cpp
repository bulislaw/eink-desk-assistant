#include "YahooFinQuote.h"

StaticJsonDocument<128> YahooFinQuote::_filter;
StaticJsonDocument<256> YahooFinQuote::_json;

YahooFinQuote::YahooFinQuote(SSLClient &client) : HTTPQuote(client, _server, _port)
{
    // Set up JSON filter to only fetch relevant data
    _filter["quoteSummary"]["result"][0]["price"]["regularMarketPrice"]["raw"] = true;
    _filter["quoteSummary"]["result"][0]["price"]["regularMarketPreviousClose"]["raw"] = true;
}

HTTPQuote::QuoteError YahooFinQuote::fetchQuote(const char *symbol, Quote &quote)
{
    String path;
    path += _yahoo_fin_path;
    path += symbol;
    path += _yahoo_fin_module;

    _http.get(path);

    // read the status code and body of the response
    int statusCode = _http.responseStatusCode();
    if (statusCode != 200) {
        return HTTP_ERROR;
    }
    String response = _http.responseBody();

    _json.clear();
    DeserializationError error = deserializeJson(_json, response, DeserializationOption::Filter(_filter));
    if (error) {
        return JSON_ERROR;
    }

    quote.price = _json["quoteSummary"]["result"][0]["price"]["regularMarketPrice"]["raw"].as<float>();
    quote.previousClose = _json["quoteSummary"]["result"][0]["price"]["regularMarketPreviousClose"]["raw"].as<float>();
    quote.changeSincePreviousClose = quote.price * 100.0 / quote.previousClose - 100;

    return OK;
}