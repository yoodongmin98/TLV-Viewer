#pragma once
#include <iostream>
#include <vector>
#include <OpenXLSX.hpp>
#include <string>
#include <mutex>
#include <thread>



using namespace OpenXLSX;

class CSV
{
public:
	CSV();
	~CSV();

	void WriteFile(std::vector<int>& _Data , std::string& _Name, std::string& _Time);
	void SaveFile();
	void WriteExcel();
protected:
	void CreateFile(std::string& _Name);
	std::string getExcelColumnName(int colNum);
private:
	bool IsCreate = false;

	int Cells = 1;

	std::mutex csvMutex;
	//test
	int Count = 0;

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


	std::vector<std::vector<std::pair<std::string, std::string>>> sheetCellData;


	std::thread CsvThread;
};