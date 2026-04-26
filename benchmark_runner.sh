#!/usr/bin/env bash

set -euo pipefail

echo "Building benchmark binary..."

# Allow overriding compiler with GXX env var
GXX=${GXX:-}
if [ -z "$GXX" ]; then
    if command -v g++ >/dev/null 2>&1; then
        GXX=$(command -v g++)
    elif [ -x "/c/msys64/ucrt64/bin/g++.exe" ]; then
        GXX="/c/msys64/ucrt64/bin/g++.exe"
    elif [ -x "/c/msys64/mingw64/bin/g++.exe" ]; then
        GXX="/c/msys64/mingw64/bin/g++.exe"
    else
        echo "Error: g++ not found. Set GXX or install MSYS/MinGW and add g++ to PATH." >&2
        exit 1
    fi
fi

echo "Using compiler: $GXX"

# EXTRA_CXXFLAGS can be set externally (e.g. -pthread)
OUT=engine_bench
case "$(uname -s 2>/dev/null || echo)" in
  *MINGW*|*MSYS*|*CYGWIN*) OUT=${OUT}.exe ;;
esac

"$GXX" -std=c++17 -O3 -DNDEBUG ${EXTRA_CXXFLAGS:-} \
        driver/main.cpp \
        parsers/eventsParser.cpp \
        orderbook/orderbook.cpp \
        debug/validator.cpp \
        orders/order.cpp \
        orders/LimitOrder.cpp \
        orders/MarketOrder.cpp \
        -I. \
    -o "$OUT"

echo "Running benchmark..."
./"$OUT"