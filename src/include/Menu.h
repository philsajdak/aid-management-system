#ifndef SDDS_MENU_H
#define SDDS_MENU_H

#include <iostream>

#include "Utils.h"

namespace sdds {
	const int MAX_OPTIONS = 15; // max num of options
	class Menu {
		char* m_optionsContent{};
		char* m_options[MAX_OPTIONS]{};
		size_t m_optionsCount{};
		bool isValid = false;
		void parse();
		void set(const char* str);

	public:
		size_t run() const;
		const char* operator[](const size_t option); // Returns the option string to caller
		std::ostream& write(std::ostream& os) const;
		Menu(const char* options = nullptr);
		Menu(const Menu& src) = delete; // prohibit copy
		Menu& operator=(const Menu& src) = delete; // prohibit assignment
		~Menu();
	};

	std::ostream& operator<<(std::ostream& os, const Menu& menu);
}
#endif