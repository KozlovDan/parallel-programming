#include <iostream>
#include <fstream>
#include <vector>
#include <cuda_runtime.h>

using namespace std;

__global__ void multiplyKernel(double* A, double* B, double* C, int n) {
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    if(row < n && col < n) {
        double sum = 0;
        for(int k = 0; k < n; k++) {
            sum += A[row*n + k] * B[k*n + col];
        }
        C[row*n + col] = sum;
    }
}

vector<double> readMatrix(string file, int &n) {
    ifstream f(file);
    f >> n;
    vector<double> m(n*n);

    for(int i=0;i<n*n;i++)
        f >> m[i];

    return m;
}

void writeMatrix(string file, vector<double>& m, int n) {
    ofstream f(file);
    f << n << endl;

    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++)
            f << m[i*n+j] << " ";
        f << endl;
    }
}

int main() {

    int n;
    vector<double> A = readMatrix("matrixA.txt", n);
    vector<double> B = readMatrix("matrixB.txt", n);
    vector<double> C(n*n);

    double *d_A, *d_B, *d_C;

    cudaMalloc(&d_A, n*n*sizeof(double));
    cudaMalloc(&d_B, n*n*sizeof(double));
    cudaMalloc(&d_C, n*n*sizeof(double));

    cudaMemcpy(d_A, A.data(), n*n*sizeof(double), cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, B.data(), n*n*sizeof(double), cudaMemcpyHostToDevice);

    dim3 block(16,16);
    dim3 grid((n+15)/16, (n+15)/16);

    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);

    cudaEventRecord(start);

    multiplyKernel<<<grid, block>>>(d_A, d_B, d_C, n);

    cudaEventRecord(stop);
    cudaEventSynchronize(stop);

    float ms;
    cudaEventElapsedTime(&ms, start, stop);

    cudaMemcpy(C.data(), d_C, n*n*sizeof(double), cudaMemcpyDeviceToHost);

    writeMatrix("result.txt", C, n);

    cout << "Time (ms): " << ms << endl;

    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);

    return 0;
}