#include "minishell.h"

volatile sig_atomic_t g_signal = 0;

void sig_handler(int signum)
{
    if (signum == SIGINT)
    {
        write(STDOUT_FILENO, "\n", 1);
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
        g_signal = 0;
    }
    else if (signum == SIGQUIT)
    {
        g_signal = signum;  // Juste enregistrer le signal
    }
}

void setup_signals(void)
{
    struct sigaction sa;

    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = sig_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGINT, &sa, NULL) == -1)
    {
        perror("Erreur configuration SIGINT");
        exit(1);
    }
    if (sigaction(SIGQUIT, &sa, NULL) == -1)
    {
        perror("Erreur configuration SIGQUIT");
        exit(1);
    }
    // Désactiver la gestion des signaux par readline
    rl_catch_signals = 0;
}

void handle_signals(void)
{
    if (g_signal == SIGINT) 
    {
        // Afficher nouvelle ligne et prompt pour Ctrl+C
        write(STDOUT_FILENO, "\n", 1);
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
        g_signal = 0;
    }
    else if (g_signal == SIGQUIT)
    {
        // Pour Ctrl+\, ne rien faire en mode interactif
        g_signal = 0;  // On réinitialise simplement g_signal
    }
}
