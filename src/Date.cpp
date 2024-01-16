#include <iostream>
#include <iomanip>
#include "Utils.h"
#include "Date.h"

using namespace sdds;
namespace sdds {
	void getCurrentDate(int& year, int& month, int& day);
	/* Private methods */
	bool Date::validate(int currYear) {
		int days = ut.daysOfMon(this->m_month, this->m_year);
		bool valid = false;
		if (this->m_year < currYear || this->m_year > MAX_YEAR) {
			this->m_state = "Invalid year in date";
			this->m_state = 1;
		}
		else if (this->m_month < 1 || this->m_month > 12) {
			this->m_state = "Invalid month in date";
			this->m_state = 2;
		}
		else if (this->m_day < 1 || this->m_day > days) {
			this->m_state = "Invalid day in date";
			this->m_state = 3;
		}
		else {
			this->m_state.clear();
			valid = true;
		}
		return valid;
	}

	int Date::getUniqueInt() const {
		return (this->m_year * 372 + this->m_month * 31 + this->m_day);
	}

	void Date::setDefaultDate(int currYear, int currMonth, int currDay) {
		this->m_year = currYear;
		this->m_month = currMonth;
		this->m_day = currDay;
	}

	bool Date::processInput(int input, int currYear) {
		int year{}, month{}, day{};

		if (input >= 100000 && input <= 999999) {
			year = (input / 10000) + 2000;
			month = (input % 10000) / 100;
			day = input % 100;
		}
		else if (input >= 1000 && input <= 9999) {
			year = currYear;
			month = input / 100;
			day = input % 100;
		}
		else {
			this->m_state = "Invalid month in date";
			this->m_state = 2;
			return false;
		}

		this->m_year = year;
		this->m_month = month;
		this->m_day = day;

		return validate(currYear);
	}

	/* Public methods */
	Date::Date(int year, int month, int day) : m_year(year), m_month(month), m_day(day) {
		int currYear, currMonth, currDay;
		getCurrentDate(currYear, currMonth, currDay);
		if (!validate(currYear)) setDefaultDate(currYear, currMonth, currDay);
	}

	Date::Date() {
		int currYear, currMonth, currDay;
		getCurrentDate(currYear, currMonth, currDay);
		setDefaultDate(currYear, currMonth, currDay);
	}

	std::ostream& Date::writeUnformatted(std::ostream& os) const {
		os << std::setw(2) << std::setfill('0') << (this->m_year % 100)
			<< std::setw(2) << std::setfill('0') << this->m_month
			<< std::setw(2) << std::setfill('0') << this->m_day;
		return os;
	}

	const Status& Date::state() const {
		return this->m_state;
	}

	Date& Date::formatted(const bool state) {
		this->m_formatted = state;
		return *this;
	}

	Date::operator bool() const {
		return this->m_state;
	}

	std::ostream& Date::write(std::ostream& os) const {
		std::cout.flags(std::ios_base::fmtflags(0)); // Reset all format flags

		if (this->m_formatted) {
			os << this->m_year << "/"
				<< std::setw(2) << std::setfill('0') << this->m_month << "/"
				<< std::setw(2) << std::setfill('0') << this->m_day;
		}
		else {
			os << std::setw(2) << std::setfill('0') << (this->m_year % 100)
				<< std::setw(2) << std::setfill('0') << this->m_month
				<< std::setw(2) << std::setfill('0') << this->m_day;
		}
		return os;
	}

	std::istream& Date::read(std::istream& is) {
		int input;
		is >> input;

		if (is.fail()) {
			m_state = "Invalid date value";
			return is;
		}

		int currYear, currMonth, currDay;
		getCurrentDate(currYear, currMonth, currDay);
		processInput(input, currYear);

		return is;
	}

	bool Date::operator==(const Date& rhs) const {
		return getUniqueInt() == rhs.getUniqueInt();
	}

	bool Date::operator!=(const Date& rhs) const {
		return getUniqueInt() != rhs.getUniqueInt();
	}

	bool Date::operator<(const Date& rhs) const {
		return getUniqueInt() < rhs.getUniqueInt();
	}

	bool Date::operator<=(const Date& rhs) const {
		return getUniqueInt() <= rhs.getUniqueInt();
	}

	bool Date::operator>(const Date& rhs) const {
		return getUniqueInt() > rhs.getUniqueInt();
	}

	bool Date::operator>=(const Date& rhs) const {
		return getUniqueInt() >= rhs.getUniqueInt();
	}

	std::ostream& operator<<(std::ostream& os, const Date& date) {
		return date.write(os);
	}

	std::istream& operator>>(std::istream& is, Date& date) {
		return date.read(is);
	}

	// Utility function to get the current system date and cache it
	void getCurrentDate(int& year, int& month, int& day) {
		ut.getSystemDate(&year, &month, &day);
	}
}