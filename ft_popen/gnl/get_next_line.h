/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josemigu <josemigu@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 14:24:49 by josemigu          #+#    #+#             */
/*   Updated: 2025/07/22 14:24:49 by josemigu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 10
#endif
#define READ_ERROR -99

typedef struct s_buf
{
    char buf[BUFFER_SIZE];
    char *bufp;
    int n;
} t_buf;

typedef struct s_string
{
    char *str;
    int len;
    int cap;
} t_string;

char *get_next_line(int fd);
