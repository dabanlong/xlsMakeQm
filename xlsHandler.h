#ifndef XLS_HANDLER_H
#define XLS_HANDLER_H
#include <set>
#include <vector>
extern "C"
{
#include <libxls/xls.h>
}

using namespace std;

class XlsHandler
{
public:
	XlsHandler(char* fileName, char* type="UTF-8");
	XlsHandler():pWB(NULL), pWS(NULL){}
	int dumpStringToUnicode();
	xlsWorkBook* getBook(){return this->pWB;}
	std::set<unsigned short> getCodeList(){return this->cellStrUnicodeList;}
	int setXlsFile(char*, char* type="UTF-8");
	void showBookInfo();
	void generateTSFile();
	void getLangCodeList();
	~XlsHandler();
private:
	xlsWorkBook* pWB;
	xlsWorkSheet* pWS;
	std::set<unsigned short> cellStrUnicodeList;
	vector<string> langCodeList;
	vector<string> sourceStringList;

};
#endif
