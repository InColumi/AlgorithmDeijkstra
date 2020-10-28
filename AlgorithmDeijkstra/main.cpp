#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

using namespace std;

class Algorithm
{
	private:
	vector<vector<int>> _matrix;
	vector<int> _weightPaths;
	vector<int> _path;
	int _size;

	public:

	Algorithm(string fileName)
	{
		ReadFromFile(fileName);
		ShowInfo(_matrix);
	}

	void ShowMinPath(int startVertex, int endVertex)
	{
		CheckNumberVertex(startVertex);
		CheckNumberVertex(endVertex);
		Solve(startVertex, endVertex);

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
		_weightPaths.clear();
		_path.clear();
	}

	private:

	void MakePath(int startVertex, int endVertex)
	{
		int indexStartVertex = startVertex - 1;
		int indexEndVertex = endVertex - 1;

		int weight = _weightPaths[indexEndVertex];
		_path.push_back(indexEndVertex);

		vector<int> allVertexesFromEndVertex;
		while(true)
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
				if(weight - weightFromMatrix == _weightPaths[index])
				{
					weight -= weightFromMatrix;
					_path.push_back(index);
				}
			}
			index = _path.back();
			if(_matrix[index][indexStartVertex] == _weightPaths[index])
			{
				break;
			}
			allVertexesFromEndVertex.clear();
		}

	}

	void Solve(int startVertex, int endVertex)
	{
		int indexStartVertex = startVertex - 1;
		int indexEndVertex = endVertex - 1;
		_weightPaths = vector<int>(_size);
		_weightPaths[0] = startVertex;

		vector<vector<int>> matrixDeijkstra;
		for(int i = 0; i < _size; i++)
		{
			matrixDeijkstra.push_back(vector<int>(_size));
		}

		int countIter = 1;
		int minWeight = 0;
		int nameMinVertex = indexStartVertex;
		vector<int> oldPath;
		oldPath.push_back(indexStartVertex);

		int nextValue;
		int prevValue;
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
				int matr = _matrix[oldPath.back()][i];
				nextValue = minWeight + matr;
				prevValue = matrixDeijkstra[countIter - 1][i];
				if(matr != 0)
				{
					matrixDeijkstra[countIter][i] = (Compea(prevValue, nextValue)) ? nextValue : prevValue;
				}
				else
				{
					matrixDeijkstra[countIter][i] = prevValue;
				}
			}


			nameMinVertex = GetIndexMin(matrixDeijkstra[countIter]);
			minWeight = matrixDeijkstra[countIter][nameMinVertex];
			oldPath.push_back(nameMinVertex);
			countIter++;
		}

		int value;
		for(int i = 0; i < _size; i++)
		{
			for(int j = _size - 1; j >= 0; j--)
			{
				if(i != j)
				{
					value = matrixDeijkstra[j][i];
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

	void SetMatrix(int**& matr, int value = 0)
	{
		for(int i = 0; i < _size; i++)
		{
			for(int j = 0; j < _size; j++)
			{
				matr[i][j] = value;
			}
		}
	}

	bool Compea(int prev, int next)
	{
		if(prev == 0 && next == 0)
		{
			return false;
		}

		if(next == 0 || prev == 0)
		{
			return (next == 0) ? false : true;
		}

		return (prev < next) ? false : true;
	}

	int GetIndexMin(vector<int> v)
	{
		int index = GetIndexNotNull(v);
		int min = v[index];
		int minIndex = index;
		for(size_t i = index + 1; i < v.size(); i++)
		{
			if(min > v[i] && v[i] != 0)
			{
				min = v[i];
				minIndex = i;
			}
		}
		return minIndex;
	}

	int GetIndexNotNull(vector<int> v)
	{
		for(size_t i = 0; i < v.size(); i++)
		{
			if(v[i] != 0)
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

		while(getline(in, line))
		{
			vector<int> row;
			for(size_t i = 0; i < line.size(); i++)
			{
				size_t endNumbmer = line.find(' ');
				if(endNumbmer == -1)
				{
					endNumbmer = line.size();
				}
				string s;
				for(size_t j = 0; j < endNumbmer; j++)
				{
					s.push_back(line[j]);
				}
				row.push_back(stoi(s));
				string newLine;
				for(size_t j = endNumbmer + 1; j < line.size(); j++)
				{
					newLine.push_back(line[j]);
				}
				line = newLine;
				if(line.size() == 1)
				{
					row.push_back(stoi(line));
				}
				i = 0;
			}
			_matrix.push_back(row);
		}
		in.close();
	}

	void ShowInfo(vector<vector<int>> matr)
	{
		for(int i = 0; i < _size; i++)
		{
			for(int j = 0; j < _size; j++)
			{
				cout << matr[i][j] << "\t";
			}
			cout << '\n';
		}
		cout << '\n';
	}
};

int main()
{
	Algorithm algorithDeixtry("matrix.txt");
	algorithDeixtry.ShowMinPath(7, 5);

	for(int i = 1; i <= 10; i++)
	{
		for(int j = 1; j <= 10; j++)
		{
			if(i != j)
			{
				algorithDeixtry.ShowMinPath(i, j);
			}
		}
	}
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