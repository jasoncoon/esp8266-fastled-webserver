const bool apMode = false;

// AP mode password
const char WiFiAPPSK[] = "";

// Wi-Fi network to connect to (if not in AP mode)
const char* ssid = "";
const char* password = "";


#define DATA_PIN      5     // for Huzzah: Pins w/o special function:  #4,#5, #12, #13, #14; // #16 does not work :(
#define LED_TYPE      WS2812
#define COLOR_ORDER   GRB
#define NUM_LEDS      150

#define MILLI_AMPS         500     // IMPORTANT: set here the maxmilli-Amps of your power supply 5V 2A = 2000
#define FRAMES_PER_SECOND  120 // here you can control the speed. With the Access Point / Web Server the animations run a bit slower.

// MQTT Broker settings
const char* mqtt_server = "";
const int mqtt_port = 8883; // Change this if u don't use a SSL connection
const char* mqtt_user = "";
const char* mqtt_password = "";
const char* mqtt_topic = "";
const char* mqtt_clientid = "";

// openssl x509 -fingerprint -in  mqttserver.crt - Only if you must verify your certs for connection issues with MQTT
const char* fingerprint = "AF FD A4 F6 3B 74 FE EE A5 71 3A 08 7F 30 F8 CF DE 8D E6 7F";






