#include "graph.h"
#include "iterator.h"
#include "iograph.h"

#include <stdlib.h>
#include <stdio.h>

int main(void)
{
    FILE *fp = fopen("examples/graph1.graph", "r");
    graph *g = freadg(fp);

    for (size_t i = 0; i < vertices(g); i++)
    {
	printf("Adjacent to v%lu:\n", i);
	for (iterator *it = make_iter(g, i); it; it = it_next(it))
	{
	    printf("%lu ", it_end(it));
	}
	putchar('\n');
    }

    graph *g2 = copy_graph(g);
    for (size_t i = 0; i < vertices(g2); i++)
    {
	printf("Adjacent to v%lu:\n", i);
	for (iterator *it = make_iter(g2, i); it; it = it_next(it))
	{
	    printf("%lu ", it_end(it));
	}
	putchar('\n');
    }

    dest_graph(g2);
    dest_graph(g);
    fclose(fp);
    return 0;
}
