/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobenais <mobenais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/16 18:00:00 by mobenais          #+#    #+#             */
/*   Updated: 2026/05/16 18:00:00 by mobenais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/codexion.h"

static void	heap_swap(t_heap_node *a, t_heap_node *b)
{
	t_heap_node	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

static void	heap_sift_up(t_heap_node *heap, int i)
{
	int	parent;

	while (i > 0)
	{
		parent = (i - 1) / 2;
		if (heap[parent].key <= heap[i].key)
			return ;
		heap_swap(&heap[parent], &heap[i]);
		i = parent;
	}
}

static void	heap_sift_down(t_heap_node *heap, int len, int i)
{
	int	smallest;
	int	left;
	int	right;

	while (1)
	{
		left = 2 * i + 1;
		right = 2 * i + 2;
		smallest = i;
		if (left < len && heap[left].key < heap[smallest].key)
			smallest = left;
		if (right < len && heap[right].key < heap[smallest].key)
			smallest = right;
		if (smallest == i)
			return ;
		heap_swap(&heap[i], &heap[smallest]);
		i = smallest;
	}
}

void	heap_push(t_heap_node *heap, int *len, t_coder *coder, long key)
{
	heap[*len].coder = coder;
	heap[*len].key = key;
	(*len)++;
	heap_sift_up(heap, *len - 1);
}

void	heap_pop(t_heap_node *heap, int *len)
{
	if (*len <= 0)
		return ;
	(*len)--;
	if (*len > 0)
	{
		heap[0] = heap[*len];
		heap_sift_down(heap, *len, 0);
	}
}
