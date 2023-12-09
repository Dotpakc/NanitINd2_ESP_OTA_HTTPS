#include <Arduino.h>

#include <UniversalTelegramBot.h>
// файлова система
// #include <FS.h>
// #include <SPIFFS.h>
//WiFi
#include <WiFi.h>

//OTA
#include <HTTPClient.h>


#define SSID "Programming" // Change to your WiFi SSID 2.4Ghz
#define PASSWORD "Panda1234" // Change to your WiFi Password

const char* rootCACertificate PROGMEM = \
    "-----BEGIN CERTIFICATE-----\n" \
    "MIIHEjCCBfqgAwIBAgIQBE1y13zdpwLdWmfyoju92TANBgkqhkiG9w0BAQsFADBP\n" \
    "MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMSkwJwYDVQQDEyBE\n" \
    "aWdpQ2VydCBUTFMgUlNBIFNIQTI1NiAyMDIwIENBMTAeFw0yMzAyMjEwMDAwMDBa\n" \
    "Fw0yNDAzMjAyMzU5NTlaMGcxCzAJBgNVBAYTAlVTMRMwEQYDVQQIEwpDYWxpZm9y\n" \
    "bmlhMRYwFAYDVQQHEw1TYW4gRnJhbmNpc2NvMRUwEwYDVQQKEwxHaXRIdWIsIElu\n" \
    "Yy4xFDASBgNVBAMMCyouZ2l0aHViLmlvMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8A\n" \
    "MIIBCgKCAQEAuLBgDhov8bGGS2TsEZ+meb7oh/GIxbRJmxC7yq/qr75UDHhDf8p7\n" \
    "TkVbCyQp8bsj/Bmkx2xwSXZT0wkjZbJIe7Ycqgca4nka+Xpe5xb4pkrVOaPiDfdX\n" \
    "7+34CHZbUtqL0OYebi/5D5lLalLKNOGkySAz05foenfFAxAmQYJhR6KvxFY/dqI4\n" \
    "y7JwrnJ6Q8F+J6Ne1uP256UwcL0qlid6e/tA0ld3ryMSJ0I6xgtqjL26Le4/nxXu\n" \
    "YlekppVQr0OwrHa44Q7Z/1bsdFCGtR+WLNGVBeW3BWeTTp7yWjgfp49DWt48V9pI\n" \
    "elDGiDgVyJcsLOz4OQk2vRmNA1ZBZgck4wIDAQABo4ID0DCCA8wwHwYDVR0jBBgw\n" \
    "FoAUt2ui6qiqhIx56rTaD5iyxZV2ufQwHQYDVR0OBBYEFI0CHHVazcamQXhpKMP3\n" \
    "qqeYO9W7MHsGA1UdEQR0MHKCCyouZ2l0aHViLmlvgglnaXRodWIuaW+CDCouZ2l0\n" \
    "aHViLmNvbYIKZ2l0aHViLmNvbYIOd3d3LmdpdGh1Yi5jb22CFyouZ2l0aHVidXNl\n" \
    "cmNvbnRlbnQuY29tghVnaXRodWJ1c2VyY29udGVudC5jb20wDgYDVR0PAQH/BAQD\n" \
    "AgWgMB0GA1UdJQQWMBQGCCsGAQUFBwMBBggrBgEFBQcDAjCBjwYDVR0fBIGHMIGE\n" \
    "MECgPqA8hjpodHRwOi8vY3JsMy5kaWdpY2VydC5jb20vRGlnaUNlcnRUTFNSU0FT\n" \
    "SEEyNTYyMDIwQ0ExLTQuY3JsMECgPqA8hjpodHRwOi8vY3JsNC5kaWdpY2VydC5j\n" \
    "b20vRGlnaUNlcnRUTFNSU0FTSEEyNTYyMDIwQ0ExLTQuY3JsMD4GA1UdIAQ3MDUw\n" \
    "MwYGZ4EMAQICMCkwJwYIKwYBBQUHAgEWG2h0dHA6Ly93d3cuZGlnaWNlcnQuY29t\n" \
    "L0NQUzB/BggrBgEFBQcBAQRzMHEwJAYIKwYBBQUHMAGGGGh0dHA6Ly9vY3NwLmRp\n" \
    "Z2ljZXJ0LmNvbTBJBggrBgEFBQcwAoY9aHR0cDovL2NhY2VydHMuZGlnaWNlcnQu\n" \
    "Y29tL0RpZ2lDZXJ0VExTUlNBU0hBMjU2MjAyMENBMS0xLmNydDAJBgNVHRMEAjAA\n" \
    "MIIBfgYKKwYBBAHWeQIEAgSCAW4EggFqAWgAdwB2/4g/Crb7lVHCYcz1h7o0tKTN\n" \
    "uyncaEIKn+ZnTFo6dAAAAYZ0gHV7AAAEAwBIMEYCIQCqfmfSO8MxeeVZ/fJzqqBB\n" \
    "p+VqeRDUOUBVGyTTOn43ewIhAJT0S27mmGUlpqNiDADP+Jo8C6kYHF+7U6TY74bH\n" \
    "XHAaAHYAc9meiRtMlnigIH1HneayxhzQUV5xGSqMa4AQesF3crUAAAGGdIB1agAA\n" \
    "BAMARzBFAiEAguB+XQVANBj2MPcJzbz+LBPrkDDOEO3op52jdHUSW3ICIF0fnYdW\n" \
    "qvdtmgQNSns13pAppdQWp4/f/jerNYskI7krAHUASLDja9qmRzQP5WoC+p0w6xxS\n" \
    "ActW3SyB2bu/qznYhHMAAAGGdIB1SgAABAMARjBEAiAT/wA2qGGHSKZqBAm84z6q\n" \
    "E+dGPQZ1aCMY52pFSfcw8QIgP/SciuZG02X2mBO/miDT2hCp4y5d2sc7FE5PThyC\n" \
    "pbMwDQYJKoZIhvcNAQELBQADggEBADekGxEin/yfyWcHj6qGE5/gCB1uDI1l+wN5\n" \
    "UMZ2ujCQoKQceRMHuVoYjZdMBXGK0CIXxhmiIosD9iyEcWxV3+KZQ2Xl17e3N0zG\n" \
    "yOXx2Kd7B13ruBxQpKOO8Ez4uGpyWb5DDoretV6Pnj9aQ2SCzODedvS+phIKBmi7\n" \
    "d+FM70tNZ6/2csdrG5xIU6d/7XYYXPD2xkwkU1dX4UKmPa7h9ZPyavopcgE+twbx\n" \
    "LxoOkcXsNb/12jOV3iQSDfXDI41AgtFc694KCOjlg+UKizpemE53T5/cq37OqChP\n" \
    "qnlPyb6PYIhua/kgbH84ltba1xEDQ9i4UYfOMiJNZEzEdSfQ498=\n" \
    "-----END CERTIFICATE-----\n";








void init_wifi() {
  WiFi.begin(SSID, PASSWORD);
  WiFi.waitForConnectResult();
  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP());

  
  // esp_http_client_config_t config = {
  //    .url = "https://raw.githubusercontent.com/path/to/firmware",
  //    .cert_pem = (char *) cert_github,
  // };

  WiFiClientSecure *client = new WiFiClientSecure;
  if(client) {
    client -> setCACert(rootCACertificate);
    // client -> setInsecure();
    HTTPClient https;
    Serial.print("[HTTPS] begin...\n");
    if (https.begin(*client, "https://raw.githubusercontent.com/lucadentella/esp32-tutorial/master/README.md")) {  // HTTPS
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


};


}

void setup() {
  Serial.begin(115200);
  init_wifi();
}

void loop() {


}
