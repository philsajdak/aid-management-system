#ifndef SDDS_UTILS_H
#define SDDS_UTILS_H

#include <fstream>

namespace sdds {
    const int sdds_testYear = 2023;
    const int sdds_testMon = 12;
    const int sdds_testDay = 9;
    class Utils {
        bool m_testMode = false;
    public:
        void getSystemDate(int* year = nullptr, int* mon = nullptr, int* day = nullptr);
        int daysOfMon(int mon, int year)const;
        void alocpy(char*& destination, const char* source);
        int getint(const char* prompt);
        int getint(int min, int max, const char* prompt, const char* errMes);
        double getDouble(const char* prompt = nullptr);
        double getDouble(double min, double max, const char* prompt = nullptr, const char* errMes = nullptr);
        void printBorder();
        std::ofstream& printBorder(std::ofstream& of);
        void printHeader();
        std::ofstream& printHeader(std::ofstream& of);
        void testMode(bool testmode = true);
    };
    extern Utils ut;
}
#endif // !SDDS_UTILS_H
