/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lem_in.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agelloz <agelloz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/14 16:33:35 by agelloz           #+#    #+#             */
/*   Updated: 2019/09/24 14:35:14 by agelloz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

size_t	get_graph_size(t_list *file)
{
	size_t size;

	size = 0;
	while (file && file->next && (is_node(file->content)
				|| is_command(file->content)
				|| is_comment_or_false_command(file->content)))
	{
		if (is_command(file->content) && !is_node(file->next->content))
				return (FAILURE);
		else if (is_node(file->content))
			size++;	
		file = file->next;
	}
	if (size < 2 || !file || !is_edge(file->content))
		return (FAILURE);
	return (size);
}

int8_t	add_edges(t_graph *graph, t_list *file)
{
	(void)file;
	add_one_edge(graph, 0, 1);
	return (SUCCESS);	
}

int8_t	check_nodes_duplicates(t_graph *graph)
{
	(void)graph;
	return (SUCCESS);
}

int8_t	check_nodes(t_graph *graph, t_list *file, size_t size)
{
	size_t	i;
	char	**node_data;

	i = 0;
	while (i < size)
	{
		graph->array[i].source = 0;
		graph->array[i].sink = 0;
		if (is_comment_or_false_command(file->content))
			file = file->next;
		if (ft_strequ(file->content, "##start"))
		{
			file = file->next;
			graph->array[i].source = 1;
		}
		if (ft_strequ(file->content, "##end"))
		{
			file = file->next;
			graph->array[i].sink = 1;
		}
		if ((node_data = ft_strsplit(file->content, ' ')) == NULL)
			return (FAILURE);
		if ((graph->array[i].name = ft_strdupfree(node_data[0])) == NULL)
			return (exit_node_error(node_data));
		if ((graph->array[i].x_coord = ft_atol(node_data[1])) < 1)
			return (FAILURE);
		free(node_data[1]);
		if ((graph->array[i].y_coord = ft_atol(node_data[2])) < 1)
			return (FAILURE);
		free(node_data[2]);
		free(node_data);
		file = file->next;
		i++;
	}
	return (check_nodes_duplicates(graph));
}

int8_t	check_edges(t_graph *graph, t_list *file, size_t size)
{
	(void)size;
	(void)file;
	(void)graph;
	return (SUCCESS);
}

t_graph	*build_graph(t_list *file)
{
	t_graph	*graph;
	size_t	ants;
	size_t	size;

	ants = 0;
	graph = NULL;
	if (!is_ants(file->content) || (ants = ft_atol(file->content)) < 1)
		return (exit_graph_error(graph, file));
	if ((size = get_graph_size(file->next)) == FAILURE)
		return (exit_graph_error(graph, file));
	if ((graph = create_graph(size)) == NULL)
		return (exit_graph_error(graph, file));
	if (check_nodes(graph, file->next, size) == FAILURE)
		return (exit_graph_error(graph, file));
	if (check_edges(graph, file->next, size) == FAILURE)
		return (exit_graph_error(graph, file));
	if (add_edges(graph, file) == FAILURE)
		return (exit_graph_error(graph, file));
	return (graph);
}

t_graph	*parse_file(void)
{
	t_graph	*graph;
	t_list	*file;
	t_list	*curr;

	if ((file = save_file()) == NULL)
		return (NULL);
	ft_printf("File saved successfully\n");
	curr = file;
	while (curr)
	{
		ft_printf("%s\n", curr->content);
		curr = curr->next;
	}
	graph = NULL;
	if ((graph = build_graph(file)) == NULL)
		return (NULL);
	ft_printf("Graph built successfully\n");
	ft_lstdel(&file, ft_delcontent);
	return (graph);
}

int		main(void)
{
	t_graph	*graph;

	graph = NULL;
	//ft_printf("size_t:%lu\ns_edge:%lu\nchar*:%lu\nint8_t:%lu\nint:%lu\n", sizeof(size_t), sizeof(t_edge), sizeof(char *), sizeof(int8_t), sizeof(int));
	if ((graph = parse_file()) == NULL)
		return (EXIT_FAILURE);
	print_graph(graph);
	if ((graph = edmonds_karp(graph)) == NULL)
		return (EXIT_FAILURE);
	free_graph(graph);
	return (EXIT_SUCCESS);
}
