#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <ctime>
#include <cstring>
#include <iomanip>

#include "Utils.h"

using namespace std;
using namespace sdds;

namespace sdds {
	Utils ut;

	void Utils::testMode(bool testmode) {
		m_testMode = testmode;
	}

	void Utils::getSystemDate(int* year, int* mon, int* day) {
		if (m_testMode) {
			if (day) *day = sdds_testDay;
			if (mon) *mon = sdds_testMon;
			if (year) *year = sdds_testYear;
		}
		else {
			time_t t = std::time(NULL);
			tm lt = *localtime(&t);
			if (day) *day = lt.tm_mday;
			if (mon) *mon = lt.tm_mon + 1;
			if (year) *year = lt.tm_year + 1900;
		}
	}

	int Utils::daysOfMon(int month, int year)const {
		int days[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31, -1 };
		int mon = (month >= 1 && month <= 12 ? month : 13) - 1;
		return days[mon] + int((mon == 1) * ((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0));
	}

	void Utils::alocpy(char*& destination, const char* source) {
		delete[] destination;
		destination = nullptr;

		if (source && source[0]) {
			destination = new char[strlen(source) + 1];
			strcpy(destination, source);
		}
	}

	int Utils::getint(const char* prompt = nullptr) {
		int value;
		bool valid;

		if (prompt) {
			cout << prompt;
		}

		do {
			valid = true;
			cin >> value;

			if (cin.fail()) {
				cin.clear();
				cin.ignore(10000, '\n');
				cout << "Invalid Integer, retry: ";
				valid = false;
			}

		} while (!valid);

		return value;
	}

	int Utils::getint(int min, int max, const char* prompt = nullptr, const char* errMes = nullptr) {
		int value;

		do {
			value = getint(prompt);

			if (value < min || value > max) {
				if (errMes) {
					cout << errMes << ", retry: ";
				}
				else {
					cout << "Value out of range [" << min << "<=val<=" << max << "]: ";
				}
			}

		} while (value < min || value > max);

		return value;
	}

	double Utils::getDouble(const char* prompt) {
		double value;
		bool valid;

		if (prompt) {
			cout << prompt;
		}

		do {
			valid = true;
			cin >> value;

			if (cin.fail()) {
				cin.clear();
				cin.ignore(10000, '\n');
				cout << "Invalid number, retry: ";
				valid = false;
			}

		} while (!valid);

		return value;
	}


	double Utils::getDouble(double min, double max, const char* prompt, const char* errMes) {
		double value;

		do {
			value = getDouble(prompt);

			if (value < min || value > max) {
				if (errMes) {
					cout << errMes << ", retry: ";
				}
				else {
					cout << "Value out of range [" << std::fixed << std::setprecision(2) << min << "<=val<=" << std::fixed << std::setprecision(2) << max << "]: ";
				}
			}

		} while (value < min || value > max);

		return value;
	}

	void Utils::printBorder() {
		std::cout << "-----+-------+-------------------------------------+------+------"
			"+---------+-----------\n";
	}

	std::ofstream& Utils::printBorder(std::ofstream& of) {
		of << "-----+-------+-------------------------------------+------+------"
			"+---------+-----------\n";

		return of;
	}

	void Utils::printHeader() {
		std::cout << std::left << std::setfill(' '); // set alignment and fill for the header
		std::cout
			<< std::setw(5) << " ROW" << "|"
			<< std::setw(7) << "  SKU" << "|"
			<< std::setw(37) << " Description" << "|"
			<< std::setw(6) << " Have" << "|"
			<< std::setw(6) << " Need" << "|"
			<< std::setw(9) << "  Price" << "|"
			<< " Expiry" << std::endl;
	}

	std::ofstream& Utils::printHeader(std::ofstream& of) {
		of << std::left << std::setfill(' '); // set alignment and fill for the header
		of
			<< std::setw(5) << " ROW" << "|"
			<< std::setw(7) << "  SKU" << "|"
			<< std::setw(37) << " Description" << "|"
			<< std::setw(6) << " Have" << "|"
			<< std::setw(6) << " Need" << "|"
			<< std::setw(9) << "  Price" << "|"
			<< " Expiry" << std::endl;

		return of;
	}
}