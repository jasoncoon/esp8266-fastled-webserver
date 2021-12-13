#!/bin/bash
# upload the web app to the board

# investigate using a single file to bring in the defaults
# ip=$(< ip.txt)

ip=${1:-"192.168.86.36"}
url="http://$ip/edit"
filename=$2

# add --trace-ascii curl.log for logging

# TODO -- update to be based on script's own directory
# TODO -- update to switch to build directory
pushd ../esp8266-fastled-webserver > /dev/null

gzip -kf data/$filename

echo $filename
curl --form "file=@data/$filename.gz;filename=$filename.gz" $url

rm -f data/$filename.gz

popd > /dev/null

