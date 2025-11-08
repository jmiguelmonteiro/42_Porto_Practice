/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josemigu <josemigu@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 14:21:24 by josemigu          #+#    #+#             */
/*   Updated: 2025/07/22 14:21:24 by josemigu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int ft_getc(int fd)
{
    static t_buf buf;

    if (buf.n == 0)
    {
        buf.n = read(fd, buf.buf, BUFFER_SIZE);
        if (buf.n < 0)
        {
            buf.n = 0;
            return (READ_ERROR);
        }
        buf.bufp = buf.buf;
    }
    if (--buf.n >= 0)
        return((unsigned char)*buf.bufp++);
    buf.n = 0;
    return (EOF);
}

int ft_putc(t_string *str, char c)
{
    int  i;
    char    *tmp;

    if (str->len + 1 > str->cap)
    {
        tmp = str->str;
        str->str = malloc((str->cap + BUFFER_SIZE) * sizeof(char));
        if (!str->str)
            return (free(tmp), 1);
        i = 0;
        while (i < str->len)
        {
            str->str[i] = tmp[i];
            i++;
        }
        free(tmp);
        str->cap += BUFFER_SIZE;
    }
    str->str[str->len] = c;
    str->len++;
    return (0);
}

char *get_next_line(int fd)
{
    t_string    str;
    char        c;

    str.str = NULL;
    str.len = 0;
    str.cap = 0;
    while (1)
    {
        c = ft_getc(fd);
        if (c == READ_ERROR)
        {
            free(str.str);
            return (NULL);
        }
        if (c == EOF)
            break;
        if(ft_putc(&str, c))
            return (NULL);
        if (c == '\n')
            break;
    }
    if (str.len > 0)
        if(ft_putc(&str, '\0'))
            return (NULL);
    return (str.str);
}

/* int main(void)
{
    int fd = 0;
    char *line;

    line = get_next_line(0);
    while (line)
    {
        printf("%s", line);
        free(line);
        line = get_next_line(0);
    }
}
 */