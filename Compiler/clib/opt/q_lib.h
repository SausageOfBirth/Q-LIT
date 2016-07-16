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
  int len;
  double complex* data;
} Qubit;

typedef struct {
  int x;
  int y;
  double complex val;
}tuple;

typedef struct {
  int row;
  int col;
  int valsize;
  tuple** data;
} Matrix;

const double complex zero;
const double complex one;

Qubit* qubitInit(int value, int length);
int freeMatrix(Matrix* m);
int freeQubit(Qubit* q);
void printQ(Qubit* q);
void outputQ(char* name, const Qubit* q);
void printTuple(Matrix* m);
void printVal(Matrix* m);
void calcSlot(Qubit** q, Matrix** mArray);
double complex calcRow(Qubit** q, Matrix** mArray, int gateNum, int qubitIndex, int relQ, int blockSize, double complex tensorRes);
Qubit* multBitGate(Matrix* m, Qubit* q);
double complex innerProduct(Qubit* qubit1, Qubit* qubit2);
Matrix* outerProduct(Qubit* qubit1, Qubit* qubit2);
Matrix* scalarG(Matrix* m, double complex s);
Qubit* scalarQ(Qubit* q, double complex scalar);
Qubit* plusQ(Qubit* q1, Qubit* q2);
Qubit* minusQ(Qubit* q1, Qubit* q2);
Matrix* multG(Matrix* m1, Matrix* m2);
Matrix* plusG(Matrix* m1, Matrix* m2);
Matrix* minusG(Matrix* m1, Matrix* m2);
Matrix* tensor(Matrix* m1, Matrix* m2);
void rebalance_row(Matrix** m);
void rebalance_col(Matrix** m);
void rebalance_row_inner(Matrix** m, int sub_start, int sub_end);
void rebalance_col_inner(Matrix** m, int sub_start, int sub_end);
void measurement(Qubit** qubit, int bitSig);
tuple* genTuple(int x, int y, double complex val);
Matrix* controlGen(int control, int target, Matrix* op);
