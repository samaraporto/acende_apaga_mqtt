# Projeto de Controle de LED com ESP32 e MQTT

## Descrição do Projeto

Este projeto, desenvolvido para a disciplina de Sistemas Embarcados do IFPE, demonstra o controle remoto de um LED na placa de desenvolvimento ESP32 através do protocolo MQTT (Message Queuing Telemetry Transport).

O firmware, desenvolvido utilizando o framework ESP-IDF, conecta o ESP32 a uma rede Wi-Fi e a um broker MQTT público. O dispositivo se inscreve em um tópico específico e aguarda por mensagens para acender ("1") ou apagar ("0") o LED integrado.

## Funcionalidades

* Conexão automática a uma rede Wi-Fi configurada.
* Conexão a um broker MQTT público (HiveMQ).
* Inscrição (subscribe) no tópico: `/ifpe/ads/embarcados/esp32/led`.
* Controle do estado do LED integrado na placa (GPIO 2).
* Logs detalhados no monitor serial para facilitar o debug.

## Hardware Necessário

* 1x Placa de Desenvolvimento ESP32.
* 1x Cabo Micro-USB C para dados e alimentação.

## Software e Ferramentas

* Visual Studio Code.
* Extensão ESP-IDF para VS Code.
* HiveMQ Web Client

# Como Executar o Projeto

Siga os passos abaixo para compilar e executar o projeto na sua placa ESP32.

### - Criar um projeto MQTT5 usando um template da extenção ESP-IDF:

1.  Abra a extenção > New project > selecionar o seu ESP-IDF > defina nome do projeto e escolha uma pasta > click em Choose Template > escolha ESP-IDF e busque por 'mqtt5' > 'create project using template mqtt5' > Project <<nome do projeto>> has been created. Open project in a new window? 'yes'

2.  Abra a pasta do projeto no Visual Studio Code com a extensão ESP-IDF instalada. Copie o código do arquivo `app_main.c` deste repositório no arquivo `app_main.c` dentro da pasta `main` do seu projeto.

### 3. Configurar Credenciais de Rede e Broker

O projeto utiliza o `menuconfig` do ESP-IDF para configurar dados sensíveis sem expô-los no código.

* Abra o editor de configuração no VS Code (atalho `Ctrl + E` seguido de `C`, ou clique no ícone de engrenagem no rodapé).
* No menu de configuração, navegue até **Example Connection Configuration**:
    * Insira o **SSID** (nome) da sua rede em `WiFi SSID`.
    * Insira a **senha** da sua rede em `WiFi Password`.
* Navegue até **Example Configuration**:
    * Verifique se o campo `Broker URL` está configurado como `mqtt://broker.hivemq.com`.
* Salve as configurações e feche o editor.

### 4. Compilar, Gravar e Monitorar

* Conecte a placa ESP32 ao seu computador.
* No VS Code, verifique se a porta serial (COM) correta está selecionada no rodapé.
* Clique no ícone de chama (🔥) na barra de status inferior (`ESP-IDF: Build, Flash and Monitor`).
* Aguarde o processo terminar. O monitor serial será aberto e exibirá os logs de conexão.

### 5. Testar o Funcionamento

Após o ESP32 se conectar ao Wi-Fi e ao broker, ele estará pronto para receber comandos.

* Abra o seu cliente MQTT de preferência (ex: HiveMQ Web Client).
    * `https://www.hivemq.com/demos/websocket-client/`
* Conecte-se ao mesmo broker: `broker.hivemq.com` na porta `1883`.
* Na seção de **Publicar (Publish)**:
    * **Tópico (Topic):** `/ifpe/ads/embarcados/esp32/led`
    * **Mensagem (Payload/Message):**
        * Envie `1` para acender o LED.
        * Envie `0` para apagar o LED.

O LED na sua placa deverá responder instantaneamente aos comandos enviados.

---
**Autora:** Samara Porto de Noronha