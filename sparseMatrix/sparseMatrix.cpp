#include <string>
#include <cstdlib>
#include <iostream>
using namespace std;

class sparseMatrix {
public:
	//y x value
	double ** data;
	//matrix size
	int size;
	//total number of elements in matrix 
	int len;
	//max number of elements
	int maxLen;

	sparseMatrix(int size,int numberOfElements) {
		this->maxLen = numberOfElements;
		this->size = size;
		// intialize length to 0
		this->len = 0;
		data = new double*[maxLen];
		for (int i = 0; i < maxLen; i++) {
			data[i] = new double[3];
		}
	}

	// insert elements into sparse matrix 
	void insert(int r, int c, double val) {
			// insert row value 
			data[len][0] = r;
			// insert col value 
			data[len][1] = c;
			// insert element's value 
			data[len][2] = val;
			// increment number of data in matrix 
			len++;
	}
	int find(int y, int x) {
		for (int i = 0; i < this->maxLen; i++) {
			if (data[i][0] == y && data[i][1]==x) {
				return i;
			}
			else if (data[i][0]>y ) return -1;
		}
		return -1;
	}

	void multiply(sparseMatrix* b) {
		sparseMatrix *ret = new sparseMatrix(size, maxLen*2);
		double sum = 0;
		int v1 = 0;
		int v2 = 0;
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				for (int k = 0; k < size; k++) {
					v1 = find(i, k);
					v2 = find(k, j);
					if (v1 != -1 && v2 != -1) {
						sum+=data[v1][2] * data[v2][2];
					}
				}
				if (sum != 0) {
					ret->insert(i, j, sum);
					sum = 0;
				}
			}
		}
		//ret->print();
	}

	// printing matrix 
	void print()
	{
		cout << "Dimension: " << size << "x" << size << endl;
		cout << "Sparse Matrix: " << endl << "Row Column Value" << endl;

		for (int i = 0; i < len; i++) {
			cout << data[i][0] << " " << data[i][1] << " " << data[i][2] << endl;
		}
	}
};