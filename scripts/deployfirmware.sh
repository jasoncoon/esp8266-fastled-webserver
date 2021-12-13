outputDir=build
binFilename=tree-v2.ino.bin
ip=${1:-"192.168.86.36"}
url="http://$ip/update"

# TODO -- update to be based on script's own directory
# TODO -- update to switch to build directory
pushd ../esp8266-fastled-webserver > /dev/null

curl -v --form "file=@$outputDir/$binFilename;filename=$binFilename" $url

popd > /dev/null
