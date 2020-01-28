#include "graph.h"
#include "iograph.h"
#include "iterator.h"
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

    free(it);
    dest_graph(g);
    fclose(fp);
    return 0;
}
