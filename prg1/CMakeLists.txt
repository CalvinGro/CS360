cmake_minimum_required(VERSION 3.21)
project(program_1_1)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(parent prg1_1_parent.c)
add_executable(child0 prg1_1_child0.c)
add_executable(child1 prg1_1_child1.c)
add_executable(child2 prg1_1_child2.c)
add_executable(child3 prg1_1_child3.c)
add_executable(grandchild prg1_1_grandchild.c)




# cmake --build .  (from inside build/ to compile)
# ./parent         (from inside build/ to run)