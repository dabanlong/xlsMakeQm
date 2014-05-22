#include <QFile>
#include <QTextStream>
#include <QList>
#include <QDir>
#include <QDebug>
#include <QProcess>
#include <QTextCodec>

#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <set>

#include "xlsHandler.h"
#include "getFileStringUnicode.h"

using namespace std;
int main(int argc, char **argv)
{
	QDir trDir(QDir::current());
	if(argc > 2)
	{
		cout << "Usage: ..." << endl;
		return -1;
	}
	else if(argc == 2)
	{
		trDir.setPath(QString(argv[1]));
	}

	GetFileStringUnicode fileHandler(QString("*.xls"), trDir);
	if(!(fileHandler.parseUsedCharacter()))
		return -1;
	fileHandler.modifyFontRange(0x3040, 0x30ff, 1);//add full japanese
	if(!(fileHandler.generateFont()))
		return -1;
	//fileHandler.generateTSFile();
	return 0;
}
