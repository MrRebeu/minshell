/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 07:25:51 by tcaccava          #+#    #+#             */
/*   Updated: 2025/04/12 11:55:17 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(void)
{
	int		i;
	pid_t	pid;
	int		status;
	pid_t	terminate;

	i = 1;
	while (i <= 4)
	{
		pid = fork();
		if (pid == 0)
		{
			sleep(i);
			printf("processo %d creato dal padre con pid: %d\n", i, getpid());
			printf("process %d terminato\n", i);
			exit(i);
		}
		i++;
	}
	terminate = 0;
	while ((terminate = waitpid(-1, &status, 0)) > 0)
		printf("padre rilevato terminazione del figlio
			%d con exit status :%d\n", terminate, WEXITSTATUS(status));
}

t_struct s_type_redirect
{
	int 1 or
		char *left_redirection   // if <
		char *right_redirection; // if >
									// if >>
									// if <<
}

if (cmd->redirect->type_redirect == 2)

	typedef struct s_metac
	{
	} t_struct s_redirect
	{
		int	*type_redirect;
	}
t_redirect;

typedef struct s_cmd
{
	char	**cmd;
	char	**paths;
	char	**envp;
	t_redirect *redirect; // <    >  = 2 t_redirect
	// int pipe;
}			t_cmd;

while (i < end of string)
{
	if (str[i] == ' ' &&)
		if (str[i] == >)
			cmd->redirect > type_red = R_direct;
	if (""
		"" == <)
		= L_direct;
	>> = RR_direct << = LL_drect;
}
agrv[0] argv[1] str = echo courage |
	ls -
		la test.txt

		while str[i]
{
	if (access(get_path(argv[0]), X_OK) = 0)
	{
		cmd nb++ argv[0]->cmd struct;
	}
	else if acces getpath (argv[0] , F_OK) == 0)
	// do something with a file
}
int			i = 0 while i<nb{

			execve()}

					input.txt>
			cat | echo |
	wc < output.txt

			if (cmd->cmd check on the path) execv(path(bin / echo), argv[0],
				envp);

int	main(void)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		printf("error\n");
	else if (pid == 0)
		printf("figlio PID: %d", getpid());
	else if (pid > 0)
		printf("padre PID: %d\n", getpid());
}

int	main(void)
{
	int	fd;

	fd = open("text4.txt", O_WRONLY);
	dup2(fd, 1);
	printf("ciaone");
	if (isatty(1))
		printf("terminal");
	else
		printf("not terminal");
}

int	main(int argc, char **argv, char **envp)
{
	char			cwd[1024];
	struct stat		file_stat;
	int				fd;
	struct stat		file_stat2;
	char			*args[] = {"input.txt", "cat", "wc", "output.txt", NULL};
	DIR				*dir;
	struct dirent	*directory;
	char			**path;
	int				i;
	char			*PATH;

	getcwd(cwd, sizeof(cwd));
	printf("%s\n", cwd);
	if (chdir("/nfs/homes/tcaccava/") == 0)
		printf("%s\n", getcwd(cwd, sizeof(cwd)));
	stat("test.txt", &file_stat);
	printf("%ld\n", file_stat.st_size);
	fd = open("test2.txt", O_RDONLY);
	fstat(fd, &file_stat2);
	close(fd);
	printf("%ld\n", file_stat2.st_ctime);
	dir = opendir("/nfs/homes/tcaccava/MINISHELL/minishell");
	while ((directory = readdir(dir)) != NULL)
		printf("File: %s\n", directory->d_name);
	closedir(dir);
	path = get_paths(envp);
	i = 0;
	while (path[i])
	{
		printf("%s\n", path[i]);
		i++;
	}
	PATH = getenv("PATH");
	printf("%s\n", PATH);
}

void	print(int signal)
{
	printf("\nsignal acquired\n");
}

int	main(void)
{
	int		fd;
	char	*line;
	char	buffer[128];
	ssize_t	bytes_read;
	int		pipefd[2];
	pid_t	pid;
		int status;
	pid_t	child_pid;

	if (access("test.txt", F_OK) == 0)
		printf("exists\n");
	if (access("test.txt", R_OK) == 0)
		printf("readable\n");
	if (access("test.txt", W_OK) == 0)
		printf("writable\n");
	fd = open("test.txt", O_RDONLY);
	if (fd < 0)
	{
		perror("Failed to open file");
		return (1);
	}
	while ((line = get_next_line(fd)) != NULL)
	{
		printf("%s", line);
		free(line);
	}
	bytes_read = read(fd, buffer, sizeof(buffer));
	printf("\n%zu\n", bytes_read);
	printf("%s", buffer);
	close(fd);
	pid = fork();
	if (pid > 0)
	{
		child_pid = waitpid(pid, &status, 0);
		printf("father : PID %d waits child : PID %d with status termination
			%d\n", getpid(), child_pid, WEXITSTATUS(status));
	}
	else
	{
		printf("child in execution PID: %d\n", getpid());
		sleep(1);
		exit(42);
	}
	signal(SIGINT, print);
	sleep(5);
	return (0);
}

t_command	*parse(t_token *tokens, char **envp, int exit_status)
{
	t_command *cmd_list;
	t_command *new_cmd;
	char *expanded_value;

	cmd_list = NULL;
	new_cmd = NULL;
	while (tokens && tokens->type != TOKEN_EOF)
	{
		new_cmd = init_cmd();
		if (!new_cmd)
			return (NULL);
		while (tokens && tokens->type != TOKEN_AND && tokens->type != TOKEN_OR
			&& tokens->type != TOKEN_PIPE && tokens->type != TOKEN_EOF)
		{
			if (tokens->type == TOKEN_WORD)
			{
				if (tokens->quote_type != '\'')
					expanded_value = expand_variables(tokens->value, envp,
							exit_status);
				else
					expanded_value = ft_strdup(tokens->value);
				add_to_args(new_cmd, expanded_value);
				if (tokens->quote_type == 0 && contain_wildcard(expanded_value))
					process_args_with_wildcards(new_cmd);
				free(expanded_value);
			}
			else if (tokens->type == TOKEN_REDIR_IN && tokens->next
				&& tokens->next->type == TOKEN_WORD)
			{
				tokens = tokens->next;
				if (tokens->quote_type != '\'')
					expanded_value = expand_variables(tokens->value, envp,
							exit_status);
				else
					expanded_value = ft_strdup(tokens->value);
				new_cmd->input_file = expanded_value;
			}
			else if (tokens->type == TOKEN_REDIR_OUT && tokens->next
				&& tokens->next->type == TOKEN_WORD)
			{
				tokens = tokens->next;
				if (tokens->quote_type != '\'')
					expanded_value = expand_variables(tokens->value, envp,
							exit_status);
				else
					expanded_value = ft_strdup(tokens->value);
				new_cmd->output_file = expanded_value;
				new_cmd->append = 0;
			}
			else if (tokens->type == TOKEN_APPEND && tokens->next
				&& tokens->next->type == TOKEN_WORD)
			{
				tokens = tokens->next;
				if (tokens->quote_type != '\'')
					expanded_value = expand_variables(tokens->value, envp,
							exit_status);
				else
					expanded_value = ft_strdup(tokens->value);
				new_cmd->output_file = expanded_value;
				new_cmd->append = 1;
			}
			else if (tokens->type == TOKEN_HEREDOC && tokens->next
				&& tokens->next->type == TOKEN_WORD)
			{
				tokens = tokens->next;
				if (tokens->quote_type != '\'')
					expanded_value = expand_variables(tokens->value, envp,
							exit_status);
				else
					expanded_value = ft_strdup(tokens->value);
				new_cmd->heredoc_limiter = expanded_value;
			}
			tokens = tokens->next;
		}
		if (tokens && tokens->type == TOKEN_PIPE)
			new_cmd->next_separator = SEP_PIPE;
		else if (tokens && tokens->type == TOKEN_AND)
			new_cmd->next_separator = SEP_AND;
		else if (tokens && tokens->type == TOKEN_OR)
			new_cmd->next_separator = SEP_OR;
		else
			new_cmd->next_separator = SEP_END;
		add_cmd_to_list(&cmd_list, new_cmd);
		if (tokens && (tokens->type == TOKEN_PIPE || tokens->type == TOKEN_AND
				|| tokens->type == TOKEN_OR))
			tokens = tokens->next;
	}
	return (cmd_list);
}