cmake_minimum_required(VERSION 3.20.0)

# set(CMAKE_CXX_STANDARD 17) # This sets the C++ standard to C++17
# set(CMAKE_CXX_STANDARD_REQUIRED ON) # This makes C++17 a requirement and prevents a "decay" to C++98 when the compiler does not support C++17
# set(CMAKE_CXX_EXTENSIONS OFF) # This disables the use of compiler-specific extensions

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

#set(ZEPHYR_PREFER "zephyr" "zephyr-v3.7.0") #Zephyr preference list
#find_package(Zephyr 3.6.0 REQUIRED HINTS $ENV{ZEPHYR_BASE}) #Define a specific version of Zephyr

project(zephyrproject)
# set(KCONFIG_ROOT ${ZEPHYR_BASE}/Kconfig)

target_sources(app PRIVATE src/main.cpp)

target_link_libraries(app PRIVATE stdc++)