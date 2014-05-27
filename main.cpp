#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <set>
#include <unistd.h>
#include <fstream>
#include <errno.h>
#include <string.h>

#include "xlsHandler.h"
#include "ParseGuiString.h"

using namespace std;

static void print_usage()
{
	cout << "Usage: genqm [arg1] [filepath]" << endl;
	cout << "args:\n";
	cout << "  -c,\tSpecify your Qt project path It will do the following:\n";
	cout <<	"     \t-Parse strings in QObject::tr() and generate xml file.\n";
	cout << "     \t-Get all source GUI strings, and make a xml file for you to make a xls file.\n";
	cout << "  -x,\tGenerate qm files from given xls files.\n";
}

int main(int argc, char **argv)
{
	char current_dir[256];
	getcwd(current_dir, 256);

	if(argc > 3 || argc<3)
	{
		print_usage();
		return -1;
	}
	ParseGuiString fileHandler;
	switch(*(argv[1]+1))
	{
		case 'c':
			fileHandler.parseStringsInTr(argv[2]);
			break;
		case 'x':
			if(fileHandler.openXlsFile(argv[2])==-1)
			{
				fprintf(stderr, "Cannot open given xls file.\n");
				return -1;
			}
			fileHandler.generateTSFile();
			if(system("lrelease ts_output/*")!=0)
				fprintf(stderr, "Generate qm file ... %s.\n", strerror(errno));
			else
				printf("Generate qm file ... done.\n");

			break;
		default:
			print_usage();
	}
	return 0;
}
