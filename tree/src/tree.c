#include "tree.h"
#include <stdlib.h>
#include <stdio.h>

int compare(unsigned int, unsigned int);
tree *tree_min(tree *);

tree *make_tree(unsigned int val)
{
    tree *ret = malloc(sizeof(tree));
    if (!ret)
    {
	fprintf(stderr, "could not allocate struct");
    }
    else
    {
	ret->data = val;
	ret->left = NULL;
	ret->right = NULL;
    }
    return ret;
}

void dest_tree(tree *t)
{
    if (t)
    {	
	dest_tree(t->left);
	dest_tree(t->right);
    }
    free(t);
}

tree *insert(tree *t, unsigned int key)
{
    if (!t)
	return make_tree(key);

    int cmp = compare(key, t->data);
    if ( cmp == 0 )
    {
	fprintf(stderr, "duplicate encountered");
    }

    if ( cmp < 0 )
	t->left = insert(t->left, key);

    if ( cmp > 0 )
	t->right = insert(t->right, key);

    return t;
}

tree *search(tree *t, unsigned int key)
{
    int cmp;
    if (!t || (cmp = compare(key, t->data)) == 0)
	return t;

    if ( cmp < 0 )
	return search(t->left, key);

    if ( cmp > 0 )
	return search(t->right, key);

    // никогда не выполнится
    return NULL;
}

tree *delete(tree *t, unsigned int key)
{
    if (!t) return NULL;

    int cmp = compare(key, t->data);

    if ( cmp < 0 )
	t->left = delete(t->left, key);
    if ( cmp > 0 )
	t->right = delete(t->right, key);
    if ( cmp == 0 )
    {
	if (!t->left)
	{
	    tree *temp = t->right;
	    free(t);
	    return temp;
	}
	else if (!t->right)
	{
	    tree *temp = t->left;
	    free(t);
	    return temp;
	}
	else
	{
	    tree *temp = tree_min(t->right);
	    t->data = temp->data;
	    t->right = delete(t->right, temp->data);
	}
    }
    return t;
}

int compare(unsigned int a, unsigned int b)
{
    return (int)(a-b);
}

tree *tree_min(tree *t)
{
    tree *ret;
    for (ret = t; ret; ret = ret->left)
	;
    return ret;
}

void prefix(tree *t, void (*f)(tree *))
{
    if (t)
    {
	f(t);
	prefix(t->left, f);
	prefix(t->right, f);
    }
}

void postfix(tree *t, void (*f)(tree *))
{
    if (t)
    {
	postfix(t->left, f);
	postfix(t->right, f);
	f(t);
    }
}

void infix(tree *t, void (*f)(tree *))
{
    if (t)
    {	
	infix(t->left, f);
	f(t);
	infix(t->right, f);
    }
}
