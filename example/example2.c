/*
	Copyright 2010-2011 Kamiar Kanani <kamiar.kanani@gmail.com>

    This file is part of multifast.

    multifast is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    multifast is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with multifast.  If not, see <http://www.gnu.org/licenses/>.

*/

/* This example illustrates how to use Aho-Corasick Library in your code */

#include <stdio.h>
#include <string.h>
#include <omp.h>
// 0. Include the header
#include "aho_corasick.h"

/* Sample pattern Database */
ALPHA strdb[30][10] = { "rec", "cent", "ece", "ce", "recent","nt", "ars", "enal", "sen", "rs", "rsena", "al" };

#define DBSIZE sizeof(strdb)/sizeof(ALPHA *)

/* Input string */
ALPHA * input_str = {"recent arsenal"};

/****************************************************************************/

// 1. Define a callback function of type MATCH_CALBACK:
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

/****************************************************************************/

int main (int argc, char ** argv)
{
	AC_AUTOMATA *aca;
	aca=(AC_AUTOMATA *)malloc(sizeof(AC_AUTOMATA)*2);
	unsigned int i,c;
	STRING tmp_str, tmp_str2;
	printf("\nInitialising...");
	#pragma omp parallel for private(c) shared(aca)
		for(c=0;c<2;c++)
			ac_automata_init (&aca[c], match_handler);
	//printf("1");
	
	printf("\nAdding Strings...");
	#pragma omp parallel for collapse(2) 
	for (c=0;c<2;c++)
	{
		for (i=0; i<DBSIZE; i++)
		{
			tmp_str.str = strdb[i+c*6];
			tmp_str.id = i+1; // optional field
			tmp_str.length = strlen(tmp_str.str);

			ac_automata_add_string (&aca[c], &tmp_str);
		}
	}
	
	#pragma omp parallel for private(c) shared(aca)
		for (c=0;c<2;c++)
			ac_automata_locate_failure (&aca[c]);
	//printf("\n2");
	
	printf("\nSearching...\n");
	#pragma omp parallel for private(c) shared(aca, strdb, tmp_str)
		for(c=0;c<2;c++)
		{
			int id=omp_get_thread_num();
			printf("\nIn thread : %d\n", id);
			printf("Automata %d:\n",c+1);
			tmp_str.str = input_str;
			tmp_str.length = strlen(tmp_str.str);		
			ac_automata_search (&aca[c], &tmp_str, c, id);
		}
	//printf("\n3");


	for(c=0;c<2;c++)
		ac_automata_release (&aca[c]);
	//printf("\n4");
	return 0;
}

