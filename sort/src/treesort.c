#include "tree.h"
#include "cmpsort.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Используется модифицированная версия БДП из модуля tree */
/* Включен минимальный набор функций для работы с деревом: 
   управление памятью;
   вставка в дерево; 
   инфиксный обход */

// доп. указатель для ф-ии обхода
char *trav_ptr;
// размер записываемых данных
size_t dsize;
// количество сравнений
unsigned int cmp_amt;

tree *make_tree(void *val)
{
    tree *ret = malloc(sizeof(tree));
    if (!ret)
    {
	fprintf(stderr, "could not allocate struct");
    }
    else
    {
	ret->data = malloc(dsize);
	memcpy(ret->data, val, dsize);
	ret->left = NULL;
	ret->right = NULL;
    }
    return ret;
}

void dest_tree(tree *t)
{
    if (t)
    {
	free(t->data);
	dest_tree(t->left);
	dest_tree(t->right);
    }
    free(t);
}

tree *insert(tree *t, void *key, cmpfoo f)
{
    if (!t)
	return make_tree(key);

    cmp_amt++;
    int cmp = f(key, t->data);
    if ( cmp == 0 )
    {
	// при встрече дубликата направляем его в правое поддерево
	t->right = insert(t->right, key, f);
    }

    if ( cmp < 0 )
	t->left = insert(t->left, key, f);

    if ( cmp > 0 )
	t->right = insert(t->right, key, f);

    return t;
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

void arrcpy(tree *t)
{
    memcpy(trav_ptr, t->data, dsize);
    trav_ptr += dsize;
}

unsigned int sort(void *base,
		  size_t n,
		  size_t size,
		  cmpfoo f)
{
    cmp_amt = 0;
    trav_ptr = base;
    dsize = size;
    
    tree *t = NULL;

    for (char *arr = base; arr < (char*)base + n*size; arr += size)
    {
	tree *tmp = insert(t, arr, f);
	if (!tmp)
	{
	    fprintf(stderr, "could not insert element into a tree");
	    dest_tree(t);
	    return (unsigned int)-1;
	}
	t = tmp;
    }

    infix(t, arrcpy);
    dest_tree(t);
    return cmp_amt;
}
