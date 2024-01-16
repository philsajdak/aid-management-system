#include "Utils.h"
#include "Status.h"

using namespace std;
using namespace sdds;

namespace sdds {
	Status::Status() = default;

	Status::Status(const char* desc) {
		this->clear();
		if (desc && desc[0])
			ut.alocpy(m_statusDesc, desc);
	}

	Status::Status(const Status& src) {
		*this = src;
	}

	Status& Status::operator=(const int code) {
		if (code)
			this->m_statusCode = code;
		return *this;
	}

	Status& Status::operator=(const char* desc) {
		if (desc && desc[0]) {
			delete[] m_statusDesc;
			ut.alocpy(m_statusDesc, desc);
		}
		return *this;
	}

	Status& Status::operator=(const Status& src) {
		if (this != &src) {
			this->clear();
			if (!src) {
				ut.alocpy(m_statusDesc, src.m_statusDesc);
				this->m_statusCode = src;
			}
		}
		return *this;
	}

	Status::~Status() {
		this->clear();
	}

	Status::operator int() const {
		return m_statusCode;
	}

	Status::operator const char* () const {
		return m_statusDesc ? m_statusDesc : nullptr;
	}

	Status::operator bool() const {
		return m_statusDesc == nullptr;
	}

	Status& Status::clear() {
		delete[] this->m_statusDesc;
		this->m_statusDesc = nullptr;
		this->m_statusCode = 0;
		return *this;
	}

	std::ostream& operator<<(std::ostream& os, const Status& status) {
		if (!status) {
			if ((int)status) os << "ERR#" << (int)status << ": ";
			os << (const char*)status;
		}
		return os;
	}
}