#!/bin/bash

WORKDIR=$(pwd)/firmware/
ROOTING=sudo
ESPTOOL=esptool
ESPPORT=/dev/ttyUSB0

BLANK_1_ADDR=0x3e000
INIT_2_ADDR=0x3fc000
BLANK_2_ADDR=0x3fe000

ESP_SDK_BIN=/opt/esp-open-sdk/sdk/bin/

INITBIN=${ESP_SDK_BIN}/esp_init_data_default.bin
BLANKBIN=${ESP_SDK_BIN}/blank.bin

FW_FILE_1_ADDR=0x00000
FW_FILE_2_ADDR=0x10000

FW_FILE_1=${WORKDIR}/${FW_FILE_1_ADDR}.bin
FW_FILE_2=${WORKDIR}/${FW_FILE_2_ADDR}.bin

make all
${ROOTING} ${ESPTOOL} --port ${ESPPORT} erase_flash
${ROOTING} ${ESPTOOL} --port ${ESPPORT} write_flash ${BLANK_1_ADDR} ${BLANKBIN} ${INIT_2_ADDR} ${INITBIN} ${BLANK_2_ADDR} ${BLANKBIN}
${ROOTING} ${ESPTOOL} --port ${ESPPORT} write_flash ${FW_FILE_1_ADDR} ${FW_FILE_1} ${FW_FILE_2_ADDR} ${FW_FILE_2}
