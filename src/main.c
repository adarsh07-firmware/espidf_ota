#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/event_groups.h>
#include <wifi/wifi.h>
#include <ota/esp_ota.h>
#include <nvs.h>
#include <nvs_flash.h>
#include "jsonversion/jsonversion.h"


int get_current_version(void)
{
    nvs_handle_t handle;
    int32_t version = 1;

    esp_err_t err = nvs_open(
        "ota_info",
        NVS_READWRITE,
        &handle
    );

    if (err != ESP_OK)
    {
        printf("Failed to open NVS\n");
        return 1;
    }

    err = nvs_get_i32(
        handle,
        "version",
        &version
    );

    if (err == ESP_ERR_NVS_NOT_FOUND)
    {
        // First time: version does not exist yet
        version = 1;

        printf("First boot. Setting current version to 1\n");

        ESP_ERROR_CHECK(
            nvs_set_i32(
                handle,
                "version",
                version
            )
        );

        ESP_ERROR_CHECK(
            nvs_commit(handle)
        );
    }

    nvs_close(handle);

    return version;
}



static void ota_update_task(void *pvParameter)
{
    (void)pvParameter;
    while (1) {
        int current_version = get_current_version();
        int server_version = get_server_version();
        printf("Current version: %d, Server version: %d\n", current_version, server_version);

        if (server_version < 0) {
            printf("Version check failed; will retry\n");
        } else if (current_version < server_version) {
            // This call finishes before another version check can begin.
            perform_ota_update(server_version);
        } else {
            printf("Current version is up to date\n");
        }
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}

void app_main(){
   esp_err_t ret = nvs_flash_init();
   if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
       ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
   {
       ESP_ERROR_CHECK(nvs_flash_erase());
       ESP_ERROR_CHECK(nvs_flash_init());
   }

   wifi_init_sta();
   if (xTaskCreate(ota_update_task, "ota_update_task", 8192, NULL, 5, NULL) != pdPASS) {
       printf("Failed to create OTA update task\n");
   }
}
