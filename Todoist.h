#pragma once

#include <vector>

#include <SSLClient.h>
#include <ArduinoHttpClient.h>
#include <ArduinoJson.h>

#include "RestError.h"

class Todoist {
  private:
    static constexpr const char *_server = "api.todoist.com";
    static constexpr const uint16_t _port = 443;
    static constexpr const char *_get_tasks_path_work = "/rest/v2/tasks?filter=%23Work";
    static constexpr const char *_get_tasks_path_home = "/rest/v2/tasks?filter=%23Home";

    HttpClient _http;

    // Filter the response so we limit the size of temporary JSON document
    static StaticJsonDocument<32> _filter; // Got 32 from the assistant https://arduinojson.org/v6/assistant/
    static StaticJsonDocument<512> _json; // Got 128 from the assistant https://arduinojson.org/v6/assistant/

    RestError _getTasks(const char *path, std::vector<String> &tasks);

  public:
    Todoist(SSLClient &client);
    RestError getTasksWork(std::vector<String> &tasks);
    RestError getTasksHome(std::vector<String> &tasks);
};