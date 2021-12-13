#!/bin/bash
# upload the web app to the board

# investigate using a single file to bring in the defaults
# ip=$(< ip.txt)

ip=${1:-"192.168.86.36"}
url="http://$ip/edit"

declare -a filenames=(
  "css/styles.css"
  "js/app.js"
  "index.htm"
  # "css/simple.css"
  # "js/simple.js"
  # "simple.htm"
  # "edit.htm"
  )

# "images/atom196.png"
# "favicon.ico"

# TODO -- update to be based on script's own directory
# TODO -- update to switch to build directory
pushd ../esp8266-fastled-webserver > /dev/null

for filename in "${filenames[@]}"
do
  # add --trace-ascii curl.log for logging

  gzip -kf data/$filename

  echo $filename
  curl --form "file=@data/$filename.gz;filename=$filename.gz" $url

  rm -f data/$filename.gz
done

popd > /dev/null
