#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>

#include "AidMan.h"
#include "Date.h"

using namespace std;
using namespace sdds;

namespace sdds {
	// Constructor: Initializes AidMan with a file name, if provided.
	AidMan::AidMan(const char* fileName) : m_menu("List Items\tAdd Item\tRemove Item\tUpdate Quantity\tSort\tShip Items\tNew/Open Aid Database"), m_currDate(Date()) {
		if (fileName) {
			set(fileName);
		}
	}

	// Destructor: Cleans up the dynamic memory allocated for the file name and product resources.
	AidMan::~AidMan() {
		delete[] m_fileName;
		deallocate();
	}

	// This function runs the main application loop.
	void AidMan::run() {
		size_t choice{};
		do {
			choice = menu();
			if (!m_fileName && choice != 7 && choice != 0) choice = 7; // Default to choice 7 if fileName is null
			if (choice) cout << "\n" << "****" << m_menu[choice] << "****" << "\n"; // Prints the menu description, as long as it's not 0
			processMenuSelection(choice); // This function handles each menu choice
		} while (choice);
		save();
		cout << "Exiting Program!" << endl;
	}

	// This function displays the main menu and returns the user's choice.
	size_t AidMan::menu() const { // Displays the main title, date, and file name
		cout << "Aid Management System" << endl <<
			"Date: " << m_currDate << endl <<
			"Data file: " << (m_fileName ? m_fileName : "No file") << endl;
		return m_menu.run(); // Starts the main menu loop
	}

	// This function displays the list menu for item selection.
	void AidMan::displayListMenu() {
		int choice{};
		int itemsFound = list(nullptr);  // Display all items

		if (itemsFound > 0) {
			cout << "Enter row number to display details or <ENTER> to continue:\n> ";
			cin.ignore();
			if (cin.peek() != '\n') {
				choice = ut.getint(0, itemsFound, nullptr, nullptr);

				if (choice > 0 && choice <= itemsFound && m_items[choice - 1]) {
					m_items[choice - 1]->linear(false);
					cout << *m_items[choice - 1];
				}
			}
		}
	}

	// This function sets the current file name.
	void AidMan::set(const char* str) { // Sets the current filename to the str passed
		if (str && m_fileName != str) { // Check for nullptr and self assignment
			char* tmp = new char[strlen(str) + 1];
			ut.alocpy(tmp, str);

			delete[] m_fileName;
			m_fileName = tmp;
		}
	}

	// This function prompts the user for a file name and tries to load it.
	bool AidMan::promptFileName() {
		bool result = false;
		cout << "Enter file name: ";
		char tmpFileName[200]{};
		cin.ignore(1000, '\n');
		cin.getline(tmpFileName, 200);

		if (!cin.fail() && tmpFileName[0] != '\0') {
			set(tmpFileName);
			if (load()) cout << m_numItems << " records loaded!\n"; // If there were items in the file, then print records loaded
			result = true;
		}

		return result;
	}

	// This function saves the current state to the file.
	void AidMan::save() {
		int i{};
		if (m_fileName && m_numItems) { // If m_fileName is not null, and actually has items:
			ofstream file(m_fileName);
			for (; i < m_numItems; i++) {
				m_items[i]->save(file);
			}
			file.close();
		}
	}

	// This function deallocates all items in the inventory.
	void AidMan::deallocate() {
		for (int i = 0; i < m_numItems; i++) {
			delete m_items[i];
			m_items[i] = nullptr;
		}
		m_numItems = 0;
	}

	// This function lists all items or those matching a given description.
	int AidMan::list(const char* sub_desc) {
		if (!m_numItems) {
			cout << "The list is empty!\n";
			return 0;
		}

		int count = 0;
		ut.printHeader();
		ut.printBorder();
		for (int i = 0; i < m_numItems; i++) {
			if (m_items[i] && (sub_desc == nullptr || *m_items[i] == sub_desc)) {
				cout << right << std::setfill(' ') << setw(4) << i + 1 << " | ";
				m_items[i]->linear(true);
				cout << *m_items[i] << endl;
				count++;
			}
		}
		ut.printBorder();

		return count; // Return the count of items matching the description or total items if no description
	}

	// This function loads item data from the file.
	bool AidMan::load() {
		if (m_fileName == nullptr) {
			return promptFileName(); // Exit function, prompting to get file name
		}
		else {
			save();
			deallocate();
			ifstream ifstr(m_fileName);

			if (ifstr.fail()) {
				processFileOpenFailure();
				return false; // Exit function afterwards, since the load failed
			}

			char nextChar;
			while (!ifstr.fail() && m_numItems < sdds_max_num_items) {
				nextChar = ifstr.peek();

				// Depending on the first character:
				if (nextChar >= '1' && nextChar <= '3') {
					m_items[m_numItems] = new Perishable;
				}
				else if (nextChar >= '4' && nextChar <= '9') {
					m_items[m_numItems] = new Item;
				}
				else {
					ifstr.setstate(ios::failbit); // If it's not a digit, then set fail
					ifstr.close();
					break; // We can exit the function now
				}

				// Attmpt to load the item data from the file
				if (m_items[m_numItems] && m_items[m_numItems]->load(ifstr)) {
					m_numItems++;  // Item is loaded and valid
				}
				else {
					// If load failed:
					delete m_items[m_numItems];
					m_items[m_numItems] = nullptr;
					cout << "Error loading item data.\n";
				}
			}

			ifstr.close();
		}

		return m_numItems > 0;
	}

	// This function sorts the items based on their quantity difference. Uses bubble sort algorithm.
	void AidMan::sort() {
		if (!m_numItems) return; // No point in sorting nothing, so exit function

		bool sorted = false;
		int swapped;
		iProduct* temp = nullptr; // Used for swapping pointers

		// Loop through the items until all of them are sorted in descending order
		while (!sorted) {
			swapped = 0;

			for (int i = 0; i < m_numItems - 1; i++) {
				int comparisonResult = compare(*m_items[i], *m_items[i + 1]);

				if (comparisonResult < 0) { // If the current iProduct element qty difference is less than the previous one:
					// Swap the pointers using good ol' bubble sort
					temp = m_items[i];
					m_items[i] = m_items[i + 1];
					m_items[i + 1] = temp;
					swapped = 1;
				}
			}

			if (!swapped) sorted = true;
		}

		cout << "Sort completed!\n";
	}

	// This function compares two products based on their quantity difference.
	int AidMan::compare(iProduct& prod1, iProduct& prod2) {
		int firstProductDiff = prod1.qtyNeeded() - prod1.qty();
		int secondProductDiff = prod2.qtyNeeded() - prod2.qty();

		if (firstProductDiff == secondProductDiff) {
			return 0; // Both products have the same difference
		}

		return (firstProductDiff > secondProductDiff) ? 1 : -1; // Greater returns 1, lesser returns -1
	}

	// This function searches for an item by SKU and returns its index.
	int AidMan::search(int sku) const {
		int i{};
		int foundIndex = -1;

		for (i = 0; i < m_numItems; i++) {
			if (*m_items[i] == sku) foundIndex = i;
		}

		return foundIndex;
	}

	// This function prompts the user to add a new item.
	void AidMan::add() {
		if (m_numItems >= sdds_max_num_items) {
			cout << "Database full!\n";
			return;
		}

		cout << "1- Perishable\n";
		cout << "2- Non-Perishable\n";
		cout << "-----------------\n";
		cout << "0- Exit\n";
		cout << "> ";
		int choice = ut.getint(0, 2, nullptr, nullptr);

		if (!choice) {
			cout << "Aborted\n";
			return;
		}

		iProduct* tmp = nullptr;
		if (choice == 1) {
			tmp = new Perishable;
		}
		else if (choice == 2) {
			tmp = new Item;
		}

		// Reading SKU and checking if it exists
		int sku = tmp->readSku(cin);
		if (search(sku) > 0) {
			cout << "Sku: " << sku << " is already in the system, try updating quantity instead.\n";
			delete tmp;
		}
		else {
			if (tmp) processItemAddition(*tmp);
		}
	}

	// This function removes an item from the array at a given index.
	void AidMan::remove(int index) {
		if (index < 0 || index >= m_numItems) return;

		delete m_items[index]; // Delete the item
		for (int i = index; i < m_numItems - 1; ++i) {
			m_items[i] = m_items[i + 1]; // Shift items to the left
		}
		m_items[m_numItems - 1] = nullptr; // Nullify the last item
		--m_numItems; // Decrease item count
	}

	// This function prompts the user to update the quantity of an item.
	void AidMan::update() {
		int index = select();
		if (index != -1) { // Check if a valid index is returned
			confirmUpdateQty(index);
		}
	}

	// This function allows the user to select an item from the list.
	int AidMan::select() {
		char desc[100];
		cout << "Item description: ";
		cin.ignore(1000, '\n'); // Clear the input buffer
		cin.getline(desc, 100, '\n');

		if (!list(desc)) {
			cout << "No matches found!\n";
			return -1; // Return -1 to indicate no match found
		}

		int sku = ut.getint(0, 99999, "Enter SKU: ", "Invalid SKU, try again: ");
		int index = search(sku);

		if (index == -1) {
			cout << "SKU not found!\n";
			return -1; // Return -1 to indicate SKU not found
		}

		return index; // Return the found index
	}

	// This function handles the case where the file fails to open, offering to create a new file.
	void AidMan::processFileOpenFailure() {
		int choice;
		cout << "Failed to open " << m_fileName << " for reading!\n"
			<< "Would you like to create a new data file?\n"
			<< "1- Yes!\n"
			<< "0- Exit\n> ";
		choice = ut.getint(0, 1, nullptr, nullptr);

		if (choice) { // 1
			ofstream ofstr(m_fileName);
		}
		else { // 0
			m_fileName = nullptr;
		}
	}

	// This function creates a shipping order file, saves eligible items, and removes them from the current inventory.
	void AidMan::processShipping() {
		int shipped{};
		ofstream ofstr("shippingOrder.txt");
		if (ofstr) {
			int i{};
			ofstr << "Shipping Order, Date: " << m_currDate << "\n";
			ut.printHeader(ofstr);
			ut.printBorder(ofstr);

			for (i = 0; i < m_numItems; i++) {
				if (m_items[i]->qty() == m_items[i]->qtyNeeded()) {
					ofstr << right << std::setfill(' ') << setw(4) << shipped + 1 << " | ";
					m_items[i]->linear(true);
					ofstr << *m_items[i] << "\n";
					remove(i);
					shipped++;
				}
			}

			ut.printBorder(ofstr);
			cout << "Shipping Order for " << shipped << " times saved!\n";
		}

	}

	// This function processes user menu selection and executes the corresponding action.
	void AidMan::processMenuSelection(size_t choice) {
		switch (choice) {
		case 1:
			displayListMenu();
			cout << "\n";
			break;
		case 2:
			add();
			cout << "\n";
			break;
		case 3:
			processItemRemoval();
			break;
		case 4:
			update();
			break;
		case 5:
			sort();
			break;
		case 6:
			processShipping();
			cout << "\n";
			break;
		case 7:
			load();
			cout << "\n";
			break;
		default: break;
		}
	}

	// This function adds a product to the inventory after validating its information.
	void AidMan::processItemAddition(iProduct& product) {
		cin >> product;
		if (product) {
			// Add product to array and increment count
			m_items[m_numItems++] = &product;
		}
		else {
			cout << product; // Display the invalid product
			delete& product;
		}
	}

	// This function initiates the item removal process based on user selection.
	void AidMan::processItemRemoval() {
		int index = select();
		if (index != -1) { // Check if a valid index is returned
			confirmRemoveItem(index);
		}
	}

	// This function updates the quantity of an item, allowing addition or reduction.
	void AidMan::confirmUpdateQty(int index) {
		int choice, qtyChange, maxQty, currentQty;
		cout << "1- Add\n"
			<< "2- Reduce\n"
			<< "0- Exit\n"
			<< "> ";
		choice = ut.getint(0, 2, nullptr, nullptr);

		switch (choice) {
		case 1: // Add
			maxQty = m_items[index]->qtyNeeded() - m_items[index]->qty();
			if (maxQty <= 0) {
				cout << "Quantity Needed already fulfilled!\n";
				break;
			}
			cout << "Quantity to add: ";
			qtyChange = ut.getint(1, maxQty, nullptr, nullptr);
			*m_items[index] += qtyChange;
			cout << qtyChange << " items added!" << endl;
			break;

		case 2: // Reduce
			currentQty = m_items[index]->qty();
			if (currentQty == 0) {
				cout << "Quantity on hand is zero!\n";
				break;
			}
			cout << "Quantity to reduce: ";
			qtyChange = ut.getint(1, currentQty, nullptr, nullptr);
			*m_items[index] -= qtyChange;
			cout << qtyChange << " items removed!" << endl;
			break;

		case 0: // Exit
			cout << "Aborted!\n";
			break;
		}
	}

	// This function confirms with the user before removing an item from the inventory.
	void AidMan::confirmRemoveItem(int index) {
		cout << "Following item will be removed:\n";
		m_items[index]->linear(false);
		m_items[index]->display(cout); // Display item details

		cout << "\n"
			"Are you sure?\n"
			"1- Yes!\n"
			"0- Exit\n> ";
		int choice = ut.getint(0, 1, nullptr, nullptr);
		if (choice) {
			remove(index);
			cout << "Item removed!" << endl;
		}
		else {
			cout << "Aborted!" << endl;
		}
	}
}