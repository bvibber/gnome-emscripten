BASE=`pwd`
ROOT="$BASE/root"
BUILD="$BASE/build"

CFLAGS="-O2 -s WASM=1 -s USE_ZLIB=1"
PKG_CONFIG_PATH="$ROOT/"
mkdir -p "$ROOT" || exit 1
mkdir -p "$BUILD" || exit 1

CHECK="$1"

build_lib() {
    LIB="$1"
    CONFIG_FLAGS="$2"

    LIB_SRC="$BASE/$1"
    LIB_BUILD="$BUILD/$1"
    

    if [ ! -e "$LIB_SRC/configure" ]
    then
        (
            cd "$LIB_SRC" || exit 1
            NOCONFIGURE=1 ./autogen.sh || exit 1
        )
    fi

    (
        mkdir -p "$LIB_BUILD" || exit 1
        cd "$LIB_BUILD" || exit 1

        CONFIG_LOG="$LIB_BUILD/config.log"
        if [ -e "$CONFIG_LOG" ] && [ $(grep -c 'configure: exit 0' "$CONFIG_LOG") -eq 1 ]
        then
            echo "Using old configuration"
        else
            EMCONFIGURE_JS=2 emconfigure \
              ../../"$LIB"/configure \
              --host=wasm32 \
              --prefix="$ROOT" \
              PKG_CONFIG_PATH="$ROOT/lib/pkgconfig" \
              CFLAGS="$CFLAGS" \
              $CONFIG_FLAGS \
              || exit 1
        fi

        make || exit 1

        if [ "$CHECK" == "check" ]
        then
            # Run any tests
            # Use emconfigure to force the generated scripts to be executable
            EMCONFIGURE_JS=2 emconfigure make check || exit 1
        fi

        make install || exit 1
    ) || exit 1
}

check_lib() {
    LIB="$1"
    CONFIG_FLAGS="$2"

    LIB_SRC="$BASE/$1"
    LIB_BUILD="$BUILD/$1"
}

build_bin() {
    BIN="$1"
    BIN_CFLAGS="$2"
    BIN_SRC="$BASE/$1"
    BIN_BUILD="$BUILD/$1"

    (
        mkdir -p "$BIN_BUILD" || exit 1
        cd "$BIN_BUILD" || exit 1

        if [ ! -e "$BIN.js" ] || [ "$BIN.js" -ot "$BIN.c" ]
        then
            PKG_CONFIG_PATH="$ROOT/lib/pkgconfig" \
            emcc \
              -I"$ROOT/include" \
              -L"$ROOT/lib" \
              $CFLAGS \
              $BIN_CFLAGS \
              -o "$BIN.js" \
              "$BIN_SRC/$BIN.c" \
              "$ROOT/lib/libffi.a" \
              || exit 1
        fi
    )
}

run_bin() {
    BIN="$1"
    BIN_BUILD="$BUILD/$1"

    (cd "$BIN_BUILD" && node "$BIN") || exit 1
}

build_lib "libffi" "--disable-docs" || exit 1
build_bin "test-libffi" "-lffi" || exit 1
run_bin "test-libffi" || exit 1

#build_lib "glib" "" || exit 1

