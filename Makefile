# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: asuc <asuc@student.42angouleme.fr>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/08/12 17:42:16 by asuc              #+#    #+#              #
#    Updated: 2024/02/03 03:08:48 by asuc             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

BGreen		=	$(shell echo "\033[1;32m")
RESET		=	$(shell echo "\033[0m")
BRed		=	$(shell echo "\033[1;31m")
BCyan		=	$(shell echo "\033[1;36m")
Green		=	$(shell echo "\033[0;32m")
NAME		=	pipex
COMP		=	clang
CFLAGS		=	-Wall -Werror -Wextra
libft		=	Libft/
SRC			=	srcs/pipex.c \
				srcs/random_utils.c \
				srcs/pipe.c \
				srcs/pipe_utils.c \
				srcs/invalid_file.c \
				srcs/here_doc.c \
				srcs/here_doc_utils.c \
				srcs/command_parsing.c \
				srcs/command_parsing_utils.c \
				srcs/clean.c \
				srcs/check_args.c \
				srcs/pipex_utils.c

OBJ = $(SRC:.c=.o)

TOTAL = $(words $(OBJ))
CURR = 0

define update_progress
  $(eval CURR=$(shell echo $$(($(CURR)+1))))
  @echo -n "\033[1A\033[K"
  @echo -n "$(Green)[`echo "scale=2; $(CURR)/$(TOTAL)*100" | bc`%] Compiling $< \n$(RESET)"
endef

all : $(NAME)

%.o : %.c
	@if [ $(CURR) -eq 0 ]; then \
		echo "\n$(BCyan)Compiling object files for $(NAME)...$(RESET)\n"; \
	fi
	@$(COMP) -gdwarf-4 -fPIE $(CFLAGS) -o $@ -c $<
	@$(B)
	@$(call update_progress)

start :
	@echo "$(BCyan)Compilation Start$(NAME)$(RESET)\n\n"

$(NAME) : $(OBJ)
	@make --quiet --no-print-directory -C $(libft)
	@cp $(libft)libft.a libft.a
	@clang -gdwarf-4 -fPIE $(CFLAGS) -o $(NAME) $(OBJ) libft.a
	@echo "\n$(BGreen)Compilation Final $(NAME)$(RESET)"

clean :
	@make clean --quiet --no-print-directory -C $(libft)
	@rm -f $(OBJ)
	@echo "$(BRed)Erase all .o files$(RESET)"

fclean : clean
	@make fclean --quiet --no-print-directory -C $(libft)
	@rm -f $(NAME) libft.a
	@echo "$(BRed)Erase $(NAME), libft.a(RESET)"

valgrind: all
	@valgrind --trace-children=yes --track-fds=yes --log-fd=2 --error-limit=no --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(NAME) in cat cat out

valgrindAll : all
	@valgrind --track-fds=yes --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(NAME) here_doc maellapute cat cat out

test: all
	@./$(NAME) in wc wc out
re : fclean all

.PHONY: all fclean clean re


