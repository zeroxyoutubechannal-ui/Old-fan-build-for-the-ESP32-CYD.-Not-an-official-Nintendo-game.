// ============================================================
//  GAMEPAD  -  ESP32 DevKit V4  ->  ESP-NOW broadcast  ->  CYD
//  Paste into Arduino IDE (new sketch), board: "ESP32 Dev Module"
//
//  WIRING: one leg of every button to the GPIO, other leg to GND.
//  (no resistors needed, internal pull-ups are used)
//
//    UP     -> GPIO 32
//    DOWN   -> GPIO 33
//    LEFT   -> GPIO 25
//    RIGHT  -> GPIO 26
//    A      -> GPIO 27
//    B      -> GPIO 14
//    START  -> GPIO 16
//    SELECT -> GPIO 17
// ============================================================
#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>

const uint8_t PINS[8] = {32, 33, 25, 26, 27, 14, 16, 17};
//                       UP DOWN LEFT RIGHT A  B START SELECT (bit 0..7)

struct __attribute__((packed)) Pkt {
  uint8_t magic;   // 0xA5
  uint8_t btn;     // bit mask of pressed buttons
  uint8_t seq;
};

uint8_t bcast[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
uint8_t seqNo = 0;

void setup() {
  for (int i = 0; i < 8; i++) pinMode(PINS[i], INPUT_PULLUP);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE);   // must be the same on the CYD

  if (esp_now_init() != ESP_OK) {
    delay(500);
    ESP.restart();
  }
  esp_now_peer_info_t peer = {};
  memcpy(peer.peer_addr, bcast, 6);
  peer.channel = 0;          // 0 = current channel
  peer.encrypt = false;
  esp_now_add_peer(&peer);
}

void loop() {
  uint8_t b = 0;
  for (int i = 0; i < 8; i++)
    if (digitalRead(PINS[i]) == LOW) b |= (1 << i);

  Pkt p;
  p.magic = 0xA5;
  p.btn = b;
  p.seq = seqNo++;
  esp_now_send(bcast, (uint8_t *)&p, sizeof(p));   // sent every ~8 ms, also as a heartbeat

  delay(8);
}
