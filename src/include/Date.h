#ifndef SDDS_DATE_H
#define SDDS_DATE_H

#include <iostream>

#include "Utils.h"
#include "Status.h"

namespace sdds {
	const int MAX_YEAR = 2030; // Maximum year value (const)
	class Date {
		int m_year{};
		int m_month{};
		int m_day{};
		Status m_state{};
		bool m_formatted = true;
		bool validate(int currYear);
		int getUniqueInt() const;
		void setDefaultDate(int currYear, int currMonth, int currDay);
		bool processInput(int input, int currYear);
	public:
		Date(int year, int month, int day);
		Date();
		const Status& state() const;
		Date& formatted(const bool state);
		operator bool() const;
		bool operator==(const Date& rhs) const;
		bool operator!=(const Date& rhs) const;
		bool operator<(const Date& rhs) const;
		bool operator<=(const Date& rhs) const;
		bool operator>(const Date& rhs) const;
		bool operator>=(const Date& rhs) const;
		std::ostream& write(std::ostream& os) const;
		std::ostream& writeUnformatted(std::ostream& os) const;
		std::istream& read(std::istream& is);
	};

	std::ostream& operator<<(std::ostream& os, const Date& date);
	std::istream& operator>>(std::istream& is, Date& date);
}
#endif