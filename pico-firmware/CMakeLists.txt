# == DO NEVER EDIT THE NEXT LINES for Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.0.0)
set(toolchainVersion 13_2_Rel1)
set(picotoolVersion 2.0.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NEVER EDIT THE NEXT LINES for Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.0.0)
set(toolchainVersion 13_2_Rel1)
set(picotoolVersion 2.0.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(pico-emmc-controller C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(ir-remote
    main.cpp
    infrared.c
    common/infrared_common_decoder.c
    common/infrared_common_encoder.c
    kaseikyo/infrared_decoder_kaseikyo.c
    kaseikyo/infrared_encoder_kaseikyo.c
    kaseikyo/infrared_protocol_kaseikyo.c
    nec/infrared_decoder_nec.c
    nec/infrared_encoder_nec.c
    nec/infrared_protocol_nec.c
    pioneer/infrared_decoder_pioneer.c
    pioneer/infrared_encoder_pioneer.c
    pioneer/infrared_protocol_pioneer.c
    rc5/infrared_decoder_rc5.c
    rc5/infrared_encoder_rc5.c
    rc5/infrared_protocol_rc5.c
    rc6/infrared_decoder_rc6.c
    rc6/infrared_encoder_rc6.c
    rc6/infrared_protocol_rc6.c
    rca/infrared_decoder_rca.c
    rca/infrared_encoder_rca.c
    rca/infrared_protocol_rca.c
    samsung/infrared_decoder_samsung.c
    samsung/infrared_encoder_samsung.c
    samsung/infrared_protocol_samsung.c
    sirc/infrared_decoder_sirc.c
    sirc/infrared_encoder_sirc.c
    sirc/infrared_protocol_sirc.c)

pico_generate_pio_header(ir-remote ${CMAKE_CURRENT_LIST_DIR}/infrared.pio)

pico_set_program_name(ir-remote "ir-remote")
pico_set_program_version(ir-remote "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(ir-remote 0)
pico_enable_stdio_usb(ir-remote 1)

# Add the standard library to the build
target_link_libraries(ir-remote
        pico_stdlib
        hardware_pio)

# Add the standard include files to the build
target_include_directories(ir-remote PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts or any other standard includes, if required
)

# Add any user requested libraries
#target_link_libraries(ir-remote
#        hardware_timer
#        )
#
#pico_add_extra_outputs(ir-remote)

