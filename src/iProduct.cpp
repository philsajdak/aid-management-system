#include "iProduct.h"

namespace sdds {
	std::ostream& operator<<(std::ostream& ostr, const sdds::iProduct& iprod)
	{
		iprod.display(ostr);
		return ostr;
	}

	std::istream& operator>>(std::istream& istr, sdds::iProduct& iprod)
	{
		iprod.read(istr);
		return istr;
	}
}
