cmake_minimum_required(VERSION 3.21)
project(program_2)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(parent parent.c)
add_executable(child child.c)
add_executable(helper helper.c)




# cmake --build .  (from inside build/ to compile)
# ./parent         (from inside build/ to run)