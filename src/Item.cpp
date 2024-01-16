#define _CRT_SECURE_NO_WARNINGS

#include <cstring>
#include <fstream>
#include <iomanip>

#include "Item.h"
#include "Utils.h"

namespace sdds {
	Item::Item(const Item& src) : m_price(src.m_price), m_qty(src.m_qty), m_neededQty(src.m_neededQty), m_desc(nullptr), m_linear(src.m_linear), m_status(src.m_status), m_sku(src.m_sku) {
		ut.alocpy(m_desc, src.m_desc);
	}

	Item& Item::operator=(const Item& src) {
		if (this != &src) {
			this->clear();
			if (src) {
				m_price = src.m_price;
				m_qty = src.m_qty;
				m_neededQty = src.m_neededQty;
				m_linear = src.m_linear;
				m_sku = src.m_sku;
				ut.alocpy(m_desc, src.m_desc);
				this->m_status = src.m_status;
			}
		}

		return *this;
	}

	Item::~Item() {
		delete[] m_desc;
		m_desc = nullptr;
	}

	int Item::qtyNeeded() const {
		return m_neededQty;
	}

	int Item::qty() const {
		return m_qty;
	}

	const char* Item::description() const
	{
		return m_desc;
	}

	Item::operator double() const {
		return m_price;
	}

	Item::operator bool() const {
		return m_sku > 0;
	}

	int Item::operator-=(int qty) {
		m_qty -= qty;
		return m_qty;
	}

	int Item::operator+=(int qty) {
		m_qty += qty;
		return m_qty;
	}

	void Item::linear(bool isLinear) {
		m_linear = isLinear;
	}

	char* Item::shortDescription(char* dest, const char* src, size_t maxLen = DESC_LEN) const {
		strncpy(dest, src, maxLen);
		dest[maxLen] = '\0';
		return dest;
	}

	bool Item::operator==(int sku) const {
		return m_sku == sku;
	}

	bool Item::operator==(const char* description) const {
		return strstr(m_desc, description);
	}

	std::ofstream& Item::save(std::ofstream& ofstr) const {
		if (*this && ofstr) {
			ofstr << m_sku << "\t" << m_desc << "\t" << m_qty << "\t" << m_neededQty
				<< "\t" << std::fixed << std::setprecision(2) << m_price << "\n";
		}

		return ofstr;
	}

	std::ifstream& Item::load(std::ifstream& ifstr) {
		char tmpDesc[500]{}; // Repo doesn't state a max description length. 500 in this case is an arbitrary number.
		this->clear();

		if (ifstr.is_open()) {
			ifstr >> m_sku;
			ifstr.ignore();
			ifstr.get(tmpDesc, 1000, '\t');
			ut.alocpy(m_desc, tmpDesc);
			ifstr >> m_qty;
			ifstr.ignore();
			ifstr >> m_neededQty;
			ifstr.ignore();
			ifstr >> m_price;
			ifstr.ignore();
		}
		else {
			m_status = "Input file stream read failed!";
		}

		return ifstr;
	}

	std::ostream& Item::display(std::ostream& ostr) const {
		if (!*this) {
			ostr << m_status;
		}
		else {
			if (m_linear) {
				char shortDesc[DESC_LEN + 1]{};
				shortDescription(shortDesc, m_desc, DESC_LEN);
				ostr << m_sku << " | " << std::left << std::setw(35) << std::setfill(' ') << shortDesc << " | "
					<< std::setw(4) << std::right << m_qty << " | " << std::setw(4)
					<< std::right << m_neededQty << " | " << std::fixed
					<< std::setprecision(2) << std::setw(7) << std::right << m_price
					<< " |";
			}
			else {
				ostr << "AMA Item:\n"
					<< m_sku << ": " << m_desc << "\n"
					<< "Quantity Needed: " << m_neededQty << "\n"
					<< "Quantity Available: " << m_qty << "\n"
					<< "Unit Price: $" << std::fixed << std::setprecision(2) << m_price
					<< "\n"
					<< "Needed Purchase Fund: $" << (m_neededQty - m_qty) * m_price
					<< "\n";
			}
		}

		return ostr;
	}

	int Item::readSku(std::istream& istr) {
		std::cout << "SKU: ";
		m_sku = ut.getint(40000, 99999, nullptr, nullptr);
		return m_sku;
	}

	std::istream& Item::read(std::istream& istr) {
		char tmp[500]{};  // Repo doesn't state a max description length. 500 in this case is an arbitrary number.
		this->clear();

		std::cout << "AMA Item:\n"
			<< "SKU: " << m_sku
			<< "\n"
			<< "Description: ";
		istr.ignore();
		istr.getline(tmp, 100, '\n');
		ut.alocpy(m_desc, tmp);
		std::cout << "Quantity Needed: ";
		m_neededQty = ut.getint(1, 9999, nullptr, nullptr);
		std::cout << "Quantity On Hand: ";
		m_qty = ut.getint(0, m_neededQty, nullptr, nullptr);
		std::cout << "Unit Price: $";
		m_price = ut.getDouble(0.0, 9999.0, nullptr, nullptr);

		if (istr.fail()) {
			m_status = "Console entry failed!";
		}

		return istr;
	}

	void Item::clear() {
		delete[] m_desc;
		m_desc = nullptr;
		this->m_status.clear();  // resets the status object back to good
	}
}  // namespace sdds