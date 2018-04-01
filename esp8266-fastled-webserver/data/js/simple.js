// used when hosting the site on the ESP8266
var address = location.hostname;
var urlBase = "";

// used when hosting the site somewhere other than the ESP8266 (handy for testing without waiting forever to upload to SPIFFS)
// var address = "192.168.1.13";
// var urlBase = "http://" + address + "/";

var postColorTimer = {};
var postValueTimer = {};

var ignoreColorChange = false;

var patterns = [
  "Pride",
  "Color Waves",

  "Rainbow Twinkles",
  "Snow Twinkles",
  "Cloud Twinkles",
  "Incandescent Twinkles",

  "Retro C9 Twinkles",
  "Red & White Twinkles",
  "Blue & White Twinkles",
  "Red, Green & White Twinkles",
  "Fairy Light Twinkles",
  "Snow 2 Twinkles",
  "Holly Twinkles",
  "Ice Twinkles",
  "Party Twinkles",
  "Forest Twinkles",
  "Lava Twinkles",
  "Fire Twinkles",
  "Cloud 2 Twinkles",
  "Ocean Twinkles",

  "Rainbow",
  "Rainbow With Glitter",
  "Solid Rainbow",
  "Confetti",
  "Sinelon",
  "Beat",
  "Juggle",
  "Fire",
  "Water"
];

var ws = new ReconnectingWebSocket('ws://' + address + ':81/', ['arduino']);
ws.debug = true;

ws.onmessage = function(evt) {
  if(evt.data != null)
  {
    var data = JSON.parse(evt.data);
    if(data == null) return;
    switch(data.name) {
      case "power":
        if(data.value == 1) {
          $("#btnOn").attr("class", "btn btn-primary");
          $("#btnOff").attr("class", "btn btn-default");
        } else {
          $("#btnOff").attr("class", "btn btn-primary");
          $("#btnOn").attr("class", "btn btn-default");
        }
        break;

      case "pattern":
        $(".grid-item-pattern").attr("class", "grid-item-pattern btn btn-default");
        $("#pattern-button-" + data.value).attr("class", "grid-item-pattern btn btn-primary");
        break;
    }
  }
}

$(document).ready(function() {
  $("#status").html("Connecting, please wait...");

  $.get(urlBase + "all", function(data) {
    $("#status").html("Loading, please wait...");

    $.each(data, function(index, field) {
      switch (field.name) {
        case "power":
          if(field.value == 1) {
            $("#btnOn").attr("class", "btn btn-primary");
          } else {
            $("#btnOff").attr("class", "btn btn-primary");
          }
          break;

        case "pattern":
          addPatternButtons(field);
          break;
      }
    });
  });

  addColorButtons();

  $("#btnOn").click(function() {
    postValue("power", 1);
    $("#btnOn").attr("class", "btn btn-primary");
    $("#btnOff").attr("class", "btn btn-default");
  });

  $("#btnOff").click(function() {
    postValue("power", 0);
    $("#btnOff").attr("class", "btn btn-primary");
    $("#btnOn").attr("class", "btn btn-default");
  });

  $("#status").html("Ready");
});

function addColorButtons() {
  var hues = 25;
  var hueStep = 360 / hues;

  var levels = 10;
  var levelStep = 60 / levels;

  for(var l = 20; l < 80; l += levelStep) {
    for(var h = 0; h < hues; h++) {
      addColorButton(h * hueStep, 100, l);
    }
  }

  $('.grid-color').isotope({
    itemSelector: '.grid-item-color',
    layoutMode: 'fitRows'
  });

}

var colorButtonIndex = 0;

function addColorButton(h, s, l) {
  var color = "hsla(" + h + ", " + s + "%, " + l + "%, 1)"
  var template = $("#colorButtonTemplate").clone();
  template.attr("id", "color-button-" + colorButtonIndex++);
  template.css("background-color", color);
  template.click(function() {
    var rgb = $(this).css('backgroundColor');
    var components = rgbToComponents(rgb);

    $(".grid-item-color").css("border", "none");
    $(this).css("border", "1px solid");

    postColor("solidColor", components);
  });

  $("#colorButtonsRow").append(template);
}

function addPatternButtons(patternField) {
  $.each(patternField.options, function(index, pattern) {
    if($.inArray(pattern, patterns) == -1)
      return;

    var template = $("#patternButtonTemplate").clone();
    template.attr("id", "pattern-button-" + index);
    template.text(pattern);
    template.click(function() {
      postValue("patternName", pattern);
      $(".grid-item-color").css("border", "none");
      $(".grid-item-pattern").attr("class", "grid-item-pattern btn btn-default");
      $(this).attr("class", "grid-item-pattern btn btn-primary");
    });

    $("#patternGrid").append(template);
  });

  $('.grid-pattern').isotope({
    itemSelector: '.grid-item-pattern',
    layoutMode: 'fitRows'
  });

  $("#pattern-button-" + patternField.value).attr("class", "grid-item-pattern btn btn-primary");
}

function postValue(name, value) {
  $("#status").html("Setting " + name + ": " + value + ", please wait...");

  var body = { name: name, value: value };

  $.post(urlBase + name, body, function(data) {
    if (data.name != null) {
      $("#status").html("Set " + name + ": " + data.name);
    } else {
      $("#status").html("Set " + name + ": " + data);
    }
  });
}

function delayPostValue(name, value) {
  clearTimeout(postValueTimer);
  postValueTimer = setTimeout(function() {
    postValue(name, value);
  }, 300);
}

function postColor(name, value) {
  $("#status").html("Setting " + name + ": " + value.r + "," + value.g + "," + value.b + ", please wait...");

  var body = { name: name, r: value.r, g: value.g, b: value.b };

  $.post(urlBase + name + "?r=" + value.r + "&g=" + value.g + "&b=" + value.b, body, function(data) {
    $("#status").html("Set " + name + ": " + data);
  })
  .fail(function(textStatus, errorThrown) { $("#status").html("Fail: " + textStatus + " " + errorThrown); });
}

function delayPostColor(name, value) {
  clearTimeout(postColorTimer);
  postColorTimer = setTimeout(function() {
    postColor(name, value);
  }, 300);
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
