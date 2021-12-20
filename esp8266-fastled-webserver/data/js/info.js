// used when hosting the site on the ESP8266
var address = location.hostname;
var urlBase = "";

// used when hosting the site somewhere other than the ESP8266 (handy for testing without waiting forever to upload to SPIFFS)
// var address = "192.168.86.36";
// var urlBase = "http://" + address + "/";

const keyToName = {
  millis: "Uptime",
  vcc: "VCC",
  wiFiChipId: "Wi-Fi Chip ID",
  flashChipId: "Flash Chip ID",
  flashChipSize: "Flash Chip Size",
  flashChipRealSize: "Flash Chip Real Size",
  sdkVersion: "SDK Version",
  coreVersion: "Core Version",
  bootVersion: "Boot Version",
  cpuFreqMHz: "CPU Frequency",
  freeHeap: "Free Heap Memory",
  sketchSize: "Sketch Size",
  freeSketchSpace: "Free Sketch Space",
  resetReason: "Last Reset Reason",
  isConnected: "Wi-Fi Connected",
  wiFiSsidDefault: "Wi-Fi SSID (Default)",
  wiFiSSID: "Wi-Fi SSID",
  localIP: "Local IP Address",
  gatewayIP: "Gateway IP Address",
  subnetMask: "Subnet Mask",
  dnsIP: "DNS IP Address",
  hostname: "Hostname",
  macAddress: "MAC Address",
  autoConnect: "Auto Connect",
  softAPSSID: "Soft AP SSID",
  softAPIP: "Soft AP IP Address",
  BSSID: "BSSID",
  softAPMacAddress: "Soft AP MAC Address",
};

$(document).ready(function () {
  $("#status").html("Connecting, please wait...");

  // get product name for main page
  $.get(urlBase + "product", function (data) {
    const name = data['productName'];
    // Set overall page title
    $(document).attr("title", name + " by EvilGenius Labs");
    // Set text of element with id 'product'
    $("#product").text(name);
  });

  $.get(urlBase + "info", function (data) {
    $("#status").html("Loading, please wait...");

    Object.keys(data).forEach((key) => {
      var dt = $("#infoDtTemplate").clone();
      var dd = $("#infoDdTemplate").clone();

      const name = keyToName[key] || key;
      let value = data[key];
      
      switch(key) {

        case 'millis':
          value = `${(value / 1000 / 60).toFixed()} minutes, ${((value / 1000) % 60).toFixed()} seconds`;
          break;
        
        case 'flashChipSize':
        case 'flashChipRealSize':
        case 'freeHeap':
        case 'freeSketchSpace':
        case 'sketchSize':
          value = `${value.toLocaleString()} bytes`;
          break;

        case 'cpuFreqMHz':
          value = `${value}MHz`;
          break;
      }

      dt.attr("id", `dt-${key}`);
      dt.html(name);

      dd.attr("id", `dd-${key}`);
      dd.html(value);

      $("#infoDl").append(dt);
      $("#infoDl").append(dd);
    });
  });

  $("#status").html("Ready");
});
