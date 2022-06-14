/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_to_lines.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Juyeong Maing <jmaing@student.42seoul.kr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/11 22:21:13 by Juyeong Maing     #+#    #+#             */
/*   Updated: 2022/06/14 23:56:22 by Juyeong Maing    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_to_lines.h"

static bool	ft_memory_find(
	const void *mem,
	size_t size,
	char to_find,
	size_t *out
)
{
	const char *const	mem_c = (char *)mem;
	size_t				i;

	i = 0;
	while (i < size)
	{
		if (mem_c[i] == to_find)
		{
			if (out)
				*out = i;
			return (true);
		}
		i++;
	}
	return (false);
}

static t_err	ft_to_lines_internal_append(
	t_ft_to_lines *context,
	char **out_line
)
{
	if (!context->current)
		context->current = new_stringbuilder(context->buffer_size);
	if (!context->current)
		return (true);
	if (stringbuilder_append(
			context->current,
			context->pending_length - context->pending_offset,
			&context->pending[context->pending_offset]))
		return (true);
	context->pending = NULL;
	*out_line = NULL;
	return (false);
}

static t_err	ft_to_lines_internal_final(
	t_ft_to_lines *context,
	char **out_line,
	size_t length
)
{
	if (!context->current)
		context->current = new_stringbuilder(context->buffer_size);
	if (!context->current)
		return (true);
	if (stringbuilder_append(
			context->current,
			length,
			&context->pending[context->pending_offset]))
		return (true);
	context->pending_offset += length;
	if (context->pending_offset == context->pending_length)
	{
		context->pending = NULL;
		context->pending_length = 0;
		context->pending_offset = 0;
	}
	*out_line = stringbuilder_to_string(context->current, 0);
	if (!*out_line)
		return (true);
	stringbuilder_free(context->current);
	context->current = NULL;
	return (false);
}

static t_err	ft_to_lines_internal(
	t_ft_to_lines *context,
	char **out_line
)
{
	size_t	position;

	if (!context->pending)
	{
		*out_line = NULL;
		return (false);
	}
	if (ft_memory_find(
			&context->pending[context->pending_offset],
			context->pending_length - context->pending_offset,
			'\n',
			&position))
		return (ft_to_lines_internal_final(context, out_line, position + 1));
	return (ft_to_lines_internal_append(context, out_line));
}

t_err	ft_to_lines(
	t_ft_to_lines *context,
	const void *append,
	size_t append_length,
	char **out_line
)
{
	if (!append || !append_length)
		return (ft_to_lines_internal(context, out_line));
	if (append && context->pending)
		return (true);
	context->pending = (const char *)append;
	context->pending_offset = 0;
	context->pending_length = append_length;
	return (ft_to_lines_internal(context, out_line));
}
