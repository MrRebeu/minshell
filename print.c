/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 22:02:08 by tcaccava          #+#    #+#             */
/*   Updated: 2025/04/12 06:48:09 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_tokens(t_token *head)
{
	char	*types[] = {"WORD", "PIPE", "REDIR_IN", "REDIR_OUT", "APPEND",
			"HEREDOC", "TOKEN_AND", " TOKEN_OR", "EOF"};
	t_token	*current;

	current = head;
	printf("Tokens: \n");
	while (current)
	{
		printf("*Type: %s, value:%s\n", types[current->type], current->value);
		current = current->next;
	}
	printf("End\n");
}

void	print_commands(t_command *cmd_list)
{
	int			i;
	const char	*separator_names[] = {"NONE", "PIPE", "AND", "OR", "END"};

	while (cmd_list)
	{
		printf("Command:\n");
		i = 0;
		if (cmd_list->args)
		{
			printf(" args:");
			while (cmd_list->args[i])
			{
				printf(" %s", cmd_list->args[i]);
				i++;
			}
			printf("\n");
		}
		else
			printf(" args: none\n");
		if (cmd_list->input_file)
			printf(" input: %s\n", cmd_list->input_file);
		else
			printf(" input: none\n");
		if (cmd_list->output_file)
			printf(" output: %s (append: %d)\n", cmd_list->output_file,
				cmd_list->append);
		else
			printf(" output: none\n");
		printf(" separator: %s\n", separator_names[cmd_list->next_separator]);
		printf("---\n");
		cmd_list = cmd_list->next;
	}
}

void	print_tree(t_tree_node *node, int level)
{
	int	i;

	i = 0;
	if (!node)
		return ;
	while (i < level)
	{
		printf(" ");
		i++;
	}
	if (node->type == NODE_COMMAND)
	{
		printf("COMMAND: ");
		i = 0;
		while (node->cmd->args && node->cmd->args[i])
		{
			printf("%s ", node->cmd->args[i]);
			i++;
		}
		printf("\n");
	}
	else if (node->type == NODE_AND)
		printf("AND\n");
	else if (node->type == NODE_OR)
		printf("OR\n");
	else if (node->type == NODE_PIPE)
		printf("PIPE\n");
	print_tree(node->left, level + 1);
	print_tree(node->right, level + 1);
}
