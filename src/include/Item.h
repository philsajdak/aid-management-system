#ifndef SDDS_ITEM_H
#define SDDS_ITEM_H

#include <iostream>

#include "iProduct.h"
#include "Status.h"

namespace sdds {
	const int DESC_LEN = 35; // length that the item description will be truncated to
	class Item : public iProduct {
		double m_price{};
		int m_qty{};
		int m_neededQty{};
		char* m_desc{};
		bool m_linear = false;
	protected:
		Status m_status{};
		int m_sku{};
		virtual bool linear() const { return m_linear; };
		char* shortDescription(char* dest, const char* src, size_t maxLen) const;
	public:
		Item() = default;
		Item(const Item& src);
		Item& operator=(const Item& src);
		virtual ~Item();
		virtual int qtyNeeded()const;
		virtual int qty()const;
		virtual const char* description() const;
		virtual void linear(bool isLinear);
		virtual operator double()const;
		operator bool()const;
		virtual int operator-=(int qty);
		virtual int operator+=(int qty);
		virtual bool operator==(int sku)const;
		virtual bool operator==(const char* description)const;
		virtual std::ofstream& save(std::ofstream& ofstr)const;
		virtual std::ifstream& load(std::ifstream& ifstr);
		virtual std::ostream& display(std::ostream& ostr)const;
		virtual int readSku(std::istream& istr);
		virtual std::istream& read(std::istream& istr);
		void clear();
	};
}
#endif