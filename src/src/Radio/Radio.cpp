#include "Radio.h"
#include <WiFi.h>
#include <esp_now.h> // esp-now is a connectionless protocol that uses wifi frequencies but doesn't need an access point
#include <esp_wifi.h>

esp_now_peer_info_t slave;
uint8_t mac[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff}; // this is broadcase mac address
datapacketstruct incomingRadioPacket,outgoingRadioPacket;
// void (*callBack)(void);
std::function<void()> callBack;

void OnDataRecv(const uint8_t *macaddr, const uint8_t *data, int len) { // Called when data packet received via radio
  FastCRC16 CRC16;
  uint16_t calccrc;

  if(len<=sizeof(datapacketstruct))
  {
    memcpy(&incomingRadioPacket, data, len);
    calccrc = CRC16.ccitt(((const uint8_t*)&incomingRadioPacket)+2, len-2);
    if(calccrc==incomingRadioPacket.crc) // Only process packet if CRC OK ...
    {
      callBack();
    }
  }
}

void sendPacket() // Sends packet via radio
{
  FastCRC16 CRC16;
  int8_t bytestosend;

  bytestosend=sizeof(outgoingRadioPacket);
  outgoingRadioPacket.crc = CRC16.ccitt(((const uint8_t*)&outgoingRadioPacket)+2, bytestosend-2);
  esp_now_send (mac, (const uint8_t*)&outgoingRadioPacket, bytestosend);
}

void setupRadio(int channel,std::function<void()> _callback) // Setup routine for radio (esp-now, which is a simple, quick protocol that doesn't go through a wifi point)
{
  uint64_t chipid; 
  chipid=ESP.getEfuseMac();
  outgoingRadioPacket.unitNum = (uint16_t)(chipid>>40); // unit num taken from high byte of 6-byte chip address

  callBack = _callback;
  
  
   /*// ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
    ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) );
    ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_STA) );
    ESP_ERROR_CHECK( esp_wifi_start());
  ESP_ERROR_CHECK(esp_wifi_set_promiscuous(true));
  esp_wifi_set_channel(channel,WIFI_SECOND_CHAN_NONE);*/
  WiFi.mode(WIFI_STA); // Station mode for esp-now 
  WiFi.disconnect(); // Conventional wifi not needed for esp-now

  Serial.begin (115200);
  delay(1500);
  Serial.println ("Initializing ...");

  if (esp_now_init () == ESP_OK) {
    Serial.println ("direct link init ok");
  } else {
    Serial.println ("dl init failed");
    ESP.restart ();
  }

  esp_now_register_recv_cb(OnDataRecv);
  slave.channel = channel;
  slave.encrypt = 0; // no encryption
  int i;
  for(i=0; i<6; i++)
    slave.peer_addr[i] = mac[i];
  bool exists = esp_now_is_peer_exist((const uint8_t *)&slave.peer_addr);
  if ( exists) {
    Serial.println("Already Paired");
  } else {
    esp_err_t addStatus = esp_now_add_peer(&slave);
    if (addStatus == ESP_OK) 
      Serial.println("Pair success");
    else if (addStatus == ESP_ERR_ESPNOW_NOT_INIT)
      Serial.println("ESPNOW Not Init");
    else if (addStatus == ESP_ERR_ESPNOW_ARG)
      Serial.println("Invalid Argument");
    else if (addStatus == ESP_ERR_ESPNOW_FULL)
      Serial.println("Peer list full");
    else if (addStatus == ESP_ERR_ESPNOW_NO_MEM)
      Serial.println("Out of memory");
    else if (addStatus == ESP_ERR_ESPNOW_EXIST)
      Serial.println("Peer Exists");
    else
      Serial.println("Not sure what happened");
  }
}
