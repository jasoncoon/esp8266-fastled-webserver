FastLED + ESP8266 Web Server
=========

Control an addressable LED strip with an ESP8266 via a web browser or infrared remote control.

Hardware
--------

##### ESP8266 development board

[![Wemos D1 Mini Pro & Headers](https://ae01.alicdn.com/kf/HTB1P1KVaMsSMeJjSsphq6xuJFXah/WEMOS-D1-mini-Pro-V1-1-0-16M-bytes-external-antenna-connector-ESP8266-WIFI-Internet-of.jpg_200x200.jpg)](https://www.aliexpress.com/item/WEMOS-D1-mini-Pro-16M-bytes-external-antenna-connector-ESP8266-WIFI-Internet-of-Things-development-board/32724692514.html)

[Wemos D1 Mini & Headers](https://www.aliexpress.com/store/product/D1-mini-Mini-NodeMcu-4M-bytes-Lua-WIFI-Internet-of-Things-development-board-based-ESP8266/1331105_32529101036.html)

or

[![Adafruit HUZZAH ESP8266 Breakout](https://cdn-shop.adafruit.com/310x233/2471-10.jpg)](https://www.adafruit.com/products/2471)

[Adafruit HUZZAH ESP8266 Breakout](https://www.adafruit.com/products/2471)

##### Addressable LED strip

[![Adafruit NeoPixel Ring](https://www.adafruit.com/images/145x109/1586-00.jpg)](https://www.adafruit.com/product/1586)

[Adafruit NeoPixel Ring]

Other hardware:

* [3.3V to 5V Logic Level Shifter](http://www.digikey.com/product-detail/en/texas-instruments/SN74HCT245N/296-1612-5-ND/277258) (required if LEDs "glitch")

Recommended by [Adafruit NeoPixel "Best Practices"](https://learn.adafruit.com/adafruit-neopixel-uberguide/best-practices) to help protect LEDs from current onrush:
* [1000µF Capacitor](http://www.digikey.com/product-detail/en/panasonic-electronic-components/ECA-1EM102/P5156-ND/245015)
* [300 to 500 Ohm resistor](https://www.digikey.com/product-detail/en/stackpole-electronics-inc/CF14JT470R/CF14JT470RCT-ND/1830342)

Optional shield to make everything more tidy:

[![Wemos D1 Mini ESP8266 LED & Level Shifter Shield](https://d3s5r33r268y59.cloudfront.net/13194/products/thumbs/2017-05-06T15:02:37.208Z-IMG_20170506_100623.jpg.114x76_q85_pad_rcrop.jpg)](https://www.tindie.com/products/jasoncoon/wemos-d1-mini-esp8266-led-and-level-shifter-shield/)

[Wemos D1 Mini ESP8266 LED & Level Shifter Shield](https://www.tindie.com/products/jasoncoon/wemos-d1-mini-esp8266-led-and-level-shifter-shield)

Features
--------
* Turn the NeoPixel Ring on and off
* Adjust the brightness
* Change the display pattern
* Adjust the color

Web App
--------

![Web App](Images/webapp.png)

Patterns are requested by the app from the ESP8266, so as new patterns are added, they're automatically listed in the app.

The web app is stored in a file system in on-board flash memory.   The file system used is LittleFS *(Note: prior versions used SPIFFS)*.

The web app is a single page app that uses [jQuery](https://jquery.com) and [Bootstrap](http://getbootstrap.com).  It has buttons for On/Off, a slider for brightness, a pattern selector, and a color picker (using [jQuery MiniColors](http://labs.abeautifulsite.net/jquery-minicolors)).  Event handlers for the controls are wired up, so you don't have to click a 'Send' button after making changes.  The brightness slider and the color picker use a delayed event handler, to prevent from flooding the ESP8266 web server with too many requests too quickly.

Installing
-----------
The app is installed via the Arduino IDE which can be [downloaded here](https://www.arduino.cc/en/main/software). The ESP8266 boards will need to be added to the Arduino IDE which is achieved as follows. Click File > Preferences and copy and paste the URL "http://arduino.esp8266.com/stable/package_esp8266com_index.json" into the Additional Boards Manager URLs field. Click OK. Click Tools > Boards: ... > Boards Manager. Find and click on ESP8266 (using the Search function may expedite this). Click on Install. After installation, click on Close and then select your ESP8266 board from the Tools > Board: ... menu.

The app depends on the following libraries. They must either be downloaded from GitHub and placed in the Arduino 'libraries' folder, or installed as [described here](https://www.arduino.cc/en/Guide/Libraries) by using the Arduino library manager.

* [FastLED](https://github.com/FastLED/FastLED)
* [Arduino WebSockets](https://github.com/Links2004/arduinoWebSockets)
* [WiFiManager](https://github.com/tzapu/WiFiManager)
* [NTPClient](https://github.com/arduino-libraries/NTPClient)
* [Arduino JSON](https://arduinojson.org)
* [lolrol LittleFS](https://github.com/lorol/LITTLEFS) -- (integrated into ESP32 core v2, which is not used here yet)


Download the app code from GitHub using the green Clone or Download button from [the GitHub project main page](https://github.com/jasoncoon/esp8266-fastled-webserver) and click Download ZIP. Decompress the ZIP file in your Arduino sketch folder.

Here are the board settings I use:

![image](https://user-images.githubusercontent.com/3598755/135755572-52d4d0db-1dba-4388-a86c-a293e4f13878.png)

The web app needs to be uploaded to the ESP8266's file system.
You can do this within the Arduino IDE after installing the [Arduino ESP8266 LittleFS](https://github.com/earlephilhower/arduino-esp8266littlefs-plugin).

With the upload tool installed, upload the web app using `ESP8266 LittleFS Data Upload` command in the Arduino Tools menu.

Compression
-----------

The web app files can be gzip compressed before uploading to the ESP8266's file system by running the following command:

`gzip -r data/`

The ESP8266WebServer will automatically serve any .gz file.  The file index.htm.gz will get served as index.htm, with the content-encoding header set to gzip, so the browser knows to decompress it.  The ESP8266WebServer doesn't seem to like the Glyphicon fonts gzipped, though, so I decompress them with this command:

`gunzip -r data/fonts/`

REST Web services
-----------------

The firmware implements basic [RESTful web services](https://en.wikipedia.org/wiki/Representational_state_transfer) using the ESP8266WebServer library.  Current values are requested with HTTP GETs, and values are set with POSTs using query string parameters.  It can run in connected or standalone access point modes.

Infrared Remote Control
-----------------------

Control via infrared remote control is also supported, via the [ESP8266 port of the IRremote library](https://github.com/sebastienwarin/IRremoteESP8266).

[Adafruit NeoPixel Ring]:https://www.adafruit.com/product/1586
[Adafruit HUZZAH ESP8266 Breakout]:https://www.adafruit.com/products/2471
