#include "minishell.h"

void execute_pipe_left(int *pipefd, t_tree_node *left, char **env)
{
    printf("DEBUG LEFT: Commande: ");
    for (int i = 0; left->cmd->args[i]; i++) {
        printf("[%s] ", left->cmd->args[i]);
    }
    printf("\n");
    
    printf("DEBUG LEFT: input_file: %s\n", left->cmd->input_file ? left->cmd->input_file : "NULL");
    printf("DEBUG LEFT: output_file: %s\n", left->cmd->output_file ? left->cmd->output_file : "NULL");
    
    close(pipefd[0]);
    printf("DEBUG LEFT: Closed read end of pipe\n");
    
    dup2(pipefd[1], STDOUT_FILENO);
    printf("DEBUG LEFT: Redirected stdout to pipe\n");
    
    close(pipefd[1]);
    printf("DEBUG LEFT: Closed write end of pipe\n");
    
    if (left->cmd->input_file || left->cmd->heredoc_limiter || left->cmd->output_file) {
        printf("DEBUG LEFT: Setting up redirections\n");
        setup_redirections(left->cmd);
    }
    
    printf("DEBUG LEFT: Executing command\n");
    exit(execute_tree(left, env));
}

void execute_pipe_right(int *pipefd, t_tree_node *right, char **env)
{
    printf("DEBUG RIGHT: Commande: ");
    for (int i = 0; right->cmd->args[i]; i++) {
        printf("[%s] ", right->cmd->args[i]);
    }
    printf("\n");
    
    printf("DEBUG RIGHT: input_file: %s\n", right->cmd->input_file ? right->cmd->input_file : "NULL");
    printf("DEBUG RIGHT: output_file: %s\n", right->cmd->output_file ? right->cmd->output_file : "NULL");
    
    close(pipefd[1]);
    printf("DEBUG RIGHT: Closed write end of pipe\n");
    
    dup2(pipefd[0], STDIN_FILENO);
    printf("DEBUG RIGHT: Redirected stdin from pipe\n");
    
    close(pipefd[0]);
    printf("DEBUG RIGHT: Closed read end of pipe\n");
    
    if (right->cmd->input_file || right->cmd->heredoc_limiter || right->cmd->output_file) {
        printf("DEBUG RIGHT: Setting up redirections\n");
        setup_redirections(right->cmd);
    }
    
    printf("DEBUG RIGHT: Executing command\n");
    exit(execute_tree(right, env));
}
int	execute_pipe(t_tree_node *root, char **env)
{
	int		pipefd[2];
	int		status;
	pid_t	pid1;
	pid_t	pid2;

	printf("DEBUG: Creating pipe\n");
	if (pipe(pipefd) == -1)
	{
		perror("Error creating pipe");
		return (1);
	}
	printf("DEBUG: Forking left process\n");
	pid1 = fork();
	if (pid1 == -1)
	{
		perror("Error fork left");
		close(pipefd[0]);
		close(pipefd[1]);
		return (1);
	}
	else if (pid1 == 0)
	{
		printf("DEBUG: Executing left command\n");
		execute_pipe_left(pipefd, root->left, env);
	}
	printf("DEBUG: Forking right process\n");
	pid2 = fork();
	if (pid2 == -1)
	{
		perror("Error fork right");
		close(pipefd[0]);
		close(pipefd[1]);
		waitpid(pid1, NULL, 0);
		return (1);
	}
	else if (pid2 == 0)
	{
		printf("DEBUG: Executing right command\n");
		execute_pipe_right(pipefd, root->right, env);
	}
	printf("DEBUG: Parent closing pipe\n");
	close(pipefd[0]);
	close(pipefd[1]);
	printf("DEBUG: Waiting for children\n");
	waitpid(pid1, NULL, 0);
	waitpid(pid2, &status, 0);
	printf("DEBUG: Pipe execution completed\n");
	return (status);
}
