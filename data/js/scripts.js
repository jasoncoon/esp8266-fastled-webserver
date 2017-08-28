var urlBase = ""; // blank when hosting the site on the ESP8266, otherwise use ip e.g. http://192.168.0.96/
var brightnessTimer = {};
var colorTimer = {};
var ignoreColorChange = true;
var allData;

$(document).ready(function() {
  $("#inputColor").minicolors({
    theme: "bootstrap",
    changeDelay: 200,
    control: "wheel",
    format: "rgb",
    position: "top right"
  });

  getAll();
});

$("#btnRefresh").click(function() {
  getAll();
});

$("#btnPowerOn").click(function() {
  setPower(1);
});

$("#btnPowerOff").click(function() {
  setPower(0);
});

$("#inputBrightness").on("change mousemove", function() {
  $("#spanBrightness").html($(this).val());
});

$("#inputBrightness").on("change", function() {
  $("#spanBrightness").html($(this).val());
  delaySetBrightness();
});

$("#inputPattern").change(function() {
  setPattern($("#inputPattern option:selected").index());
  if ($("#inputPattern option:selected").index() === 9) {
    $("#groupPalette").hide();
    $("#groupColor").show();
    $("#groupBrightness").hide();
  } else {
    $("#groupPalette").show();
    $("#groupColor").hide();
    $("#groupBrightness").show();
  }
});

$("#inputPalette").change(function() {
  setPalette($("#inputPalette option:selected").index());
});

$("#inputColor").change(function() {
  if (ignoreColorChange) return;

  var rgb = $("#inputColor").minicolors("rgbObject");
  delaySetColor(rgb);
});

$(".btn-color").click(function() {
  if (ignoreColorChange) return;

  var rgb = $(this).css("backgroundColor");
  var components = rgbToComponents(rgb);
  delaySetColor(components);

  var hexString = rgbToHex(components.r, components.g, components.b);
  ignoreColorChange = true;
  $("#inputColor").minicolors("value", hexString);
  ignoreColorChange = false;
});

function getAll() {
  $.get(urlBase + "all", function(data) {
    allData = data;

    $("#status").html("Connecting...");
    $("#inputBrightness").val(data.brightness);
    $("#spanBrightness").html(data.brightness);

    var hexString = rgbToHex(
      data.solidColor.r,
      data.solidColor.g,
      data.solidColor.b
    );
    ignoreColorChange = true;
    $("#inputColor").minicolors("value", hexString);
    ignoreColorChange = false;

    updatePowerButtons(data.power);

    // clear pattern list
    $("#inputPattern").find("option").remove();

    // load pattern list
    for (var i = 0; i < data.patterns.length; i++) {
      var pattern = data.patterns[i];
      $("#inputPattern").append(
        "<option value='" + i + "'>" + pattern + "</option>"
      );
    }

    // select the current pattern
    $("#inputPattern").val(data.currentPattern.index);
    if (data.currentPattern.index === 9) {
      $("#groupPalette").hide();
      $("#groupBrightness").hide();
    } else {
      $("#groupColor").hide();
    }

    // clear palette list
    $("#inputPalette").find("option").remove();

    // load palette list
    for (var i = 0; i < data.palettes.length; i++) {
      var palette = data.palettes[i];
      $("#inputPalette").append(
        "<option value='" + i + "'>" + palette + "</option>"
      );
    }

    // select the current palette
    $("#inputPalette").val(data.currentPalette.index);

    $("#status").html("Ready");
  });
}

function updatePowerButtons(value) {
  if (value == 0) {
    $("#btnPowerOn").attr("class", "btn btn-outline-dark");
    $("#btnPowerOff").attr("class", "btn btn-dark");
  } else {
    $("#btnPowerOn").attr("class", "btn btn-dark");
    $("#btnPowerOff").attr("class", "btn btn-outline-dark");
  }
}

function setPower(value) {
  $.post(urlBase + "power?value=" + value, function(data) {
    updatePowerButtons(data);
    $("#status").html("Set Power: " + data);
  });
}

function delaySetBrightness() {
  clearTimeout(brightnessTimer);
  brightnessTimer = setTimeout(function() {
    setBrightness($("#inputBrightness").val());
  }, 300);
}

function setBrightness(value) {
  $.post(urlBase + "brightness?value=" + value, function(data) {
    $("#status").html("Set Brightness: " + data);
  });
}

function setPattern(value) {
  $.post(urlBase + "pattern?value=" + value, function(data) {
    $("#status").html("Set Pattern: " + data.name);
  });
}

function setPalette(value) {
  $.post(urlBase + "palette?value=" + value, function(data) {
    $("#status").html("Set Palette: " + data.name);
  });
}

function delaySetColor(value) {
  clearTimeout(colorTimer);
  colorTimer = setTimeout(function() {
    setColor(value);
  }, 300);
}

function setColor(value) {
  $.post(
    urlBase + "solidColor?r=" + value.r + "&g=" + value.g + "&b=" + value.b,
    function(data) {
      $("#status").html(
        "Set Color: rgb(" + data.r + ", " + data.g + ", " + data.b + ")"
      );
      $("#inputPattern").val(allData.patterns.length - 1);
    }
  );
}

function componentToHex(c) {
  var hex = c.toString(16);
  return hex.length == 1 ? "0" + hex : hex;
}

function rgbToHex(r, g, b) {
  return "#" + componentToHex(r) + componentToHex(g) + componentToHex(b);
}

function rgbToComponents(rgb) {
  var components = {};

  rgb = rgb.match(/^rgb\((\d+),\s*(\d+),\s*(\d+)\)$/);
  components.r = parseInt(rgb[1]);
  components.g = parseInt(rgb[2]);
  components.b = parseInt(rgb[3]);

  return components;
}
