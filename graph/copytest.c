#include "graph.h"
#include "iterator.h"
#include "iograph.h"

#include <stdlib.h>
#include <stdio.h>

int main(void)
{
    FILE *fp = fopen("examples/graph1.graph", "r");
    graph *g = freadg(fp);
    iterator *it = make_iter();
    
    for (size_t i = 0; i < vertices(g); i++)
    {	
	printf("Adjacent to v%lu:\n", i);
	for (it_init(it, g, i); it_valid(it); it_next(it))
	{
	    printf("%lu ", it_end(it));
	}
	putchar('\n');
    }

    graph *g2 = copy_graph(g);
    for (size_t i = 0; i < vertices(g2); i++)
    {
	printf("Adjacent to v%lu:\n", i);
	for (it_init(it, g2, i); it_valid(it); it_next(it))
	{
	    printf("%lu ", it_end(it));
	}
	putchar('\n');
    }

    free(it);
    dest_graph(g2);
    dest_graph(g);
    fclose(fp);
    return 0;
}
