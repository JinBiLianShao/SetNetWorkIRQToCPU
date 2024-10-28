cmake_minimum_required(VERSION 3.10)
project(SetNetWorkIRQToCPU)

set(CMAKE_CXX_STANDARD 17)
#FT
#set(CMAKE_HOST_SYSTEM_NAME Linux)
#set(CMAKE_C_COMPILER "/bin/aarch64-linux-gnu-gcc")
#set(CMAKE_CXX_COMPILER "/bin/aarch64-linux-gnu-g++")

#7045
set(CMAKE_HOST_SYSTEM_NAME Linux)
set(CMAKE_C_COMPILER "/usr/bin/arm-linux-gnueabihf-gcc-6")
set(CMAKE_CXX_COMPILER "/usr/bin/arm-linux-gnueabihf-g++-6")
#set(CMAKE_C_COMPILER "/home/lsx/gcc-linaro-5.4.1-2017.05-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc")
#set(CMAKE_CXX_COMPILER "/home/lsx/gcc-linaro-5.4.1-2017.05-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-g++")
#set(CMAKE_CXX_LINK_EXECUTABLE "/home/lsx/gcc-linaro-5.4.1-2017.05-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-ld")


add_executable(SetNetWorkIRQToCPU main.cpp)
