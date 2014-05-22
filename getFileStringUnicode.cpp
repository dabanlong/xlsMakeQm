#include "getFileStringUnicode.h"
#include <iterator>
using namespace std;

GetFileStringUnicode::GetFileStringUnicode(const QString &filterType, QDir inPath)
	: path(inPath)
{
	fileList = getFilterFileList(filterType, path);
	xlsHandlers.resize(fileList.count());	
}
	

/*Get the file list with specific type*/
QStringList GetFileStringUnicode::getFilterFileList(const QString &filterType, QDir path)
{
	QStringList nameFilter(filterType);
	path.setNameFilters(nameFilter);
	QStringList fileStringList;
	fileStringList = path.entryList(nameFilter, QDir::Files, QDir::Name);
	return fileStringList;
}


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

void GetFileStringUnicode::generateScriptContext(QTextStream &script, QString &fileName)
{

	script << QString("Open(\"%1/%2\");").arg(path.path()).arg(fileName) << endl
	       << "Select(arr);" << endl
	       << "Clear();" << endl
	       << QString("PostNotice(\"preparing generating fonts of %1 ...\")").arg(fileName) << endl
	       << QString("Generate(\"%1/%2\");").arg(path.path()).arg(fileName) << endl;
}

/*Generate script file for fontforge*/
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

/*Export used character list and their unicode*/
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

/*Reset*/
void GetFileStringUnicode::clear()
{
	result.clear();
	delRange.clear();
	fileList.clear();
	xlsHandlers.clear();
	path.refresh();
}


void GetFileStringUnicode::setPath(const QString &fileName)
{
	path.setPath(fileName);
}

/*Using fontforge to make the smaller font files*/
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

/*
*  Give the range for adding or removing characters
*  status 1:add, 2:remove
*/
void GetFileStringUnicode::modifyFontRange(ushort start, ushort end, int status)
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
	xlsHandlers[1].generateTSFile();
}

/*Get all used character from xls file*/
int GetFileStringUnicode::parseUsedCharacter()
{
	if(fileList.empty())
	{
		cerr<<"xls file not found"<<endl;
		return -1;
	}
	QString filePath;
	for(int i = 0; i < fileList.count(); ++i)
	{
		filePath = path.path() + QString("/") + fileList[i];
		cout << filePath.toStdString() << endl;;
		xlsHandlers[i].setXlsFile(const_cast<char*>(filePath.toStdString().c_str()));
		xlsHandlers[i].dumpStringToUnicode();
		set<ushort> tmpList = xlsHandlers[i].getCodeList();
		result.insert(tmpList.begin(), tmpList.end());
	}	
	return 1;
}
	
GetFileStringUnicode::~GetFileStringUnicode()
{

}

