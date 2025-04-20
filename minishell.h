#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdarg.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>

volatile sig_atomic_t g_signal = 0;


typedef enum e_node_type
{
	NODE_COMMAND,
	NODE_PIPE,
	NODE_AND,
	NODE_OR,
}						t_node_type;

typedef enum e_separator
{
	SEP_NONE,
	SEP_PIPE,
	SEP_AND,
	SEP_OR,
	SEP_END
}						t_separator;

typedef enum e_toke_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC,
	TOKEN_AND,
	TOKEN_OR,
	TOKEN_EOF,

}						t_token_type;

typedef struct s_token
{
	t_token_type		type;
	char				*value;
	struct s_token		*next;
	char            quote_type;
}						t_token;

typedef struct s_command
{
	char				**args;
	char				*input_file;
	int					input_fd;
	int					output_fd;
	char				*output_file;
	char				*heredoc_limiter;
	int					append;
	t_separator			next_separator;
	struct s_command	*next;
}						t_command;

typedef struct s_tree_node
{
	t_node_type			type;
	t_command			*cmd;
	struct s_tree_node	*left;
	struct s_tree_node	*right;
}						t_tree_node;


int ft_echo(t_command *cmd);
int ft_env(char **envp);
void ft_exit(t_command *cmd);
void ft_unset(t_command *cmd, char **envp);

#endif
