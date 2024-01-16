#ifndef SDDS_PERISHABLE_H
#define SDDS_PERISHABLE_H

#include "Item.h"
#include "Date.h"

namespace sdds {
	class Perishable : public Item {
		Date m_expiry{};
		char* m_instructions{};
	protected:
		void setDateFormat(bool format) { m_expiry.formatted(format); }
	public:
		Perishable() = default;
		Perishable(const Perishable& src);
		Perishable& operator=(const Perishable& src);
		virtual ~Perishable();
		const Date& expiry() const;
		int readSku(std::istream& istr);
		std::ofstream& save(std::ofstream& ofstr) const override;
		std::ifstream& load(std::ifstream& ifstr);
		std::ostream& display(std::ostream& ostr)const;
		std::istream& read(std::istream& istr);
	};
}
#endif