#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <set>
#include <unistd.h>

#include "xlsHandler.h"
#include "getFileStringUnicode.h"

using namespace std;

int main(int argc, char **argv)
{
	char current_dir[256];
	getcwd(current_dir, 256);

	if(argc > 2)
	{
		cout << "Usage: ..." << endl;
		return -1;
	}
	else if(argc == 2)
	{
		snprintf(current_dir, 256, "%s", argv[1]);
	}

	GetFileStringUnicode fileHandler("gui_strings_20140522.xls");
	//if(!(fileHandler.parseUsedCharacter()))
		//return -1;
	//fileHandler.modifyFontRange(0x3040, 0x30ff, 1);//add full japanese
	//if(!(fileHandler.generateFont()))
		//return -1;
	//fileHandler.generateTSFile();
	return 0;
}
