/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csodini <csodini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/14 23:43:29 by csodini           #+#    #+#             */
/*   Updated: 2026/05/15 02:08:58 by csodini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*free_all(char *buffer, char *save)
{
	free(buffer);
	free(save);
	return (NULL);
}

static char	*read_to_save(int fd, char *save)
{
	char	*buffer;
	ssize_t	bytes_read;

	buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
	{
		free(save);
		return (NULL);
	}
	bytes_read = 1;
	while (!ft_strchr(save, '\n') && bytes_read > 0)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read == -1)
			return (free_all(buffer, save));
		buffer[bytes_read] = '\0';
		if (bytes_read > 0)
		{
			save = ft_strjoin_free(save, buffer);
			if (!save)
				return (free(buffer), NULL);
		}
	}
	free(buffer);
	return (save);
}

static char	*extract_line(char *save)
{
	size_t	i;

	i = 0;
	if (!save || !save[0])
		return (NULL);
	while (save[i] && save[i] != '\n')
		i++;
	if (save[i] == '\n')
		i++;
	return (ft_substr(save, 0, i));
}

static char	*update_save(char *save)
{
	char	*new_save;
	size_t	i;

	i = 0;
	while (save[i] && save[i] != '\n')
		i++;
	if (!save[i])
	{
		free(save);
		return (NULL);
	}
	i++;
	if (!save[i])
	{
		free(save);
		return (NULL);
	}
	new_save = ft_substr(save, i, ft_strlen(save) - i);
	free(save);
	return (new_save);
}

char	*get_next_line(int fd)
{
	static char	*save;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, 0, 0) < 0)
	{
		free(save);
		save = NULL;
		return (NULL);
	}
	save = read_to_save(fd, save);
	if (!save)
		return (NULL);
	line = extract_line(save);
	if (!line)
	{
		free(save);
		save = NULL;
		return (NULL);
	}
	save = update_save(save);
	return (line);
}
