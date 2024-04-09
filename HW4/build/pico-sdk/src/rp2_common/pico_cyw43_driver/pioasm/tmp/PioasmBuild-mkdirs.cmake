# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/Users/prarthana/Pico/pico-sdk/tools/pioasm"
  "/Users/prarthana/Pico/ME433/ME-433/HW2-3/build/pioasm"
  "/Users/prarthana/Pico/ME433/ME-433/HW2-3/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm"
  "/Users/prarthana/Pico/ME433/ME-433/HW2-3/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/tmp"
  "/Users/prarthana/Pico/ME433/ME-433/HW2-3/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/PioasmBuild-stamp"
  "/Users/prarthana/Pico/ME433/ME-433/HW2-3/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src"
  "/Users/prarthana/Pico/ME433/ME-433/HW2-3/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/PioasmBuild-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/prarthana/Pico/ME433/ME-433/HW2-3/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/PioasmBuild-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/prarthana/Pico/ME433/ME-433/HW2-3/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/PioasmBuild-stamp${cfgdir}") # cfgdir has leading slash
endif()
