#include "common.h"

String WiFi_SSID(bool persistent) {
    struct station_config conf;
    if(persistent) wifi_station_get_config_default(&conf);
    else wifi_station_get_config(&conf);

    const size_t buffer_len = sizeof(conf.ssid)+1; //ssid can be up to 32chars, => plus null term
    char tmp[buffer_len];
    memcpy(tmp, conf.ssid, sizeof(conf.ssid));
    tmp[buffer_len-1] = 0; //nullterm in case of 32 char ssid
    return String(reinterpret_cast<char*>(tmp));
}

#if 1 // This is documentation, useful to calculate minimum JsonDocument buffer size for "info"
//
// Note the use of a raw string literals below.  This makes it easier to copy/paste into:
// https://arduinojson.org/v6/assistant/
//
//  uint32 max: 4294967295 ... but assistant requires ARDUINOJSON_USE_LONG_LONG
//                             if any value is >= 2,000,000,000 (which is odd, as that is
//                             not related to any power-of-two value...)
//  uint32 mux: 1999999999 ... maximum value assistant allows w/o ARDUINOJSON_USE_LONG_LONG
//  uint16 max: 65535
//  uint8  max: 255
//     boolean: "false"
//        SSID: "0123456789ABCDEF0123456789ABCDEF" (spec limit 32 octets)
//  macAddress: "00:11:22:33:44:55"
//        IPv4: "255.255.255.255"
//        IPv6: "0011:2233:4455:6677:8899:AABB:CCDD:EEFF"
//    hostname: spec limit 65 octets ... (67 chars) [65+2 for string quotes]
//
//
// Any decent compiler will throw away this unreferenced data during optimization.
//
// This pretty-printed version takes 1031 characters, minified version takes 890 characters.
static const char MaximumLengthJson[] { R"RAW_STRING(
{
  "millis" : 4294967295,
  "vcc" : 65535,
  "wiFiChipId" : "FF22CC44",
  "flashChipId" : "FF22CC44",
  "flashChipSize" : 4294967295,
  "flashChipRealSize" : 4294967295,
  "sdkVersion" : "2.2.2-SOME_RANDOM_STRING_LENGTH",
  "coreVersion" : "2.3.4-SOME_RANDOM_STRING",
  "bootVersion" : 255,
  "cpuFreqMHz" : 255,
  "freeHeap" : 4294967295,
  "sketchSize" : 4294967295,
  "freeSketchSpace" : 4294967295,
  "resetReason" : "Software/System restart",
  "isConnected" : false,
  "wiFiSsidDefault" : "0123456789ABCDEF0123456789ABCDEF",
  "wiFiSSID" : "0123456789ABCDEF0123456789ABCDEF",
  "localIP" : "255.255.255.255",
  "gatewayIP" : "255.255.255.255",
  "subnetMask" : "255.255.255.255",
  "dnsIP" : "255.255.255.255",
  "hostname" : "0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF",
  "macAddress" : "00:11:22:33:44:55",
  "autoConnect" : false,
  "softAPSSID" : "0123456789ABCDEF0123456789ABCDEF",
  "softAPIP" : "255.255.255.255",
  "BSSID" : "00:11:22:33:44:55",
  "softAPMacAddress" : "00:11:22:33:44:55"
}
)RAW_STRING" };
static const char MinifiedMaximumLengthJson[] { R"RAW_STRING(
{"millis":2147483640,"vcc":65535,"wiFiChipId":"FF22CC44","flashChipId":"FF22CC44","flashChipSize":1073741823,"flashChipRealSize":1073741823,"sdkVersion":"2.2.2-SOME_RANDOM_STRING_LENGTH","coreVersion":"2.3.4-SOME_RANDOM_STRING","bootVersion":255,"cpuFreqMHz":255,"freeHeap":1073741823,"sketchSize":1073741823,"freeSketchSpace":1073741823,"resetReason":"Software/System restart","isConnected":false,"wiFiSsidDefault":"0123456789ABCDEF0123456789ABCDEF","wiFiSSID":"0123456789ABCDEF0123456789ABCDEF","localIP":"255.255.255.255","gatewayIP":"255.255.255.255","subnetMask":"255.255.255.255","dnsIP":"255.255.255.255","hostname":"0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF","macAddress":"00:11:22:33:44:55","autoConnect":false,"softAPSSID":"0123456789ABCDEF0123456789ABCDEF","softAPIP":"255.255.255.255","BSSID":"00:11:22:33:44:55","softAPMacAddress":"00:11:22:33:44:55"}
)RAW_STRING" };
#endif
static const size_t infoJsonDocumentAllocationSize = 1024;

String getInfoJson()
{
  DynamicJsonDocument jsonDoc(infoJsonDocumentAllocationSize);
  jsonDoc[F("millis")]             = millis();                          // uint32_t (unsigned long)
  jsonDoc[F("vcc")]                = ESP.getVcc();                      // uint16_t
  jsonDoc[F("wiFiChipId")]         = String(WIFI_getChipId(), HEX);     // HEX(uint32_t)
  jsonDoc[F("flashChipId")]        = String(ESP.getFlashChipId(), HEX); // HEX(uint32_t)
  jsonDoc[F("flashChipSize")]      = ESP.getFlashChipSize();            // uint32_t
  jsonDoc[F("flashChipRealSize")]  = ESP.getFlashChipRealSize();        // uint32_t
  jsonDoc[F("sdkVersion")]         = system_get_sdk_version();          // String; example: "2.2.2-"
  jsonDoc[F("coreVersion")]        = ESP.getCoreVersion();              // String; example: ""
  jsonDoc[F("bootVersion")]        = system_get_boot_version();         // uint8_t
  jsonDoc[F("cpuFreqMHz")]         = ESP.getCpuFreqMHz();               // uint8_t
  jsonDoc[F("freeHeap")]           = ESP.getFreeHeap();                 // uint32_t
  jsonDoc[F("sketchSize")]         = ESP.getSketchSize();               // uint32_t
  jsonDoc[F("freeSketchSpace")]    = ESP.getFreeSketchSpace();          // uint32_t
  jsonDoc[F("resetReason")]        = ESP.getResetReason();              // longest: "Software/System restart"
  jsonDoc[F("isConnected")]        = WiFi.isConnected();                // boolean
  jsonDoc[F("wiFiSsidDefault")]    = WiFi_SSID(true);                   // SSID
  jsonDoc[F("wiFiSSID")]           = WiFi_SSID(false);                  // SSID
  jsonDoc[F("localIP")]            = WiFi.localIP().toString();         // IPAddress (IPv4 only currently)
  jsonDoc[F("gatewayIP")]          = WiFi.gatewayIP().toString();       // IPAddress (IPv4 only currently)
  jsonDoc[F("subnetMask")]         = WiFi.subnetMask().toString();      // IPAddress (IPv4 only currently)
  jsonDoc[F("dnsIP")]              = WiFi.dnsIP().toString();           // IPAddress (IPv4 only currently)
  jsonDoc[F("hostname")]           = WiFi.hostname();                   // hostname
  jsonDoc[F("macAddress")]         = WiFi.macAddress();                 // macAddress
  jsonDoc[F("autoConnect")]        = WiFi.getAutoConnect();             // boolean
  jsonDoc[F("softAPSSID")]         = WiFi.softAPSSID();                 // SSID
  jsonDoc[F("softAPIP")]           = WiFi.softAPIP().toString();        // IPv4
  jsonDoc[F("BSSID")]              = WiFi.BSSIDstr();                   // macAddress
  jsonDoc[F("softAPMacAddress")]   = WiFi.softAPmacAddress();           // macAddress

  // what to do if overflow the ArduinoJSON buffer?
  if (jsonDoc.overflowed()) {
    // NOTE: this should *NEVER* occur if the maximum string lengths were calculated properly
    return String(F("{}"));
  };

  // convert to String
  String result;
  // avoid heap fragmentation by pre-allocating 1023 bytes for result string.
  // As of 2021-11-21, maximum 890 chars needed for minified result.
  // As of 2021-11-21, maximum 1031 characters needed for prettified result.
  result.reserve(1023); 
  serializeJson(jsonDoc, result);
  return result;
}
