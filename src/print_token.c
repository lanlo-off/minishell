#include "../includes/minishell.h"
#include <stdio.h>

static const char *token_type_str(t_token_type type)
{
    if (type == WORD)
        return ("WORD");
    if (type == PIPE)
        return ("PIPE");
    if (type == HEREDOC)
        return ("HEREDOC");
    if (type == REDIR_IN)
        return ("REDIR_IN");
    if (type == APPEND)
        return ("APPEND");
    if (type == REDIR_OUT)
        return ("REDIR_OUT");
    return ("UNKNOWN");
}

static const char *sub_type_str(t_sub_type st)
{
    if (st == SUB_UNQUOTED)
        return ("SUB_UNQUOTED");
    if (st == SUB_SQUOTED)
        return ("SUB_SQUOTED");
    if (st == SUB_DQUOTED)
        return ("SUB_DQUOTED");
    return ("UNKNOWN");
}

void	print_tokens(t_token *token)
{
    t_token		*tk;
    t_subword	*sw;
    int			ti;
    int			si;

    tk = token;
    ti = 0;
    while (tk)
    {
        printf("Token %d: full token : %s[%s]\n",
            ti,
            tk->full_token ? tk->full_token : "(null)",
            token_type_str(tk->type));
        sw = tk->subword;
        si = 0;
        while (sw)
        {
            printf("  Subword %d: %s[%s]\n",
                si,
                sw->val ? sw->val : "(null)",
                sub_type_str(sw->type));
            sw = sw->next;
            si++;
        }
		printf("\n");
        tk = tk->next;
        ti++;
    }
}

void print_commands(t_cmd **cmds)
{
    int ci = 0;
    t_cmd *cmd;

    if (!cmds)
        return;

    cmd = *cmds;
    while (cmd)
    {
        printf("Command %d:\n", ci);

        // Affichage des arguments
        if (cmd->av)
        {
            int ai = 0;
            while (cmd->av[ai])
            {
                printf("  Arg %d: %s\n", ai, cmd->av[ai]);
                ai++;
            }
        }
        else
            printf("  (no arguments)\n");

        // Affichage des redirections
        t_redir *redir = cmd->redirs;
        int ri = 0;
        while (redir)
        {
            const char *type_str;
            if (redir->type == REDIR_IN) type_str = "REDIR IN from :";
            else if (redir->type == REDIR_OUT) type_str = "REDIR OUT to :";
            else if (redir->type == APPEND) type_str = "APPEND to :";
            else if (redir->type == HEREDOC) type_str = "HEREDOC with limiter :";
            else type_str = "?";

            printf("  Redir %d: %s %s\n", ri, type_str, redir->file);
            redir = redir->next;
            ri++;
        }
        printf("\n");
        ci++;
        cmd = cmd->next;
    }
}

void print_tokens_and_cmds(t_shell *shell)
{
    printf("=== TOKENS ===\n");
    print_tokens(shell->token);

    printf("=== COMMANDS ===\n");
    print_commands(&shell->cmds);
}
