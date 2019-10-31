#pragma once
#include <fstream>
#include <vector>
#include <iterator>

#include "global.h"

class file : public std::vector <u8> {
	std::fstream _Fstream;

	inline bool openFile(std::string _Fname) {
		this->_Fstream.open(
			_Fname,
			std::ios::in | std::ios::ate | std::ios::binary
		);
		if (this->_Fstream.is_open()) {
			_log(i, "Opened '" << _Fname << "' ");
			return true;
		}
		else {
			_log(i, "Couldn't open '" << _Fname << "'");
			return false;
		}
	}

	inline void loadFile() {
		size_t length = _Fstream.tellg();
		this->resize(length);
		_Fstream.seekg(0, std::ios::beg);
		_Fstream.read((char*)this->data(), length);
		_log(i, "Loaded into memory")
	}

public:
	file(std::string _Fname) {
		if (this->openFile(_Fname)) this->loadFile();
	}

	file(void) {}
};
