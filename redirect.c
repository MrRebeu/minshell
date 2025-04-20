#include "minishell.h"

void direct_input(t_command *cmd)
{
    int fd;
    
    fd = open(cmd->input_file, O_RDONLY);
    if (fd == -1)
    {
        perror("minishell");
        exit(1);
    }
    dup2(fd, STDIN_FILENO);
    close(fd);
}

void direct_heredoc(t_command *cmd)
{
    char *line;
    int pipefd[2];
    
    if (pipe(pipefd) == -1)
    {
        perror("minishell: pipe");
        exit(1);
    }
    while (1)
    {
        line = readline("> ");
        if (!line || strcmp(line, cmd->heredoc_limiter) == 0)
        {
            free(line);
            break;
        }
        write(pipefd[1], line, strlen(line));
        write(pipefd[1], "\n", 1);
        free(line);
    }
    close(pipefd[1]);
    dup2(pipefd[0], STDIN_FILENO);
    close(pipefd[0]);
}

void direct_output(t_command *cmd)
{
    int fd;
    int flags;
    
    flags = O_WRONLY | O_CREAT;
    if (cmd->append)
        flags |= O_APPEND;  // >>
    else
        flags |= O_TRUNC;   // >
    fd = open(cmd->output_file, flags, 0644);
    if (fd == -1)
    {
        perror("minishell");
        exit(1);
    }
    dup2(fd, STDOUT_FILENO);
    close(fd);
}
void setup_redirections(t_command *cmd)
{
    // Redirection d'entrée (<)
    if (cmd->input_file)
        direct_input(cmd);
    // Heredoc (<<)
    if (cmd->heredoc_limiter)
        direct_heredoc(cmd);
    // Redirection de sortie (> ou >>)
    if (cmd->output_file)
        direct_output(cmd);
}
