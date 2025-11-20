#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h> // change this to <stdlib.h>
#include <unistd.h>

typedef struct	json {
	enum {
		MAP,
		INTEGER,
		STRING
	} type;
	union {
		struct {
			struct pair	*data;
			size_t		size;
		} map;
		int	integer;
		char	*string;
	};
}	json;

typedef struct	pair {
	char	*key;
	json	value;
}	pair;

void	free_json(json j);
int	argo(json *dst, FILE *stream);

int	peek(FILE *stream)
{
	int	c = getc(stream);
	ungetc(c, stream);
	return c;
}

void	unexpected(FILE *stream)
{
	if (peek(stream) != EOF)
		printf("unexpected token '%c'\n", peek(stream));
	else
		printf("unexpected end of input\n");
}

int	accept(FILE *stream, char c)
{
	if (peek(stream) == c)
	{
		(void)getc(stream);
		return 1;
	}
	return 0;
}

int	expect(FILE *stream, char c)
{
	if (accept(stream, c))
		return 1;
	unexpected(stream);
	return 0;
}

void	free_json(json j)
{
	switch (j.type)
	{
		case MAP:
			for (size_t i = 0; i < j.map.size; i++)
			{
				free(j.map.data[i].key);
				free_json(j.map.data[i].value);
			}
			free(j.map.data);
			break ;
		case STRING:
			free(j.string);
			break ;
		default:
			break ;
	}
}

void	serialize(json j)
{
	switch (j.type)
	{
		case INTEGER:
			printf("%d", j.integer);
			break ;
		case STRING:
			putchar('"');
			for (int i = 0; j.string[i]; i++)
			{
				if (j.string[i] == '\\' || j.string[i] == '"')
					putchar('\\');
				putchar(j.string[i]);
			}
			putchar('"');
			break ;
		case MAP:
			putchar('{');
			for (size_t i = 0; i < j.map.size; i++)
			{
				if (i != 0)
					putchar(',');
				serialize((json){.type = STRING, .string = j.map.data[i].key});
				putchar(':');
				serialize(j.map.data[i].value);
			}
			putchar('}');
			break ;
	}
}

int	main(int argc, char **argv)
{
	if (argc != 2)
		return 1;
	char *filename = argv[1];
	FILE *stream = fopen(filename, "r");
	json	file;
	if (argo (&file, stream) != 1)
	{
		free_json(file);
		return 1;
	}
	serialize(file);
	printf("\n");
}

int	parse_integer(json *dst, FILE *stream)
{
	int n = 0;

	if (fscanf(stream, "%d", &n) == 1)
	{
		dst->type = INTEGER;
		dst->integer = n;
		return 1;
	}
	return -1;
}

char *get_str(FILE *stream)
{
	int		c;
	size_t	size = 16;
	size_t	i = 0;
	char	*str = malloc(size * sizeof(char));
	char	*tmp;
	if (!str)
		return NULL;
	getc(stream);	// consume fist "

	while (true)
	{
		c = getc(stream);
		if (c == '"')
			break;
		if (c == EOF)
		{
			free(str);
			unexpected(stream);
			return NULL;
		}
		if (i >= (size - 1))
		{
			size *= 2;
			tmp = str;
			str = realloc(tmp, size * sizeof(char));
			if (!str)
			{
				free(tmp);
				return NULL;
			}
		}
		if (c == '\\')
		{
			c = peek(stream);
			if ((c == '\\') || (c == '\"'))
				str[i++] = getc(stream);
			else
			{
				free(str);
				unexpected(stream);
				return NULL;
			}
		} else
			str[i++] = c;
	}
	return str;
}

int	parse_string(json *dst, FILE *stream)
{
	char *str = NULL;

	if ((str = get_str(stream)))
	{
		dst->type = STRING;
		dst->string = str;
		return 1;
	}
	return -1;
}

int	parse_map(json *dst, FILE *stream)
{
	dst->type = MAP;
	dst->map.size = 0;
	dst->map.data = NULL;
	char c = getc(stream);	// consume first {

	if (peek(stream) == '}')
	{
		getc(stream);
		return 1;
	}

	while (true)
	{
		c = peek(stream);
		if (c != '"')
		{
			unexpected(stream);
			return -1;
		}
		dst->map.data = realloc(dst->map.data, (dst->map.size + 1) * sizeof(pair));
		pair *node = &dst->map.data[dst->map.size];
		dst->map.size++;
		node->key = get_str(stream);
		if (!node->key)
			return -1;
		if (expect(stream, ':') == 0)
			return -1;
		if (argo(&node->value, stream) == -1)
			return -1;
		c = peek(stream);
		if (c == '}')
		{
			accept(stream, c);
			break;
		}
		if (c == ',')
			accept(stream, c);
		else
		{
			unexpected(stream);
			return -1;
		}
	}
	return 1;
}

int parse_value(json *dst, FILE *stream)
{
	char c = peek(stream);

	if (isdigit(c) || c == '-' || c == '+')
		return(parse_integer(dst, stream));
	else if (c == '"')
		return(parse_string(dst, stream));
	else if (c == '{')
		return(parse_map(dst, stream));
	else
	{
		unexpected(stream);
		return -1;
	}
}

int	argo(json *dst, FILE *stream)
{
	if (parse_value(dst, stream) == -1)
		return -1;
	return 1;
}
