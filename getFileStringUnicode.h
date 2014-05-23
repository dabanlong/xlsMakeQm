#ifndef GETFILESTRINGUNICODE_H
#define GETFILESTRINGUNICODE_H

#include <vector>
#include <iostream>
#include "xlsHandler.h"

class GetFileStringUnicode
{
public:
	GetFileStringUnicode(std::string path);
	//void dumpCharacter(const QStringList &stringList, QList<ushort> &list, QString path);
	//int generateFont();
	//void exportCharAndNumber(const QString &textFileName, const QString &codeFileName);
	void clear();
	void setPath(std::string &fileName);
	void generateTSFile();
	void modifyFontRange(unsigned short start, unsigned short end, int status);
	int parseUsedCharacter();
	int openXlsFile();
	~GetFileStringUnicode();
private:
	//void generateScriptContext(QTextStream &script, QString &fileName);
	//int generateScript();


	std::set<unsigned short> result;
	//QList<ushort> delRange;
	XlsHandler xlsHandlers;
	std::string path;
};


#endif
