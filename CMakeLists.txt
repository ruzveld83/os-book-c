cmake_minimum_required(VERSION 3.16)
project(os_book C)

set(CMAKE_C_STANDARD 11)

include_directories("~/linux-src/lib/modules/4.4.0-186-generic/build/include")
include_directories("~/linux-src/lib/modules/4.4.0-186-generic/build/arch/x86/include")

add_executable(sandbox sandbox.c)
add_executable(ex2_24 chap2/2_24_filecopy/main.c)
add_executable(proj2 chap2/proj/jiffies.c)
add_executable(ex3_18 chap3/3_18_zombies/main.c)
add_executable(ex3_19_pipe chap3/3_19_time/pipe/main.c)
add_executable(ex3_19_shm chap3/3_19_time/shared_memory/main.c)
add_executable(ex3_20 chap3/3_20_pid_allocation/main.c)
add_executable(ex3_26 chap3/3_26_case_reverse/main.c)
add_executable(proj3_1 chap3/proj1/main.c)
add_executable(proj3_2 chap3/proj2/pid.c)
add_executable(proj3_3 chap3/proj3/tasks_linear.c chap3/proj3/tasks_dfs.c)
