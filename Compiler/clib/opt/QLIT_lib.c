#include "QLIT_lib.h"

Matrix* identityG(){
  tuple** identityData = malloc(sizeof(tuple*) * 2);

  identityData[0] = genTuple(0, 0, one);
  identityData[1] = genTuple(1, 1, one);

  Matrix* res = (Matrix*)malloc(sizeof (Matrix));
  res->row = 2;
  res->col = 2;
  res->valsize = 2;
  res->data = identityData;

  return res;
}

Matrix* hadamardG(){
  double base = 1.0/sqrt(2);
  tuple** hadamardData = malloc(sizeof(tuple*) * 4);

  hadamardData[0] = genTuple(0, 0, one * base);
  hadamardData[1] = genTuple(0, 1, one * base);
  hadamardData[2] = genTuple(1, 0, one * base);
  hadamardData[3] = genTuple(1, 1, (-1.0 + 0.0 * I) * base);

  Matrix* res = (Matrix*)malloc(sizeof (Matrix));
  res->row = 2;
  res->col = 2;
  res->valsize = 4;
  res->data = hadamardData;

  return res;
}

Matrix* hadamardT(){
  tuple** hadamardData = malloc(sizeof(tuple*) * 4);

  hadamardData[0] = genTuple(0, 0, one);
  hadamardData[1] = genTuple(0, 1, one);
  hadamardData[2] = genTuple(1, 0, one);
  hadamardData[3] = genTuple(1, 1, (-1.0 + 0.0 * I));

  Matrix* res = (Matrix*)malloc(sizeof (Matrix));
  res->row = 2;
  res->col = 2;
  res->valsize = 4;
  res->data = hadamardData;

  return res;
}

Matrix* notG(){
  tuple** notData = malloc(sizeof(tuple*) * 2);

  notData[0] = genTuple(0, 1, one);
  notData[1] = genTuple(1, 0, one);

  Matrix* res = (Matrix*)malloc(sizeof (Matrix));
  res->row = 2;
  res->col = 2;
  res->valsize = 2;
  res->data = notData;

  return res;
}

Matrix* cnotG(){
  tuple** cnotData = malloc(sizeof(tuple*) * 4);

  cnotData[0] = genTuple(0, 0, one);
  cnotData[1] = genTuple(1, 1, one);
  cnotData[2] = genTuple(2, 3, one);
  cnotData[3] = genTuple(3, 2, one);

  Matrix* res = (Matrix*)malloc(sizeof (Matrix));
  res->row = 4;
  res->col = 4;
  res->valsize = 4;
  res->data = cnotData;

  return res;
}
