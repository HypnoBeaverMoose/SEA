
#include <iostream>
#include <string>

#include "tinyxml2.h"

using namespace tinyxml2;

int main( int argc, char* argv[] )
{
	std::cout << "Hello TinyXML2 world!" << std::endl;

	XMLDocument doc;
	if ( doc.LoadFile("database.xml") == XML_SUCCESS )
	{
		std::cout << "database.xml successfully loaded." << std::endl;
		std::cout << "doc.NoChildren: " << doc.NoChildren() << std::endl;
		std::string text = doc.FirstChildElement( "database" )->GetText();
		std::cout << "childText: " << text << std::endl;


	} else
	{
		std::cout << "database.xml failed to load!" << std::endl;
	}

	system("Pause");

	return 0;
}