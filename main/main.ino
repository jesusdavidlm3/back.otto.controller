#include <stdio.h>
#include <string.h>
// #include "esp_http_server.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_wifi.h"
// #include "tcpip_adapter.h"
#include "WiFi.h"
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "esp_system.h"
// #include "esp_event.h"
#include "WebServer.h"

// Declaramos identificadores para los logs
static const char *TAG_SERVER = "HTTP_SERVER";
static const char *TAG_WIFI = "Wifi";

const char* ssid = "Xiaomi_E370";
const char* pass = "15c-24.123";
WebServer server(3001);

void test_connection() {
  server.send(200);
}

void fordward_route() {
  server.send(200);
}

void backward_route() {
  server.send(200);
}

void left_route() {
  server.send(200);
}

void right_route() {
  server.send(200);
}

void stop_route() {
  server.send(200);
}

void setup(){
  Serial.begin(115200);

  // Configuramos el wifi
  Serial.println("Iniciando Wi-Fi en modo STA...");
  WiFi.begin(ssid, pass);       //Iniciamos el modulo de red
  while(WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.println( ".");
  }
  Serial.println("WiFi Conectado");

  IPAddress ip = WiFi.softAPIP();
  Serial.println(ip);

  server.on("/otto/fordward", fordward_route);
  server.on("/otto/backward", backward_route);
  server.on("/otto/left", left_route);
  server.on("/otto/right", right_route);
  server.on("/otto/stop", stop_route);
  server.on("/otto/test_connection", test_connection);

  server.begin();

  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA)); // Configurar como cliente (Station)
  // ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config_t)); // Aplicar la configuración
  ESP_ERROR_CHECK(esp_wifi_start()); // Iniciar Wi-Fi

}

void loop() {
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ESP_ERROR_CHECK(nvs_flash_init());
    }

    ESP_LOGI(TAG_SERVER, "Iniciando servidor HTTP...");
}