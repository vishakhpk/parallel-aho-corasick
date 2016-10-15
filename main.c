#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NO_OF_THREADS 2
#define MAX_PATTERN_SIZE 128

int main(int argc, char const *argv[])
{
	AC_AUTOMATA *aca;
	char **patterns;
	int i, j;

	if (argc < 4) {
		print_usage(argv[0]);
		exit(1);
	}

	while ((clopt == getopt(argc, argv, "P:v")) != -1) {
		switch (clopt) {
			case 'P':
				config.pattern_file = optarg;
				break;
			case 'v':
				config.verbosity = 1;
				break;
			case 'h':
			case '?':
			default:
				print_usage(argv[0]);
				exit(1);
		}
	}

	if (config.verbosity)
		printf("Loading patterns from file - %s\n", config.pattern_file);	
	read_patterns (config.pattern_file, patterns);

	return 0;
}

void read_patterns (const char *filename, char **patterns)
{
	unsigned int no_of_patterns, i;
	char *buffer = (char *) malloc(MAX_PATTERN_SIZE * sizeof(char));
	FILE *fp;
	
	fp = fopen(filename, "r");
	fscanf(fp, "%u\n", &no_of_patterns);
	*patterns = (char *) malloc(no_of_patterns * sizeof(char *));

	for (i = 0; i < no_of_patterns; i++) {
		fscanf(fp, "{%s}\n", &buffer);
		patterns[i] = (char *) malloc((strlen(buffer)+1) * sizeof(char));
		strcpy(patterns[i], buffer);
	}
}
