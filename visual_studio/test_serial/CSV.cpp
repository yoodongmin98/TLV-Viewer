#include "CSV.h"
#include <OpenXLSX.hpp>
#include "imgui.h"
#include <string>

#include "MyTime.h"


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




void CSV::WriteFile(std::vector<int>& _Data , std::string& _Name)
{
	if (_Data.empty() || _Data.size()>1000)
		return;


	CreateFile(_Name);

	for (auto k=0; k<SheetList.size(); ++k)
	{
		for (auto i = 1; i <= _Data.size() / 4; ++i)
		{
			std::string TimeStampLine = "A" + std::to_string(Cells);
			std::string colName = getExcelColumnName(i + 1);  // 1부터 시작하는 열 인덱스
			std::string cellAddress = colName + std::to_string(Cells);  // "A1", "B1", "C1" ...
			SheetList[k].cell(TimeStampLine).value() = "[ " + MyTime::Time->GetLocalDay() + " " +MyTime::Time->GetLocalTime() + " ]";
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



void CSV::SaveFile()
{

}