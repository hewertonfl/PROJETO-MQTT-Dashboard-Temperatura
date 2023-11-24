#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include "DHT.h"
#include <ESP32Time.h>

/****** WiFi Connection Details *******/
#define WIFI_SSID "hewerton"
#define WIFI_PASSWORD "elmq1972"

/******* MQTT Broker Connection Details *******/
#define MQTT_HOST "19e44f8335dc4b77a96191a5117e643b.s2.eu.hivemq.cloud"
#define MQTT_PORT 8883
#define MQTT_USER "hewerton"
#define MQTT_PASSWORD "Mqttcomunicac4o"

// www.howsmyssl.com root certificate authority, to verify the server
// openssl s_client -connect YOUR_URL.hivemq.cloud:8883 -showcerts

const char* ROOT_CERT = "-----BEGIN CERTIFICATE-----\n"
                        "MIIFYDCCBEigAwIBAgIQQAF3ITfU6UK47naqPGQKtzANBgkqhkiG9w0BAQsFADA/\n"
                        "MSQwIgYDVQQKExtEaWdpdGFsIFNpZ25hdHVyZSBUcnVzdCBDby4xFzAVBgNVBAMT\n"
                        "DkRTVCBSb290IENBIFgzMB4XDTIxMDEyMDE5MTQwM1oXDTI0MDkzMDE4MTQwM1ow\n"
                        "TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh\n"
                        "cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwggIiMA0GCSqGSIb3DQEB\n"
                        "AQUAA4ICDwAwggIKAoICAQCt6CRz9BQ385ueK1coHIe+3LffOJCMbjzmV6B493XC\n"
                        "ov71am72AE8o295ohmxEk7axY/0UEmu/H9LqMZshftEzPLpI9d1537O4/xLxIZpL\n"
                        "wYqGcWlKZmZsj348cL+tKSIG8+TA5oCu4kuPt5l+lAOf00eXfJlII1PoOK5PCm+D\n"
                        "LtFJV4yAdLbaL9A4jXsDcCEbdfIwPPqPrt3aY6vrFk/CjhFLfs8L6P+1dy70sntK\n"
                        "4EwSJQxwjQMpoOFTJOwT2e4ZvxCzSow/iaNhUd6shweU9GNx7C7ib1uYgeGJXDR5\n"
                        "bHbvO5BieebbpJovJsXQEOEO3tkQjhb7t/eo98flAgeYjzYIlefiN5YNNnWe+w5y\n"
                        "sR2bvAP5SQXYgd0FtCrWQemsAXaVCg/Y39W9Eh81LygXbNKYwagJZHduRze6zqxZ\n"
                        "Xmidf3LWicUGQSk+WT7dJvUkyRGnWqNMQB9GoZm1pzpRboY7nn1ypxIFeFntPlF4\n"
                        "FQsDj43QLwWyPntKHEtzBRL8xurgUBN8Q5N0s8p0544fAQjQMNRbcTa0B7rBMDBc\n"
                        "SLeCO5imfWCKoqMpgsy6vYMEG6KDA0Gh1gXxG8K28Kh8hjtGqEgqiNx2mna/H2ql\n"
                        "PRmP6zjzZN7IKw0KKP/32+IVQtQi0Cdd4Xn+GOdwiK1O5tmLOsbdJ1Fu/7xk9TND\n"
                        "TwIDAQABo4IBRjCCAUIwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMCAQYw\n"
                        "SwYIKwYBBQUHAQEEPzA9MDsGCCsGAQUFBzAChi9odHRwOi8vYXBwcy5pZGVudHJ1\n"
                        "c3QuY29tL3Jvb3RzL2RzdHJvb3RjYXgzLnA3YzAfBgNVHSMEGDAWgBTEp7Gkeyxx\n"
                        "+tvhS5B1/8QVYIWJEDBUBgNVHSAETTBLMAgGBmeBDAECATA/BgsrBgEEAYLfEwEB\n"
                        "ATAwMC4GCCsGAQUFBwIBFiJodHRwOi8vY3BzLnJvb3QteDEubGV0c2VuY3J5cHQu\n"
                        "b3JnMDwGA1UdHwQ1MDMwMaAvoC2GK2h0dHA6Ly9jcmwuaWRlbnRydXN0LmNvbS9E\n"
                        "U1RST09UQ0FYM0NSTC5jcmwwHQYDVR0OBBYEFHm0WeZ7tuXkAXOACIjIGlj26Ztu\n"
                        "MA0GCSqGSIb3DQEBCwUAA4IBAQAKcwBslm7/DlLQrt2M51oGrS+o44+/yQoDFVDC\n"
                        "5WxCu2+b9LRPwkSICHXM6webFGJueN7sJ7o5XPWioW5WlHAQU7G75K/QosMrAdSW\n"
                        "9MUgNTP52GE24HGNtLi1qoJFlcDyqSMo59ahy2cI2qBDLKobkx/J3vWraV0T9VuG\n"
                        "WCLKTVXkcGdtwlfFRjlBz4pYg1htmf5X6DYO8A4jqv2Il9DjXA6USbW1FzXSLr9O\n"
                        "he8Y4IWS6wY7bCkjCWDcRQJMEhg76fsO3txE+FiYruq9RUWhiF1myv4Q6W+CyBFC\n"
                        "Dfvp7OOGAN6dEOM4+qR9sdjoSYKEBpsr6GtPAQw4dy753ec5\n"
                        "-----END CERTIFICATE-----\n";

int ultimoMinuto = 0;

#define DHT_PIN 2
DHT dht(DHT_PIN, DHT11);

WiFiClientSecure espClient;
PubSubClient client(espClient);

ESP32Time rtc;

void setup() {
  Serial.begin(115200);

  // Configura a hora e a data com base na hora da compilação
  rtc.setTime(convertCompileTime(__DATE__, __TIME__));

  // Inicializa o sensor DHT
  dht.begin();

  // Conecta-se ao broker MQTT
  client.setServer(MQTT_HOST, MQTT_PORT);
  client.setCallback(callback);
  connectWiFi();
  connectBroker();
}

void loop() {
  // Armazena as informações da hora atual nas variáveis
  int hora = rtc.getHour(true);
  int minuto = rtc.getMinute();
  int segundo = rtc.getSecond();

  // Lê a data atual do módulo RTC
  int dia = rtc.getDay();
  int mes = rtc.getMonth();
  int ano = rtc.getYear() % 100;

  // Verifica se passou o intervalo de 1 minuto desde a última execução
  if (minuto - ultimoMinuto >= 1) {
    // Lê os dados do sensor DHT
    int temperatura = dht.readTemperature();
    int umidade = dht.readHumidity();

    // Verifica se os dados foram lidos corretamente
    if (isnan(temperatura) || isnan(umidade)) {
      Serial.println("Erro ao ler os dados do sensor DHT");
      return;
    }
    // Coloca a data e hora no formato adequado para publicação no tópico
    String data = (dia < 10) ? "0" + String(dia) + "/" + String(mes) + "/" + String(ano) : String(dia) + "/" + String(mes) + "/" + String(ano);
    String horario = (minuto < 10) ? String(hora) + ":" + "0" + String(minuto) : String(hora) + ":" + String(minuto);
    String data_hora = "{" + data + "-" + horario + "}";

    // Formata a mensagem no formato desejado
    String mensagem = "[" + data_hora + " { " + String(temperatura) + " C } { " + String(umidade) + " %}]";

    // Reconecta Wi-Fi se desconectado
    if (!WiFi.isConnected()) {
      Serial.println("wifi.....");
      connectWiFi();
    }

    // Verifica se o broker ainda esta conectado
    if (!client.connected()) {
      connectBroker();
    }
    // Publica os dados no MQTT
    client.publish("dados", mensagem.c_str());

    // Atualiza o tempo da última execução
    ultimoMinuto = minuto;
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Mensagem recebida no tópico ");
  Serial.println(topic);

  // Imprime a mensagem recebida
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

// Função para converter a data e hora da compilação para um formato aceitável pela ESP32Time
unsigned long convertCompileTime(const char* date, const char* time) {
  String dateTimeString = String(date) + " " + String(time);
  struct tm tm;
  strptime(dateTimeString.c_str(), "%b %d %Y %H:%M:%S", &tm);

  // Adiciona um mês
  tm.tm_mon += 1;
  if (tm.tm_mon > 11) {
    tm.tm_mon = 0;    // Se ultrapassar dezembro, volta para janeiro
    tm.tm_year += 1;  // Adiciona um ano
  }

  time_t t = mktime(&tm);
  return static_cast<unsigned long>(t);
}

void connectBroker() {
  while (!client.connected()) {
    Serial.print("Tentando conectar ao broker MQTT...");
    if (client.connect("esp32", MQTT_USER, MQTT_PASSWORD)) {
      Serial.println("Conectado ao broker MQTT");
    } else {
      Serial.println("Falha ao conectar ao broker MQTT");
      delay(5000);
    }
  }
}

void connectWiFi() {
  // Conecta-se à rede Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConectado à rede Wi-Fi\nIP address: ");
  Serial.println(WiFi.localIP());
  espClient.setCACert(ROOT_CERT);
}