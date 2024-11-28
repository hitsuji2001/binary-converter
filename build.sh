#!/bin/sh

COMPLIER=g++
BIN_FOLDER=bin
TEST_FOLDER=tests
GG_TEST_NAME=ggtest.a

if [ ! -f "${BIN_FOLDER}/${GG_TEST_NAME}" ]; then
  echo "[INFO]: Building Google Test Framework..."
  ${COMPLIER} -Wall -Wextra -Werror -O3 -std=c++17 -pthread -c \
    -I "googletest/googletest/include" \
    -I "googletest/googletest" \
    -I "googletest/googlemock/include" \
    -I "googletest/googlemock" \
    googletest/googletest/src/gtest-all.cc \
    googletest/googletest/src/gtest_main.cc \
    googletest/googlemock/src/gmock-all.cc \
    googletest/googlemock/src/gmock_main.cc

    mkdir -p ${BIN_FOLDER}
    mv -v *.o ${BIN_FOLDER}

    echo "[INFO]: Building static library..."
    ar rcs ${BIN_FOLDER}/ggtest.a ${BIN_FOLDER}/*.o

    rm -v ${BIN_FOLDER}/*.o
else
      echo "[INFO]: ${BIN_FOLDER}/${GG_TEST_NAME} already exists. Skip building"
fi

echo "[INFO]: Building tests..."

TEST_NAME=ieee754
${COMPLIER} -Wall -Wextra -Werror -ggdb -D_DEBUG -o ${BIN_FOLDER}/${TEST_NAME} \
    -I "googletest/googletest/include" \
    -I "googletest/googletest" \
    -I "googletest/googlemock/include" \
    -I "googletest/googlemock" \
    -I . \
    ./api.c \
    ${BIN_FOLDER}/${GG_TEST_NAME} \
    ${TEST_FOLDER}/${TEST_NAME}.cpp

TEST_NAME=conversion
${COMPLIER} -Wall -Wextra -Werror -ggdb -D_DEBUG -o ${BIN_FOLDER}/${TEST_NAME} \
    -I "googletest/googletest/include" \
    -I "googletest/googletest" \
    -I "googletest/googlemock/include" \
    -I "googletest/googlemock" \
    -I . \
    ./api.c \
    ${BIN_FOLDER}/${GG_TEST_NAME} \
    ${TEST_FOLDER}/${TEST_NAME}.cpp

echo "[INFO]: Done."
