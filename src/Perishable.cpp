#include <iostream>
#include <iomanip>
#include <fstream>

#include "Perishable.h"

namespace sdds {
	Perishable::Perishable(const Perishable& src) : Item(src) {
		if (src.m_instructions && src.m_instructions[0] != 0) ut.alocpy(m_instructions, src.m_instructions);

		if (src.m_expiry) m_expiry = src.m_expiry;
	}

	Perishable& Perishable::operator=(const Perishable& src) {
		if (this != &src) {
			this->Item::operator=(src);
			if (src.m_instructions && src.m_instructions[0] != 0) ut.alocpy(m_instructions, src.m_instructions);
			else m_instructions[0] = 0;

			if (src.m_expiry) m_expiry = src.m_expiry;
		}

		return *this;
	}

	Perishable::~Perishable() {
		delete[] m_instructions;
		m_instructions = nullptr;
	}

	const Date& Perishable::expiry() const {
		return m_expiry;
	}

	int Perishable::readSku(std::istream& istr) {
		std::cout << "SKU: ";
		m_sku = ut.getint(10000, 39999, nullptr, nullptr);
		return m_sku;
	}

	std::ofstream& Perishable::save(std::ofstream& ofstr) const {
		if (*this && ofstr) {
			ofstr << m_sku << "\t" << description() << "\t" << qty() << "\t" << qtyNeeded()
				<< "\t" << std::fixed << std::setprecision(2) << double(*this) << "\t";

			// Add perishable-specific details
			if (m_instructions && m_instructions[0] != '\0') {
				ofstr << m_instructions;
			}

			ofstr << "\t";
			m_expiry.writeUnformatted(ofstr);
			ofstr << "\n";
		}
		return ofstr;
	}

	std::ifstream& Perishable::load(std::ifstream& ifstr) {
		if (!ifstr.fail()) {
			this->Item::load(ifstr);
			char tmp[500]{}; // Repo doesn't state a max description length. 500 in this case is an arbitrary number.
			if (ifstr.peek() != '\t') {
				ifstr.get(tmp, 500, '\t');
				ut.alocpy(m_instructions, tmp);
			}
			else {
				if (m_instructions) m_instructions[0] = 0;
			}
			ifstr.ignore();
			ifstr >> m_expiry;
			ifstr.ignore();
		}
		else {
			m_status = "Input file stream read (perishable) failed!";
		}

		return ifstr;
	}

	std::ostream& Perishable::display(std::ostream& ostr) const {
		if (!*this) {
			ostr << m_status;
		}
		else {
			if (linear()) { // linear
				this->Item::display(ostr);
				if (m_instructions && m_instructions[0] != 0) {
					ostr << "*";
				}
				else {
					ostr << " ";
				}
				ostr << m_expiry;
			}
			else { // not linear
				ostr << "Perishable ";
				this->Item::display(ostr);
				ostr << "Expiry date: ";
				ostr << m_expiry << "\n";
				if (m_instructions && m_instructions[0]) {
					ostr << "Handling Instructions: " << m_instructions << '\n';
				}
			}
		}

		return ostr;
	}

	std::istream& Perishable::read(std::istream& istr) {
		Item::read(istr);
		char tmp[500]{};  // Repo doesn't state a max description length. 500 in this case is an arbitrary number.
		delete m_instructions;
		m_instructions = nullptr;
		std::cout << "Expiry date (YYMMDD): ";
		istr >> m_expiry;
		istr.ignore();
		std::cout << "Handling Instructions, ENTER to skip: ";

		if (istr.peek() != '\n') {
			istr.getline(tmp, 500);
			ut.alocpy(m_instructions, tmp);
		}

		if (istr.fail()) {
			m_status = "Perishable console date entry failed!";
		}

		return istr;
	}
}