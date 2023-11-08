#include <WiFi.h>
#include "esp_camera.h"

#include "config.h"
#include "cam_config.h"

camera_config_t camera_config;
WiFiServer server(WEBCAM_PORT);

void error(const char *str)
{
  Serial.print("error: ");
  Serial.println(str);

  while (true) {
  }
}

#ifdef USE_STAMODE
void init_wifi()
{
  Serial.print("WIFI-SSID: ");
  Serial.println(SSID);
  Serial.print("WIFI-PASSWD: ");
  Serial.println(PASSWD);

  WiFi.begin(SSID, PASSWD);
  Serial.print("Waiting for Wi-Fi connection");

  bool connected = false;
  int v = 0;
  for (int count = 0; count < 30; count ++) {
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println();
      Serial.println("Connected!");
      connected = true;
      break;
    }
    Serial.print(".");
    v = 20 - v;
    delay(500);
  }
  if (!connected) {
    error("Timed out.");
  }
}
#endif /* USE_STAMODE */

#ifdef USE_APMODE
const IPAddress ip(192,168,4,1);
const IPAddress subnet(255,255,255,0);

void init_wifi()
{
  Serial.print("WIFI-SSID: ");
  Serial.println(SSID);
  Serial.print("WIFI-PASSWD: ");
  Serial.println(PASSWD);

  WiFi.softAP(SSID, PASSWD);
  delay(100);
  WiFi.softAPConfig(ip, ip, subnet);
  Serial.println("WiFi started");
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("IP Addr = ");
  Serial.println(ip);
}
#endif /* USE_APMODE */

void init_camera()
{
  camera_config.pin_reset = CAM_PIN_RESET;
  camera_config.pin_xclk = CAM_PIN_XCLK;
  camera_config.pin_sscb_sda = CAM_PIN_SIOD;
  camera_config.pin_sscb_scl = CAM_PIN_SIOC;

  camera_config.pin_d7 = CAM_PIN_D7;
  camera_config.pin_d6 = CAM_PIN_D6;
  camera_config.pin_d5 = CAM_PIN_D5;
  camera_config.pin_d4 = CAM_PIN_D4;
  camera_config.pin_d3 = CAM_PIN_D3;
  camera_config.pin_d2 = CAM_PIN_D2;
  camera_config.pin_d1 = CAM_PIN_D1;
  camera_config.pin_d0 = CAM_PIN_D0;
  camera_config.pin_vsync = CAM_PIN_VSYNC;
  camera_config.pin_href = CAM_PIN_HREF;
  camera_config.pin_pclk = CAM_PIN_PCLK;

  //XCLK 20MHz or 10MHz
  camera_config.xclk_freq_hz = CAM_XCLK_FREQ;
  camera_config.ledc_timer = LEDC_TIMER_0;
  camera_config.ledc_channel = LEDC_CHANNEL_0;

  camera_config.pixel_format = PIXEL_FORMAT;
  camera_config.frame_size = FRAME_SIZE;

  camera_config.jpeg_quality = 31; //0-63 lower number means higher quality
  camera_config.fb_count = 2; //if more than one, i2s runs in continuous mode. Use only with JPEG

  esp_err_t err = esp_camera_init(&camera_config);

  if (err != ESP_OK) {
    error("Camera Init Failed");
  }

  sensor_t *s = esp_camera_sensor_get();
  s->set_vflip(s, 1);
  s->set_hmirror(s, 1);
}

void setup()
{
  Serial.begin(115200);
  Serial.println("Initializing ...");

  init_wifi();
  init_camera();
  server.begin();

  Serial.print("Starting Web Server at ");
  Serial.println(WiFi.localIP());
}

#define PART_BOUNDARY "randomboundary"

void loop()
{
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  Serial.println("New Client.");
  while (client.connected()) {
    if (!client.available()) {
      continue;
    }
    client.println("HTTP/1.1 200 OK");
    client.println("MIME-Version: 1.0");
    client.println("Server: m5camera-webcam");
    client.print("Content-Type: multipart/x-mixed-replace;boundary=" PART_BOUNDARY "\r\n");

    camera_fb_t *fb = esp_camera_fb_get();
    if (!fb) {
      Serial.println("Camera capture failed");
      break;
    }
    client.print("\r\n--" PART_BOUNDARY "\r\n");
    client.println("Content-Type: image/jpeg");
    client.print("Content-Length: ");
    client.println(fb->len);
    client.println();
    client.write(fb->buf, fb->len);
    esp_camera_fb_return(fb);
    delay(100);
  }
  client.stop();
  Serial.println("Client Disconnected.");
}
