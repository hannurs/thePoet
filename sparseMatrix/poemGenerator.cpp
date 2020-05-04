#include <string>
#include <cstdlib>
#include <iostream>
#include <time.h>
#include "dictionary.cpp"

using namespace std;

class PoemGenerator {
public:
	int wordsInLine;
	int lines;
	Dictionary *dictionary;

	PoemGenerator(Dictionary *d) {
		srand(time(NULL));
		this->dictionary = d;
	}

	~PoemGenerator() {
		delete[] this;
	}

	// returns only the part of sparseMatrix
	sparseMatrix *shorterMatrix(int id) {
		Word *word = dictionary->searchForWordById(id);
		int length = word->numberOfFollowers;
		sparseMatrix *shorter = new sparseMatrix(length, length);
		sparseMatrix *originalMatrix = dictionary->matrix;
		double *row = originalMatrix->data[0];
		int i = 0;
		while (row) {
			if (row[0] == (double)id)		// find first row in matrix which data[0] == id
				break;
			i++;
			row = originalMatrix->data[i];
		}

		for (int j = i; j < i + length; j++) {
			shorter->data[j - i] = row;
			row = originalMatrix->data[j];
		}

		return shorter;
	}

	Word *randRhyme(Word *word) {
		int size = dictionary->countRhymes(word);
		Word *rhymes = dictionary->searchForRhymes(word);
		int randomPlaceInTable = 0;
		if (size > 1) {
			randomPlaceInTable = rand() % (size - 1);
			Word *rhyme = &rhymes[randomPlaceInTable];
			return rhyme;
		}
		else if (size == 1) return &rhymes[0];
		else return dictionary->getWord(rand() % dictionary->getNumberOfWords());
	}

	Word *randNextWord(Word *prevWord) {
		sparseMatrix *matrix = shorterMatrix(prevWord->wordIndex);
		int numberOfFollowers = prevWord->numberOfFollowers;
		Word *nextWord = nullptr;
		if (numberOfFollowers > 0) {
			double random = ((double)rand() / (RAND_MAX));
			double limit = 0;
			int idOfNextWord = NULL;

			for (int i = 0; i < numberOfFollowers; i++) {
				limit += matrix->data[i][2];
				if (random <= limit) {
					idOfNextWord = matrix->data[i][1];
					break;
				}
			}
			if (!idOfNextWord)
				idOfNextWord = matrix->data[numberOfFollowers - 1][1];

			nextWord = dictionary->searchForWordById(idOfNextWord);
			return nextWord;
		}
		else return dictionary->getWord(rand() % dictionary->getNumberOfWords());
		
	}

	Word *createLineToFile(Word *first, string fileName, bool odd, Word *wordToRhyme) {
		fstream file;
		file.open(fileName, ios::out | ios::app);
		Word *prevWord = first, *nextWord = first;

		if (file.good()) {
			file << first->word << " ";
			for (int i = 0; i < wordsInLine-1; i++) {
				if (i == wordsInLine - 2 && !odd)
					nextWord = randRhyme(wordToRhyme);
				else if (odd && i == wordsInLine - 2) {
					nextWord = randNextWord(prevWord);
					int chances = 50;
					while (nextWord->rhyme == "" && chances) {
						nextWord = randNextWord(prevWord);
						chances--;
					}
					wordToRhyme = nextWord;
				}
				else {
					nextWord = randNextWord(prevWord);
				}

				if (nextWord && nextWord->word != "") {
					file << nextWord->word << " ";
					prevWord = nextWord;
				}
				else  file << " ";
				
			}
			file << "\n";
			return prevWord;
		}
	}

	void createPoemToFile(int wordsInLine, int lines, string fileName) {
		bool odd = true;
		this->wordsInLine = wordsInLine;
		int i = rand() % dictionary->getNumberOfWords();
		Word *wordToRhyme = dictionary->getWord(i);
		wordToRhyme = createLineToFile(dictionary->getWord(i), fileName, odd, wordToRhyme);
		for (int i = 0; i < lines - 1; i++) {
			if (odd) odd = false;
			else odd = true;
			wordToRhyme = createLineToFile(randNextWord(wordToRhyme), fileName, odd, wordToRhyme);
		}
		fstream file;
		file.open(fileName, ios::out | ios::app);
		if (file.good() == true) file << '\n';
	}

};