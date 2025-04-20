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
void execute_pipe_left(int *pipefd, t_tree_node *left, char **env)
{
    // Fermer l'extrémité de lecture que nous n'utilisons pas
    close(pipefd[0]);
    // Rediriger la sortie standard vers le pipe
    dup2(pipefd[1], STDOUT_FILENO);
    close(pipefd[1]);
    // Configurer les redirections si nécessaires
    if (left->cmd->input_file || left->cmd->heredoc_limiter || left->cmd->output_file)
        setup_redirections(left->cmd);
    // Exécuter la commande de gauche et sortir
    exit(execute_tree(left, env));
}

void execute_pipe_right(int *pipefd, t_tree_node *right, char **env)
{
    // Fermer l'extrémité d'écriture que nous n'utilisons pas
    close(pipefd[1]);
    // Rediriger l'entrée standard depuis le pipe
    dup2(pipefd[0], STDIN_FILENO);
    close(pipefd[0]);
    // Configurer les redirections si nécessaires
    if (right->cmd->input_file || right->cmd->heredoc_limiter || right->cmd->output_file)
        setup_redirections(right->cmd);
    // Exécuter la commande de droite et sortir
    exit(execute_tree(right, env));
}

int execute_pipe(t_tree_node *root, char **env)
{
    int pipefd[2];
    pid_t pid1;
    int pid2;
    int status;
    
    if (pipe(pipefd) == -1)
    {
        perror("Error creating pipe");
        return 1;
    }
    pid1 = fork();
    if (pid1 == 0)
        execute_pipe_left(pipefd, root->left, env);
    pid2 = fork();
    if (pid2 == 0)
        execute_pipe_right(pipefd, root->right, env);
    // frmer les extrémités du pipe dans le processus parent
    close(pipefd[0]);
    close(pipefd[1]);
    // attendre les processus enfants
    waitpid(pid1, NULL, 0);
    waitpid(pid2, &status, 0);
    return status;
}
