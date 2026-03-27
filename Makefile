NAME        = minishell
CC          = cc
CFLAGS      = -Wall -Wextra -Werror -Iinclude
LIBFT_DIR   = libft
LIBFT       = $(LIBFT_DIR)/libft.a
OBJ_DIR     = obj
SRC_DIR     = src
DEPS        = include/minishell.h

# MacOS readline paths (for Apple Silicon and Intel Macs via Homebrew)
READLINE_LIB = -lreadline
READLINE_INC = 
ifeq ($(shell uname), Darwin)
    ifneq ($(wildcard /opt/homebrew/opt/readline/lib),)
        READLINE_LIB = -L/opt/homebrew/opt/readline/lib -lreadline
        READLINE_INC = -I/opt/homebrew/opt/readline/include
    else ifneq ($(wildcard /usr/local/opt/readline/lib),)
        READLINE_LIB = -L/usr/local/opt/readline/lib -lreadline
        READLINE_INC = -I/usr/local/opt/readline/include
    endif
endif

SRC_PARSING = lexer.c lexer_utils.c parser.c parser_utils.c parser_redir.c \
              token_utils.c expand_utils.c expand_vars.c expand_helpers.c \
              wildcards.c wildcard_helpers.c wildcards_exec.c
SRC_EXEC    = executor.c executor_main.c builtin_executor.c exec_utils.c \
              exec_loop.c redirection.c redirection_utils.c path_utils.c
SRC_BUILTIN = builtins.c builtins_utils.c env_builtins.c
SRC_UTILS   = env_utils.c
SRC_ROOT    = main.c signals.c

SRCS = $(addprefix $(SRC_DIR)/parsing/, $(SRC_PARSING)) \
       $(addprefix $(SRC_DIR)/execution/, $(SRC_EXEC)) \
       $(addprefix $(SRC_DIR)/builtins/, $(SRC_BUILTIN)) \
       $(addprefix $(SRC_DIR)/utils/, $(SRC_UTILS)) \
       $(addprefix $(SRC_DIR)/, $(SRC_ROOT))

OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -L$(LIBFT_DIR) -lft $(READLINE_LIB) -o $(NAME)

$(LIBFT):
	@make -C $(LIBFT_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(DEPS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(READLINE_INC) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)
	@make -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	@make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
