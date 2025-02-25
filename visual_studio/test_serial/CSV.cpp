#include "CSV.h"
#include <OpenXLSX.hpp>
#include "imgui.h"
#include <string>
#include "ThreadPool.h"
#include "MyTime.h"
#include <functional>

using namespace OpenXLSX;
using namespace std;
CSV::CSV()
{
	IsCreate = false;
}

CSV::~CSV()
{
	/*std::cout << "저장을 시작합니다" << std::endl;
	std::function<void()> Functions = std::bind(&CSV::WriteExcel, this);
	ThreadPool::TP->AddWork(Functions);*/
	if (RX1.isOpen())
	{
		RX1.save();
		RX2.save();
		RX3.save();
		RX4.save();
		std::cout << "저장이 완료되었습니다." << std::endl;
		RX1.close();
		RX2.close();
		RX3.close();
		RX4.close();
		std::cout << "파일을 닫습니다. 프로그램이 종료됩니다." << std::endl;
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



void CSV::WriteExcel()
{
	
}



void CSV::WriteFile(std::vector<int>& _Data, std::string& _Name, std::string& _Time)
{
	if (_Data.empty() || _Data.size() > 1000) { return; }

	sheetCellData.clear();
	sheetCellData.resize(4);
	size_t dataSize = _Data.size() / 4;
	CreateFile(_Name);
	
	for (auto k = 0; k < 4; ++k)
	{
		std::string TimeStampLine = "A" + std::to_string(Cells);
		{
			sheetCellData[k].emplace_back(TimeStampLine, _Time);
		}

		for (size_t i = 1; i <= dataSize; ++i)
		{
			std::string cellAddress = getExcelColumnName(i + 1) + std::to_string(Cells);
			// 각 시트마다 다른 데이터를 넣음
			int idx = k * dataSize + i - 1;
			sheetCellData[k].emplace_back(cellAddress, std::to_string(_Data[idx]));
		}
	}
	Cells++;
	for (auto k = 0; k < 4; ++k)
	{
		for (auto& [address, value] : sheetCellData[k])
		{
			SheetList[k].cell(address).value() = value;
		}
	}
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


//std::cout << "파싱시작 : " << MyTime::Time->GetLocalTime() << std::endl;
//std::cout << "파싱끝 : " << MyTime::Time->GetLocalTime() << std::endl;