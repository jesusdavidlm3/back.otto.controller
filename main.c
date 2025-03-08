#include <stdio.h>
#include <string.h>
#include "esp_http_server.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "tcpip_adapter.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_event.h"

// Declaramos identificadores para los logs
static const char *TAG_SERVER = "HTTP_SERVER";
static const char *TAG_WIFI = "Wifi";

// Configuracion del wifi
void wifi_init(){
    ESP_LOGI(TAG_WIFI, "Iniciando Wi-Fi en modo STA...");
    tcpip_adapter_init();       //Iniciamos el modulo de red
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();    //Inciamos modulo wifi
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    wifi_config_t wifi_config = {       //Establecemos la configuracion de la red
        .sta{
            .ssid: "Xiaomi_E370",
            .password: "15c-24.123"
        }
    }

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA)); // Configurar como cliente (Station)
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config)); // Aplicar la configuración
    ESP_ERROR_CHECK(esp_wifi_start()); // Iniciar Wi-Fi

    ESP_LOGI(TAG, "Conexión Wi-Fi iniciada, conectando a la red: %s", wifi_config.sta.ssid);
}

// Configuracion del servidor http y sus rutas

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

//rutina para arrancar el servidor HTTP
httpd_handle_t start_server(void) {
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.server_port = 3001; 

    httpd_handle_t server = NULL;

    if (httpd_start(&server, &config) == ESP_OK) {
        httpd_uri_t test_connection = {
            .uri = "/otto/testConnection",
            .method = HTTP_GET,
            .handler = test_connection,
            .user_ctx = NULL
        };
        httpd_register_uri_handler(server, test_connection);

        httpd_uri_t fordward_route = {
            .uri = "/otto/fordward",
            .method = HTTP_GET,
            .handler = fordward_route,
            .user_ctx = NULL
        };
        httpd_register_uri_handler(server, fordward_route);

        httpd_uri_t backward_route = {
            .uri = "/otto/fordward",
            .method = HTTP_GET,
            .handler = backward_route,
            .user_ctx = NULL
        };
        httpd_register_uri_handler(server, backward_route);

        httpd_uri_t left_route = {
            .uri = "/otto/fordward",
            .method = HTTP_GET,
            .handler = left_route,
            .user_ctx = NULL
        };
        httpd_register_uri_handler(server, left_route);

        httpd_uri_t right_route = {
            .uri = "/otto/fordward",
            .method = HTTP_GET,
            .handler = right_route,
            .user_ctx = NULL
        };
        httpd_register_uri_handler(server, right_route);

        httpd_uri_t stop_route = {
            .uri = "/otto/fordward",
            .method = HTTP_GET,
            .handler = stop_route,
            .user_ctx = NULL
        };
        httpd_register_uri_handler(server, stop_route);
    } else {
        ESP_LOGE(TAG_SERVER, "Fallo al iniciar el servidor HTTP");
    }
    return server;
}

void main(void) {
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ESP_ERROR_CHECK(nvs_flash_init());
    }

    wifi_init()

    ESP_LOGI(TAG_SERVER, "Iniciando servidor HTTP...");
    start_server();
}