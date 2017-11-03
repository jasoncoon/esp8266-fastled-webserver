FastLED + ESP8266 Web Server
=========

Control an addressable LED strip with an ESP8266 via a web browser or via HomeBridge and a HomeKit iOS device.

Changes in this fork
--------------------
In order to have this library work with HomeKit, or more specifically the 
[Homebridge Better HTTP RGB](https://github.com/jnovack/homebridge-better-http-rgb) plugin, two extra API routes were needed:
* One, for hexadecimal color values (`/solidHexColor`)
* Two, for percentage based brightness values (`/brightnessPercent`)

In addition to this, web interface updates include:
* Updating to the latest Bootstrap 4 beta
* Switching to CDN scripts to reduce upload size
* Hiding irrelevant fields based on selections made
* General UI tweaks

Additional changes include:
* Removal of IR support, HomeKit is implemented as a replacement
* Setting default power state to "off"

This is a brilliant piece of software originally written by @jnovack, all credit goes to him.
The changes I made have simply been to bring this software up-to-date with current technologies, I hope you can find it useful!


Hardware
--------

An ESP8266 development board, such as the [Adafruit HUZZAH ESP8266 Breakout]:

[![Adafruit HUZZAH ESP8266 Breakout](https://cdn-shop.adafruit.com/310x233/2471-10.jpg)](https://www.adafruit.com/products/2471)

Addressable LED strip, such as the [Adafruit NeoPixel Ring]:

[![Adafruit NeoPixel Ring](https://www.adafruit.com/images/145x109/1586-00.jpg)](https://www.adafruit.com/product/1586)

Features
--------
* Turn the strip on and off
* Adjust the brightness
* Change the display pattern
* Adjust the color

Web App
--------

![Web App](screenshot.png)

Patterns are requested by the app from the ESP8266, so as new patterns are added, they're automatically listed in the app.

The web app is stored in SPIFFS (on-board flash memory).

The web app is a single page app that uses [jQuery](https://jquery.com) and [Bootstrap](http://getbootstrap.com).  It has buttons for On/Off, a slider for brightness, a pattern selector, and a color picker (using [jQuery MiniColors](http://labs.abeautifulsite.net/jquery-minicolors)).  Event handlers for the controls are wired up, so you don't have to click a 'Send' button after making changes.  The brightness slider and the color picker use a delayed event handler, to prevent from flooding the ESP8266 web server with too many requests too quickly.

The only drawback to SPIFFS that I've found so far is uploading the files can be extremely slow, requiring several minutes, sometimes regardless of how large the files are.  It can be so slow that I've been just developing the web app and debugging locally on my desktop (with a hard-coded IP for the ESP8266), before uploading to SPIFFS and testing on the ESP8266.

Installing
-----------
The app is installed via the Arduino IDE which can be [downloaded here](https://www.arduino.cc/en/main/software). The ESP8266 boards will need to be added to the Arduino IDE which is achieved as follows. Click File > Preferences and copy and paste the URL "http://arduino.esp8266.com/stable/package_esp8266com_index.json" into the Additional Boards Manager URLs field. Click OK. Click Tools > Boards: ... > Boards Manager. Find and click on ESP8266 (using the Search function may expedite this). Click on Install. After installation, click on Close and then select your ESP8266 board from the Tools > Board: ... menu.

The app depends on the following libraries. They must either be downloaded from GitHub and placed in the Arduino 'libraries' folder, or installed as [described here](https://www.arduino.cc/en/Guide/Libraries) by using the Arduino library manager.

* [FastLED](https://github.com/FastLED/FastLED)
* [Arduino WebSockets](https://github.com/Links2004/arduinoWebSockets)

Download the app code from GitHub using the green Clone or Download button from [the GitHub project main page](https://github.com/jasoncoon/esp8266-fastled-webserver) and click Download ZIP. Decompress the ZIP file in your Arduino sketch folder.

The web app needs to be uploaded to the ESP8266's SPIFFS.  You can do this within the Arduino IDE after installing the [Arduino ESP8266FS tool](https://github.com/esp8266/Arduino/blob/master/doc/filesystem.md#uploading-files-to-file-system).

With ESP8266FS installed upload the web app using `ESP8266 Sketch Data Upload` command in the Arduino Tools menu.

Then enter your wi-fi network SSID and password in the .ino file, and upload the sketch using the Upload button.

Compression
-----------

The web app files can be gzip compressed before uploading to SPIFFS by running the following command:

`gzip -r data/`

The ESP8266WebServer will automatically serve any .gz file.  The file index.html.gz will get served as index.html, with the content-encoding header set to gzip, so the browser knows to decompress it.

REST Web services
-----------------

The firmware implements basic [RESTful web services](https://en.wikipedia.org/wiki/Representational_state_transfer) using the ESP8266WebServer library.  Current values are requested with HTTP GETs, and values are set with POSTs using query string parameters.  It can run in connected or standalone access point modes.
