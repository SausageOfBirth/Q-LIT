#include "QLIT_lib.h"

Matrix* identityG(){
  double complex **identityData = malloc(sizeof(double complex) * 2);
  identityData[0] = malloc(sizeof(double complex) * 2);
  identityData[1] = malloc(sizeof(double complex) * 2);

  identityData[0][0] = one;
  identityData[0][1] = zero;
  identityData[1][0] = zero;
  identityData[1][1] = one;

  Matrix* res = (Matrix*)malloc(sizeof (*res));
  res->rows = 2;
  res->cols = 2;
  res->data = identityData;

  return res;
}

Matrix* hadamardG(){
  double base = 1.0/sqrt(2);
  double complex **hadamardData = malloc(sizeof(double complex) * 2);
  hadamardData[0] = malloc(sizeof(double complex) * 2);
  hadamardData[1] = malloc(sizeof(double complex) * 2);

  hadamardData[0][0] = one * base;
  hadamardData[0][1] = one * base;
  hadamardData[1][0] = one * base;
  hadamardData[1][1] = (-1.0 + 0.0 * I) * base;

  Matrix* res = (Matrix*)malloc(sizeof (*res));
  res->rows = 2;
  res->cols = 2;
  res->data = hadamardData;

  return res;
}

Matrix* notG(){
  double complex **notData = malloc(sizeof(double complex) * 2);
  notData[0] = malloc(sizeof(double complex) * 2);
  notData[1] = malloc(sizeof(double complex) * 2);

  notData[0][0] = zero;
  notData[0][1] = one;
  notData[1][0] = one;
  notData[1][1] = zero;

  Matrix* res = (Matrix*)malloc(sizeof (*res));
  res->rows = 2;
  res->cols = 2;
  res->data = notData;

  return res;
}

Matrix* cnotG(){
  double complex **cnotData = malloc(sizeof(double complex) * 4);
  cnotData[0] = malloc(sizeof(double complex) * 4);
  cnotData[1] = malloc(sizeof(double complex) * 4);
  cnotData[2] = malloc(sizeof(double complex) * 4);
  cnotData[3] = malloc(sizeof(double complex) * 4);

  cnotData[0][0] = one;
  cnotData[1][0] = zero;
  cnotData[2][0] = zero;
  cnotData[3][0] = zero;
  cnotData[0][1] = zero;
  cnotData[1][1] = one;
  cnotData[2][1] = zero;
  cnotData[3][1] = zero;
  cnotData[0][2] = zero;
  cnotData[1][2] = zero;
  cnotData[2][2] = zero;
  cnotData[3][2] = one;
  cnotData[0][3] = zero;
  cnotData[1][3] = zero;
  cnotData[2][3] = one;
  cnotData[3][3] = zero;

  Matrix* res = (Matrix*)malloc(sizeof (*res));
  res->rows = 4;
  res->cols = 4;
  res->data = cnotData;

  return res;
}
