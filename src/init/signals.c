#include "../../includes/minishell.h"

void rl_replace_line(const char *text, int clear_undo);

void handle_sigint(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	if(g_signal_received == 0)
		rl_redisplay();
	// g_signal_received = 130;
}

void init_signals(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}
