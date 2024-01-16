#ifndef SDDS_STATUS_H
#define SDDS_STATUS_H

#include <iostream>
#include "Utils.h"

namespace sdds {
	class Status {
		char* m_statusDesc{};
		int m_statusCode{};

	public:
		Status();
		Status(const char* desc);
		Status(const Status& src);
		Status& operator=(const int code);
		Status& operator=(const char* desc);
		Status& operator=(const Status& src);
		~Status();
		operator int() const;
		operator const char* () const;
		operator bool() const;
		Status& clear();
	};

	std::ostream& operator<<(std::ostream& os, const Status& status);
}
#endif