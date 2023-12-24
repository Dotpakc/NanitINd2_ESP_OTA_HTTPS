#include <Arduino.h>

// #include <UniversalTelegramBot.h>
// файлова система
// #include <FS.h>
// #include <SPIFFS.h>
//WiFi
#include <WiFi.h>

//Json
#include <ArduinoJson.h>

//OTA
#include <HTTPClient.h>
#include <esp_https_ota.h>


#define SSID "Programming" // Change to your WiFi SSID 2.4Ghz
#define PASSWORD "Panda1234" // Change to your WiFi Password


#define CURRENT_VERSION "0.3"

#define URL_FIRMWARE "https://raw.githubusercontent.com/Dotpakc/NanitINd2_ESP_OTA_HTTPS/main/firmware/firmware.json"


const char CERTIFICATE_ROOT[] = R"=EOF=(
-----BEGIN CERTIFICATE-----
MIIDrzCCApegAwIBAgIQCDvgVpBCRrGhdWrJWZHHSjANBgkqhkiG9w0BAQUFADBh
MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3
d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBD
QTAeFw0wNjExMTAwMDAwMDBaFw0zMTExMTAwMDAwMDBaMGExCzAJBgNVBAYTAlVT
MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3dy5kaWdpY2VydC5j
b20xIDAeBgNVBAMTF0RpZ2lDZXJ0IEdsb2JhbCBSb290IENBMIIBIjANBgkqhkiG
9w0BAQEFAAOCAQ8AMIIBCgKCAQEA4jvhEXLeqKTTo1eqUKKPC3eQyaKl7hLOllsB
CSDMAZOnTjC3U/dDxGkAV53ijSLdhwZAAIEJzs4bg7/fzTtxRuLWZscFs3YnFo97
nh6Vfe63SKMI2tavegw5BmV/Sl0fvBf4q77uKNd0f3p4mVmFaG5cIzJLv07A6Fpt
43C/dxC//AH2hdmoRBBYMql1GNXRor5H4idq9Joz+EkIYIvUX7Q6hL+hqkpMfT7P
T19sdl6gSzeRntwi5m3OFBqOasv+zbMUZBfHWymeMr/y7vrTC0LUq7dBMtoM1O/4
gdW7jVg/tRvoSSiicNoxBN33shbyTApOB6jtSj1etX+jkMOvJwIDAQABo2MwYTAO
BgNVHQ8BAf8EBAMCAYYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4EFgQUA95QNVbR
TLtm8KPiGxvDl7I90VUwHwYDVR0jBBgwFoAUA95QNVbRTLtm8KPiGxvDl7I90VUw
DQYJKoZIhvcNAQEFBQADggEBAMucN6pIExIK+t1EnE9SsPTfrgT1eXkIoyQY/Esr
hMAtudXH/vTBH1jLuG2cenTnmCmrEbXjcKChzUyImZOMkXDiqw8cvpOp/2PV5Adg
06O/nVsJ8dWO41P0jmP6P6fbtGbfYmbW0W5BjfIttep3Sp+dWOIrWcBAI+0tKIJF
PnlUkiaY4IBIqDfv8NZ5YBberOgOzW6sRBc4L0na4UU+Krk2U886UAb3LujEV0ls
YSEY1QSteDwsOoBrp+uvFRTp2InBuThs4pFsiv9kuXclVzDAGySj4dzp30d8tbQk
CAUw7C29C79Fv1C5qfPrmAESrciIxpg0X40KPMbp1ZWVbd4=
-----END CERTIFICATE-----
)=EOF=";

// strcmp(version, CURRENT_VERSION) - сравнение версий якщо 0 то версии одинаковые

void check_update(String payload){
  DynamicJsonDocument doc(1024);
  deserializeJson(doc, payload);
  const char* version = doc["version"];
  const char* url = doc["link"];

  Serial.print("New version: ");
  Serial.println(version);
  Serial.print("current version: ");
  Serial.println(CURRENT_VERSION);

  Serial.print("URL: ");
  Serial.println(url);

  if (strcmp(version, CURRENT_VERSION) == 0) {
    Serial.println("No new version");
  } else {
    Serial.println("New version available");
    Serial.println("Start update");

    esp_http_client_config_t config = {
      .url = url,
      .cert_pem = (char *) CERTIFICATE_ROOT,
    };

    esp_err_t ret = esp_https_ota(&config);
    
    if (ret == ESP_OK) {
        Serial.println("Firmware Upgrades Successfully");
        delay(5000);
        Serial.println("Now Curent Version: ");
        Serial.println(version);
        esp_restart();
    } else {
        Serial.print("Error code: ");
        Serial.println(ret);
        Serial.println("Firmware Upgrades Failed");
        Serial.println("Now Curent Version: ");
        Serial.println(CURRENT_VERSION);
    }
  }
}



void init_wifi() {
  WiFi.begin(SSID, PASSWORD);
  WiFi.waitForConnectResult();
  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP());
  delay(5000);
  Serial.println("Start OTA");
  

  WiFiClientSecure *client = new WiFiClientSecure;
  if(client) {
    client -> setCACert(CERTIFICATE_ROOT);
    // client -> setCACert(rootCACertificate);
    // client -> setInsecure();
    HTTPClient https;
    Serial.print("[HTTPS] begin...\n");
    if (https.begin(*client, URL_FIRMWARE)) {  // HTTPS
      Serial.print("[HTTPS] GET...\n");
      // start connection and send HTTP header
      int httpCode = https.GET();
      Serial.println(httpCode);
      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTPS] GET... code: %d\n", httpCode);
        // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = https.getString();
          Serial.println(payload);
          // Check if there is a new version
          check_update(payload);
        }
      } else {
        Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
      }
      https.end();
    } else {
      Serial.printf("[HTTPS] Unable to connect\n");
    }
    delete client;
  } else {
    Serial.println("Unable to create client");

  Serial.println("End OTA");
};

Serial.println("ALL Complete");
}

void setup() {
  Serial.begin(115200);
  Serial.println("Start");
  Serial.print("Current version: ");
  Serial.println(CURRENT_VERSION);
  init_wifi();
  
}

void loop() {

  Serial.println("Loop");
  delay(1000);
  

}
