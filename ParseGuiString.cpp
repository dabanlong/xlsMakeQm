#include "ParseGuiString.h"
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <iterator>
#include <algorithm>
#include <functional>


using namespace std;

static vector<string> filename_list;
static int display_info(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf)
{
	if(strstr(fpath, ".cpp") || strstr(fpath, ".h"))
	{
		if(!strstr(fpath, "moc_"))
			filename_list.push_back(fpath);
	}
	return 0;
}

ParseGuiString::ParseGuiString()
{

}

/*get all strings in QObject::tr() from all cpp file*/
void ParseGuiString::parseStringsInTr(const char* filename)
{
	int flags = 0;
	if(nftw(filename, display_info, 20, flags) == -1)
	{
		perror("nftw");
		exit(EXIT_FAILURE);
	}

	FILE *fp;
	size_t len = 0;
	ssize_t read;

	for(auto it = filename_list.begin(); it != filename_list.end(); ++it)
	{
		fp = fopen(it->c_str(), "rb");
		if(fp == NULL)
			continue;

		char *line = NULL;
		while((read = getline(&line, &len, fp)) != -1)
		{
			char *tr =line;
			//cout<<*it<<endl;
			while(tr = strstr(tr, "QObject::tr(\""))
			{
				char *end_of_tr = strstr(tr, "\")");
				size_t s = (int)(end_of_tr-((tr+sizeof("QObject::tr(\""))-1));
				char tr_string[s+1];
				memset(tr_string, 0, s+1);
				strncpy(tr_string, (tr+sizeof("QObject::tr(\""))-1, s);
				std::string tmp_str(tr_string);
				std::size_t found = tmp_str.find("\\n");
				while(found!=std::string::npos)
				{
					tmp_str.replace(found, 2, "\n");
					found = tmp_str.find("\\n", found);
				}
				found = tmp_str.find("\\t");
				while(found!=std::string::npos)
				{
					tmp_str.replace(found, 2, "\t");
					found = tmp_str.find("\\t", found);
				}
				std::cout<<tmp_str<<std::endl;
				trStringList.push_back(tmp_str);
				printf("Get string : %s\n", tr_string);
				tr = end_of_tr+sizeof("\")");
			}
		}
		free(line);
		fclose(fp);
	}
	sort(trStringList.begin(), trStringList.end(), less<string>());
	trStringList.erase( unique( trStringList.begin(), trStringList.end() ), trStringList.end() );
	/*for(auto it = trStringList.begin(); it != trStringList.end(); ++it)
	{
		cout<<*it<<endl;
	}*/
	genXmlFromTrStrings();
}

/*Generate the ts of source strings*/
void ParseGuiString::genXmlFromTrStrings()
{
	if(access("ts_output", X_OK)!=0)
	{
		int status = mkdir("ts_output", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		if(status == -1)
			fprintf(stderr, "create dir error: %s\n", strerror(errno));
	}
	FILE *fp;
	fp = fopen("ts_output/source.xml", "w");
	if(fp==NULL)
	{
		fprintf(stderr, "open file error: %s\n. stop generating xml file.", strerror(errno));
		return;
	}
	fputs("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n",fp);
	fputs("<context>\n",fp);
	fputs("<name>QObject</name>\n",fp);
	for(auto it = trStringList.begin(); it != trStringList.end(); ++it)
	{
		fputs("<message>\n",fp);
		fputs("<source>",fp);
		fputs(it->c_str(),fp);
		fputs("</source>\n",fp);
		fputs("</message>\n",fp);
		//cout<<*it<<endl;
	}
	fputs("</context>\n",fp);
	fclose(fp);
	printf("Generate ts_output/source.xml done.\n");
}


/*
void ParseGuiString::dumpCharacter(const QStringList &stringList, QList<ushort> &list, QString path)
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
void ParseGuiString::generateScriptContext(QTextStream &script, QString &fileName)
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
int ParseGuiString::generateScript()
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
void ParseGuiString::exportCharAndNumber(const QString &textFileName, const QString &codeFileName)
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
void ParseGuiString::clear()
{
	result.clear();
	//xlsHandlers.clear();
	path == "";
}


void ParseGuiString::setPath(std::string& fileName)
{
	path = fileName;
}

/*Using fontforge to make the smaller font files*/
/*
int ParseGuiString::generateFont()
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
void ParseGuiString::modifyFontRange(unsigned short start, unsigned short end, int status)
{
	int range = end - start;
	switch(status)
	{
		case 1:
			std::generate_n(inserter(result, result.begin()), range, [&start]()
			{
				return start++;
			});
			break;
		case 2:
			for(auto i = result.begin(); i != result.end(); ++i)
				if((*i) < end && (*i) > start)result.erase(i);
			break;
		default:
			break;
	}
}

/*Generate TS fils for Qt gui*/
void ParseGuiString::generateTSFile()
{
	//for(auto i:xlsHandlers)i.generateTSFile();
	xlsHandlers.generateTSFile();
}

/*Get all used character from xls file*/
int ParseGuiString::parseUsedCharacter()
{
	if(access(path.c_str(), F_OK) == -1)
	{
		cerr << "xls file not found" << endl;
		return -1;
	}

	for(int i = 0; i < 1; ++i) //504g6vu0 53u.3u ek7 one xlshandler
	{
		xlsHandlers.setXlsFile(const_cast<char*>(path.c_str()));
		//xlsHandlers[i].dumpStringToUnicode();
	}
	return 0;
}

int ParseGuiString::openXlsFile(const char* filename)
{
	if(access(filename, F_OK) == -1)
	{
		cerr << "xls file not found" << endl;
		return -1;
	}
	return xlsHandlers.setXlsFile(filename);
}


ParseGuiString::~ParseGuiString()
{

}

