#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <stdexcept>
#include <algorithm>
#include <iomanip>

#include <windows.h>
#include <psapi.h>

#pragma comment(lib, "psapi.lib")
using Matrix = std::vector<std::vector<double>>;

double get_memory_mb() {
 PROCESS_MEMORY_COUNTERS pmc;
 if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
  return pmc.WorkingSetSize / 1024.0 / 1024.0;
 }
 return 0.0;
}
Matrix create_matrix(int n) {
 Matrix mat(n, std::vector<double>(n));
 std::random_device rd;
 std::mt19937 gen(rd());
 std::uniform_real_distribution<> dis(0.0, 1.0);
 for (int i = 0; i < n; ++i) {
 for (int j = 0; j < n; ++j) {
 mat[i][j] = dis(gen);
 }
 }
 return mat;
}

Matrix naive_multiply(const Matrix& A, const Matrix& B) {
 int n = A.size();
 Matrix C(n, std::vector<double>(n, 0.0));
 for (int i = 0; i < n; ++i) {
 for (int k = 0; k < n; ++k) {
 for (int j = 0; j < n; ++j) {
 C[i][j] += A[i][k] * B[k][j];
 }
 }
 }
 return C;
}

Matrix add_matrices(const Matrix& A, const Matrix& B) {
 int n = A.size();
 Matrix C(n, std::vector<double>(n));
 for (int i = 0; i < n; ++i) {
 for (int j = 0; j < n; ++j) {
 C[i][j] = A[i][j] + B[i][j];
 }
 }
 return C;
}
Matrix subtract_matrices(const Matrix& A, const Matrix& B) {
 int n = A.size();
 Matrix C(n, std::vector<double>(n));
 for (int i = 0; i < n; ++i) {
 for (int j = 0; j < n; ++j) {
 C[i][j] = A[i][j] - B[i][j];
 }
 }
 return C;
}
void split_matrix(const Matrix& P, Matrix& C11, Matrix& C12, Matrix& C21, Matrix& C22) {
 int mid = P.size() / 2;
 for (int i = 0; i < mid; ++i) {
 for (int j = 0; j < mid; ++j) {
 C11[i][j] = P[i][j];
 C12[i][j] = P[i][j + mid];
 C21[i][j] = P[i + mid][j];
 C22[i][j] = P[i + mid][j + mid];
 }
 }
}
void combine_matrices(Matrix& P, const Matrix& C11, const Matrix& C12, const Matrix& C21, const Matrix& C22) {
 int mid = C11.size();
 for (int i = 0; i < mid; ++i) {
 for (int j = 0; j < mid; ++j) {
 P[i][j] = C11[i][j];
 P[i][j + mid] = C12[i][j];
 P[i + mid][j] = C21[i][j];
 P[i + mid][j + mid] = C22[i][j];
 }
 }
}
Matrix strassen_multiply(const Matrix& A, const Matrix& B) {
 int n = A.size();
 if (n <= 64) {
 return naive_multiply(A, B);
 }
 int mid = n / 2;
 Matrix A11(mid, std::vector<double>(mid)), A12(mid, std::vector<double>(mid)), A21(mid, std::vector<double>(mid)), A22(mid, std::vector<double>(mid));
 Matrix B11(mid, std::vector<double>(mid)), B12(mid, std::vector<double>(mid)), B21(mid, std::vector<double>(mid)), B22(mid, std::vector<double>(mid));
 split_matrix(A, A11, A12, A21, A22);
 split_matrix(B, B11, B12, B21, B22);
 Matrix M1 = strassen_multiply(add_matrices(A11, A22), add_matrices(B11, B22));
 Matrix M2 = strassen_multiply(add_matrices(A21, A22), B11);
 Matrix M3 = strassen_multiply(A11, subtract_matrices(B12, B22));
 Matrix M4 = strassen_multiply(A22, subtract_matrices(B21, B11));
 Matrix M5 = strassen_multiply(add_matrices(A11, A12), B22);
 Matrix M6 = strassen_multiply(subtract_matrices(A21, A11), add_matrices(B11, B12));
 Matrix M7 = strassen_multiply(subtract_matrices(A12, A22), add_matrices(B21, B22));
 Matrix C11 = add_matrices(subtract_matrices(add_matrices(M1, M4), M5), M7);
 Matrix C12 = add_matrices(M3, M5);
 Matrix C21 = add_matrices(M2, M4);
 Matrix C22 = add_matrices(subtract_matrices(add_matrices(M1, M3), M2), M6);
 Matrix C(n, std::vector<double>(n));
 combine_matrices(C, C11, C12, C21, C22);
 return C;
}
int main(int argc, char* argv[]) {
 if (argc != 2) {
 std::cerr << "Usage: " << argv[0] << " <n>" << std::endl;
 return 1;
 }
 int n;
 try {
 n = std::stoi(argv[1]);
 } catch (const std::exception& e) {
 std::cerr << "Invalid number: " << argv[1] << std::endl;
 return 1;
 }
 if ((n & (n - 1)) != 0 || n == 0) {
 std::cerr << "Error: n must be a power of 2." << std::endl;
 return 1;
 }
 std::cout << "--- C++ Matrix Multiplication (n=" << n << ") ---" << std::endl;
 Matrix A = create_matrix(n);
 Matrix B = create_matrix(n);
 double peak = 0.0;
 auto start = std::chrono::high_resolution_clock::now();
 naive_multiply(A, B);
 peak = std::max(peak, get_memory_mb());
 auto end = std::chrono::high_resolution_clock::now();
 std::chrono::duration<double> diff = end - start;
 std::cout << "Naive Algorithm Time: " << diff.count() << " seconds" << std::endl;
 start = std::chrono::high_resolution_clock::now();
 strassen_multiply(A, B);
 peak = std::max(peak, get_memory_mb());
 end = std::chrono::high_resolution_clock::now();
 diff = end - start;
 std::cout << "Strassen Algorithm Time: " << diff.count() << " seconds" << std::endl;
 std::cout << "Peak RAM (MB): " << std::fixed << std::setprecision(2) << peak << std::endl;
 return 0;
}