#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <time.h>

using namespace std;

class Algorithm
{
	private:
	size_t _size;
	int** _matrix;
	int** _matrixDeijkstra;
	int* _weightPaths;
	vector<int> _path;

	public:

	Algorithm(string fileName)
	{
		ReadFromFile(fileName);
	}

	~Algorithm()
	{
		for(size_t i = 0; i < _size; i++)
		{
			delete[]_matrix[i];
			delete[] _matrixDeijkstra[i];
		}
		delete[]_matrix;
		delete[] _matrixDeijkstra;
		delete[] _weightPaths;
	}

	void ShowMinPath(int startVertex, int endVertex)
	{
		CheckNumberVertex(startVertex);
		CheckNumberVertex(endVertex);
		Solve(startVertex, endVertex);

		ShowMatrix(_matrixDeijkstra);
		cout << "All min paths from:\n";
		for(int i = 0; i < _size; i++)
		{
			if(i != startVertex - 1)
			{
				cout << startVertex << " => " << i + 1 << " = " << _weightPaths[i] << '\n';
			}
		}

		MakePath(startVertex, endVertex);
		cout << "Path: " << _weightPaths[endVertex - 1] << "\n";
		for(int i = _path.size() - 1; i >= 1; i--)
		{
			cout << _path[i] + 1 << " => ";
		}
		cout << _path[0] + 1 << '\n';
	}

	private:

	void MakePath(int startVertex, int endVertex)
	{
		int indexStartVertex = startVertex - 1;
		int indexEndVertex = endVertex - 1;

		int weight = _weightPaths[indexEndVertex];
		_path.push_back(indexEndVertex);

		vector<int> allVertexesFromEndVertex;
		int countLoop = 0;
		while(countLoop <= 999999)
		{
			for(int i = 0; i < _size; i++)
			{
				if(_matrix[i][_path.back()] != 0)
				{
					allVertexesFromEndVertex.push_back(i);
				}
			}

			int index;
			int weightFromMatrix;
			for(size_t i = 0; i < allVertexesFromEndVertex.size(); i++)
			{
				index = allVertexesFromEndVertex[i];
				weightFromMatrix = _matrix[_path.back()][index];
				if(weight - weightFromMatrix == 0)
				{
					weight -= weightFromMatrix;
					_path.push_back(index);
					break;
				}
				else if(weight - weightFromMatrix == _weightPaths[index])
				{
					weight -= weightFromMatrix;
					_path.push_back(index);
					break;
				}
			}

			if(weight == 0)
			{
				break;
			}
			allVertexesFromEndVertex.clear();
			countLoop++;
		}
		if(countLoop >= 999999)
		{
			cout << "Loop\n";
			exit(0);
		}
	}

	void Solve(int startVertex, int endVertex)
	{
		int indexStartVertex = startVertex - 1;
		int indexEndVertex = endVertex - 1;
		_weightPaths = new int[_size];
		_weightPaths[0] = startVertex;

		SetValue(_matrixDeijkstra);

		int countIter = 1;
		int minWeight = 0;
		int nameMinVertex = indexStartVertex;
		vector<int> oldPath;
		oldPath.push_back(indexStartVertex);

		int nextValue;
		int prevValue;
		int matr;
		while(oldPath.size() != _size)
		{
			for(int i = 0; i < _size; i++)
			{
				bool isContinue = false;
				for(size_t j = 0; j < oldPath.size(); j++)
				{
					if(i == oldPath[j])
					{
						isContinue = true;
						break;
					}
				}
				if(isContinue)
				{
					continue;
				}
				matr = _matrix[oldPath.back()][i];
				nextValue = minWeight + matr;
				prevValue = _matrixDeijkstra[countIter - 1][i];
				if(matr != 0)
				{
					_matrixDeijkstra[countIter][i] = (Compea(prevValue, nextValue)) ? nextValue : prevValue;
				}
				else
				{
					_matrixDeijkstra[countIter][i] = prevValue;
				}
			}
			nameMinVertex = GetIndexMin(_matrixDeijkstra[countIter]);

			minWeight = _matrixDeijkstra[countIter][nameMinVertex];
			oldPath.push_back(nameMinVertex);
			countIter++;
		}
		ShowMatrix(_matrixDeijkstra);
		int value;
		for(int i = 0; i < _size; i++)
		{
			for(int j = _size - 1; j >= 0; j--)
			{
				if(i != j)
				{
					value = _matrixDeijkstra[j][i];
					if(value == 0)
					{
						continue;
					}
					else
					{
						_weightPaths[i] = value;
						break;
					}
				}
			}
		}
	}

	void SetValue(int**& matrix, int value = 0)
	{
		for(size_t i = 0; i < _size; i++)
		{
			for(size_t j = 0; j < _size; j++)
			{
				matrix[i][j] = value;
			}
		}
	}

	bool Compea(int prev, int next, int infinity = 0)
	{
		if(prev == infinity && next == infinity)
		{
			return false;
		}

		if(next == infinity || prev == infinity)
		{
			return (next == infinity) ? false : true;
		}

		return (prev < next) ? false : true;
	}

	int GetIndexMin(int*& line)
	{
		int index = GetIndexNotNull(line);
		int min = line[index];
		int minIndex = index;
		for(size_t i = index + 1; i < _size; i++)
		{
			if(min > line[i] && line[i] != 0)
			{
				min = line[i];
				minIndex = i;
			}
		}
		return minIndex;
	}

	int GetIndexNotNull(int*& line)
	{
		for(size_t i = 0; i < _size; i++)
		{
			if(line[i] != 0)
			{
				return i;
			}
		}
		cout << "Not positiv number!\n";
		exit(0);
	}

	void CheckNumberVertex(int numberOfVertex)
	{
		if(IsCorrectNumberVertex(numberOfVertex) == false)
		{
			cout << "Check input number vertex!\n";
			exit(0);
		}
	}

	bool IsCorrectNumberVertex(int numberOfVertex)
	{
		return numberOfVertex >= 1 && numberOfVertex <= _size;
	}

	void ReadFromFile(string fileName)
	{
		ifstream in;
		in.open(fileName);
		if(in.is_open() == false)
		{
			cout << "Файл не найден.\n";
		}

		string line;
		getline(in, line);
		_size = stoi(line);
		MemorryAllocation();

		int size;
		int row = 0;

		string* numbers = new string[_size];
		while(getline(in, line, '\n'))
		{
			Split(numbers, line);
			for(size_t i = 0; i < _size; i++)
			{
				_matrix[row][i] = stoi(numbers[i]);
			}
			row++;
		}
		delete[] numbers;
		in.close();
	}

	void Split(string*& numbers, string line, char s = ' ')
	{
		size_t size = line.size();
		string temp = "";
		int index = 0;
		for(size_t i = 0; i < size; i++)
		{
			if(s == line[i])
			{
				numbers[index++] = temp;
				temp = "";
			}
			else
			{
				temp += line[i];
			}
		}
		numbers[index] = temp;
	}

	void ShowMatrix(int**& matr)
	{
		for(size_t i = 0; i < _size; i++)
		{
			for(size_t j = 0; j < _size; j++)
			{
				cout << matr[i][j] << '\t';
			}
			cout << '\n';
		}
	}

	void MemorryAllocation()
	{
		_matrix = new int* [_size];
		_matrixDeijkstra = new int* [_size];
		for(size_t i = 0; i < _size; i++)
		{
			_matrix[i] = new int[_size];
			_matrixDeijkstra[i] = new int[_size];
		}
	}
};

void GenegateMatrix(string fileName, int sizeMatrix, int maxValue = 999999)
{
	srand(time(0));
	ofstream  out;
	out.open(fileName);
	out << sizeMatrix << '\n';
	char space;
	int number;
	for(int i = 0; i < sizeMatrix; i++)
	{
		for(int j = 0; j < sizeMatrix; j++)
		{			
			number = (0 + rand() % maxValue);
			if(j != sizeMatrix - 1)
			{
				if(i != j)
				{
					out << number << ' ';
				}
				else
				{
					out << 0 << ' ';
				}
			}
			else
			{
				if(i != j)
				{
					out << number;
				}
				else
				{
					out << 0;
				}
			}
			
		}
		if(i != sizeMatrix - 1)
		{
			out << '\n';
		}
	}
	out.close();
}

int main(int argc, char* argv[])
{
	int size = 5;
	//string name = "rand.txt";
	//GenegateMatrix(name, size, 10);
	Algorithm algorithDeixtry("matrix.txt");
	//Algorithm algorithDeixtry(name);
	algorithDeixtry.ShowMinPath(1, 7);
	return 0;
}

//10
//0 79 0 0 0 0 0 38 62 0
//79 0 76 129 0 206 0 0 58 0
//0 76 0 61 0 0 170 0 102 0
//0 129 61 0 40 0 0 0 0 90
//0 0 0 40 0 63 113 0 0 97
//0 206 0 0 63 0 69 0 0 0
//0 0 170 0 113 69 0 72 0 0
//38 0 0 0 0 0 72 0 0 65
//62 58 102 0 0 0 0 0 0 25
//0 0 0 90 97 0 0 65 25 0