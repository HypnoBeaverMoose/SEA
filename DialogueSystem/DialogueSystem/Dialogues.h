#include <map>
#include <string>

class Dialogues
{
	public:
		struct studentType 
		{
			std::string text;
			enum Speaker { None, Student, Farmer, Scientist } ;
			Speaker speaker;
		};
		std::multimap<std::string, studentType> dialogues;
		Dialogues();
		
	
	private:
	void insertDialogue(std::string tag, std::string _text, studentType::Speaker _speaker);
};