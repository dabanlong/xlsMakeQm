#include <algorithm>
#include <set>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "xlsHandler.h"

/*
const std::string LANGUAGE_STRINGS[]=
{
	"en_US",
	"zh_TW",
	"ja_JP",
	"fi_FI",
	"bg_BG",
	"ru_RU",
	"pl_PL",
	"es_ES",
	"de_DE",
	"it_IT",
	"fr_FR",
	"nl_NL",
	"da_DK",
	"sv_SE",
	"pt_PT",
	"hu_HU",
	"sk_SK",
	"el_GR",
	"fa_IR",
	"zh_CN",
	"nn_NO",
	"tr_TR",
	"en_US2",
	"cs_CZ"
};
*/
XlsHandler::XlsHandler(char* fileName, char* type)
	: pWB(NULL), pWS(NULL)
{
	setXlsFile(fileName, type);
}

int XlsHandler::dumpStringToUnicode()
{
	if(pWB != NULL)
	{
		/*dump all cell's string*/
		//QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8"));
		for(int i = 0; i < pWB->sheets.count; i++)
		{
			pWS = xls_getWorkSheet(pWB, i);
			xls_parseWorkSheet(pWS);
			for(int t = 0; t <= pWS->rows.lastrow; t++)
			{
				for(int tt = 0; tt <= pWS->rows.lastcol; tt++)
				{
					//QString tmpString(pWS->rows.row[t].cells.cell[tt].str);
					//for(auto i : tmpString)cellStrUnicodeList.insert(i.unicode());
				}
			}
		}
		return 1;
	}
	else
		return -1;

}
int XlsHandler::setXlsFile(const char* fileName, char* type)
{
	pWB = xls_open(const_cast<char*>(fileName), type);
	if(pWB == NULL)
		return -1;
	else
		return 0;
}

void XlsHandler::getLangCodeList()
{
	for(int c = 0; c < pWB->sheets.count; c++)
	{
		pWS = xls_getWorkSheet(pWB, c);
		xls_parseWorkSheet(pWS);
		for(int tt = 1; tt <= pWS->rows.lastcol; tt++)
		{
			string tmpString;
			//QString tmpString(pWS->rows.row[t].cells.cell[tt].str);
			if(pWS->rows.row[1].cells.cell[tt].str)
			{
				tmpString = pWS->rows.row[1].cells.cell[tt].str;
				langCodeList.push_back(tmpString);
			}
		}
	}
}
void XlsHandler::showBookInfo()
{
	for(int c = 0; c < pWB->sheets.count; c++)
	{
		pWS = xls_getWorkSheet(pWB, c);
		xls_parseWorkSheet(pWS);
		for(int t = 0; t <= pWS->rows.lastrow; t++)
		{
			int char_num=0;
			for(int tt = 0; tt <= pWS->rows.lastcol; tt++)
			{
				string tmpString;
				//QString tmpString(pWS->rows.row[t].cells.cell[tt].str);
				if(pWS->rows.row[t].cells.cell[tt].str)
					tmpString = pWS->rows.row[t].cells.cell[tt].str;
				else
					tmpString = "";
				char_num+=tmpString.size()+1;
				std::cout << tmpString << "|";
			}
			std::cout << endl;
			for(int tt = 0; tt <= char_num; tt++)
			{
				std::cout << "-";
			}
			std::cout << endl;
		}
	}
}

/*
*  Generate TS file according to the form of xls file.
*  This must be modified if the form is changed.
*/
void XlsHandler::generateTSFile()
{
	if(access("ts_output", X_OK)!=0)
	{
		int status = mkdir("ts_output", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		if(status == -1)
			fprintf(stderr, "create dir error: %s\n", strerror(errno));
	}

	pWS = xls_getWorkSheet(pWB, 0);
	xls_parseWorkSheet(pWS);
	for(int tt = 0; tt <= pWS->rows.lastcol; tt++)
	{
		if(tt == 0)
		{
			for(int t = 2; t <= pWS->rows.lastrow; t++)
			{
				if(pWS->rows.row[t].cells.cell[tt].str)
					sourceStringList.push_back(pWS->rows.row[t].cells.cell[tt].str);
				else
					sourceStringList.push_back("");
			}
		}
		else if(tt > 0)
		{
			if(pWS->rows.row[1].cells.cell[tt].str)//check the language code
			{
				string filename;
				filename+="ts_output/";
				filename+="i80_";
				filename+=pWS->rows.row[1].cells.cell[tt].str;
				filename+=".ts";
				std::ofstream ofs(filename, std::ofstream::out | std::ofstream::trunc);
				ofs << "<?xml version=\"1.0\" encoding=\"utf-8\"?>" << endl
				    << "<TS version=\"2.0\" language=\""<< "" <<pWS->rows.row[1].cells.cell[tt].str<<"\">"<<endl
				    << "<context>"<<endl
				    << "<name>QObject</name>" << endl;

				vector<string>::iterator iter = sourceStringList.begin();
				for(int t = 2; t <= pWS->rows.lastrow; t++)
				{
					string targetString;
					if(pWS->rows.row[t].cells.cell[tt].str)
						targetString = pWS->rows.row[t].cells.cell[tt].str;
					else
						targetString = "";
					ofs << "<message>" <<endl
						<< "<source>" << *iter++ << "</source>"<<endl
						<< "<translation>" << targetString << "</translation>" <<endl
						<< "</message>" <<endl;
				}
				ofs << "</context>" <<endl
					<< "</TS>" <<endl;
				ofs.close();
				cout<< "Generate "<<filename<<" ... done."<<endl;
			}
			else
				break;
		}
	}
#if 0
	for(int i = 0; i < pWB->sheets.count; ++i)
	{
		QFile fileo(QString("./ts_output/%1_%2.ts").arg("i80").arg(QString::fromStdString(LANGUAGE_STRINGS[i])));
		fileo.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate);
		QTextStream streamo(&fileo);
		streamo.setCodec("UTF-8");
		streamo << "<?xml version=\"1.0\" encoding=\"utf-8\"?>" << endl
		        << QString("<TS version=\"2.0\" language=\"%1\"><context>").arg(QString::fromStdString(LANGUAGE_STRINGS[i])) << endl
		        << "<name>QObject</name>" << endl;
		pWS = xls_getWorkSheet(pWB, i);
		xls_parseWorkSheet(pWS);
		for(int j = 1; j < pWS->rows.lastrow; ++j)
		{
			QString tmpTitleStr(QString(pWS->rows.row[j].cells.cell[0].str).replace("&", "&amp;"));
			QString transStr(QString(pWS->rows.row[j].cells.cell[1].str).replace("&", "&amp;"));
			if(tmpTitleStr.isEmpty() || tmpTitleStr == QString::number(0))break;
			streamo << "<message>" << endl
			        << QString("<source>%1</source>").arg(tmpTitleStr) << endl;
			/*Filter null character and weird 0 character*/
			if(!(transStr.isEmpty() || transStr == QString::number(0)))
				streamo << QString("<translation>%1</translation>").arg(transStr) << endl;
			streamo << "</message>" << endl;
		}
		streamo << "</context>" << endl
		        << "</TS>" << endl;
		fileo.close();
	}
	system("lrelease ./ts_output/*.ts");
#endif
}

XlsHandler::~XlsHandler() {}

