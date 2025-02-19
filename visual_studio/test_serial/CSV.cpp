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


//����Ǿ��� ������ �Ǳ� �Ǵϱ� �ٸ��� �����ؾ�¡
void CSV::WriteFile(std::vector<int>& _Data)
{
	CreateFile();
	//���� �ϳ�����
	//512���� ������ �� ���Ϸ� ������ ��Ʈ�� ������ �ؼ� 128 x 4�� ����(642,7 ����)
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