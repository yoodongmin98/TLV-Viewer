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
	if (RX1.isOpen())
	{
		RX1.save();
		RX2.save();
		RX3.save();
		RX4.save();
	}
}





void CSV::CreateFile(std::string& _Name)
{
	if (!IsCreate)
	{
		RX1.create(_Name + "RX1.xlsx");
		RX2.create(_Name + "RX2.xlsx");
		RX3.create(_Name + "RX3.xlsx");
		RX4.create(_Name + "RX4.xlsx");

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


static int Cells = 1;
//어찌되었든 저장이 되긴 되니까 다른거 먼저해야징
void CSV::WriteFile(std::vector<int>& _Data , std::string& _Name)
{
	CreateFile(_Name);

	for (auto k=0; k<SheetList.size(); ++k)
	{
		for (auto i = 1; i <= _Data.size() / 4; ++i)
		{
			std::string colName = getExcelColumnName(i + 1);  // 1부터 시작하는 열 인덱스
			std::string cellAddress = colName + std::to_string(Cells);  // "A1", "B1", "C1" ...
			SheetList[k].cell(cellAddress).value() = _Data[i];  // 값 쓰기
		}
	}
	Cells++;
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