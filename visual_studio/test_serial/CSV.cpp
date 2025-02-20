#include "CSV.h"
#include <OpenXLSX.hpp>

#include <string>




using namespace OpenXLSX;
using namespace std;
CSV::CSV()
{

}

CSV::~CSV()
{
	for (XLDocument& Document : DocumentList)
	{
		Document.save();

		if(Document.isOpen())
			Document.close();
	}
}





void CSV::CreateFile()
{
	if (!IsCreate)
	{
		RX1.create("RX1.xlsx");
		RX2.create("RX2.xlsx");
		RX3.create("RX3.xlsx");
		RX4.create("RX4.xlsx");

		RX1Sheet = RX1.workbook().worksheet("Sheet1");
		RX2Sheet = RX2.workbook().worksheet("Sheet1");
		RX3Sheet = RX3.workbook().worksheet("Sheet1");
		RX4Sheet = RX4.workbook().worksheet("Sheet1");

		SheetList.emplace_back(RX1Sheet);
		SheetList.emplace_back(RX2Sheet);
		SheetList.emplace_back(RX3Sheet);
		SheetList.emplace_back(RX4Sheet);
		IsCreate = true;
	}
}


//어찌되었든 저장이 되긴 되니까 다른거 먼저해야징
void CSV::WriteFile(std::vector<int>& _Data)
{
	CreateFile();


	for (auto& Sheets : SheetList)
	{
		for (auto i = 1; i <= _Data.size() / 4; ++i)
		{
			std::string colName = getExcelColumnName(i + 1);  // 1부터 시작하는 열 인덱스
			std::string cellAddress = colName + "1";  // "A1", "B1", "C1" ...
			Sheets.cell(cellAddress).value() = _Data[i];  // 값 쓰기
		}
	}
	
	RX1.save();
	RX2.save();
	RX3.save();
	RX4.save();
	
}


std::string CSV::getExcelColumnName(int colNum)
{
	string columnName;
	while (colNum > 0) 
	{
		int remainder = (colNum - 1) % 26;  
		columnName = char('A' + remainder) + columnName;
		colNum = (colNum - 1) / 26;
	}
	return columnName;
}