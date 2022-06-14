/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Juyeong Maing <jmaing@student.42seoul.kr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/15 10:58:57 by Juyeong Maing     #+#    #+#             */
/*   Updated: 2022/06/15 00:51:40 by Juyeong Maing    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ft_to_lines.h>

static t_err	read_line(
	t_ft_to_lines *context,
	char **out_line,
	bool *out_eof
)
{
	char	buffer[43];
	size_t	bytes_read;
	char	*line;

	*out_eof = false;
	if (ft_to_lines(context, NULL, 0, &line))
		return (true);
	while (!line)
	{
		bytes_read = fread(buffer, 1, rand() % 42 + 1, stdin);
		fprintf(stderr, "read %zu(%.*s)\n", bytes_read, (int)bytes_read, buffer);
		if (ferror(stdin))
			return (true);
		if (!bytes_read)
		{
			*out_line = ft_to_lines_end(context);
			*out_eof = true;
			return (false);
		}
		if (ft_to_lines(context, buffer, bytes_read, &line))
			return (true);
	}
	if (!line)
		return (true);
	*out_line = line;
	return (false);
}

int	main(int argc, char **argv)
{
	t_ft_to_lines	context;
	bool			eof;
	char			*line;

	bzero(&context, sizeof(context));
	srand(0);
	if (argc <= 2)
		srand(atoi(argv[1]));
	if (argc == 3)
		freopen(argv[2], "r", stdin);
	eof = false;
	while (!eof)
	{
		if (read_line(&context, &line, &eof))
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
