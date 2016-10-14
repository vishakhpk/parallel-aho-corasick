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

// 0. Include the header
#include "aho_corasick.h"

/* Sample pattern Database */
ALPHA * strdb[] = {
	"rec",
	"cent",
	"ece",
	"ce",
	"recent",
	"nt",
};


#define DBSIZE sizeof(strdb)/sizeof(ALPHA *)

/* Input string */
ALPHA * input_str = {"recent"};

/****************************************************************************/

// 1. Define a callback function of type MATCH_CALBACK:
int match_handler(MATCH * m, void * param)
{
	unsigned int j;

	printf ("@ position %ld string(s) ", m->position);

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
	// 2. Define an Aho-Corasick automata
	AC_AUTOMATA aca;

	unsigned int i;
	STRING tmp_str;

	printf("Example Search Program\n");

	// 3. Intitialize the automata
	ac_automata_init (&aca, match_handler);

	// 4. add patterns to automata
	for (i=0; i<DBSIZE; i++)
	{
		tmp_str.str = strdb[i];
		tmp_str.id = i+1; // optional field
		tmp_str.length = strlen(tmp_str.str);

		ac_automata_add_string (&aca, &tmp_str);
	}

	// 5. Build index: after you add all patterns you must call ac_automata_locate_failure()
	ac_automata_locate_failure (&aca);

	// from now you can not add patterns anymore.

	#ifdef DEBUG_DISPLAY_AC
	// if you are intrested to see the automata uncomment below
	// ac_automata_dbg_show (&aca);
	#endif

	tmp_str.str = input_str;
	tmp_str.length = strlen(tmp_str.str);

	// 6. Now you can do search
	ac_automata_search (&aca, &tmp_str, (void *)strdb);
	/*
		here we pass 'strdb' as a parameter to our callback function.
		if there are more variables to pass to callback function,
		you must define an struct that combines those variables and
		send the pointer of the struct as a parameter.
	*/

	// 7. Reset
	/* if you want to do another search with same automata 
	   you must reset automata.
	*/
	// ac_automata_reset(&aca);


	// 8. Release
	/* Release Automata: 
	   if you have finished with the automata and will not
	   use it in the rest of your program please release it.
	*/
	ac_automata_release (&aca);

	return 0;
}

