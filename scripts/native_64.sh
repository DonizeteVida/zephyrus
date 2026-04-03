build() {
    west build . -b native_sim/native/64 --build-dir build_native_64
}

run() {
    ./build_native_64/zephyr/zephyr.exe
}

CMD=$1
shift

$CMD "$@"