#include "graph.h"
#include "iograph.h"

#include <stdlib.h>
#include <stdio.h>

int main(void)
{
    FILE *fp = fopen("examples/dag1.graph", "r");
    graph *g = freadg(fp);
    fclose(fp);

    printf("Pre-transpose:\n");
    writeg(g);
    transpose(g);
    printf("Post-transpose:\n");
    writeg(g);
    transpose(g);
    printf("Re-transpose:\n");
    writeg(g);

    dest_graph(g);
    return 0;
}
