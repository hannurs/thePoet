#include <iostream>
#include <string>
#include <fstream>
#include <Windows.h>
//#include "dictionary.cpp"
#include "poemGenerator.cpp"

#include <time.h>

using namespace std;

//write words on the screen
void writeWord(string word) {
	for (int i = 0; i < word.length(); i++) {
		if (word[i] >= '!' && word[i] <= 'z') {
			cout << word[i];
		}
		else cout << ' ';
	}
	cout << " ";
}

//deletes all special characters from word (aside from ')
void deleteSpecialCharacters(string& word) {
	char notCharacters[] = { ',', '.', '?', ':', '!', ';', '"', ']', '[', '(', ')', '*' };
	for (int i = 0; i < sizeof(notCharacters) / sizeof(char); i++) {
		for (int j = 0; j < word.length(); j++) {
			if (word[j] == '¹' || word[j] == '¥' || word[j] == 'æ' || word[j] == 'Æ'
				|| word[j] == 'ê' || word[j] == 'Ê' || word[j] == '³' || word[j] == '£'
				|| word[j] == 'ñ' || word[j] == 'Ñ' || word[j] == 'œ' || word[j] == 'Œ'
				|| word[j] == 'ó' || word[j] == 'Ó' || word[j] == 'Ÿ' || word[j] == ''
				|| word[j] == '¿' || word[j] == '¯') continue;
			else if (word[j] == notCharacters[i]) {
				word=word.erase(j, 1);
				j--;
			}
			else if (!((word[j] >= 'A' && word[j] <= 'Z') || (word[j] >= 'a' && word[j] <= 'z'))) {
				word=word.erase(j, 1);
				j--;
			}

		}
	}
}

//is the special character 
bool isASpecialCharacter(char sign) {
	char notCharacters[] = { ',', '.', '?', ':', '!', ';', '"', ']', '[', '(', ')', '-', '\'', '*' };
	for (int i = 0; i < sizeof(notCharacters) / sizeof(char); i++) {
		if (sign == notCharacters[i]) return true;
		
	}
	if (!((sign >= 'A' && sign <= 'Z') || (sign >= 'a' && sign <= 'z'))) return true;
	return false;
}

bool doesTheWordConsistOnlySpecialCharacters(string word) {
	for (int i = 0; i < word.length(); i++) {
		if (!isASpecialCharacter(word[i]))
			return false;
	}
	return true;
}

void changeBigLettersToSmall(string& word) {
	if (word == "I") return;
	for (int i = 0; i < word.length(); i++) {
		if (word[i] >= 'A' && word[i] <= 'Z')
			word.at(i) += 'a' - 'A';
	}
}

//prepares a word before adding it to the dictionary (changing the size of lettes, deleting glued
// dots, comas ect)
void prepareWord(string& word) {
	deleteSpecialCharacters(word);
	while (word[0] == '\'' || word[0] == '-')
		word.erase(0, 1);
	while (word[word.length() - 1] == '\'' || word[word.length() - 1] == '-')
		word.erase(word.length() - 1, 1);
	changeBigLettersToSmall(word);
}


//loads words to the dictionary
void load(string fileName, Dictionary* d) {
	string word1, word2;
	ifstream poem;
	poem.open(fileName, ios::in);
	if (!poem) {
		cerr << "Failed to open the file\n";
	}


	while (poem >> word1) {
		if (!doesTheWordConsistOnlySpecialCharacters(word1))
			break;
	}

	while (poem >> word2) {
		if (!doesTheWordConsistOnlySpecialCharacters(word2)) {
			//writeWord(word1); //PROBLEM WITH WRITING POLISH CHARACTERS PLS SEND HELP
			prepareWord(word1);
			prepareWord(word2);
			
			d->addWordToDictionary(word1, word2);
			word1 = word2;
		}
	}
}

int main() {
	Dictionary* dictionary = new Dictionary();
	load("wiersze_polskie.txt", dictionary);
	dictionary->init();		// creates sparse matrix and writes dictionary to a file
	PoemGenerator *poemMachine = new PoemGenerator(dictionary);
	poemMachine->createPoemToFile(6,6,"wiersze_barda1.txt");
	poemMachine->createPoemToFile(3, 6, "wiersze_barda.txt");
	poemMachine->createPoemToFile(7, 3, "wiersze_barda.txt");
	poemMachine->createPoemToFile(4, 4, "wiersze_barda.txt");
	poemMachine->createPoemToFile(8, 5, "wiersze_barda.txt");
	poemMachine->createPoemToFile(2, 4, "wiersze_barda.txt");
	poemMachine->createPoemToFile(12, 4, "wiersze_barda.txt");

	
	return 0;
}