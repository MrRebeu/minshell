#include "minishell.h"

int	execute_external_command(t_tree_node *root, char **env)
{
	pid_t		pid;
	int			status;
	const char	*path;

	path = find_cmd_path(root, env);
	if (!path)
	{
		write(2, "minishell: command not found: ", 30);
		write(2, root->cmd->args[0], strlen(root->cmd->args[0]));
		write(2, "\n", 1);
		return (127); // code pour "commande non trouvée"
	}
	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		return (1);
	}
	else if (pid == 0)
	{
		struct sigaction sa_quit;
        sa_quit.sa_handler = SIG_DFL;  // Comportement par défaut
        sigemptyset(&sa_quit.sa_mask);
        sa_quit.sa_flags = 0;
        sigaction(SIGQUIT, &sa_quit, NULL);
		if (root->cmd->input_file || root->cmd->output_file
			|| root->cmd->heredoc_limiter)
		{
			setup_redirections(root->cmd);
		}
		if (execve(path, root->cmd->args, env) == -1)
		{
			perror("minishell");
			exit(1);
		}
	}
	else
	{
		waitpid(pid, &status, 0);
		return (status);
	}
	return (0);
}

int	execute_command(t_tree_node *root, char **env)
{
	if (!root->cmd || !root->cmd->args || !root->cmd->args[0])
		return (1);
	if (strcmp(root->cmd->args[0], "echo") == 0)
		return (ft_echo(root));
	else if (strcmp(root->cmd->args[0], "cd") == 0)
		return (ft_cd(root, env));
	else if (strcmp(root->cmd->args[0], "pwd") == 0)
		return (ft_pwd());
	else if (strcmp(root->cmd->args[0], "export") == 0)
		return (ft_export(root, env));
	else if (strcmp(root->cmd->args[0], "unset") == 0)
		return (ft_unset(root, env));
	else if (strcmp(root->cmd->args[0], "env") == 0)
		return (ft_env(env));
	else if (strcmp(root->cmd->args[0], "exit") == 0)
		return (ft_exit(root));
	else
		return (execute_external_command(root, env));
}

int	execute_tree(t_tree_node *root, char **env)
{
	int	status;

	status = 0;
	if (!root)
		return (0);
	if (root->type == NODE_COMMAND)
	{
		status = execute_command(root, env);
	}
	else if (root->type == NODE_PIPE)
	{
		status = execute_pipe(root, env);
	}
	else if (root->type == NODE_AND)
	{
		status = execute_tree(root->left, env);
		if (status == 0)
			status = execute_tree(root->right, env);
	}
	else if (root->type == NODE_OR)
	{
		status = execute_tree(root->left, env);
		if (status != 0)
			status = execute_tree(root->right, env);
	}
	return (status);
}
void	free_commands(t_command *cmd)
{
	int			i;
	t_command	*temp;

	while (cmd)
	{
		temp = cmd->next;
		if (cmd->args)
		{
			i = 0;
			while (cmd->args[i])
			{
				free(cmd->args[i]);
				i++;
			}
			free(cmd->args);
		}
		if (cmd->input_file)
			free(cmd->input_file);
		if (cmd->output_file)
			free(cmd->output_file);
		free(cmd);
		cmd = temp;
	}
}
void	free_tree(t_tree_node *node)
{
	if (!node)
		return ;
	free_tree(node->left);
	free_tree(node->right);
	if (node->type == NODE_COMMAND && node->cmd)
		free_commands(node->cmd);
	free(node);
}

void	free_tokens(t_token *tokens)
{
	t_token	*temp;

	while (tokens)
	{
		temp = tokens->next;
		if (tokens->value)
			free(tokens->value);
		free(tokens);
		tokens = temp;
	}
}
int	main(int argc, char **argv, char **envp)
{
	char		*line;
	t_tree_node	*root;
	int			status;
	t_token		*tokens;
	t_command	*cmds;
	(void)argc;
	(void)argv;
	status = 0;
	setup_signals();
	while (1)
	{
		handle_signals(); 
        line = readline("minishell> ");
		if (!line) // si readline == null alors -> ctrl+D
		{
			printf("\nexit\n");
			break ;
		}
		if (*line)
			add_history(line);
		tokens = tokenize(line);
		cmds = parse(tokens, envp, status);
		root = build_tree(cmds);
		if (root)
			status = execute_tree(root, envp);
		free(line);
		if (root)
			free_tree(root);
	}
	rl_clear_history();
	return (status);
}
