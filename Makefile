# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hko-ko <hko-ko@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/12/26 12:00:00 by hko-ko            #+#    #+#              #
#    Updated: 2026/03/14 21:10:00 by hko-ko           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME        = minishell

CC          = cc
CFLAGS      = -Wall -Wextra -Werror -I./include -I./libft

LIBFT_DIR   = ./libft
LIBFT       = $(LIBFT_DIR)/libft.a

SRC_DIR     = src
OBJ_DIR     = obj

# Source files (to be expanded as we implement)
SRC_FILES   = main.c \
			parsing/lexer.c \
			parsing/lexer_utils.c \
			parsing/parser.c \
			parsing/parser_redir.c \
			parsing/token_utils.c \
			parsing/parser_utils.c \
			parsing/expand_utils.c \
			parsing/expand_helpers.c \
			parsing/wildcards.c \
			parsing/wildcard_helpers.c \
			parsing/wildcards_exec.c \
			execution/executor.c \
			execution/exec_loop.c \
			execution/redirection.c \
			execution/exec_utils.c \
			execution/path_utils.c \
			execution/builtin_executor.c \
			builtins/builtins.c \
			builtins/builtins_utils.c \
			builtins/env_builtins.c \
			utils/env_utils.c \
			signals.c

SRCS        = $(addprefix $(SRC_DIR)/, $(SRC_FILES))


OBJS        = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

# Readline path for Mac
ifeq ($(shell uname),Darwin)
	READLINE_PREFIX = $(shell brew --prefix readline)
	CFLAGS += -I$(READLINE_PREFIX)/include
	LDFLAGS += -L$(READLINE_PREFIX)/lib
endif

LIBS        = -lreadline -L$(LIBFT_DIR) -lft

all: $(NAME)

$(LIBFT):
	@make -C $(LIBFT_DIR)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) $(LIBS) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@make -C $(LIBFT_DIR) clean
	@rm -rf $(OBJ_DIR)

fclean: clean
	@make -C $(LIBFT_DIR) fclean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
