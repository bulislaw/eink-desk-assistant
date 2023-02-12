#pragma once

#include <SSLClient.h>
#include <ArduinoHttpClient.h>
#include <ArduinoJson.h>

#include "RestError.h"

class Weather {
    public:
        uint condition_id;
        float temp;
        float feels_like;
};

class WeatherService {
  private:
    static constexpr const char *_server = "api.openweathermap.org";
    static constexpr const uint16_t _port = 443;

    static constexpr const char *_weather_path = "/data/2.5/weather";
    static constexpr const char *_location_param = "q=";
    static constexpr const char *_appid_param = "APPID=";
    static constexpr const char *_units_param = "units=metric";

    HttpClient _http;

    static StaticJsonDocument<256> _filter;
    static StaticJsonDocument<512> _json;

  public:
    WeatherService(SSLClient &client);
    RestError currentWeather(const char *location, Weather &weather);
};