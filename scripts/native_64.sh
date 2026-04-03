build() {
    west build . -b native_sim/native/64 --build-dir build_native_64
}

CMD=$1
shift

$CMD "$@"