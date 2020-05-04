#include <string>
#include <fstream>
#include "word.cpp"
#include "sparseMatrix.cpp"
#include <iostream>
using namespace std;

class Dictionary {
private:
	int numberOfWords;	//current count of words in the dictionary
	Word * words;		//array with words
public:
	sparseMatrix * matrix;

	Dictionary() {
		this->numberOfWords = 0;
		this->words = nullptr;
		this->matrix = nullptr;
	}

	~Dictionary() {
	}

	void init() {
		//load(fileName, this);
		write2file();
		matrix = createTransitionMatrix();
	}

	int countRhymes(Word *word) {
		string ending = word->rhyme;
		Word* tmp = this->words;
		int numberOfRhymes = 0;
		while (tmp != nullptr) {
			if (ending != "" && tmp->rhyme == ending && tmp->word != word->word)
				numberOfRhymes++;
			tmp = tmp->next;
		}

		return numberOfRhymes;
	}

	Word *searchForRhymes(Word *word) {
		string ending = word->rhyme;
		Word* tmp = this->words;
		int size = countRhymes(word);
		Word *rhymes = new Word[size];
		int index = 0;
		while (tmp != nullptr) {
			if (ending != "" && tmp->rhyme == ending && tmp->word != word->word) {
				rhymes[index] = *tmp;
				index++;
			}
			tmp = tmp->next;
		}

		return rhymes;
	}


	//dodawanie slowa do slownika
	Word* addWord(string word) {
		Word * tmpbefor = nullptr;
		Word * tmp = words;
		while (tmp != nullptr) {
			//jesli znaleziono wyraz to nie trzeba go dodawac
			if (word == tmp->word) return tmp;
			tmpbefor = tmp;
			tmp = tmp->next;
		}
		//jesli nie znaleziono
		//jezeli to pierwszy wyraz Obiektu Dictionary
		if (tmpbefor == nullptr) {
			this->words = new Word(word,numberOfWords);
			this->numberOfWords++;
			return words;
		}
		//jezeli nie pierwszy
		else {
			tmpbefor->next = new Word(word,numberOfWords);
			this->numberOfWords++;
			return tmpbefor->next;
		}

	}


	Word *searchForWordById(int id) {
		Word* tmp = this->words;
		while (tmp != nullptr) {
			if (tmp->wordIndex == id)
				return tmp;
			tmp = tmp->next;
		}

		return NULL;
	}

	Word *searchForWord(string word) {
		Word* tmp = this->words;
		while (tmp != nullptr) {
			if (tmp->word == word)
				return tmp;
			tmp = tmp->next;
		}

		return NULL;
	}

	Word* getWord(int indeks) {
		Word* tmp = this->words;
		for (int i = 0; i < indeks; i++) {
			tmp = tmp->next;
		}
		return tmp;
	}

	void write() {
		cout << "Number of words in the directory: " << this->numberOfWords << endl;
		Word*tmp = this->words;
		int i = 1;
		while (tmp != nullptr) {
			cout << i << ". " << tmp->word << "\t\t\t ";
			Occurences* tmpO = tmp->listOfFollowers;
			while (tmpO != nullptr) {
				cout << tmpO->word << "(" << tmpO->nrOfOccurences << ")-> ";
				tmpO = tmpO->next;
			}
			tmp = tmp->next;
			cout << endl;
			i++;
		}
	}

	void write2file(){
	string linia;
	fstream plik;

	plik.open("plik.txt", ios::out | ios::app);
	if (plik.good() == true)
	{
		plik << "Number of words in the directory: " << this->numberOfWords << endl;
		Word*tmp = this->words;
		int i = 1;
		while (tmp != nullptr) {
			plik << i << ". " << tmp->word <<":"<<tmp->wordIndex<< "\t\t\t ";
			Occurences* tmpO = tmp->listOfFollowers;
			while (tmpO != nullptr) {
				plik << tmpO->word << ":" << tmpO->wordIndex << "(" << tmpO->nrOfOccurences << ")-> ";
				tmpO = tmpO->next;
			}
			tmp = tmp->next;
			plik << endl;
			i++;
		}
		plik.close();
	}
	
	}

	//chwilowo, do zastapienia zwraca true 
	bool prepareWord(string* word) { return true; }

	int getNumberOfWords() {
		return this->numberOfWords;
	}

	//dodawanie slowa z nastepnikiem do slownika
	void addWordToDictionary(string word, string word2) {
		if (word.length() != 0) {
			Word* w = this->addWord(word);
			if (word2.length() != 0) {
				Word* w2 = this->addWord(word2);
				w->addFollower(word2, w2->wordIndex);
			}
		}
	}
	sparseMatrix* createTransitionMatrix() {
		Word*tmp = this->words;
		sparseMatrix * matrix = new sparseMatrix(numberOfWords, 50*numberOfWords);
		while (tmp != nullptr) {
			Occurences*tmpO = tmp->listOfFollowers;
			while(tmpO!=nullptr){
				matrix->insert(tmp->wordIndex, tmpO->wordIndex,tmpO->nrOfOccurences/(double)tmp->numberOfFollowers);
				tmpO = tmpO->next;
			}
			tmp=tmp->next;
		}
		//matrix->multiply(matrix);
		//matrix->print();

		return matrix;
	}

	void serialize(string fileName) {
		ofstream dict;
		dict.open(fileName);

		if (dict.fail()) {
			cerr << "Failed";
			exit(1);
		}

		dict << this->numberOfWords << "\n";

		Word* tmp = this->words;
		Occurences* tm;
		while (tmp != nullptr) {
			dict << tmp->word << " ";
			dict << tmp->numberOfFollowers << " ";
			tm = tmp->listOfFollowers;
			while (tm != nullptr) {
				dict << tm->word << " ";
				dict << tm->nrOfOccurences << " ";
				tm = tm->next;
			}
			dict << "\n";
			tmp = tmp->next;
		}
		dict.close();
	}

	int toInt(string number) {
		int x = 0;
		for (int i = (number.length() - 1), m = 0; i >= 0; i--, m++) {
			x += (number[i] - '0')*pow(10, m);
		}
		return x;
	}

	Word* nextWord(Word* tmp, string name, string number) {
		int x = this->toInt(number);
		if (this->words == nullptr) {
			this->words = new Word(name, 1);
			tmp = this->words;
		}
		else {
			tmp->next = new Word(name, 1);
			tmp = tmp->next;
		}
		tmp->setNumberOfFollowers(x);
		return tmp;
	}

	Occurences* addFollowers(Word* tmp, Occurences* tm, string name, string number) {
		if (tmp->listOfFollowers == nullptr) {
			tmp->listOfFollowers = new Occurences(name, 1);
			tm = tmp->listOfFollowers;
		}
		else {
			tm->next = new Occurences(name, 1);
			tm = tm->next;
		}
		tm->setNrOfOccurences(this->toInt(number));
		return tm;
	}

	void deserialize(string fileName) {
		ifstream dict;
		dict.open(fileName);

		if (dict.fail()) {
			cerr << "Failed";
			exit(1);
		}
		Word* tmp = this->words;
		Occurences *tm = nullptr;
		string value;
		string number;
		int numberOfFoll;

		dict >> number;
		this->numberOfWords = this->toInt(number);

		for (int i = 0; i < this->numberOfWords; i++) {
			dict >> value;
			dict >> number;
			tmp = nextWord(tmp, value, number);

			for (int i = 0; i < toInt(number); i++) {
				dict >> value;
				dict >> number;
				tm = addFollowers(tmp, tm, value, number);
			}
		}
	}
};
