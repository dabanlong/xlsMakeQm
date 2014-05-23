#include <algorithm>
#include <set>
#include <iostream>
#include "xlsHandler.h"
#include <stdio.h>
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
	:pWB(NULL), pWS(NULL)
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
int XlsHandler::setXlsFile(char* fileName, char* type)
{
	pWB=xls_open(fileName, type);
	if(pWB==NULL)
		return -1;
	return 0;
}

void XlsHandler::getLangCodeList()
{

}
void XlsHandler::showBookInfo()
{
	for(int c = 0; c < pWB->sheets.count; c++)
	{
		pWS = xls_getWorkSheet(pWB, c);
		xls_parseWorkSheet(pWS);
		for(int t = 0; t <= pWS->rows.lastrow; t++)
		{
			for(int tt = 0; tt <= pWS->rows.lastcol; tt++)
			{
				//QString tmpString(pWS->rows.row[t].cells.cell[tt].str);
				std::string tmpString(pWS->rows.row[t].cells.cell[tt].str);
				std::cout<<tmpString<<std::endl;
			}
		}
	}
}

/*
*  Generate TS file according to the form of xls file.
*  This must be modified if the form is changed.
*/
void XlsHandler::generateTSFile()
{
#if 0
	for(int i=0; i<pWB->sheets.count; ++i)
	{
		QFile fileo(QString("./ts_output/%1.ts").arg(QString::fromStdString(LANGUAGE_STRINGS[i])));
		fileo.open(QIODevice::WriteOnly | QIODevice::Text |QIODevice::Truncate);
		QTextStream streamo(&fileo);
		streamo.setCodec("UTF-8");
		streamo<<"<?xml version=\"1.0\" encoding=\"utf-8\"?>"<<endl
			   <<QString("<TS version=\"2.0\" language=\"%1\"><context>").arg(QString::fromStdString(LANGUAGE_STRINGS[i]))<<endl
			   <<"<name>QObject</name>"<<endl;
		pWS = xls_getWorkSheet(pWB, i);
		xls_parseWorkSheet(pWS);
		for(int j=1; j<pWS->rows.lastrow; ++j)
		{
			QString tmpTitleStr(QString(pWS->rows.row[j].cells.cell[0].str).replace("&","&amp;"));
			QString transStr(QString(pWS->rows.row[j].cells.cell[1].str).replace("&","&amp;"));
			if(tmpTitleStr.isEmpty()||tmpTitleStr==QString::number(0))break;
			streamo<<"<message>"<<endl
				   <<QString("<source>%1</source>").arg(tmpTitleStr)<<endl;
			/*Filter null character and weird 0 character*/
			if(!(transStr.isEmpty()||transStr==QString::number(0)))
				streamo<<QString("<translation>%1</translation>").arg(transStr)<<endl;
				streamo<<"</message>"<<endl;
		}
		streamo<<"</context>"<<endl
			   <<"</TS>"<<endl;
		fileo.close();
	}
	system("lrelease ./ts_output/*.ts");
#endif
}

XlsHandler::~XlsHandler(){}

