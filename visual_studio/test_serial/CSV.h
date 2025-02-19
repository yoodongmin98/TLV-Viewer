#pragma once
#include <iostream>
#include <vector>
#include <OpenXLSX.hpp>

using namespace OpenXLSX;
//여기에 OpenXlsx넣을듯 ㅇㅇ
class CSV
{
public:
	CSV();
	~CSV();

	void WriteFile(std::vector<int>& _Data);
protected:
	void CreateFile();
private:
	bool IsCreate = false;
	std::vector<XLDocument> DocumentList;
	XLDocument RX1;
	XLDocument RX2;
	XLDocument RX3;
	XLDocument RX4;

	std::vector<XLWorksheet> SheetList;
	XLWorksheet RX1Sheet;
	XLWorksheet RX2Sheet;
	XLWorksheet RX3Sheet;
	XLWorksheet RX4Sheet;
};