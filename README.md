# Projeto de Controle de LED com ESP32 e MQTT

## 📝 Descrição do Projeto

Este projeto, desenvolvido para a disciplina de Sistemas Embarcados do IFPE, demonstra o controle remoto de um LED na placa de desenvolvimento ESP32 através do protocolo MQTT (Message Queuing Telemetry Transport).

O firmware, desenvolvido utilizando o framework ESP-IDF, conecta o ESP32 a uma rede Wi-Fi e a um broker MQTT público. O dispositivo se inscreve em um tópico específico e aguarda por mensagens para acender ("1") ou apagar ("0") o LED integrado.

✨ Funcionalidades

Conexão automática a uma rede Wi-Fi configurada.

Conexão a um broker MQTT público (HiveMQ).

Inscrição (subscribe) no tópico: /ifpe/ads/embarcados/esp32/led.

Controle do estado do LED integrado na placa (GPIO 2).

Logs detalhados no monitor serial para facilitar o debug.

## 🛠️ Hardware Necessário

1x Placa de Desenvolvimento ESP32 (ex: ESP32-DevKitC V4).

1x Cabo Micro-USB para dados e alimentação.

## 💻 Software e Ferramentas

Visual Studio Code: Editor de código.

Extensão ESP-IDF para VS Code: Para desenvolvimento, compilação e gravação no ESP32.

Um Cliente MQTT para teste:

HiveMQ Web Client: Ferramenta online, sem necessidade de instalação.

🚀 Como Executar o Projeto

Siga os passos abaixo para compilar e executar o projeto na sua placa ESP32.

- Criar um projeto MQTT5 usando um template da extenção ESP-IDF:

Abra a extenção > New project > selecionar o seu esp-idf > defina nome do projeto e escolha uma pasta > click em Choose Template > escolha ESP-IDF e busque por 'mqtt5' > 'create project using template mqtt5' > Project projetoUnidade1 has been created. Open project in a new window? 'yes'

ou

1. Clonar o Repositório

git clone https://github.com/samaraporto/acende_apaga_mqtt

cd acende_apaga_mqtt

2. Abrir no VS Code

Abra a pasta do projeto no Visual Studio Code com a extensão ESP-IDF instalada.

3. Configurar Credenciais de Rede e Broker
O projeto utiliza o menuconfig do ESP-IDF para configurar dados sensíveis sem expô-los no código.

Abra o editor de configuração no VS Code (atalho Ctrl + E seguido de C, ou clique no ícone de engrenagem no rodapé).

No menu de configuração, navegue até Example Connection Configuration:

Insira o SSID (nome) da sua rede em WiFi SSID.

Insira a senha da sua rede em WiFi Password.

Navegue até Example Configuration:

Verifique se o campo Broker URL está configurado como mqtt://broker.hivemq.com.

Salve as configurações e feche o editor.

4. Compilar, Gravar e Monitorar
Conecte a placa ESP32 ao seu computador.

No VS Code, verifique se a porta serial (COM) correta está selecionada no rodapé.

Clique no ícone de chama (🔥) na barra de status inferior (ESP-IDF: Build, Flash and Monitor).

Aguarde o processo terminar. O monitor serial será aberto e exibirá os logs de conexão.

5. Testar o Funcionamento
Após o ESP32 se conectar ao Wi-Fi e ao broker, ele estará pronto para receber comandos.

Abra o seu cliente MQTT de preferência (ex: HiveMQ Web Client).
https://www.hivemq.com/demos/websocket-client/

Conecte-se ao mesmo broker: broker.hivemq.com na porta 1883.

Na seção de Publicar (Publish):

Tópico (Topic): /ifpe/ads/embarcados/esp32/led

Mensagem (Payload/Message):

Envie 1 para acender o LED.

Envie 0 para apagar o LED.

O LED na sua placa deverá responder instantaneamente aos comandos enviados.

Autor: Samara Porto de Noronha