#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
#include <omp.h>

using namespace std;

// Чтение матрицы из файла
vector<vector<double>> readMatrix(string filename, int &n) {
    ifstream file(filename);

    if (!file.is_open()) {
        cout << "Error: cannot open file " << filename << endl;
        exit(1);
    }

    file >> n;

    if (n <= 0 || n > 5000) {
        cout << "Invalid matrix size: " << n << endl;
        exit(1);
    }

    vector<vector<double>> matrix(n, vector<double>(n));

    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            file >> matrix[i][j];

    file.close();
    return matrix;
}

// Запись матрицы в файл
void writeMatrix(string filename, vector<vector<double>> matrix) {
    ofstream file(filename);
    int n = matrix.size();

    file << n << endl;

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++)
            file << matrix[i][j] << " ";
        file << endl;
    }

    file.close();
}

// Параллельное умножение матриц
vector<vector<double>> multiply_parallel(vector<vector<double>> A,
                                         vector<vector<double>> B) {

    int n = A.size();
    vector<vector<double>> C(n, vector<double>(n, 0));

    #pragma omp parallel for
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            for(int k = 0; k < n; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    return C;
}

int main() {

    int n1, n2;

    // Ввод количества потоков
    int threads;
    cout << "Enter number of threads: ";
    cin >> threads;

    omp_set_num_threads(threads);

    // Чтение матриц
    vector<vector<double>> A = readMatrix("matrixA.txt", n1);
    vector<vector<double>> B = readMatrix("matrixB.txt", n2);

    if (n1 != n2) {
        cout << "Matrices must be same size!" << endl;
        return 1;
    }

    cout << "Matrix size: " << n1 << "x" << n1 << endl;

    // Измерение времени
    auto start = chrono::high_resolution_clock::now();

    vector<vector<double>> C = multiply_parallel(A, B);

    auto end = chrono::high_resolution_clock::now();

    chrono::duration<double> duration = end - start;

    // Запись результата
    writeMatrix("result.txt", C);

    // Подсчёт операций
    long long operations = 2LL * n1 * n1 * n1;

    // Вывод
    cout << "Execution time: " << duration.count() << " seconds" << endl;
    cout << "Operations: " << operations << endl;
    cout << "Threads used: " << threads << endl;

    return 0;
}