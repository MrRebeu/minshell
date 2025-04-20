/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abkhefif <abkhefif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:32:06 by tcaccava          #+#    #+#             */
/*   Updated: 2025/04/20 14:49:06 by abkhefif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_separator(t_command *cmd, t_token *token)
{
	if (token && token->type == TOKEN_PIPE)
		cmd->next_separator = SEP_PIPE;
	else if (token && token->type == TOKEN_AND)
		cmd->next_separator = SEP_AND;
	else if (token && token->type == TOKEN_OR)
		cmd->next_separator = SEP_OR;
	else
		cmd->next_separator = SEP_END;
}

// void	handle_redirection_token(t_token **tokens, t_command *cmd, char **envp,
// 		int status)
// {
// 	char	*expanded;
// 	int		type;

// 	type = (*tokens)->type;
// 	if ((*tokens)->next && (*tokens)->next->type == TOKEN_WORD)
// 	{
// 		*tokens = (*tokens)->next;
// 		if ((*tokens)->quote_type != '\'')
// 			expanded = expand_variables((*tokens)->value, envp, status);
// 		else
// 			expanded = ft_strdup((*tokens)->value);
// 		if (type == IN)
// 			cmd->input_file = expanded;
// 		else if (type == OUT)
// 			cmd->output_file = expanded;
// 		else if (type == APPEND)
// 		{
// 			cmd->output_file = expanded;
// 			cmd->append = 1;
// 		}
// 		else if (type == HERE)
// 			cmd->heredoc_limiter = expanded;
// 	}
// }

int handle_redirection_token(t_token **tokens, t_command *cmd, char **envp, int status)
{
    char    *expanded;
    int     type;

    type = (*tokens)->type;
    if ((*tokens)->next && (*tokens)->next->type == TOKEN_WORD)
    {
        *tokens = (*tokens)->next;
        if ((*tokens)->quote_type != '\'')
            expanded = expand_variables((*tokens)->value, envp, status);
        else
            expanded = ft_strdup((*tokens)->value);
        if (type == IN)
            cmd->input_file = expanded;
        else if (type == OUT)
            cmd->output_file = expanded;
        else if (type == APPEND)
        {
            cmd->output_file = expanded;
            cmd->append = 1;
        }
        else if (type == HERE)
            cmd->heredoc_limiter = expanded;
		printf("DEBUG: Redirection détectée: %s -> %s\n", 
       (*tokens)->type == OUT ? ">" : (*tokens)->type == APPEND ? ">>" : (*tokens)->type == IN ? "<" : "<<", 
       (*tokens)->value);
        return 1;  // Indique que nous avons avancé au token suivant
    }
    return 0;  // Indique que nous n'avons pas avancé
}

// void	process_tokens(t_token **tokens, t_command *cmd, char **envp,
// 		int exit_status)
// {
// 	char	*expanded;

// 	while (*tokens && (*tokens)->type != TOKEN_AND
// 		&& (*tokens)->type != TOKEN_OR && (*tokens)->type != TOKEN_EOF
// 		&& (*tokens)->type != TOKEN_PIPE)
// 	{
// 		if ((*tokens)->type == TOKEN_WORD)
// 		{
// 			if ((*tokens)->quote_type != '\'')
// 				expanded = expand_variables((*tokens)->value, envp,
// 						exit_status);
// 			else
// 				expanded = ft_strdup((*tokens)->value);
// 			add_to_args(cmd, expanded);
// 			free(expanded);
// 		}
// 		else if (((*tokens)->type == IN || (*tokens)->type == OUT
// 				|| (*tokens)->type == APPEND || (*tokens)->type == HERE))
// 		{
// 			handle_redirection_token(tokens, cmd, envp, exit_status);
// 			continue ;
// 		}
// 		*tokens = (*tokens)->next;
// 	}
// }
void process_tokens(t_token **tokens, t_command *cmd, char **envp, int exit_status)
{
    char    *expanded;
    int     token_handled;
    t_token *prev_token = NULL;

    while (*tokens && (*tokens)->type != TOKEN_AND
        && (*tokens)->type != TOKEN_OR && (*tokens)->type != TOKEN_EOF
        && (*tokens)->type != TOKEN_PIPE)
    {
        // Si le token actuel a déjà été traité comme fichier de redirection, le sauter
        if (prev_token && (prev_token->type == IN || prev_token->type == OUT
                || prev_token->type == APPEND || prev_token->type == HERE))
        {
            prev_token = *tokens;
            *tokens = (*tokens)->next;
            continue;
        }

        token_handled = 0;
        
        if ((*tokens)->type == TOKEN_WORD)
        {
            if ((*tokens)->quote_type != '\'')
                expanded = expand_variables((*tokens)->value, envp, exit_status);
            else
                expanded = ft_strdup((*tokens)->value);
            add_to_args(cmd, expanded);
            free(expanded);
        }
        else if (((*tokens)->type == IN || (*tokens)->type == OUT
                || (*tokens)->type == APPEND || (*tokens)->type == HERE))
        {
            prev_token = *tokens;  // Mémoriser ce token comme étant une redirection
            token_handled = handle_redirection_token(tokens, cmd, envp, exit_status);
            if (token_handled)
                continue;
        }
        
        prev_token = *tokens;
        *tokens = (*tokens)->next;
    }
}


void	apply_wildcards_to_cmd_list(t_command *cmd_list)
{
	t_command	*current;

	current = cmd_list;
	while (current)
	{
		process_args_with_wildcards(current);
		current = current->next;
	}
}

t_command	*parse(t_token *tokens, char **envp, int exit_status)
{
	t_command	*cmd_list;
	t_command	*new_cmd;

	cmd_list = NULL;
	while (tokens && tokens->type != TOKEN_EOF)
	{
		new_cmd = init_cmd();
		if (!new_cmd)
			return (NULL);
		process_tokens(&tokens, new_cmd, envp, exit_status);
		if (!tokens)
			return (NULL);
		set_separator(new_cmd, tokens);
		add_cmd_to_list(&cmd_list, new_cmd);
		if (tokens && (tokens->type == TOKEN_PIPE || tokens->type == TOKEN_AND
				|| tokens->type == TOKEN_OR))
			tokens = tokens->next;
	}
	/////
	printf("DEBUG: Commande finale: ");
	for (int i = 0; cmd_list->args[i]; i++) {
		printf("[%s] ", cmd_list->args[i]);
	}
	printf("\noutput_file: %s, append: %d\n", 
		cmd_list->output_file ? cmd_list->output_file : "NULL", 
		cmd_list->append);
	////
	apply_wildcards_to_cmd_list(cmd_list);
	return (cmd_list);
}
