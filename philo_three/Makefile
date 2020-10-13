# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: deddara <deddara@student.21-school.ru>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/10/12 17:51:28 by deddara           #+#    #+#              #
#    Updated: 2020/10/12 17:51:29 by deddara          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo_three
OBJ_DIR = obj/
HDR_DIR = ./includes/

COMPILE_FLGS = gcc -I$(HDR_DIR) -Wall -Wextra -pthread

.PHONY: all clean fclean re

SRC_DIR = src/
SRC = $(addprefix $(SRC_DIR), utils atoi itoa init_and_processes philo_utils philo_simulation)
SRC.O = $(addprefix $(OBJ_DIR), $(SRC:=.o))

MAIN_SRC = main
MAIN_SRC.O = $(addprefix $(OBJ_DIR), $(MAIN_SRC:=.o))

all: $(OBJ_DIR) $(NAME)


$(NAME): $(SRC.O) $(MAIN_SRC.O)
	@$(COMPILE_FLGS) $(SRC.O) $(MAIN_SRC.O) -o $(NAME)
	@echo "\033[32m[+] Make completed!\033[0m"

$(SRC.O): $(OBJ_DIR)%.o: %.c
	$(COMPILE_FLGS) -O2 -c $< -o $@

$(MAIN_SRC.O): $(OBJ_DIR)%.o: %.c
	$(COMPILE_FLGS) -O2 -c  $< -o $@

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)/$(SRC_DIR)

clean:
	@rm -rf $(OBJ_DIR)

fclean : clean
	@rm -f $(NAME)

re : fclean all