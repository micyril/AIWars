#pragma once

#include <exception>
#include <string>
#include <sstream>

class NotSupportedCommandException : public std::exception {
private:
	std::string command;

public:
	NotSupportedCommandException(std::string command) :
		command(command) {}

	virtual const char* what() const throw() {
		std::stringstream stream;
		stream << "Can't execute command " << command;
		return stream.str().c_str();
	}
};