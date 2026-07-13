#include <stdio.h>
#include <stdlib.h>

static int	min3(int a, int b, int c)
{
	if (b < a)
		a = b;
	return ((c < a) ? c : a);
}

static int	clean(char **grid, char *line)
{
	for (int i = 0; grid && grid[i]; i++)
		free(grid[i]);
	free(grid);
	free(line);
	return (-1);
}

static int	execute_bsq(FILE *f)
{
	char	e, o, x, **grid, *line = NULL;
	size_t	cap = 0;
	int		h, w = 0, j, best = 0, bi = 0, bj = 0;

	if (fscanf(f, "%d%c%c%c", &h, &e, &o, &x) != 4 || h <= 0
		|| e == o || e == x || o == x || e < 32 || e > 126
		|| o < 32 || o > 126 || x < 32 || x > 126
		|| getline(&line, &cap, f) == -1
		|| !(grid = calloc(h + 1, sizeof(char *))))
		return (clean(NULL, line));
	for (int i = 0; i < h; i++)
	{
		int n = getline(&line, &cap, f);
		if (n < 1 || line[n - 1] != '\n' || (!i && (w = n - 1) < 0) || n - 1 != w)
			return (clean(grid, line));
		for (j = 0; j < w && (line[j] == e || line[j] == o); j++)
			;
		if (j < w || !(grid[i] = malloc(w + 1)))
			return (clean(grid, line));
		for (j = 0; j <= w; j++)
			grid[i][j] = (j < w) ? line[j] : '\0';
	}
	free(line);
	int dp[h][w];
	for (int i = 0; i < h; i++)
		for (j = 0; j < w; j++)
		{
			if (grid[i][j] == o)
				dp[i][j] = 0;
			else if (!i || !j)
				dp[i][j] = 1;
			else
				dp[i][j] = min3(dp[i - 1][j], dp[i - 1][j - 1], dp[i][j - 1]) + 1;
			if (dp[i][j] > best)
			{
				best = dp[i][j];
				bi = i - best + 1;
				bj = j - best + 1;
			}
		}
	for (int i = bi; i < bi + best; i++)
		for (j = bj; j < bj + best; j++)
			grid[i][j] = x;
	for (int i = 0; i < h; i++)
	{
		fputs(grid[i], stdout);
		fputs("\n", stdout);
	}
	clean(grid, NULL);
	return (0);
}

int	main(int argc, char **argv)
{
	if (argc == 1 && execute_bsq(stdin) == -1)
		fprintf(stderr, "map error\n");
	for (int i = 1; i < argc; i++)
	{
		FILE *f = fopen(argv[i], "r");
		if (!f || execute_bsq(f) == -1)
			fprintf(stderr, "map error\n");
		if (f)
			fclose(f);
		if (i < argc - 1)
			fprintf(stdout, "\n");
	}
	return (0);
}
