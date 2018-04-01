// used when hosting the site on the ESP8266
var address = location.hostname;
var urlBase = "";

// used when hosting the site somewhere other than the ESP8266 (handy for testing without waiting forever to upload to SPIFFS)
// var address = "esp8266-1920f7.local";
// var urlBase = "http://" + address + "/";

var postColorTimer = {};
var postValueTimer = {};

var ignoreColorChange = false;

var ws = new ReconnectingWebSocket('ws://' + address + ':81/', ['arduino']);
ws.debug = true;

ws.onmessage = function(evt) {
  if(evt.data != null)
  {
    var data = JSON.parse(evt.data);
    if(data == null) return;
    updateFieldValue(data.name, data.value);
  }
}

$(document).ready(function() {
  $("#status").html("Connecting, please wait...");

  $.get(urlBase + "all", function(data) {
      $("#status").html("Loading, please wait...");

      $.each(data, function(index, field) {
        if (field.type == "Number") {
          addNumberField(field);
        } else if (field.type == "Boolean") {
          addBooleanField(field);
        } else if (field.type == "Select") {
          addSelectField(field);
        } else if (field.type == "Color") {
          addColorFieldPalette(field);
          addColorFieldPicker(field);
        } else if (field.type == "Section") {
          addSectionField(field);
        }
      });

      $(".minicolors").minicolors({
        theme: "bootstrap",
        changeDelay: 200,
        control: "wheel",
        format: "rgb",
        inline: true
      });

      $("#status").html("Ready");
    })
    .fail(function(errorThrown) {
      console.log("error: " + errorThrown);
    });
});

function addNumberField(field) {
  var template = $("#numberTemplate").clone();

  template.attr("id", "form-group-" + field.name);
  template.attr("data-field-type", field.type);

  var label = template.find(".control-label");
  label.attr("for", "input-" + field.name);
  label.text(field.label);

  var input = template.find(".input");
  var slider = template.find(".slider");
  slider.attr("id", "input-" + field.name);
  if (field.min) {
    input.attr("min", field.min);
    slider.attr("min", field.min);
  }
  if (field.max) {
    input.attr("max", field.max);
    slider.attr("max", field.max);
  }
  if (field.step) {
    input.attr("step", field.step);
    slider.attr("step", field.step);
  }
  input.val(field.value);
  slider.val(field.value);

  slider.on("change mousemove", function() {
    input.val($(this).val());
  });

  slider.on("change", function() {
    var value = $(this).val();
    input.val(value);
    field.value = value;
    delayPostValue(field.name, value);
  });

  input.on("change", function() {
    var value = $(this).val();
    slider.val(value);
    field.value = value;
    delayPostValue(field.name, value);
  });

  $("#form").append(template);
}

function addBooleanField(field) {
  var template = $("#booleanTemplate").clone();

  template.attr("id", "form-group-" + field.name);
  template.attr("data-field-type", field.type);

  var label = template.find(".control-label");
  label.attr("for", "btn-group-" + field.name);
  label.text(field.label);

  var btngroup = template.find(".btn-group");
  btngroup.attr("id", "btn-group-" + field.name);

  var btnOn = template.find("#btnOn");
  var btnOff = template.find("#btnOff");

  btnOn.attr("id", "btnOn" + field.name);
  btnOff.attr("id", "btnOff" + field.name);

  btnOn.attr("class", field.value ? "btn btn-primary" : "btn btn-default");
  btnOff.attr("class", !field.value ? "btn btn-primary" : "btn btn-default");

  btnOn.click(function() {
    setBooleanFieldValue(field, btnOn, btnOff, 1)
  });
  btnOff.click(function() {
    setBooleanFieldValue(field, btnOn, btnOff, 0)
  });

  $("#form").append(template);
}

function addSelectField(field) {
  var template = $("#selectTemplate").clone();

  template.attr("id", "form-group-" + field.name);
  template.attr("data-field-type", field.type);

  var id = "input-" + field.name;

  var label = template.find(".control-label");
  label.attr("for", id);
  label.text(field.label);

  var select = template.find(".form-control");
  select.attr("id", id);

  for (var i = 0; i < field.options.length; i++) {
    var optionText = field.options[i];
    var option = $("<option></option>");
    option.text(optionText);
    option.attr("value", i);
    select.append(option);
  }

  select.val(field.value);

  select.change(function() {
    var value = template.find("#" + id + " option:selected").index();
    postValue(field.name, value);
  });

  var previousButton = template.find(".btn-previous");
  var nextButton = template.find(".btn-next");

  previousButton.click(function() {
    var value = template.find("#" + id + " option:selected").index();
    var count = select.find("option").length;
    value--;
    if(value < 0)
      value = count - 1;
    select.val(value);
    postValue(field.name, value);
  });

  nextButton.click(function() {
    var value = template.find("#" + id + " option:selected").index();
    var count = select.find("option").length;
    value++;
    if(value >= count)
      value = 0;
    select.val(value);
    postValue(field.name, value);
  });

  $("#form").append(template);
}

function addColorFieldPicker(field) {
  var template = $("#colorTemplate").clone();

  template.attr("id", "form-group-" + field.name);
  template.attr("data-field-type", field.type);

  var id = "input-" + field.name;

  var input = template.find(".minicolors");
  input.attr("id", id);

  if(!field.value.startsWith("rgb("))
    field.value = "rgb(" + field.value;

  if(!field.value.endsWith(")"))
    field.value += ")";

  input.val(field.value);

  var components = rgbToComponents(field.value);

  var redInput = template.find(".color-red-input");
  var greenInput = template.find(".color-green-input");
  var blueInput = template.find(".color-blue-input");

  var redSlider = template.find(".color-red-slider");
  var greenSlider = template.find(".color-green-slider");
  var blueSlider = template.find(".color-blue-slider");

  redInput.attr("id", id + "-red");
  greenInput.attr("id", id + "-green");
  blueInput.attr("id", id + "-blue");

  redSlider.attr("id", id + "-red-slider");
  greenSlider.attr("id", id + "-green-slider");
  blueSlider.attr("id", id + "-blue-slider");

  redInput.val(components.r);
  greenInput.val(components.g);
  blueInput.val(components.b);

  redSlider.val(components.r);
  greenSlider.val(components.g);
  blueSlider.val(components.b);

  redInput.on("change", function() {
    var value = $("#" + id).val();
    var r = $(this).val();
    var components = rgbToComponents(value);
    field.value = r + "," + components.g + "," + components.b;
    $("#" + id).minicolors("value", "rgb(" + field.value + ")");
    redSlider.val(r);
  });

  greenInput.on("change", function() {
    var value = $("#" + id).val();
    var g = $(this).val();
    var components = rgbToComponents(value);
    field.value = components.r + "," + g + "," + components.b;
    $("#" + id).minicolors("value", "rgb(" + field.value + ")");
    greenSlider.val(g);
  });

  blueInput.on("change", function() {
    var value = $("#" + id).val();
    var b = $(this).val();
    var components = rgbToComponents(value);
    field.value = components.r + "," + components.g + "," + b;
    $("#" + id).minicolors("value", "rgb(" + field.value + ")");
    blueSlider.val(b);
  });

  redSlider.on("change", function() {
    var value = $("#" + id).val();
    var r = $(this).val();
    var components = rgbToComponents(value);
    field.value = r + "," + components.g + "," + components.b;
    $("#" + id).minicolors("value", "rgb(" + field.value + ")");
    redInput.val(r);
  });

  greenSlider.on("change", function() {
    var value = $("#" + id).val();
    var g = $(this).val();
    var components = rgbToComponents(value);
    field.value = components.r + "," + g + "," + components.b;
    $("#" + id).minicolors("value", "rgb(" + field.value + ")");
    greenInput.val(g);
  });

  blueSlider.on("change", function() {
    var value = $("#" + id).val();
    var b = $(this).val();
    var components = rgbToComponents(value);
    field.value = components.r + "," + components.g + "," + b;
    $("#" + id).minicolors("value", "rgb(" + field.value + ")");
    blueInput.val(b);
  });

  redSlider.on("change mousemove", function() {
    redInput.val($(this).val());
  });

  greenSlider.on("change mousemove", function() {
    greenInput.val($(this).val());
  });

  blueSlider.on("change mousemove", function() {
    blueInput.val($(this).val());
  });

  input.on("change", function() {
    if (ignoreColorChange) return;

    var value = $(this).val();
    var components = rgbToComponents(value);

    redInput.val(components.r);
    greenInput.val(components.g);
    blueInput.val(components.b);

    redSlider.val(components.r);
    greenSlider.val(components.g);
    blueSlider.val(components.b);

    field.value = components.r + "," + components.g + "," + components.b;
    delayPostColor(field.name, components);
  });

  $("#form").append(template);
}

function addColorFieldPalette(field) {
  var template = $("#colorPaletteTemplate").clone();

  var buttons = template.find(".btn-color");

  var label = template.find(".control-label");
  label.text(field.label);

  buttons.each(function(index, button) {
    $(button).click(function() {
      var rgb = $(this).css('backgroundColor');
      var components = rgbToComponents(rgb);

      field.value = components.r + "," + components.g + "," + components.b;
      postColor(field.name, components);

      ignoreColorChange = true;
      var id = "#input-" + field.name;
      $(id).minicolors("value", "rgb(" + field.value + ")");
      $(id + "-red").val(components.r);
      $(id + "-green").val(components.g);
      $(id + "-blue").val(components.b);
      $(id + "-red-slider").val(components.r);
      $(id + "-green-slider").val(components.g);
      $(id + "-blue-slider").val(components.b);
      ignoreColorChange = false;
    });
  });

  $("#form").append(template);
}

function addSectionField(field) {
  var template = $("#sectionTemplate").clone();

  template.attr("id", "form-group-section-" + field.name);
  template.attr("data-field-type", field.type);

  $("#form").append(template);
}

function updateFieldValue(name, value) {
  var group = $("#form-group-" + name);

  var type = group.attr("data-field-type");

  if (type == "Number") {
    var input = group.find(".form-control");
    input.val(value);
  } else if (type == "Boolean") {
    var btnOn = group.find("#btnOn" + name);
    var btnOff = group.find("#btnOff" + name);

    btnOn.attr("class", value ? "btn btn-primary" : "btn btn-default");
    btnOff.attr("class", !value ? "btn btn-primary" : "btn btn-default");

  } else if (type == "Select") {
    var select = group.find(".form-control");
    select.val(value);
  } else if (type == "Color") {
    var input = group.find(".form-control");
    input.val("rgb(" + value + ")");
  }
};

function setBooleanFieldValue(field, btnOn, btnOff, value) {
  field.value = value;

  btnOn.attr("class", field.value ? "btn btn-primary" : "btn btn-default");
  btnOff.attr("class", !field.value ? "btn btn-primary" : "btn btn-default");

  postValue(field.name, field.value);
}

function postValue(name, value) {
  $("#status").html("Setting " + name + ": " + value + ", please wait...");

  var body = { name: name, value: value };

  $.post(urlBase + name + "?value=" + value, body, function(data) {
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
