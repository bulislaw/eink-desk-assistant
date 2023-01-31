#include "Todoist.h"
#include "config.h"

StaticJsonDocument<32> Todoist::_filter;
StaticJsonDocument<512> Todoist::_json;

Todoist::Todoist(SSLClient &client) : _http(client, _server, _port) {
    _filter[0]["content"] = true;
}

RestError Todoist::getTasksWork(std::vector<String> &tasks) {
    return _getTasks(_get_tasks_path_work, tasks);
}

RestError Todoist::getTasksHome(std::vector<String> &tasks) {
    return _getTasks(_get_tasks_path_home, tasks);
}

RestError Todoist::_getTasks(const char *path, std::vector<String> &tasks) {
    _http.beginRequest();
    _http.get(path);
    _http.sendHeader("Authorization", "Bearer " TODOIST_TOKEN);
    _http.endRequest();

    int statusCode = _http.responseStatusCode();
    if (statusCode != 200) {
        Serial.println("HTTP error: " + String(statusCode));
        return RestError::HTTP_ERROR;
    }

    _http.skipResponseHeaders();
    DeserializationError error = deserializeJson(_json, _http, DeserializationOption::Filter(_filter));
    if (error) {
        Serial.println("HTTP error: " + String(statusCode));
        return RestError::JSON_ERROR;
    }

    for (JsonObject task : _json.as<JsonArray>()) {
        tasks.push_back(task["content"].as<String>());
    }

    return RestError::OK;
}
