#ifndef ESP_OTA_H
#define ESP_OTA_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_http_client.h"
#include "esp_https_ota.h"

#define URL "http://192.168.0.17:8000/.pio/build/esp32-c6-devkitm-1/firmware.bin"
// Runs synchronously; returns on failure and restarts on success.
void perform_ota_update(int target_version);
#endif
