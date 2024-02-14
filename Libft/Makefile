# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: asuc <asuc@student.42angouleme.fr>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/08/12 17:42:16 by asuc              #+#    #+#              #
#    Updated: 2023/08/12 17:42:16 by asuc             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a
COMP = clang
CFLAGS = -Wall -Werror -Wextra
RESET	=	$(shell echo "\033[0m")
BCyan	=	$(shell echo "\033[1;36m")
BGreen	=	$(shell echo "\033[1;32m")
Green	=	$(shell echo "\033[0;32m")



LIB = ft_strlen.c\
	ft_atoi.c\
	ft_strdup.c\
	ft_strlcat.c\
	ft_strlcpy.c\
	ft_strncmp.c\
	ft_isalpha.c\
	ft_isdigit.c\
	ft_isalnum.c\
	ft_isascii.c\
	ft_bzero.c\
	ft_calloc.c\
	ft_isprint.c\
	ft_tolower.c\
	ft_toupper.c\
	ft_strchr.c\
	ft_strrchr.c\
	ft_memset.c\
	ft_strnstr.c\
	ft_memcmp.c\
	ft_memcpy.c\
	ft_memmove.c\
	ft_memchr.c\

ADDITIONAL = ft_substr.c\
	ft_strjoin.c\
	ft_strtrim.c\
	ft_split.c\
	ft_itoa.c\
	ft_strmapi.c\
	ft_striteri.c\
	ft_putchar_fd.c\
	ft_putstr_fd.c\
	ft_putendl_fd.c\
	ft_putnbr_fd.c\

BONUS =	ft_lstadd_back_bonus.c\
	ft_lstadd_front_bonus.c\
	ft_lstlast_bonus.c\
	ft_lstnew_bonus.c\
	ft_lstsize_bonus.c\
	ft_lstdelone_bonus.c\
	ft_lstclear_bonus.c\
	ft_lstiter_bonus.c\
	ft_lstmap_bonus.c\
	get_next_line.c\
	get_next_line_utils.c\
	ft_printf_fomat_utils.c\
	ft_printf.c\
	ft_printf_utils.c\

SRC = $(LIB) $(ADDITIONAL) $(BONUS)

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
		echo "\n$(BCyan)Compiling object files for $(NAME)...\n$(RESET)"; \
	fi
	@$(COMP) -gdwarf-4 -fPIE -O3 -march=native $(CFLAGS) -o $@ -c $<
	@$(call update_progress)


$(NAME) : $(OBJ)
	@ar -rcs $(NAME) $(OBJ)
	@echo "$(BGreen)Compilation OK $(NAME)$(RESET)\n"


clean :
	@rm -f $(OBJ)

fclean : clean
	@rm -f $(NAME)

re : fclean all

.PHONY: all fclean clean re
