#include "Weather.h"

#include "config.h"

StaticJsonDocument<256> WeatherService::_filter;
StaticJsonDocument<512> WeatherService::_json;

WeatherService::WeatherService(SSLClient &client) : _http(client, _server, _port)
{
    // Filter json: {"weather": [{"id":true, "main": true}], "main": {"temp": true, "feels_like":true}}
    _filter["weather"][0]["id"] = true;
    _filter["weather"][0]["main"] = true;
    _filter["main"]["temp"] = true;
    _filter["main"]["feels_like"] = true;
}

RestError WeatherService::currentWeather(const char *location, Weather &weather)
{
    String path = String(_weather_path) + "?" + _location_param + location + "&" + _appid_param + OPENWEATHERMAP_TOKEN + "&" + _units_param;
    _http.get(path);

    int statusCode = _http.responseStatusCode();
    if (statusCode != 200) {
        Serial.println("HTTP error: " + String(statusCode));
        return RestError::HTTP_ERROR;
    }

    _http.skipResponseHeaders();
    String response = _http.responseBody();
    DeserializationError error = deserializeJson(_json, response, DeserializationOption::Filter(_filter));
    if (error) {
        Serial.println("JSON error: " + String(statusCode));
        return RestError::JSON_ERROR;
    }

    _http.stop();

    weather.condition_id = _json["weather"][0]["id"];
    weather.temp = _json["main"]["temp"];
    weather.feels_like = _json["main"]["feels_like"];

    return RestError::OK;
}