#ifndef GETFILESTRINGUNICODE_H
#define GETFILESTRINGUNICODE_H
#define _XOPEN_SOURCE 500

#include <vector>
#include <iostream>
#include <fstream>
#include <ftw.h>
#include "xlsHandler.h"

class ParseGuiString
{
public:
    ParseGuiString();
    void parseStringsInTr(const char *filename);
    //void dumpCharacter(const QStringList &stringList, QList<ushort> &list, QString path);
    //int generateFont();
    //void exportCharAndNumber(const QString &textFileName, const QString &codeFileName);
    void clear();
    void setPath(std::string& fileName);
    void generateTSFile();
    void genXmlFromTrStrings();
    void modifyFontRange(unsigned short start, unsigned short end, int status);
    int parseUsedCharacter();
    int openXlsFile(const char *filename);
    ~ParseGuiString();
private:
    //void generateScriptContext(QTextStream &script, QString &fileName);
    //int generateScript();

    std::vector<std::string> trStringList;
    std::set<unsigned short> result;
    //QList<ushort> delRange;
    XlsHandler xlsHandlers;
    std::string path;
};


#endif
