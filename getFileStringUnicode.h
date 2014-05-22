#ifndef GETFILESTRINGUNICODE_H
#define GETFILESTRINGUNICODE_H

#include <QList>
#include <QStringList>
#include <QString>
#include <QDir>
#include <QTextStream>
#include <vector>
#include <iostream>
#include "xlsHandler.h"

class GetFileStringUnicode
{
public:
	GetFileStringUnicode(const QString &filterType, QDir path);
	QStringList getFilterFileList(const QString &filterType, QDir path);
	void dumpCharacter(const QStringList &stringList, QList<ushort> &list, QString path);
	int generateFont();
	void exportCharAndNumber(const QString &textFileName, const QString &codeFileName);
	void clear();
	void setPath(const QString &fileName);
	void generateTSFile();
	void modifyFontRange(ushort start, ushort end, int status);
	int parseUsedCharacter();
	~GetFileStringUnicode();
private:
	void generateScriptContext(QTextStream &script, QString &fileName);
	int generateScript();	
	
	
	std::set<ushort> result;
	QList<ushort> delRange;
	QStringList fileList;
	std::vector<XlsHandler> xlsHandlers;
	QDir path;
};


#endif
