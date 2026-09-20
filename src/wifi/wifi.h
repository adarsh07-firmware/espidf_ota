#ifndef WIFI_H
#define WIFI_H
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"

#include "lwip/err.h"
#include "lwip/sys.h"
#include "esp_log.h"
#include <string.h>

#define CONFIG_ESP_MAXIMUM_RETRY 5
#define EXAMPLE_ESP_MAXIMUM_RETRY  CONFIG_ESP_MAXIMUM_RETRY
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1
#define EXAMPLE_ESP_WIFI_SSID "CABINMATE"   
#define EXAMPLE_ESP_WIFI_PASS "CABINMATE1" 

void event_handler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data);

void wifi_init_sta(void);

#endif
