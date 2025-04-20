#include "minishell.c"

// volatile sig_atomic_t g_signal = 0;

void sig_handler(int signum)
{
    g_signal = signum;
}

//initialisation des signaux
void setup_signals(void)
{
    struct sigaction sa;

    sa.sa_handler = sig_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);//SIGINT (Ctrl+C)
    sigaction(SIGQUIT, &sa, NULL);//SIGQUIT (Ctrl+\)
}

//gérer les signaux dans la boucle principale
void handle_signals(void)
{
	if (g_signal == SIGINT) // ctl+C
	{
        //remet le prompt a 0
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_signal = 0; // global var a 0
	}
    else if (g_signal == SIGQUIT) // ctl+ '\'
        g_signal = 0;
}
