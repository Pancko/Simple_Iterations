#pragma once
using namespace std;
#include <iostream>
#include <fstream>
#include <time.h>
#include <cstdlib>
#include <random>
#include <iomanip>
#include <string>

void Diagonal(double** matrixA, double* matrixB, int size);
double* Iteration(double** matrixA, double* matrixB, int k, int size);

int main()
{
	double Epsilon = 0.00001;
	double Nevyazka = 5.;
	int abg = 0;
	int tries = 0;
	//cout << "Epsilon = ";
	//cin >> Epsilon;
	ifstream f("Input.txt");

	char CurrentChar;
	int elements_counter = 0;
	bool elements_counted = 0;

	while (!f.eof())
	{
		f.get(CurrentChar);
		if (CurrentChar == ' ')
		{
			elements_counter++;
		}
		if (CurrentChar == '\n')
		{
			break;
		}
	}
	f.clear();
	f.seekg(0);

	double** matrixA = new double* [elements_counter];
	double** Original_matrix = new double* [elements_counter];
	for (int i = 0; i < elements_counter; i++)
	{
		matrixA[i] = new double[elements_counter];
		Original_matrix[i] = new double[elements_counter];
	}

	double* matrixB = new double[elements_counter];

	elements_counter--;
	int current_element = 0;
	int current_row = 0;

	// Считывание элементов матрицы из файла
	while (!f.eof())
	{
		f >> matrixA[current_row][current_element];
		cout << setw(5) << matrixA[current_row][current_element] << " ";
		if (current_element == elements_counter)
		{
			f >> matrixB[current_row];
			cout << setw(5) << matrixB[current_row];
			current_element = 0;
			current_row++;
			cout << endl;
		}
		else
		{
			current_element++;
		}

	}
	for (int i = 0; i < elements_counter + 1; i++)
	{
		for (int j = 0; j < elements_counter + 1; j++)
		{
			Original_matrix[i][j] = matrixA[i][j];
		}
	}

	while (abs(Nevyazka) > 1)//(Epsilon + (Epsilon * 100)))
	{
		tries++;
		system("CLS");
		for (int i = 0; i < elements_counter + 1; i++)
		{
			for (int j = 0; j < elements_counter + 1; j++)
			{
				matrixA[i][j] = Original_matrix[i][j];
				cout << setw(10) << matrixA[i][j] << " ";
			}
			cout << setw(10) << matrixB[i] << endl;
		}
		// Приведение к диагональному преобладанию
		Diagonal(matrixA, matrixB, elements_counter + 1);

		for (int i = 0; i < elements_counter + 1; i++)
		{
			for (int j = 0; j < elements_counter + 1; j++)
			{
				cout << setw(10) << matrixA[i][j] << " ";
			}
			cout << fixed << setw(10) << matrixB[i] / matrixA[i][i] << endl;
		}
		cout << endl;

		// Вычисление коэффициентов альфа, бета, гамма
		double alpha = -1;
		double beta = -1;
		double gamma = 0;
		double SumA = 0;
		double SumB = 0;
		for (int i = 0; i < elements_counter + 1; i++)
		{
			for (int j = 0; j < elements_counter + 1; j++)
			{
				if (i != j)
				{
					SumA += abs(matrixA[i][j] / matrixA[i][i]);
					SumB += abs(matrixA[j][i] / matrixA[j][j]);
					gamma += (matrixA[i][j] / matrixA[i][i]) * (matrixA[i][j] / matrixA[i][i]);
					cout << fixed << setw(10) << matrixA[i][j] / matrixA[i][i];
				}
				else cout << setw(10) << 0;
			}
			cout << fixed << setw(10) << matrixB[i] / matrixA[i][i] << endl;

			if (SumA > alpha)
			{
				alpha = SumA;
			}
			if (SumB > beta)
			{
				beta = SumB;
			}
			SumA = 0;
			SumB = 0;
		}

		cout << endl << "alpha = " << alpha << ", beta = " << beta << ", gamma = " << gamma << endl << endl;

		// Вычисление числа итераций
		int k = 0;
		double ro = 0;
		double ro_new = 0;
		if ((alpha < beta) && (alpha < gamma))
		{
			cout << "Using alpha" << endl;
			abg = 0;
			while (true)
			{
				for (int i = 0; i < elements_counter + 1; i++)
				{
					ro_new = abs(matrixB[i] / matrixA[i][i]);
					if (ro_new > ro)
					{
						ro = ro_new;
					}
				}
				if (((pow(alpha, k) * ro) / (1 - alpha)) >= Epsilon)
				{
					k++;
				}
				else break;
			}
		}
		else if ((beta < alpha) && (beta < gamma))
		{
			cout << "Using beta" << endl;
			abg = 1;
			while (true)
			{
				for (int i = 0; i < elements_counter + 1; i++)
				{
					ro += abs(matrixB[i] / matrixA[i][i]);
				}
				if (((pow(beta, k) * ro) / (1 - beta)) >= Epsilon)
				{
					k++;
				}
				else break;
			}
		}
		else
		{
			cout << "Using gamma" << endl;
			abg = 2;
			while (true)
			{
				for (int i = 0; i < elements_counter + 1; i++)
				{
					ro += abs(matrixB[i] / matrixA[i][i]) * abs(matrixB[i] / matrixA[i][i]);
				}
				ro = sqrt(ro);
				if (((pow(gamma, k) * ro) / (1 - gamma)) >= Epsilon)
				{
					k++;
				}
				else break;
			}
		}

		cout << "k = " << k << endl;

		// Проведение итераций
		double* result = new double[elements_counter + 1];
		double* r = new double[elements_counter + 1];
		result = Iteration(matrixA, matrixB, k, elements_counter + 1);

		// Подсчет невязки
		cout << "r:" << endl;
		for (int i = 0; i < elements_counter + 1; i++)
		{
			r[i] = 0;
			for (int j = 0; j < elements_counter + 1; j++)
			{
				r[i] += Original_matrix[i][j] * result[j];
			}
			r[i] -= matrixB[i];
			cout << setw(10) << setprecision(15) << r[i] << endl;
		}

		Nevyazka = 0.;
		switch (abg)
		{
		case 0:
		{
			if (alpha < 0 || alpha >= 1)
			{
				Nevyazka = 10;
				break;
			}
			for (int i = 0; i < elements_counter + 1; i++)
			{
				if (abs(Nevyazka) < abs(r[i])) Nevyazka = abs(r[i]);
			}
			break;
		}
		case 1:
		{
			if (beta < 0 || beta >= 1)
			{
				Nevyazka = 10;
				break;
			}
			for (int i = 0; i < elements_counter + 1; i++)
			{
				Nevyazka += r[i];
			}
			break;
		}
		case 2:
		{
			if (gamma < 0 || gamma >= 1)
			{
				Nevyazka = 10;
				break;
			}
			for (int i = 0; i < elements_counter + 1; i++)
			{
				Nevyazka += pow(r[i], 2);
			}
			Nevyazka = sqrt(Nevyazka);
			break;
		}
		}

		cout << "r = " << Nevyazka << endl;

		if (Nevyazka != Nevyazka) Nevyazka = 10;

		delete[] result;
	}

	cout << "tries = " << tries;

	f.close();

	for (int i = 0; i < elements_counter; i++)
	{
		delete[] matrixA[i];
	}
	for (int i = 0; i < elements_counter; i++)
	{
		delete[] Original_matrix[i];
	}
	delete[] matrixA;
	delete[] Original_matrix;
	delete[] matrixB;

	return 0;
}

void Diagonal(double** matrixA, double* matrixB, int size)
{
	double** ResetA = new double* [size];
	for (int i = 0; i < size; i++)
	{
		ResetA[i] = new double[size];
	}
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			ResetA[i][j] = matrixA[i][j];
		}
	}

	double* ResetB = new double[size];
	for (int i = 0; i < size; i++)
	{
		ResetB[i] = matrixB[i];
	}
	srand(time(0));
	double Sum = 0;
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (j != i)
			{
				Sum += abs(matrixA[i][j]);
			}
		}
		cout << endl << "For row " << i << " Sum = " << Sum << endl;
		int tries = 0;
		int low = -10;
		int high = 10;
		string calculations;
		while (abs(matrixA[i][i]) < Sum) //Диагонального преобладания в строке нет
		{
			Sum = 0;
			int row = rand() % size;
			double multiplier = low + (double)rand() * (high - low) / RAND_MAX;

			for (int j = 0; j < size; j++)
			{
				if (row < i)
				{
					matrixA[i][j] += multiplier * matrixA[row][j];
				}
				else
				{
					matrixA[i][j] += multiplier * ResetA[row][j];
				}
				
				if (j != i)
				{
					Sum += abs(matrixA[i][j]);
				}
			}
			matrixB[i] += multiplier * matrixB[row];

			calculations += to_string(multiplier) + " * (" + to_string(row) + ") " + " |Sum| = " + to_string(Sum) + " |x| = " + to_string(abs(matrixA[i][i])) + '\n';

			tries++;
			if (tries > 10)
			{
				for (int r = 0; r < size; r++)
				{
					matrixA[i][r] = ResetA[i][r];
				}
				matrixB[i] = ResetB[i];
				tries = 0;
				low = rand() % 100 * (-1);
				high = rand() % 100;
				//cout << "ResetA" << endl;
				calculations.clear();
			}
		}
		cout << calculations;
		Sum = 0;
	}

	for (int i = 0; i < size; i++)
	{
		matrixB[i] = ResetB[i];
		delete[] ResetA[i];
	}
	delete[] ResetA;
	delete[] ResetB;
}

double* Iteration(double** matrixA, double* matrixB, int k, int size)
{
	double* Result = new double[size];
	for (int i = 0; i < size; i++)
	{
		Result[i] = 0;
	}
	double* Prev = new double[size];
	if (k > 0)
	{
		Prev = Iteration(matrixA, matrixB, k - 1, size);
	}
	cout << "k = " << k << endl;

	for (int i = 0; i < size; i++)
	{
		if (k == 0)
		{
			Result[i] = 0;
		}
		else if (k == 1)
		{
			Result[i] = matrixB[i] / matrixA[i][i];
		}
		else
		{
			for (int j = 0; j < size; j++)
			{
				if (i != j)
				{
					Result[i] -= (matrixA[i][j] / matrixA[i][i]) * Prev[j];
				}
			}
			Result[i] += matrixB[i] / matrixA[i][i];
		}
		cout << fixed << Result[i] << endl;
	}
	
	return Result;

	delete[] Result;
	delete[] Prev;
}