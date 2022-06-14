/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Juyeong Maing <jmaing@student.42seoul.kr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/15 10:58:57 by Juyeong Maing     #+#    #+#             */
/*   Updated: 2022/06/15 01:57:55 by Juyeong Maing    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ft_to_lines.h>

static t_err	read_line(
	t_ft_to_lines *context,
	char **out_line,
	size_t *out_length,
	bool *out_eof
)
{
	static char	buffer[43];
	size_t		bytes_read;

	*out_eof = false;
	if (ft_to_lines_drain(context, out_line, out_length))
		return (true);
	while (!*out_line)
	{
		bytes_read = fread(buffer, 1, rand() % 42 + 1, stdin);
		if (ferror(stdin))
			return (true);
		if (!bytes_read)
		{
			*out_eof = true;
			if (ft_to_lines_get_current_line(context, out_line, out_length))
				return (true);
			ft_to_lines_free(context);
			return (false);
		}
		if (ft_to_lines_feed(context, buffer, bytes_read)
			|| ft_to_lines_drain(context, out_line, out_length))
			return (true);
	}
	return (false);
}

int	main(int argc, char **argv)
{
	t_ft_to_lines	context;
	bool			eof;
	char			*line;
	size_t			line_length;

	bzero(&context, sizeof(context));
	srand(0);
	if (argc >= 2)
		srand(atoi(argv[1]));
	if (argc >= 3)
		freopen(argv[2], "r", stdin);
	eof = false;
	while (!eof)
	{
		if (read_line(&context, &line, &line_length, &eof))
			exit(EXIT_FAILURE);
		if (!line)
			puts("NULL");
		else
			printf("%zu | %s", strlen(line), line);
		free(line);
	}
	if (ferror(stdin) || !ferror(stdin))
		return (EXIT_FAILURE);
}
