#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "aho_corasick.h"

#define MAX_PATTERN_SIZE 128

STRING read_file_to_search (const char *filename);
STRING* read_patterns (const char *filename, unsigned int *no_of_patterns);
void print_usage (const char *exec_file);
int match_handler(MATCH * m, int automata_num, int thread_num);

int main(int argc, char **argv)
{
	clock_t start = clock(), end, difference;
	AC_AUTOMATA aca;
	STRING *patterns, input_buffer;
	unsigned int i, j, no_of_patterns;
	int clopt;

	/* Command line config*/
	const char *pattern_file;
	const char *input_file;
	short verbosity = 0;
	short timeit = 0;

	if (argc < 4) {
		print_usage(argv[0]);
		exit(1);
	}

	while ((clopt = getopt(argc, argv, "P:vth?")) != -1) {
		switch (clopt) {
			case 'P':
				pattern_file = optarg;
				break;
			case 'v':
				verbosity = 1;
				break;
			case 't':
				timeit = 1;
				break;
			case 'h':
			case '?':
			default:
				print_usage(argv[0]);
				exit(1);
		}
	}
	input_file = *(argv + optind);

	if (verbosity)
		printf("Loading patterns from file - %s\n", pattern_file);
	
	patterns =  read_patterns (pattern_file, &no_of_patterns);

	if (verbosity)
		printf("Initialising automata\n");

	ac_automata_init (&aca, match_handler);

	if (verbosity)
		printf("Adding strings\n");

	for (i = 0; i < no_of_patterns; i++)
		ac_automata_add_string(&aca, &patterns[i]);

	input_buffer = read_file_to_search(input_file);

	if (verbosity)
		printf("Locating failure nodes\n");

	ac_automata_locate_failure (&aca);

	if (verbosity)
		printf("Searching\n");

	ac_automata_search(&aca, &input_buffer, 0, 0);
	
	if (verbosity)
		printf("Freeing resources\n");

	ac_automata_release (&aca);

	if (timeit) {
		int msec;
		end = clock();
		difference = end - start;
		msec = difference * 1000 / CLOCKS_PER_SEC;
		printf("\nTotal time taken - %d milliseconds\n", msec);
	}

	return 0;
}


STRING read_file_to_search (const char *filename)
{
	STRING buffer;
	FILE *fp;

	fp = fopen(filename, "r");

    fseek(fp, 0, SEEK_END);
    buffer.length = ftell(fp) + 1;
    rewind(fp);

    buffer.str = (ALPHA *) malloc((buffer.length) * sizeof(ALPHA));
    fread(buffer.str, buffer.length - 1, 1, fp);
    buffer.str[buffer.length - 1] = '\0';

    fclose(fp);

    return buffer;
}


STRING* read_patterns (const char *filename, unsigned int *no_of_patterns)
{
	unsigned int i, j, chunk;
	ALPHA *buffer = (ALPHA *) malloc(MAX_PATTERN_SIZE * sizeof(char));
	STRING *patterns;
	FILE *fp;

	fp = fopen(filename, "r");

	fscanf(fp, "%u\n", no_of_patterns);
	patterns = (STRING *) malloc(*no_of_patterns * sizeof(STRING));

	for(i = 0; i < *no_of_patterns; i++) {
		
		if (fscanf(fp, "%s\n", buffer) == -1)
			break;
			
		patterns[i].str = (ALPHA *) malloc((strlen(buffer)+1) * sizeof(ALPHA));
			
		strcpy(patterns[i].str, buffer);
		patterns[i].length = strlen(buffer);
		patterns[i].id = i + 1;
	}

	fclose(fp);

	return patterns;
}


void print_usage (const char *exec_file)
{
    printf("Usage: %s [-v] -P pattern_file file1\n", exec_file);
}


int match_handler(MATCH * m, int automata_num, int thread_num)
{
	unsigned int j;

	printf ("@ Thread %ld Automata %ld position %ld string(s) ", thread_num, automata_num, m->position);

	for (j=0; j < m->match_num; j++)
		printf("%ld (%s), ", m->matched_strings[j].id, m->matched_strings[j].str);
	/*
	CAUTION: be carefull about using m->matched_strings[j].str
	if 'str' has permanent string allocation inside your program 
	memory area, you can use this form. otherwise it will point to
	an incorrect memory place. in this case you must reconstruct
	the recognized pattern from the input string.
	*/
	printf("matched\n");

	/* to find all matches alwas return 0 */
	return 0;
	/* 
		return 0 : contiue searching
		return none zero : stop searching
		
		as soon as you satisfied with search results, you can stop search and 
		exit from ac_automata_search() and return to the rest of your program.
		
		as an example if you only need first N matches,
		define a counter and return none zero after the counter exceeds N.
	*/
}