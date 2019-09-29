/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bfs.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelkel <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/24 13:32:38 by ekelkel           #+#    #+#             */
/*   Updated: 2019/09/29 10:27:58 by agelloz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

static void		print_results(ssize_t *queue, ssize_t *prev, size_t size)
{
	size_t	i;

	i = 0;
	while (i < size)
	{
		printf("queue[%2zu] = %2zd prev[%2zu] = %2zd\n", i, queue[i], i, prev[i]);
		i++;
	}
	return ;
}

static t_bfs	*reconstruct_path(t_bfs *bfs, t_graph *graph)
{
	t_list		*tmp;
	ssize_t		i;

	tmp = NULL;
	i = graph->size - 1;
	while (graph->nodes[i].sink == FALSE)
		i--;
	while (i != -1)
	{
		tmp = ft_lstnew(&i, sizeof(ssize_t));
		ft_lstappend(&bfs->best_path, tmp);	
		i = bfs->prev[i];
	}
	ft_lstrev(&bfs->best_path);
	if (graph->nodes[*(ssize_t *)bfs->best_path->content].source != TRUE)
		return (NULL);
	return (bfs);
}

t_bfs			*bfs(t_graph *graph)
{
	size_t		node;
	t_bfs 		*bfs;
	t_edge		*neighbours;

	node = 0;
	neighbours = NULL;
	bfs = create_queue(graph);
	while (is_queue_empty(bfs) == FALSE)
	{
		node = bfs->queue_front;
		neighbours = graph->nodes[bfs->queue_front].head;
		dequeue(bfs);
		while (neighbours)
		{
			if (graph->nodes[neighbours->dest].bfs_marked != TRUE)
			{
				enqueue(bfs, neighbours->dest);
				bfs->prev[neighbours->dest] = node;
				graph->nodes[neighbours->dest].bfs_marked = TRUE;
			}
			neighbours = neighbours->next;
		}
	}
	print_results(bfs->queue, bfs->prev, graph->size);
	return (reconstruct_path(bfs, graph));
}
