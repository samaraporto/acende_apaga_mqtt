/*
 * Controle de LED com ESP32 via MQTT v5
 *
 * Autor: Samara Porto de Noronha
 * Disciplina: Sistemas Embarcados
 * Professor: Dr. David Alain
 * Instituição: IFPE - Campus Garanhuns
 * 06/2025
 *
 * Descrição:
 * Este projeto implementa um cliente MQTT em um microcontrolador ESP32
 * utilizando o framework ESP-IDF. O dispositivo se conecta a uma rede Wi-Fi e a um
 * broker MQTT público para controlar o estado do LED integrado na placa.
 *
 * Tópico de Inscrição (Subscribe): /ifpe/ads/embarcados/esp32/led
 * Comandos:
 * - "1": Acende o LED.
 * - "0": Apaga o LED.
 *
 */

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "protocol_examples_common.h"
#include "esp_log.h"
#include "mqtt_client.h"

// --- SEÇÃO DE DEFINIÇÕES DO HARDWARE ---
#include "driver/gpio.h"    // Biblioteca para controlar os pinos de Entrada/Saída (GPIO)
#define LED_PIN GPIO_NUM_2  // Define o pino do LED. O LED azul na maioria das placas DevKit C está no pino 2.

// Tag para facilitar a visualização dos logs no monitor serial.
static const char *TAG = "MQTT_LED_CONTROLLER";

/**
 * @brief Função de callback para tratar todos os eventos gerados pelo cliente MQTT.
 *
 * Esta função é registrada no cliente MQTT e é chamada automaticamente pela
 * biblioteca sempre que um evento ocorre (ex: conexão, desconexão, recebimento de dados).
 *
 * @param handler_args Argumentos do usuário (não utilizado neste exemplo).
 * @param base Base do evento (sempre MQTT neste exemplo).
 * @param event_id O ID do evento que ocorreu.
 * @param event_data Os dados associados ao evento.
 */
static void mqtt5_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    // Converte os dados do evento para um formato mais fácil de usar.
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;

    // Estrutura switch-case para tratar cada tipo de evento MQTT.
    switch ((esp_mqtt_event_id_t)event_id) {
    
    // Evento disparado quando a conexão com o broker é estabelecida com sucesso.
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED: Conectado com sucesso ao broker MQTT.");
        
        // Assim que conectar, nos inscrevemos no tópico para receber os comandos.
        // A Qualidade de Serviço (QoS) 1 garante que a mensagem será entregue pelo menos uma vez.
        msg_id = esp_mqtt_client_subscribe(client, "/ifpe/ads/embarcados/esp32/led", 1);
        ESP_LOGI(TAG, "Inscrição enviada para o tópico /ifpe/ads/embarcados/esp32/led, msg_id=%d", msg_id);
        break;

    // Evento disparado quando o cliente MQTT é desconectado do broker.
    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED: Desconectado do broker MQTT.");
        break;

    // Evento disparado quando o broker confirma a inscrição em um tópico.
    case MQTT_EVENT_SUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
        ESP_LOGI(TAG, "ESP32 pronto para receber comandos no tópico do LED.");
        break;

    // Evento disparado quando o recebimento de uma mensagem (publicação) é concluído.
    case MQTT_EVENT_DATA:
        ESP_LOGI(TAG, "MQTT_EVENT_DATA: Mensagem recebida!");
        // Imprime o tópico e o dado recebido para facilitar o debug.
        printf("TOPICO: %.*s\r\n", event->topic_len, event->topic);
        printf("DADO: %.*s\r\n", event->data_len, event->data);

        // --- LÓGICA PRINCIPAL DE CONTROLE ---
        // 1. Compara se o tópico da mensagem recebida é o que nos interessa.
        //    'event->topic' não tem um terminador nulo, por isso usamos 'strncmp' com o tamanho do tópico.
        if (strncmp(event->topic, "/ifpe/ads/embarcados/esp32/led", event->topic_len) == 0) {
            ESP_LOGI(TAG, "Comando recebido para o LED!");
            
            // 2. Compara o conteúdo (payload) da mensagem para decidir se liga ou desliga o LED.
            if (strncmp(event->data, "1", event->data_len) == 0) {
                ESP_LOGI(TAG, "Comando: LIGAR. Acendendo o LED.");
                gpio_set_level(LED_PIN, 1); // Acende o LED
            } else if (strncmp(event->data, "0", event->data_len) == 0) {
                ESP_LOGI(TAG, "Comando: DESLIGAR. Apagando o LED.");
                gpio_set_level(LED_PIN, 0); // Apaga o LED
            } else {
                ESP_LOGW(TAG, "Comando desconhecido: '%.*s'. Use '1' para ligar ou '0' para desligar.", event->data_len, event->data);
            }
        }
        break;

    // Evento disparado em caso de erro na comunicação MQTT.
    case MQTT_EVENT_ERROR:
        ESP_LOGE(TAG, "MQTT_EVENT_ERROR: Ocorreu um erro.");
        break;

    default:
        // Outros eventos não tratados neste exemplo.
        ESP_LOGI(TAG, "Outro evento MQTT recebido, id:%d", event->event_id);
        break;
    }
}

/**
 * @brief Inicializa e inicia o cliente MQTT.
 */
static void mqtt_app_start(void)
{
    // Estrutura de configuração do cliente MQTT.
    esp_mqtt_client_config_t mqtt_cfg = {
        // A URL do broker é lida do menuconfig (CONFIG_BROKER_URL).
        .broker.address.uri = CONFIG_BROKER_URL,
        // Define a versão do protocolo a ser usada.
        .session.protocol_ver = MQTT_PROTOCOL_V_5,
    };

    // Inicializa o cliente MQTT com as configurações definidas.
    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);

    // Registra a nossa função 'mqtt5_event_handler' para tratar todos os eventos do cliente.
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt5_event_handler, NULL);
    
    // Inicia o cliente MQTT. Ele tentará se conectar ao broker em segundo plano.
    esp_mqtt_client_start(client);
}

/**
 * @brief Função principal da aplicação. Ponto de entrada do programa.
 */
void app_main(void)
{
    ESP_LOGI(TAG, "[APP] Inicializando a aplicação...");

    // --- CONFIGURAÇÃO INICIAL DO HARDWARE E SISTEMA ---
    
    // Configura o pino do LED definido em LED_PIN como uma saída digital.
    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    ESP_LOGI(TAG, "Pino do LED (GPIO %d) configurado como saída.", LED_PIN);

    // Inicializa o NVS (Non-Volatile Storage), necessário para o Wi-Fi salvar configurações.
    ESP_ERROR_CHECK(nvs_flash_init());
    // Inicializa a pilha de rede (TCP/IP).
    ESP_ERROR_CHECK(esp_netif_init());
    // Cria o loop de eventos padrão do sistema.
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    // --- CONEXÃO COM A REDE ---
    
    // Função auxiliar do ESP-IDF que conecta ao Wi-Fi usando as credenciais do menuconfig.
    ESP_ERROR_CHECK(example_connect());

    // --- INICIALIZAÇÃO DO MQTT ---
    mqtt_app_start();
}
