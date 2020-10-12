cmake_minimum_required(VERSION 3.17)
project(Philosophers)

set(CMAKE_CXX_STANDARD 14)

include_directories(philo_two/includes
                    philo_one/includes)

add_executable(Philosophers
#        philo_one/includes/philo.h
#        philo_one/src/atoi.c
#        philo_one/src/utils.c
#        philo_one/src/itoa.c
#        philo_one/src/philo_simulation.c
#        philo_one/src/philo_utils.c
#        philo_one/src/init_and_threads.c
#        philo_one/main.c
#        philo_one/Makefile
        philo_two/includes/philo.h
        philo_two/src/atoi.c
        philo_two/src/utils.c
        philo_two/src/itoa.c
        philo_two/src/philo_simulation.c
        philo_two/src/philo_utils.c
        philo_two/src/init_and_threads.c
        philo_two/main.c
        philo_two/Makefile)
