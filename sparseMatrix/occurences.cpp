#include <string>
using namespace std;

class Occurences {
public:
	string word;
	int wordIndex;
	Occurences* next;
	int nrOfOccurences;

	Occurences() {
		this->word = "null";
		this->next = nullptr;
		this->nrOfOccurences = 1;
	}
	Occurences(string word,int index) {
		this->word = word;
		this->wordIndex = index;
		this->next = nullptr;
		this->nrOfOccurences = 1;
	}

	~Occurences() {
		delete[] this;
	}

	void setNrOfOccurences(int number) {
		this->nrOfOccurences = number;
	}
};