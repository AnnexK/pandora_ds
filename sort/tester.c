#include <stdio.h>

int main(int argc, char **argv)
{
    if (argc != 2)
    {
	fprintf(stderr, "usage: %s filename\n", argv[0]);
	return -1;
    }
    FILE *fp = fopen(argv[1], "r");
    if (!fp)
    {
	fprintf(stderr, "%s: could not open file %s\n", argv[0], argv[1]);
	return -2;
    }

    double cur;
    fscanf(fp, "%lf", &cur);
    double buf;
    int fail = 0;

    while (fscanf(fp, "%lf", &buf) != EOF && !fail)
    {
	if (buf < cur)
	{
	    fail = 1;
	}
	cur = buf;
    }

    fclose(fp);
    return fail;
}
