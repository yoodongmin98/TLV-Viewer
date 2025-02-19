#include "CSV.h"
#include <OpenXLSX.hpp>






using namespace OpenXLSX;

CSV::CSV()
{

}

CSV::~CSV()
{
	for (XLDocument& Document : DocumentList)
	{
		Document.save();
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
		IsCreate = true;
	}
}


//어찌되었든 저장이 되긴 되니까 다른거 먼저해야징
void CSV::WriteFile(std::vector<int>& _Data)
{
	CreateFile();
	//파일 하나생성
	//512개로 나눈거 뭐 파일로 나누든 시트로 나누든 해서 128 x 4로 만듬(642,7 따로)
	int SheetCount = 1;
	//for (auto p = 0; p < 4; ++p)
	{
		//for (auto i = 1; i <= _Data.size() / 4; ++i)
		for (auto i = 1; i <= 64; ++i)
		{
			RX1Sheet.cell("A" + std::to_string(i)).value() = _Data[i];
		}
		//SheetCount++;
	}
	
	RX1.save();
	RX2.save();
	RX3.save();
	RX4.save();
	
}