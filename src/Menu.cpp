#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <cstring>

#include "Menu.h"
#include "Utils.h"

using namespace std;
using namespace sdds;

namespace sdds {
	/* Private Methods */
	void Menu::parse() { // This takes the menu content, parses it via the \t delim, and inserts each option into the m_option array. m_optionsContent is unusable after it is parsed, but we won't need it afterwards.
		char* token = strtok(m_optionsContent, "\t"); // m_optionsContent is searched until the \t character is found
		m_optionsCount = 0;

		while (token != nullptr && m_optionsCount < MAX_OPTIONS) { // Loop through m_optionsContent, extracting each option
			m_options[m_optionsCount] = new char[strlen(token) + 1];
			ut.alocpy(m_options[m_optionsCount], token);
			token = strtok(nullptr, "\t");
			m_optionsCount++;
		}

		isValid = m_optionsCount > 0 && m_optionsCount <= MAX_OPTIONS; // Set isValid based on criteria. Other methods will use this bool value to determine whether or not to proceed.
	}

	void Menu::set(const char* str) {
		delete[] m_optionsContent;
		m_optionsContent = new char[strlen(str) + 1];
		ut.alocpy(m_optionsContent, str);
	}

	/* Public Methods */
	size_t Menu::run() const { // Initialization of the menu
		int choice{};
		if (isValid) {
			cout << *this; // Writes menu to ostream
			choice = ut.getint(0, m_optionsCount, nullptr, nullptr); // Extracts input from istream
		}
		else {
			cout << "Invalid Menu!" << endl;
			choice = 0;
		}

		return choice;
	}

	const char* Menu::operator[](const size_t option) { // Returns the option string to caller
		return m_options[option - 1]; // getint already makes sure that the option is within bounds, no need to do safety check.
	}

	std::ostream& Menu::write(std::ostream& os) const { // Writes the main menu to ostream
		os << "---------------------------------\n";
		for (size_t i = 0; i < m_optionsCount; ++i) {
			os << i + 1 << "- " << m_options[i] << endl;
		}

		os << "---------------------------------" <<
			"\n" << "0- Exit" <<
			"\n" << "> ";

		return os;
	}

	Menu::Menu(const char* options) {
		if (options) {
			set(options);
			parse();
		}
		else {
			isValid = false;
		}
	}

	Menu::~Menu() {
		for (size_t i = 0; i < m_optionsCount; ++i) {
			delete[] m_options[i]; // delete each option string
		}
		delete[] m_optionsContent;
	}

	std::ostream& operator << (std::ostream& os,
		const Menu& menu) {
		return menu.write(os);
	}
}