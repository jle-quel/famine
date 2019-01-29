# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jle-quel <jle-quel@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/12/07 10:51:42 by jle-quel          #+#    #+#              #
#    Updated: 2019/01/28 13:34:02 by jle-quel         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# **************************************************************************** #
# INIT
# **************************************************************************** #

NAME = famine 

SRC_PATH_C = src_c
OBJ_PATH_C = obj_c
SRC_PATH_S = src_s
OBJ_PATH_S = obj_s
INC_PATH = inc

SRC_NAME_C =	main.c			\

SRC_NAME_S =

OBJ_NAME_C = $(SRC_NAME_C:.c=.o)
OBJ_NAME_S = $(SRC_NAME_S:.s=.o)

CFLAGS = -Wall -Wextra -Werror -g3 -fsanitize=address

RED=[1;31m
GREEN=[1;32m
NC=[0m

SRC_C = $(addprefix $(SRC_PATH_C)/,$(SRC_NAME_C))
SRC_S = $(addprefix $(SRC_PATH_S)/,$(SRC_NAME_S))
OBJ_C = $(addprefix $(OBJ_PATH_C)/,$(OBJ_NAME_C))
OBJ_S = $(addprefix $(OBJ_PATH_S)/,$(OBJ_NAME_S))

.PHONY: all, clean, fclean, re

# **************************************************************************** #
# COMPILATION 
# **************************************************************************** #

all: $(NAME)

$(NAME): $(OBJ_C) $(OBJ_S)
	@gcc $(CFLAGS) $^ -o $@

$(OBJ_PATH_C)/%.o: $(SRC_PATH_C)/%.c Makefile
	@mkdir $(OBJ_PATH_C) 2> /dev/null || true
	@gcc $(CFLAGS) -c $< -o $@ -I$(INC_PATH)
	@echo "$(GREEN)[✓]$(NC) Source compiled : $<"

$(OBJ_PATH_S)/%.o: $(SRC_PATH_S)/%.s Makefile
	@mkdir $(OBJ_PATH_S) 2> /dev/null || true
	@nasm -f elf64 $< -o $@
	@echo "$(GREEN)[✓]$(NC) Source compiled : $<"

clean:
	@rm -f $(OBJ_C) $(OBJ_S)
	@rmdir $(OBJ_PATH_C) $(OBJ_PATH_S) 2> /dev/null || true
	@echo "$(RED)[-]$(NC) Objects cleaned"

fclean: clean
	@rm -rf $(NAME)
	@echo "$(RED)[-]$(NC) Program clear"

re:fclean
	@make

