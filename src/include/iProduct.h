#ifndef SDDS_IPRODUCT_H
#define SDDS_IPRODUCT_H

#include <iostream>

namespace sdds {
	class iProduct {
	public:
		virtual ~iProduct() = default;
		virtual int readSku(std::istream& istr) = 0; // to read the Stock-Keeping Unit from the console before main data entry
		virtual int qtyNeeded()const = 0; // returns the number of products needed
		virtual int qty()const = 0; // returns the quantity on hand
		virtual const char* description() const = 0; // returns the desc
		virtual void linear(bool isLinear) = 0; // determines if the iProduct is displayed in a linear format or descriptive format
		virtual std::ofstream& save(std::ofstream& ofstr)const = 0; // saves the iProduct into a file
		virtual std::ifstream& load(std::ifstream& ifstr) = 0; // loads an iProduct from a file
		virtual std::ostream& display(std::ostream& ostr)const = 0; // displays the iProduct on the screen
		virtual std::istream& read(std::istream& istr) = 0; // reads the iProduct from the console
		virtual int operator-=(int qty) = 0; // to reduce the quantity on hand
		virtual int operator+=(int qty) = 0; // to increase the quantity on hand
		virtual operator double()const = 0; // returns the price of the produce
		virtual operator bool()const = 0; // returns if the iProduct is in a good state
		virtual bool operator==(int sku)const = 0; // return true if the SKU is a match to the iProduct's SKU
		virtual bool operator==(const char* description)const = 0; // returns true if the description is found in the iPorduct's description
	};

	std::ostream& operator<<(std::ostream& ostr, const sdds::iProduct& iprod);
	std::istream& operator>>(std::istream& istr, sdds::iProduct& iprod);
}
#endif