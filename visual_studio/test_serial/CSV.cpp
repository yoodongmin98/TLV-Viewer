#include "CSV.h"
#include <OpenXLSX.hpp>
#include "imgui.h"
#include <string>
#include "ThreadPool.h"
#include "MyTime.h"
#include <functional>
#include <string>
using namespace OpenXLSX;
using namespace std;
CSV::CSV()
{
	IsCreate = true;
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
	if (IsCreate)
	{
		IsCreate = false;
		RX1.create(_Name + "RX1.xlsx");
		RX2.create(_Name + "RX2.xlsx");
		RX3.create(_Name + "RX3.xlsx");
		RX4.create(_Name + "RX4.xlsx");

		RX1Sheet = RX1.workbook().sheet("Sheet1");
		RX2Sheet = RX2.workbook().sheet("Sheet1");
		RX3Sheet = RX3.workbook().sheet("Sheet1");
		RX4Sheet = RX4.workbook().sheet("Sheet1");

		SheetList.emplace_back(RX1Sheet);
		SheetList.emplace_back(RX2Sheet);
		SheetList.emplace_back(RX3Sheet);
		SheetList.emplace_back(RX4Sheet);
	}
}



void CSV::WriteExcel()
{
	
}



void CSV::WriteFile(std::vector<int> _Data, std::string& _Name, std::string _Time,const int _Frame)
{
	if (_Data.empty() || _Data.size() > 1000 || _Data.size()==0) 
	{ 
		std::cout << "_�����Ͱ� �̻��ؼ� �����߾ ������ : " << _Data.size() << std::endl;
		return; 
	}

	std::vector<std::vector<std::pair<std::string, std::string>>> sheetCellData;
	sheetCellData.resize(4);
	size_t dataSize = _Data.size() / 4;
	CreateFile(_Name);
	
	for (auto k = 0; k < 4; ++k)
	{
		std::string TimeStampLine = "A" + std::to_string(Cells);
		{
			//std::cout << "��Ʈ�� �� time : " << _Time << std::endl;
			sheetCellData[k].emplace_back(TimeStampLine, _Time + " " + std::to_string(_Frame));
		}

		for (size_t i = 1; i <= dataSize; ++i)
		{
			std::string cellAddress = getExcelColumnName(i + 1) + std::to_string(Cells);
			// �� ��Ʈ���� �ٸ� �����͸� ����
			int idx = k * dataSize + i - 1;
			sheetCellData[k].emplace_back(cellAddress, std::to_string(_Data[idx]));
		}
	}
	Cells++;
	for (auto k = 0; k < 4; ++k)
	{
		for (auto& [address, value] : sheetCellData[k])
		{
			std::lock_guard<std::shared_mutex> lock(sheetmutex);
			if (SheetList.size() == 4)
				SheetList[k].cell(address).value() = value;
			else
				std::cerr << "������4�� �ƴ϶� �����Ƚ�" << std::endl;
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
	if (RX1.isOpen())
	{
		RX1.save();
		RX2.save();
		RX3.save();
		RX4.save();
	}
}


//std::cout << "�Ľ̽��� : " << MyTime::Time->GetLocalTime() << std::endl;
//std::cout << "�Ľ̳� : " << MyTime::Time->GetLocalTime() << std::endl;