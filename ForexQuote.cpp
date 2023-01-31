#include "ForexQuote.h"
#include "config.h"

StaticJsonDocument<16> ForexQuote::_filter;
StaticJsonDocument<48> ForexQuote::_json;

ForexQuote::ForexQuote(SSLClient &client) : HTTPQuote(client, _server, _port)
{
    // Set up JSON filter to only fetch relevant data
    _filter["result"] = true;
}
RestError ForexQuote::fetchQuote(const char *symbol, Quote &quote)
{
    String path;
    path += _forex_path;
    path += symbol;

    _http.beginRequest();
    _http.get(path);
    _http.sendHeader("apikey", FOREX_TOKEN);
    _http.endRequest();

    // read the status code and body of the response
    int statusCode = _http.responseStatusCode();
    if (statusCode != 200) {
        Serial.println("HTTP error: " + String(statusCode));
        return RestError::HTTP_ERROR;
    }
    String response = _http.responseBody();

    _json.clear();
    DeserializationError error = deserializeJson(_json, response, DeserializationOption::Filter(_filter));
    if (error) {
        return RestError::JSON_ERROR;
    }

    quote.price = _json["result"].as<float>();
    quote.previousClose = 0.0;
    quote.changeSincePreviousClose = 0.0;

    return RestError::OK;
}