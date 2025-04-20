/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abkhefif <abkhefif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 16:39:25 by tcaccava          #+#    #+#             */
/*   Updated: 2025/04/20 14:44:45 by abkhefif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H


# include <dirent.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <termcap.h>
# include <unistd.h>
# include <termios.h>

# define IN TOKEN_REDIR_IN
# define OUT TOKEN_REDIR_OUT
# define APPEND TOKEN_APPEND
# define HERE TOKEN_HEREDOC

extern volatile sig_atomic_t g_signal;

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
	char				quote_type;
}						t_token;

typedef struct s_command
{
	char				**args;
	char				*input_file;
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

int						ft_strcmp(char *s1, char *s2);
char					**ft_split(const char *str, char delimiter);
char					**split_words(const char *str, char delimiter,
							char **split, int *i);
char					*find_path(char *cmd, char **envp);
char					*concat_path(char *dir, char *cmd);
char					**get_paths(char **envp);
size_t					ft_strlen(const char *str);
char					*ft_strcpy(char *dest, const char *src);
char					*ft_strcat(char *dest, const char *src);
void					is_command(char *s, int i);
int						is_builtin(char *s);
char					*ft_substr(char *s, unsigned int start, size_t len);
int						is_special(char c);
int						is_whitespace(char c);
void					free_tokens(t_token *head);
void					print_tokens(t_token *head);
t_command				*init_cmd(void);
void					add_cmd_to_list(t_command **list, t_command *new_cmd);
void					add_to_args(t_command *cmd, char *arg);
t_command				*parse(t_token *tokens, char **envp, int exit_status);
void					print_commands(t_command *cmd_list);
t_tree_node				*create_cmd_node(t_command *cmd);
t_tree_node				*create_operator_node(t_node_type type,
							t_tree_node *left, t_tree_node *right);
void					print_tree(t_tree_node *node, int level);
char					*ft_strchr(char *s, char c);
char					*ft_strncpy(char *dest, const char *src, size_t n);
t_token					*tokenize(char *input);
t_tree_node				*build_tree(t_command *cmd_list);
char					*ft_strdup(const char *s);
char					*expand_variables(char *token_value, char **envp,
							int exit_status);
int						get_var_expanded_len(char *var_name, char **envp);
char					*ft_itoa(int n);
int						ft_strncmp(const char *s1, const char *s2, size_t n);
int						contain_wildcard(char *str);
int						match_wildcard_pattern(char *pattern, char *filename);
char					**expand_wildcard(char *pattern);
void					process_args_with_wildcards(t_command *cmd);
char					*ft_strstr(const char *haystack, const char *needle);
char					*ft_strjoin_char(char *str, char c);
char					*ft_strjoin(char const *s1, char const *s2);
int						handle_less_operator(char *input, int *i,
							t_token **head, t_token **current);
int						handle_greater_operator(char *input, int *i,
							t_token **head, t_token **current);
int						handle_pipe_or(char *input, int *i, t_token **head,
							t_token **current);
int						handle_and(char *input, int *i, t_token **head,
							t_token **current);
int						handle_word_token(char *input, int *i, t_token **head,
							t_token **current);
int						handle_token(char *input, int *i, t_token **head,
							t_token **current);
int						handle_redirection(char *input, int *i, t_token **head,
							t_token **current);
int						handle_quotes(char *input, int *i, t_token **head,
							t_token **current);
int						handle_word(char *input, int *i, t_token **head,
							t_token **current);
t_token					*create_token(t_token_type type, char *value);
void					add_token(t_token **head, t_token **current,
							t_token *new_token);

							
void					set_separator(t_command *cmd, t_token *token);
// void					handle_redirection_token(t_token **tokens,
// 							t_command *cmd, char **envp, int status);
int handle_redirection_token(t_token **tokens, t_command *cmd, char **envp, int status);



void					process_tokens(t_token **tokens, t_command *cmd,
							char **envp, int exit_status);
char					*ft_strndup(const char *s, size_t len);
char					*handle_dollar_sign(char *token_value, int *i,
							char *result, int exit_status);
char					*expand_status(char *result, int exit_status);
char					*expand_var_name(char *token_value, int *i);
char					*expand_env_var(char *result, char *var_name);
char					*append_char(char *result, char c);
void					free_commands(t_command *cmd);
void					free_tree(t_tree_node *node);
int						match_middle_wildcard(char *pattern, char *filename);
int						match_prefix_wildcard(char *pattern, char *filename);
int						match_suffix_wildcard(char *pattern, char *filename);
int						count_matching_entries(char *pattern);
int						fill_matches_array(char *pattern, char **matches);
char					**allocate_matches(int count);
int						count_total_args(t_command *cmd);
void					free_string_array(char **arr);
char					**create_new_args_array(t_command *cmd, int count);
int						match_middle_asterisk(char *pattern, char *filename);
int						count_wildcard_matches(char *arg);
int						add_arg_to_array(char *arg, char **new_args,
							int new_count);
int						handle_match_failure(char **matches, int i, DIR *dir);
char *find_cmd_path(t_tree_node *root, char **env);
void					direct_input(t_command *cmd);
void					direct_heredoc(t_command *cmd);
void					direct_output(t_command *cmd);
void					setup_redirections(t_command *cmd);
void					execute_pipe_left(int *pipefd, t_tree_node *left,
							char **env);
void					execute_pipe_right(int *pipefd, t_tree_node *right,
							char **env);
int						execute_pipe(t_tree_node *root, char **env);
void					direct_input(t_command *cmd);
void					direct_heredoc(t_command *cmd);
void					direct_output(t_command *cmd);
void					setup_redirections(t_command *cmd);
void					sig_handler(int signum);
void					setup_signals(void);
void					handle_signals(void);
int						set_env_var(char ***envp, const char *name,
							const char *value);
int						match_wildcards(char *wildcards, char *filename);
char					**collect_files(char *pattern);
int						exexcmd_wildcards(t_command *cmd, char *pattern,
							char **env);
int						ft_cd(t_tree_node *root, char **envp);
int						save_std_fds(int *stdin_copy, int *stdout_copy);
void					restore_std_fds(int stdin_copy, int stdout_copy);
int						ft_echo(t_tree_node *root);
int						ft_env(char **envp);
int						ft_exit(t_tree_node *root);
int						ft_export(t_tree_node *root, char **envp);
int						ft_pwd(void);
int						ft_unset(t_tree_node *root, char **envp);
int						execute_tree(t_tree_node *root, char **env);
int	ft_atoi(char *str);
char *my_getenv(char **envp, const char *name);

char **filter_redirection_args(t_command *cmd);

#endif
