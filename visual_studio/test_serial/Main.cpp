#include <iostream>
#include <fstream>
#include "EngineDebug.h"
#include "EngineCore.h"


//Main�� �⺻ ����ó��

int main()
{
    EngineDebug::LeakCheck(); //LeakCheck
    try
    {
        std::shared_ptr<EngineCore> Cores = std::make_shared<EngineCore>();
        Cores->Instance();
    }
    catch (const std::exception& e) {
        std::ofstream logFile("error_log.txt", std::ios::app);
        logFile << "������ �߻��߽��ϴ�. Error : " << e.what() << "\n";
        logFile.close();
        std::cerr << "������ �߻��߽��ϴ�. Error : " << e.what() << "\n";
        return EXIT_FAILURE;
    }
    catch (...) {
        std::ofstream logFile("error_log.txt", std::ios::app);
        logFile << "�˼����� ������ �߻��߽��ϴ�.\n";
        logFile.close();
        std::cerr << "�˼����� ������ �߻��߽��ϴ�.\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}