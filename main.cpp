#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "parg.h"

#include "convert.hpp"

#define VERSION_NUMBER "0.1.0"

int main(int argc, char *argv[])
{
	std::string help_text = "Usage: csv2xml [-a] [-i input.csv] [-o output.xml] [-r RootName] [-e ElementName] [-s ,]";
	if (argc <= 1)
	{
		printf("%s\n", help_text.c_str());
		return EXIT_SUCCESS;
	}

	struct parg_state ps;
	int c;

	parg_init(&ps);

	std::string input_file_path = "stdin";
	std::string output_file_path = "stdout";
	std::string root_name = "root";
	std::string element_name = "element";
	char sep = ',';
	bool use_attributes = false;

	while ((c = parg_getopt(&ps, argc, argv, "hvai:o:r:e:s:")) != -1) 
	{
		switch (c) 
		{
			case 'h':
				printf("%s\n", help_text.c_str());
				return EXIT_SUCCESS;
				break;
			case 'i':
				// Print information to stderr in case they want to use stdout as the output file
				fprintf(stderr, "Opening input file '%s'\n", ps.optarg);
				input_file_path = std::string(ps.optarg);
				break;
			case 'o':
				fprintf(stderr, "Opening output file '%s'\n", ps.optarg);
				output_file_path = std::string(ps.optarg);
				break;
			case 'r':
				fprintf(stderr, "Naming root element '%s'\n", ps.optarg);
				root_name = std::string(ps.optarg);
				break;
			case 'e':
				fprintf(stderr, "Naming elements '%s'\n", ps.optarg);
				element_name = std::string(ps.optarg);
				break;
			case 's':
				fprintf(stderr, "Using separator symbol '%c'\n", ps.optarg[0]);
				sep = ps.optarg[0];
				break;
			case 'a':
				fprintf(stderr, "Using attributes instead of sub-elements.\n");
				use_attributes = true;
				break;
			case 'v':
				printf("csv2xml %s\n", VERSION_NUMBER);
				return EXIT_SUCCESS;
				break;
			default:
				fprintf(stderr, "ERROR: Unhandled option -%c\n", c);
				return EXIT_FAILURE;
				break;
		}
	}

	for (c = ps.optind; c < argc; ++c) 
	{
		fprintf(stderr, "WARNING: Unhandled option '%s'\n", argv[c]);
	}

	return convert(input_file_path.c_str(), output_file_path.c_str(), root_name.c_str(), element_name.c_str(), sep, use_attributes);
}