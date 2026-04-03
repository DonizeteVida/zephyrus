build() {
    west build . -b esp32s3_devkitc/esp32s3/procpu --build-dir build_esp32s3
}

CMD=$1
shift

$CMD "$@"