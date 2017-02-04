FastLED + ESP8266 + MQTT
=========

Control an addressable LED strip with an ESP8266 via a MQTT Broker like mosquitto.
In this case you need AUTH and SSL enabled (no own cert, you need a CA trusted i.e. www.letsencrypt.org)
TLS v1.2 only will maybe cause some erros.
It's also possible to use your own cert, but u have to upload it to the ESP8266 and register it in the init phase.
When disable SSL you have to switch the port. 

Hardware
--------

An ESP8266 development board, such as the [Adafruit HUZZAH ESP8266 Breakout]:

[![Adafruit HUZZAH ESP8266 Breakout](https://cdn-shop.adafruit.com/310x233/2471-10.jpg)](https://www.adafruit.com/products/2471)

Addressable LED strip, such as the [Adafruit NeoPixel Ring]:

[![Adafruit NeoPixel Ring](https://www.adafruit.com/images/145x109/1586-00.jpg)](https://www.adafruit.com/product/1586)

Features
--------
* Turn the NeoPixel Ring on and off
* Adjust the brightness
* Change the display pattern
* Adjust the color
* Control it via MQTT

MQTT Message Format
--------
rgb(r,g,b)              - rbg(0,0,0) for black or rbg(255,255,255) white
power:x                 - x=0 off x=1 on
solidcolor:r:x:g:y:b:z  - other RGB format x, y, z are the RGB values
pattern:x               - 1-9 for the animation patterns
brightness:x            - x is the value for brightness (0-255)
brightnessAdjust:x      - x=0 =\> up x=1 =\> down
patternAdjust:x         - x=0 =\> up x=1 =\> down



Installing
-----------
These programs are installed via the Arduino IDE which can be [downloaded here](https://www.arduino.cc/en/main/software). The ESP8266 boards will need to be added to the Arduino IDE which is achieved as follows. Click File > Preferences and copy and paste the URL "http://arduino.esp8266.com/stable/package_esp8266com_index.json" into the Additional Boards Manager URLs field. Click OK. Click Tools > Boards: ... > Boards Manager. Find and click on ESP8266 (using the Search function may expedite this). Click on Install. After installation, click on Close and then select your ESP8266 board from the Tools > Board: ... menu.

Download the programs from GitHub using the green Clone or Download button from [the GitHub project main page](https://github.com/jasoncoon/esp8266-fastled-webserver) and click Download ZIP. Decompress the ZIP file in your Arduino sketch folder.

MQTT
-----------
You need to have one MQTT broker like mosquitto https://mosquitto.org/