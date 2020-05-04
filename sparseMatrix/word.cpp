#include <string>
#include "occurences.cpp"


using namespace std;
class Word {
public:

	string word;//wyraz
	string rhyme;
	int wordIndex;
	Word * next;//wskaznik na kolejne
	Occurences* listOfFollowers;//wskaznik na pierwsze slowo
	int numberOfFollowers;	//liczba followerow po danym slowie

	Word() {
		this->word = "default";
		this->rhyme = "aaa";
		this->numberOfFollowers = 0;
		this->next = nullptr;
		listOfFollowers = nullptr;
	}

	Word(string word, int index) {
		this->word = word;
		this->rhyme = cutTheEnding(word);
		this->wordIndex = index;
		this->numberOfFollowers = 0;
		this->next = nullptr;
		listOfFollowers = nullptr;
	}

	~Word() {}

	string reverseString(string word) {
		int size = word.length();
		for (int i = 0; i < size / 2; i++)
			swap(word[i], word[size - i - 1]);

		return word;
	}

	string cutTheEnding(string word) {
		string ending = "";
		if (word.length() >= 3) {
			int sizeOfRhyme = 3;
			for (int i = 0; i < sizeOfRhyme; i++) {
				ending.push_back(word.at(word.length() - 1 - i));
			}
			ending = reverseString(ending);
			//cout << ending << endl;
		}

		return ending;
	}

	void setNumberOfFollowers(int number) {
		this->numberOfFollowers = number;
	}

	void addFollower(string word,int index) {
		Occurences *tmpbefor = nullptr;
		Occurences *tmp = this->listOfFollowers;
		while (tmp != nullptr) {
			//jesli znaleziono wyraz
			if (tmp->word == word) {
				tmp->nrOfOccurences++;//zwiekszamy ilosc wystapien wyrazu nastepujacego
				this->numberOfFollowers++;//zwiekszamy ilosc s³ow nastepujacych po slowie
				return;
			}
			tmpbefor = tmp;
			tmp = tmp->next;
		}
		//jesli nie znaleziono
		//jezeli to pierwszy wyraz nastepujacy dla tego slowa to tworzymy Obiekt Occurences jako poczatek listy slow
		if (tmpbefor == nullptr) {
			this->listOfFollowers = new Occurences(word,index);
			this->numberOfFollowers++;
		}
		//jezeli nie pierwsze dopisujemy na koniec listy
		else {
			tmpbefor->next = new Occurences(word,index);
			this->numberOfFollowers++;
		}

	}
};