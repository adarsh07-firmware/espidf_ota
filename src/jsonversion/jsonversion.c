#include "jsonversion.h"
#include "esp_http_client.h"
#include <string.h>
#include <stdlib.h>

char response[128];
int response_len = 0;

esp_err_t version_http_handler(esp_http_client_event_t *evt)
{
    if (evt->event_id == HTTP_EVENT_ON_DATA)
    {
        if (response_len + evt->data_len < sizeof(response))
        {
            memcpy(response + response_len,
                   evt->data,
                   evt->data_len);

            response_len += evt->data_len;
        }
    }

    return ESP_OK;
}
int get_server_version(void)
{
    response_len = 0;
    memset(response, 0, sizeof(response));

    esp_http_client_config_t config = {
        .url = "http://192.168.0.17:8000/src/version.json",
        .event_handler = version_http_handler,
    };

    esp_http_client_handle_t client =
        esp_http_client_init(&config);

    esp_err_t err =
        esp_http_client_perform(client);

    if (err != ESP_OK)
    {
        esp_http_client_cleanup(client);
        return -1;
    }

    response[response_len] = '\0';

    const char *version_key = strstr(response, "\"version\"");
    if (version_key == NULL)
    {
        esp_http_client_cleanup(client);
        return -1;
    }

    const char *colon = strchr(version_key, ':');
    if (colon == NULL)
    {
        esp_http_client_cleanup(client);
        return -1;
    }

    char *end = NULL;
    long parsed_version = strtol(colon + 1, &end, 10);
    if (end == colon + 1 || parsed_version < 0 || parsed_version > 2147483647L)
    {
        esp_http_client_cleanup(client);
        return -1;
    }

    esp_http_client_cleanup(client);

    return (int)parsed_version;
}
