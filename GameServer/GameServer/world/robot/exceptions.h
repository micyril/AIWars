#pragma once

#include <exception>
#include <string>
#include <sstream>

class NotSupportedCommandException : public std::exception { // ToDo why 2 files with exeptions
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


class NotDefinedTaskException : public std::exception {
private:
	std::string task;

public:
	NotDefinedTaskException(std::string task) :
		task(task) {}

	virtual const char* what() const throw() {
		std::stringstream stream;
		stream << "Can't perform task " << task;
		return stream.str().c_str();
	}
};