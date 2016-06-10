#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <complex.h>
#include <time.h>

typedef int bool;
#define true 1
#define false 0

typedef struct {
  int cols;
  int rows;
  double complex** data;
} Matrix;

typedef struct {
  int len;
  double complex* data;
} Qubit;

const double complex zero;
const double complex one;

Qubit* qubitInit(int value, int length);
Matrix* matrixInit(int col, int row);
int freeMatrix(Matrix* m);
int freeQubit(Qubit* q);

Matrix* tensor(const Matrix* g1, const Matrix* g2);
Qubit* multBitGate(const Matrix* gate, const Qubit* qubit);
double complex innerProduct(const Qubit* q1, const Qubit* q2);
Matrix* outerProduct(const Qubit* q1, const Qubit* q2);
Matrix* plusG(const Matrix* q1, const Matrix* q2);
Matrix* minusG(const Matrix* q1, const Matrix* q2);
Matrix* multG(const Matrix* q1, const Matrix* q2);
Qubit* scalarQ(Qubit* qubit, double complex scalar);
Matrix* scalarG(Matrix* gate, double complex scalar);
Qubit* minusQ(const Qubit* qubit1, const Qubit* qubit2);
Qubit* plusQ(const Qubit* qubit1, const Qubit* qubit2);
void outputQ(char* name, const Qubit* q);
void printM(const Matrix* m);
void printQ(const Qubit* q);
Qubit* measurement(const Qubit* qubit, int bitSig);
Matrix* controlGen(int control, int target, Matrix* m);
