#include "../../includes/minishell.h"

volatile sig_atomic_t g_signal = 0;

void handle_sigint(int sig) {
  (void)sig;
  g_signal = SIGINT;
  ft_putchar_fd('\n', 1);
  rl_on_new_line();
  rl_replace_line("", 0);
  rl_redisplay();
}

void handle_heredoc_sigint(int sig) {
  (void)sig;
  g_signal = SIGINT;
  ioctl(STDIN_FILENO, TIOCSTI, "\n");
  rl_on_new_line();
  rl_replace_line("", 0);
}

void init_signals(void) {
  signal(SIGINT, handle_sigint);
  signal(SIGQUIT, SIG_IGN);
}

void ignore_signals(void) {
  signal(SIGINT, SIG_IGN);
  signal(SIGQUIT, SIG_IGN);
}

void default_signals(void) {
  signal(SIGINT, SIG_DFL);
  signal(SIGQUIT, SIG_DFL);
}
