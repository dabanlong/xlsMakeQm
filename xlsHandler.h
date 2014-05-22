#ifndef XLS_HANDLER_H
#define XLS_HANDLER_H
#include <set>
#include <vector>
extern "C"
{
#include <libxls/xls.h>
}
class XlsHandler
{
public:
	XlsHandler(char* fileName, char* type="UTF-8");
	XlsHandler():pWB(nullptr), pWS(nullptr){}
	int dumpStringToUnicode();
	xlsWorkBook* getBook(){return this->pWB;}
	std::set<ushort> getCodeList(){return this->cellStrUnicodeList;}
	void setXlsFile(char*, char* type="UTF-8");
	void showBookInfo();
	void generateTSFile();
	~XlsHandler();
private:
	xlsWorkBook* pWB;
	xlsWorkSheet* pWS;
	std::set<ushort> cellStrUnicodeList;
	std::vector<std::string> langNameList;
};
#endif
