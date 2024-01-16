#ifndef SDDS_AIDMAN_H
#define SDDS_AIDMAN_H

#include <iostream>
#include <string>

#include "Utils.h"
#include "Perishable.h"
#include "Menu.h"
#include "Date.h"
#include "iProduct.h"

namespace sdds {
	const int sdds_max_num_items = 100;
	class AidMan {
		char* m_fileName{};
		Menu m_menu{};
		Date m_currDate{};
		iProduct* m_items[sdds_max_num_items]{};
		int m_numItems{};
		size_t menu() const;
		void displayListMenu();
		void set(const char* str);
		bool promptFileName();
		void save(); 
		void deallocate(); 
		int list(const char* sub_desc = nullptr); 
		bool load();
		void sort();
		int compare(iProduct& prod1, iProduct& prod2);
		int search(int sku) const;
		void add();
		void remove(int index);
		void update();
		int select();
		void processFileOpenFailure();
		void processShipping();
		void processMenuSelection(size_t choice);
		void processItemAddition(iProduct& product);
		void processItemRemoval();
		void confirmUpdateQty(int index);
		void confirmRemoveItem(int index);
	public:
		AidMan(const char* fileName = nullptr);
		AidMan(const AidMan& src) = delete; // prohibit copy
		AidMan& operator=(const AidMan& src) = delete; // prohibit assignment
		~AidMan();
		void run();
	};
}
#endif