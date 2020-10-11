cmake_minimum_required(VERSION 3.17)
project(Philosophers)

set(CMAKE_CXX_STANDARD 14)

include_directories(philo_one/includes)

add_executable(Philosophers
        philo_one/includes/philo.h
        philo_one/src/atoi.c
        philo_one/src/ft_putnbr.c
        philo_one/src/utils.c
        philo_one/src/itoa.c
        philo_one/main.c
        philo_one/Makefile)
