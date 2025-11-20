#include <stdio.h>
#include <stdlib.h> // change this to <stdlib.h>
#include <ctype.h>
#include "vbc.h"



node *factor(char **s);
node *multi(char **s);
node *add(char **s);

node    *new_node(node n)
{
    node *ret = calloc(1, sizeof(n));
    if (!ret)
        return (NULL);
    *ret = n;
    return (ret);
}

void    destroy_tree(node *n)
{
    if (!n)
        return ;
    if (n->type != VAL)
    {
        destroy_tree(n->l);
        destroy_tree(n->r);
    }
    free(n);
}

void    unexpected(char c)
{
    if (c)
        printf("Unexpected token '%c'\n", c);
    else
        printf("Unexpected end of input\n");
}

int accept(char **s, char c)
{
    if (**s == c)
    {
        (*s)++;
        return (1);
    }
    return (0);
}

int expect(char **s, char c)
{
    if (accept(s, c))
        return (1);
    unexpected(**s);
    return (0);
}

node *factor(char **s)
{
	node *ret;

	if (accept(s, '('))
	{
		ret = add(s);
		if (!ret || !expect(s, ')'))
		{
			destroy_tree(ret);
			return NULL;
		}
		return ret;
	}
	else if (isdigit(**s))
	{
		ret = new_node((node){.type = VAL, .val = **s - '0'});
		(*s)++;
		return ret;
	}
	unexpected(**s);
	return NULL;
}

node *multi(char **s)
{
	node *left = factor(s);
	node *right;

	if (!left)
		return NULL;
	while (**s == '*')
	{
		(*s)++;
		right = factor(s);
		if (!right)
		{
			destroy_tree(left);
			return NULL;
		}
		left = new_node((node){.type = MULTI, .l = left, .r = right});
		if (!left)
		{
			destroy_tree(right);
			return NULL;
		}
	}
	return left;
}

node *add(char **s)
{
	node *left = multi(s);
	node *right;

	if (!left)
		return NULL;

	while (**s == '+')
	{
		(*s)++;
		right = multi(s);
		if (!right)
		{
			destroy_tree(left);
			return NULL;
		}
		left = new_node((node){.type = ADD, .l = left, .r = right});
		if (!left)
		{
			destroy_tree(right);
			return NULL;
		}
	}
	return left;
}

node    *parse_expr(char *s)
{
    node *ret = add(&s);

	if (!ret)
		return NULL;
	
    if (*s) 
    {
		unexpected(*s);
        destroy_tree(ret);
        return (NULL);
    }
    return (ret);
}

int eval_tree(node *tree)
{
    switch (tree->type)
    {
        case ADD:
            return (eval_tree(tree->l) + eval_tree(tree->r));
        case MULTI:
            return (eval_tree(tree->l) * eval_tree(tree->r));
        case VAL:
            return (tree->val);
    }
	return 0;
}

int main(int argc, char **argv)
{
    if (argc != 2)
        return (1);
    node *tree = parse_expr(argv[1]);
    if (!tree)
        return (1);
    printf("%d\n", eval_tree(tree));
    destroy_tree(tree);
	return 0;
}
