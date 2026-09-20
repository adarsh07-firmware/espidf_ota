
#include "esp_ota.h"
#include "esp_system.h"
#include "esp_ota_ops.h"
#include "nvs.h"
#include "nvs_flash.h"

static const char *TAG1 = "OTA";

void save_current_version(int version)
{
    nvs_handle_t handle;

    esp_err_t err = nvs_open("ota_info", NVS_READWRITE, &handle);

    if (err != ESP_OK) {
        printf("Failed to open NVS\n");
        return;
    }

    nvs_set_i32(handle, "version", version);

    nvs_commit(handle);

    nvs_close(handle);

    printf("Saved version: %d\n", version);
}


void perform_ota_update(int target_version){
    ESP_LOGI(TAG1, "Starting OTA update to version %d", target_version);
    esp_http_client_config_t config = {
        .url = URL,
    };
    esp_https_ota_config_t ota_config = {
        .http_config = &config,
    };
    ESP_LOGI(TAG1, "Attempting to download update from %s", config.url);

    const esp_partition_t *running =
        esp_ota_get_running_partition();
    
    ESP_LOGI(TAG1,
             "Running partition: %s",
             running->label);
    
    const esp_partition_t *next =
        esp_ota_get_next_update_partition(NULL);
    
    if (next == NULL)
    {
        ESP_LOGE(TAG1, "No OTA partition available");
        return;
    }
    else
    {
        ESP_LOGI(TAG1,
                 "Next OTA partition: %s",
                 next->label);
    }
    esp_err_t ret = esp_https_ota(&ota_config);
        if (ret == ESP_OK) {
        ESP_LOGI(TAG1, "OTA Succeed, Rebooting...");
        save_current_version(target_version);
        esp_restart();
    } else {
        ESP_LOGE(TAG1, "Firmware upgrade failed: %s; will retry", esp_err_to_name(ret));
    }
}
