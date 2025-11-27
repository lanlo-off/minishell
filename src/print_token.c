// ...existing code...
#include "../includes/minishell.h"
#include <stdio.h>

/* helper: token type -> string */
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

/* helper: subword type -> string */
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

/* imprime tous les tokens et, pour chaque token, ses subwords */
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
