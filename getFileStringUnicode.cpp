#define _XOPEN_SOURCE 500
#include "getFileStringUnicode.h"
#include <unistd.h>
#include <ftw.h>
#include <iterator>
#include <algorithm>

using namespace std;

GetFileStringUnicode::GetFileStringUnicode(string filePath)
	: path(filePath)
{
	openXlsFile();
}
/*
void GetFileStringUnicode::dumpCharacter(const QStringList &stringList, QList<ushort> &list, QString path)
{
	cout << path.toStdString() << endl;
	for(auto i : stringList)
	{
		std::cout << "processing file: " << i.toStdString() << " ..." << std::endl;
		QString filePath = path + QString("/") + i;
		QFile file(filePath);
		file.open(QIODevice::ReadOnly | QIODevice::Text);
		QTextStream stream(&file);
		while(!stream.atEnd())
		{
			QString tmpstring;
			stream >> tmpstring;
			for(auto j : tmpstring)
				list.push_back(j.unicode());
		}
	}
	list.push_back(QChar(' ').unicode());//space is trimmed
	qSort(list);
	list.erase(std::unique(list.begin(), list.end()), list.end());//erase duplicate element
}
*/
/*
void GetFileStringUnicode::generateScriptContext(QTextStream &script, QString &fileName)
{

	script << QString("Open(\"%1/%2\");").arg(path.path()).arg(fileName) << endl
	       << "Select(arr);" << endl
	       << "Clear();" << endl
	       << QString("PostNotice(\"preparing generating fonts of %1 ...\")").arg(fileName) << endl
	       << QString("Generate(\"%1/%2\");").arg(path.path()).arg(fileName) << endl;
}
*/
/*Generate script file for fontforge*/
/*
int GetFileStringUnicode::generateScript()
{
	QFile scriptFile("./script.pe");
	scriptFile.open(QIODevice::Truncate | QIODevice::WriteOnly | QIODevice::Text);
	QTextStream script(&scriptFile);
	const QStringList &fileStringList = getFilterFileList(QString("*.ttf"), path);
	if(fileStringList.empty())
	{
		scriptFile.close();
		return -1;
	}
	script << "arr=Array(65536);" << endl;
	auto cnt = result.begin();
	for(int i = 0; i < 65536; ++i)
	{
		if(i ==(*cnt))
		{
			script << QString("arr[%1]=0;").arg(i) << endl;
			++cnt;
		}
		else
			script << QString("arr[%1]=1;").arg(i) << endl;
	}
	for(auto i : fileStringList)
	{
		cout << i.toStdString() << endl;
		generateScriptContext(script, i);
	}
	scriptFile.close();
	return 1;
}
*/
/*Export used character list and their unicode*/
/*
void GetFileStringUnicode::exportCharAndNumber(const QString &textFileName, const QString &codeFileName)
{
	QFile fileo(codeFileName);
	fileo.open(QIODevice::WriteOnly | QIODevice::Text);
	QTextStream streamo(&fileo);
	QFile textResultFile(textFileName);
	textResultFile.open(QIODevice::WriteOnly | QIODevice::Text);
	QTextStream streamoText(&textResultFile);
	streamoText.setCodec("UTF-8");
	for(auto i : result)
	{
		streamo << i << endl;
		streamoText << QChar(i) << endl;
	}
	fileo.close();
	textResultFile.close();
}
*/

/*Reset*/
void GetFileStringUnicode::clear()
{
	result.clear();
	//xlsHandlers.clear();
	path=="";
}


void GetFileStringUnicode::setPath(std::string& fileName)
{
	path=fileName;
}

/*Using fontforge to make the smaller font files*/
/*
int GetFileStringUnicode::generateFont()
{
	if(!generateScript())
	{
		cerr<<"ttf files not found\n";
		return -1;
	}
	try
	{
		int error = system("fontforge -script script.pe");
		if(error==-1)throw -1;
	}
	catch(int err)
	{
		perror("System function error: ");
		return err;
	}
}
*/
/*
*  Give the range for adding or removing characters
*  status 1:add, 2:remove
*/
void GetFileStringUnicode::modifyFontRange(unsigned short start, unsigned short end, int status)
{
	int range=end-start;
	switch(status)
	{
		case 1:
			std::generate_n(inserter(result, result.begin()), range, [&start](){return start++;});
			break;
		case 2:
			for(auto i=result.begin(); i!=result.end(); ++i)
				if((*i)<end && (*i)>start)result.erase(i);
			break;
		default:break;
	}
}

/*Generate TS fils for Qt gui*/
void GetFileStringUnicode::generateTSFile()
{
	//for(auto i:xlsHandlers)i.generateTSFile();
	xlsHandlers.generateTSFile();
}

/*Get all used character from xls file*/
int GetFileStringUnicode::parseUsedCharacter()
{
	if(access(path.c_str(), F_OK)==-1)
	{
		cerr<<"xls file not found"<<endl;
		return -1;
	}

	for(int i = 0; i < 1; ++i) //504g6vu0 53u.3u ek7 one xlshandler
	{
		xlsHandlers.setXlsFile(const_cast<char*>(path.c_str()));
		//xlsHandlers[i].dumpStringToUnicode();
	}
	return 0;
}

int GetFileStringUnicode::openXlsFile()
{
	if(access(path.c_str(), F_OK)==-1)
	{
		cerr<<"xls file not found"<<endl;
		return -1;
	}
	if(xlsHandlers.setXlsFile(const_cast<char*>(path.c_str())))
		return -1;
	xlsHandlers.showBookInfo();
	return 0;
}


GetFileStringUnicode::~GetFileStringUnicode()
{

}

