cmake_minimum_required(VERSION 3.28)
project(sae_s21 C)

set(CMAKE_C_STANDARD 11)

add_executable(sae_s21
        main.c
        src/adresse.c
        src/reseau.c
        src/algos.c
        src/graphe.c
        src/station.c
        src/switch.c
        src/config.c
        src/utils.c
        src/trame.c
        include/trame.h
        src/stp.c
        include/stp.h
)

