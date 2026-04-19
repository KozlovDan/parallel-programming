#include <mpi.h>
#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

// чтение матрицы (только на root)
vector<vector<double>> readMatrix(string file, int &n) {
    ifstream f(file);
    f >> n;

    vector<vector<double>> m(n, vector<double>(n));

    for(int i=0;i<n;i++)
        for(int j=0;j<n;j++)
            f >> m[i][j];

    return m;
}

void writeMatrix(string file, vector<vector<double>> &m) {
    ofstream f(file);
    int n = m.size();

    f << n << endl;

    for(auto &row : m){
        for(auto v : row)
            f << v << " ";
        f << endl;
    }
}

int main(int argc, char** argv) {

    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n;
    vector<vector<double>> A, B, C;

    // ROOT читает данные
    if(rank == 0) {
        A = readMatrix("matrixA.txt", n);
        B = readMatrix("matrixB.txt", n);
        C.assign(n, vector<double>(n, 0));
    }

    // broadcast size
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // flatten matrix A
    vector<double> A_flat(n*n), B_flat(n*n), C_flat(n*n, 0);

    if(rank == 0) {
        for(int i=0;i<n;i++)
            for(int j=0;j<n;j++){
                A_flat[i*n+j] = A[i][j];
                B_flat[i*n+j] = B[i][j];
            }
    }

    MPI_Bcast(A_flat.data(), n*n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(B_flat.data(), n*n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    double start = MPI_Wtime();

    // parallel computation
    for(int i = rank; i < n; i += size){
        for(int j=0;j<n;j++){
            double sum = 0;
            for(int k=0;k<n;k++){
                sum += A_flat[i*n+k] * B_flat[k*n+j];
            }
            C_flat[i*n+j] = sum;
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);
    double end = MPI_Wtime();

    // gather results
    MPI_Reduce(
        C_flat.data(),
        (rank==0 ? C_flat.data() : NULL),
        n*n,
        MPI_DOUBLE,
        MPI_SUM,
        0,
        MPI_COMM_WORLD
    );

    if(rank == 0) {
        cout << "Time: " << end - start << " sec" << endl;
        writeMatrix("result.txt", C);
    }

    MPI_Finalize();
    return 0;
}