#include <sstream>
#include "mapelement.h"

using namespace std;

std::string MapElement::Serialize() {
	stringstream stream;
	stream << "{"									  << endl <<
				"\"width\" : "		<< this->width    << endl << 
				"\"height\" : "		<< this->height   << endl << 
				"\"x\" : "			<< this->x		  << endl <<
				"\"y\" : "			<< this->y		  << endl <<
				"\"rotation\" : "	<< this->rotation << endl <<
				"\"layer\" : "		<< this->layer    << endl <<
			  "}";
	return stream.str();
}