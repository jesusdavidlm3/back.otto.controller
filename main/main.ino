#include <stdio.h>
#include <string.h>
#include "esp_http_server.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_wifi.h"
// #include "tcpip_adapter.h"
#include "WiFi.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_event.h"

// Declaramos identificadores para los logs
static const char *TAG_SERVER = "HTTP_SERVER";
static const char *TAG_WIFI = "Wifi";

const char* ssid = "Xiaomi_E370";
const char* pass = "15c-24.123";

esp_err_t test_connection(httpd_req_t *req) {
    const char *response_message = "Conectado";
    ESP_LOGI(TAG_SERVER, "Conectado");
    httpd_resp_set_status(req, "200 OK");
    httpd_resp_set_type(req, "application/json");
    httpd_resp_send(req, response_message, strlen(response_message));
    return ESP_OK;
}

esp_err_t fordward_route(httpd_req_t *req) {
    const char *response_message = "Otto esta avanzando";
    ESP_LOGI(TAG_SERVER, "Otto esta avanzando");
    httpd_resp_set_status(req, "200 OK");
    httpd_resp_set_type(req, "application/json");
    httpd_resp_send(req, response_message, strlen(response_message));
    return ESP_OK;
}

esp_err_t backward_route(httpd_req_t *req) {
    const char *response_message = "Otto esta retrocediendo";
    ESP_LOGI(TAG_SERVER, "Otto esta retrocediendo");
    httpd_resp_set_status(req, "200 OK");
    httpd_resp_set_type(req, "application/json");
    httpd_resp_send(req, response_message, strlen(response_message));
    return ESP_OK;
}

esp_err_t left_route(httpd_req_t *req) {
    const char *response_message = "Otto esta girando a la izquierda";
    ESP_LOGI(TAG_SERVER, "Otto esta girando a la izquierda");
    httpd_resp_set_status(req, "200 OK");
    httpd_resp_set_type(req, "application/json");
    httpd_resp_send(req, response_message, strlen(response_message));
    return ESP_OK;
}

esp_err_t right_route(httpd_req_t *req) {
    const char *response_message = "Otto esta girando a la derecha";
    ESP_LOGI(TAG_SERVER, "Otto esta girando a la derecha");
    httpd_resp_set_status(req, "200 OK");
    httpd_resp_set_type(req, "application/json");
    httpd_resp_send(req, response_message, strlen(response_message));
    return ESP_OK;
}

esp_err_t stop_route(httpd_req_t *req) {
    const char *response_message = "Otto esta en espera";
    ESP_LOGI(TAG_SERVER, "Otto esta en espera");
    httpd_resp_set_status(req, "200 OK");
    httpd_resp_set_type(req, "application/json");
    httpd_resp_send(req, response_message, strlen(response_message));
    return ESP_OK;
}


// Configuracion del servidor http y sus rutas
httpd_handle_t start_server(void) {
  httpd_config_t config = HTTPD_DEFAULT_CONFIG();
  config.server_port = 3001; 

  httpd_handle_t server = NULL;

  if (httpd_start(&server, &config) == ESP_OK) {
      httpd_uri_t uri_test_connection = {
          .uri = "/otto/testConnection",
          .method = HTTP_GET,
          .handler = test_connection,
          .user_ctx = NULL
      };
      httpd_register_uri_handler(server, &uri_test_connection);

      httpd_uri_t uri_fordward_route = {
          .uri = "/otto/fordward",
          .method = HTTP_GET,
          .handler = fordward_route,
          .user_ctx = NULL
      };
      httpd_register_uri_handler(server, &uri_fordward_route);

      httpd_uri_t uri_backward_route = {
          .uri = "/otto/fordward",
          .method = HTTP_GET,
          .handler = backward_route,
          .user_ctx = NULL
      };
      httpd_register_uri_handler(server, &uri_backward_route);

      httpd_uri_t uri_left_route = {
          .uri = "/otto/fordward",
          .method = HTTP_GET,
          .handler = left_route,
          .user_ctx = NULL
      };
      httpd_register_uri_handler(server, &uri_left_route);

      httpd_uri_t uri_right_route = {
          .uri = "/otto/fordward",
          .method = HTTP_GET,
          .handler = right_route,
          .user_ctx = NULL
      };
      httpd_register_uri_handler(server, &uri_right_route);

      httpd_uri_t uri_stop_route = {
          .uri = "/otto/fordward",
          .method = HTTP_GET,
          .handler = stop_route,
          .user_ctx = NULL
      };
      httpd_register_uri_handler(server, &uri_stop_route);
  } else {
      ESP_LOGE(TAG_SERVER, "Fallo al iniciar el servidor HTTP");
  }
  return server;
}

void setup(){
  // Configuramos el wifi
  ESP_LOGI(TAG_WIFI, "Iniciando Wi-Fi en modo STA...");
  WiFi.begin(ssid, pass);       //Iniciamos el modulo de red
  while(WiFi.status() != WL_CONNECTED){
    delay(1000);
    ESP_LOGI(TAG_WIFI, ".");
  }
  ESP_LOGI(TAG_WIFI, "WiFi Conectado");

  // ESP_ERROR_CHECK(esp_event_loop_create_default());
  esp_event_loop_create_default();


  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA)); // Configurar como cliente (Station)
  // ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config_t)); // Aplicar la configuración
  ESP_ERROR_CHECK(esp_wifi_start()); // Iniciar Wi-Fi

  ESP_LOGI(TAG, "Conexión Wi-Fi iniciada, conectando a la red: %s", wifi_config.sta.ssid);  
}

void loop() {
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ESP_ERROR_CHECK(nvs_flash_init());
    }

    ESP_LOGI(TAG_SERVER, "Iniciando servidor HTTP...");
    // start_server();
}