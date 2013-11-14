#pragma once

#include <exception>
#include <string>
#include <sstream>

class NotSupportedCommandException : public std::exception { // ToDo why 2 files with exeptions
private:
	

public:
	std::string command;
	NotSupportedCommandException(std::string command) :
		command(command) {}

	virtual const char* what() const throw() {
		std::stringstream stream;
		stream << "Can't execute command " << command;
		return stream.str().c_str();
	}
};