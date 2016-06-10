#include "QLIT_lib.h"

bool matrixEqual(Matrix* m1, Matrix* m2){
  if(m1->row == m2->row && m1->col == m2->col && m1->valsize == m2->valsize){
    for(int n = 0; n < m1->valsize; n++){
      if(m1->data[n]->x != m2->data[n]->x ||
         m1->data[n]->y != m2->data[n]->y ||
         m1->data[n]->val != m2->data[n]->val){
        return false;
      }
    }
    return true;
  }else{
    return false;
  }
}

bool qubitEqual(Qubit* q1, Qubit* q2){
  if(q1->len == q2->len){
    for(int n = 0; n < q1->len; n++){
      if(q1->data[n] != q2->data[n]){
        return false;
      }
    }
    return true;
  }else{
    return false;
  }
}

int unitTest(){
  Matrix* rowMatrix = (Matrix*)malloc(sizeof(Matrix));
  tuple** rowMatrixData = (tuple**)malloc(sizeof(tuple*)*4);
  rowMatrixData[0] = genTuple(0, 0, 1.0 + 0.0 * I);
  rowMatrixData[1] = genTuple(0, 1, 2.0 + 0.0 * I);
  rowMatrixData[2] = genTuple(0, 2, 3.0 + 0.0 * I);
  rowMatrixData[3] = genTuple(0, 3, 4.0 + 0.0 * I);
  rowMatrix->data = rowMatrixData;
  rowMatrix->row = 1;
  rowMatrix->col = 4;
  rowMatrix->valsize = 4;

  Matrix* colMatrix = (Matrix*)malloc(sizeof(Matrix));
  tuple** colMatrixData = (tuple**)malloc(sizeof(tuple*)*4);
  colMatrixData[0] = genTuple(0, 0, 5.0 + 0.0 * I);
  colMatrixData[1] = genTuple(1, 0, 6.0 + 0.0 * I);
  colMatrixData[2] = genTuple(2, 0, 7.0 + 0.0 * I);
  colMatrixData[3] = genTuple(3, 0, 8.0 + 0.0 * I);
  colMatrix->data = colMatrixData;
  colMatrix->row = 4;
  colMatrix->col = 1;
  colMatrix->valsize = 4;

  Matrix* singularMatrix = (Matrix*)malloc(sizeof(Matrix));
  tuple** singularMatrixData = (tuple**)malloc(sizeof(tuple*));
  singularMatrixData[0] = genTuple(0, 0, 2.0 + 0.0 * I);
  singularMatrix->data = singularMatrixData;
  singularMatrix->row = 1;
  singularMatrix->col = 1;
  singularMatrix->valsize = 1;

  Matrix* sparseMat1 = (Matrix*)malloc(sizeof(Matrix));
  tuple** sparseMat1Data = (tuple**)malloc(sizeof(tuple*)*4);
  sparseMat1Data[0] = genTuple(1, 0, 1.0 + 0.0 * I);
  sparseMat1Data[1] = genTuple(0, 3, 1.0 + 0.0 * I);
  sparseMat1->data = sparseMat1Data;
  sparseMat1->row = 4;
  sparseMat1->col = 4;
  sparseMat1->valsize = 2;

  Matrix* sparseMat2 = (Matrix*)malloc(sizeof(Matrix));
  tuple** sparseMat2Data = (tuple**)malloc(sizeof(tuple*)*2);
  sparseMat2Data[0] = genTuple(1, 0, 1.0 + 0.0 * I);
  sparseMat2Data[1] = genTuple(3, 2, 1.0 + 0.0 * I);
  sparseMat2->data = sparseMat2Data;
  sparseMat2->row = 4;
  sparseMat2->col = 4;
  sparseMat2->valsize = 2;

  Matrix* negRowMatrix = scalarG(-1.0 + 0.0 * I, rowMatrix);
  Matrix* negColMatrix = scalarG(-1.0 + 0.0 * I, colMatrix);

  Qubit* fullQubit0 = (Qubit*)malloc(sizeof(Qubit));
  double complex* fullQubit0Data = (double complex*)malloc(sizeof(double complex));
  fullQubit0Data[0] = 2.0 + 0.0 * I;
  fullQubit0->data = fullQubit0Data;
  fullQubit0->len = 1;

  Qubit* fullQubit1 = (Qubit*)malloc(sizeof(Qubit));
  double complex* fullQubit1Data = (double complex*)malloc(sizeof(double complex)*4);
  fullQubit1Data[0] = 1.0 + 0.0 * I;
  fullQubit1Data[1] = 2.0 + 0.0 * I;
  fullQubit1Data[2] = 3.0 + 0.0 * I;
  fullQubit1Data[3] = 4.0 + 0.0 * I;
  fullQubit1->data = fullQubit1Data;
  fullQubit1->len = 4;

  Matrix* unsortedMatrix0 = (Matrix*)malloc(sizeof(Matrix));
  tuple** unsortedMatrix0Data = (tuple**)malloc(sizeof(tuple*)*4);
  unsortedMatrix0Data[0] = genTuple(3, 0, 1.0 + 0.0 * I);
  unsortedMatrix0Data[1] = genTuple(2, 3, 1.0 + 0.0 * I);
  unsortedMatrix0Data[2] = genTuple(1, 2, 1.0 + 0.0 * I);
  unsortedMatrix0Data[3] = genTuple(0, 0, 1.0 + 0.0 * I);
  unsortedMatrix0->data = unsortedMatrix0Data;
  unsortedMatrix0->row = 4;
  unsortedMatrix0->col = 4;
  unsortedMatrix0->valsize = 4;


  Matrix* unsortedMatrix1 = (Matrix*)malloc(sizeof(Matrix));
  tuple** unsortedMatrix1Data = (tuple**)malloc(sizeof(tuple*));
  unsortedMatrix1Data[0] = genTuple(0, 0, 1.0 + 0.0 * I);
  unsortedMatrix1->data = unsortedMatrix1Data;
  unsortedMatrix1->row = 4;
  unsortedMatrix1->col = 4;
  unsortedMatrix1->valsize = 1;

  Matrix* postsortedMatrix0 = unsortedMatrix1;

  Matrix* multRes1 = (Matrix*)malloc(sizeof(Matrix));
  tuple** multRes1Data = (tuple**)malloc(sizeof(tuple*));
  multRes1Data[0] = genTuple(0, 0, 70.0 + 0.0 * I);
  multRes1->data = multRes1Data;
  multRes1->row = 1;
  multRes1->col = 1;
  multRes1->valsize = 1;

  Matrix* multRes2 = (Matrix*)malloc(sizeof(Matrix));
  tuple** multRes2Data = (tuple**)malloc(sizeof(tuple*)*16);
  multRes2Data[0] = genTuple(0, 0, 5.0 + 0.0 * I);
  multRes2Data[1] = genTuple(0, 1, 10.0 + 0.0 * I);
  multRes2Data[2] = genTuple(0, 2, 15.0 + 0.0 * I);
  multRes2Data[3] = genTuple(0, 3, 20.0 + 0.0 * I);
  multRes2Data[4] = genTuple(1, 0, 6.0 + 0.0 * I);
  multRes2Data[5] = genTuple(1, 1, 12.0 + 0.0 * I);
  multRes2Data[6] = genTuple(1, 2, 18.0 + 0.0 * I);
  multRes2Data[7] = genTuple(1, 3, 24.0 + 0.0 * I);
  multRes2Data[8] = genTuple(2, 0, 7.0 + 0.0 * I);
  multRes2Data[9] = genTuple(2, 1, 14.0 + 0.0 * I);
  multRes2Data[10] = genTuple(2, 2, 21.0 + 0.0 * I);
  multRes2Data[11] = genTuple(2, 3, 28.0 + 0.0 * I);
  multRes2Data[12] = genTuple(3, 0, 8.0 + 0.0 * I);
  multRes2Data[13] = genTuple(3, 1, 16.0 + 0.0 * I);
  multRes2Data[14] = genTuple(3, 2, 24.0 + 0.0 * I);
  multRes2Data[15] = genTuple(3, 3, 32.0 + 0.0 * I);
  multRes2->data = multRes2Data;
  multRes2->row = 4;
  multRes2->col = 4;
  multRes2->valsize = 16;

  Matrix* multRes3 = (Matrix*)malloc(sizeof(Matrix));
  tuple** multRes3Data = (tuple**)malloc(sizeof(tuple*));
  multRes3Data[0] = genTuple(0, 2, 1.0 + 0.0 * I);
  multRes3->data = multRes3Data;
  multRes3->row = 4;
  multRes3->col = 4;
  multRes3->valsize = 1;

  Matrix* plusRes0 = (Matrix*)malloc(sizeof(Matrix));
  tuple** plusRes0Data = (tuple**)malloc(sizeof(tuple*));
  plusRes0Data[0] = genTuple(0, 0, 0.0 + 0.0 * I);
  plusRes0->data = plusRes0Data;
  plusRes0->row = 1;
  plusRes0->col = 4;
  plusRes0->valsize = 1;

  Matrix* minusRes0 = (Matrix*)malloc(sizeof(Matrix));
  minusRes0->data = plusRes0Data;
  minusRes0->row = 4;
  minusRes0->col = 1;
  minusRes0->valsize = 1;

  Matrix* minusRes1 = (Matrix*)malloc(sizeof(Matrix));
  minusRes1->data = plusRes0Data;
  minusRes1->row = 1;
  minusRes1->col = 4;
  minusRes1->valsize = 1;

  Matrix* scalarRes0 = (Matrix*)malloc(sizeof(Matrix));
  tuple** scalarRes0Data = (tuple**)malloc(sizeof(tuple*)*4);
  scalarRes0Data[0] = genTuple(0, 0, 2.0 + 0.0 * I);
  scalarRes0Data[1] = genTuple(0, 1, 4.0 + 0.0 * I);
  scalarRes0Data[2] = genTuple(0, 2, 6.0 + 0.0 * I);
  scalarRes0Data[3] = genTuple(0, 3, 8.0 + 0.0 * I);
  scalarRes0->data = scalarRes0Data;
  scalarRes0->row = 1;
  scalarRes0->col = 4;
  scalarRes0->valsize = 4;

  Matrix* scalarRes1 = minusRes1;

  Matrix* tensorRes0 = (Matrix*)malloc(sizeof(Matrix));
  tuple** tensorRes0Data = (tuple**)malloc(sizeof(tuple*)*16);
  tensorRes0Data[0] = genTuple(0, 0, 1.0 + 0.0 * I);
  tensorRes0Data[1] = genTuple(0, 1, 2.0 + 0.0 * I);
  tensorRes0Data[2] = genTuple(0, 2, 3.0 + 0.0 * I);
  tensorRes0Data[3] = genTuple(0, 3, 4.0 + 0.0 * I);
  tensorRes0Data[4] = genTuple(0, 4, 2.0 + 0.0 * I);
  tensorRes0Data[5] = genTuple(0, 5, 4.0 + 0.0 * I);
  tensorRes0Data[6] = genTuple(0, 6, 6.0 + 0.0 * I);
  tensorRes0Data[7] = genTuple(0, 7, 8.0 + 0.0 * I);
  tensorRes0Data[8] = genTuple(0, 8, 3.0 + 0.0 * I);
  tensorRes0Data[9] = genTuple(0, 9, 6.0 + 0.0 * I);
  tensorRes0Data[10] = genTuple(0, 10, 9.0 + 0.0 * I);
  tensorRes0Data[11] = genTuple(0, 11, 12.0 + 0.0 * I);
  tensorRes0Data[12] = genTuple(0, 12, 4.0 + 0.0 * I);
  tensorRes0Data[13] = genTuple(0, 13, 8.0 + 0.0 * I);
  tensorRes0Data[14] = genTuple(0, 14, 12.0 + 0.0 * I);
  tensorRes0Data[15] = genTuple(0, 15, 16.0 + 0.0 * I);
  tensorRes0->data = tensorRes0Data;
  tensorRes0->row = 1;
  tensorRes0->col = 16;
  tensorRes0->valsize = 16;
  
  Matrix* tensorRes1 = (Matrix*)malloc(sizeof(Matrix));
  tuple** tensorRes1Data = (tuple**)malloc(sizeof(tuple*)*16);
  tensorRes1Data[0] = genTuple(0, 0, 25.0 + 0.0 * I);
  tensorRes1Data[1] = genTuple(1, 0, 30.0 + 0.0 * I);
  tensorRes1Data[2] = genTuple(2, 0, 35.0 + 0.0 * I);
  tensorRes1Data[3] = genTuple(3, 0, 40.0 + 0.0 * I);
  tensorRes1Data[4] = genTuple(4, 0, 30.0 + 0.0 * I);
  tensorRes1Data[5] = genTuple(5, 0, 36.0 + 0.0 * I);
  tensorRes1Data[6] = genTuple(6, 0, 42.0 + 0.0 * I);
  tensorRes1Data[7] = genTuple(7, 0, 48.0 + 0.0 * I);
  tensorRes1Data[8] = genTuple(8, 0, 35.0 + 0.0 * I);
  tensorRes1Data[9] = genTuple(9, 0, 42.0 + 0.0 * I);
  tensorRes1Data[10] = genTuple(10, 0, 49.0 + 0.0 * I);
  tensorRes1Data[11] = genTuple(11, 0, 56.0 + 0.0 * I);
  tensorRes1Data[12] = genTuple(12, 0, 40.0 + 0.0 * I);
  tensorRes1Data[13] = genTuple(13, 0, 48.0 + 0.0 * I);
  tensorRes1Data[14] = genTuple(14, 0, 56.0 + 0.0 * I);
  tensorRes1Data[15] = genTuple(15, 0, 64.0 + 0.0 * I);
  tensorRes1->data = tensorRes1Data;
  tensorRes1->row = 16;
  tensorRes1->col = 1;
  tensorRes1->valsize = 16;

  Matrix* tensorRes2 = (Matrix*)malloc(sizeof(Matrix));
  tuple** tensorRes2Data = (tuple**)malloc(sizeof(tuple*)*16);
  tensorRes2Data[0] = genTuple(0, 0, 5.0 + 0.0 * I);
  tensorRes2Data[1] = genTuple(1, 0, 6.0 + 0.0 * I);
  tensorRes2Data[2] = genTuple(2, 0, 7.0 + 0.0 * I);
  tensorRes2Data[3] = genTuple(3, 0, 8.0 + 0.0 * I);
  tensorRes2Data[4] = genTuple(0, 1, 10.0 + 0.0 * I);
  tensorRes2Data[5] = genTuple(1, 1, 12.0 + 0.0 * I);
  tensorRes2Data[6] = genTuple(2, 1, 14.0 + 0.0 * I);
  tensorRes2Data[7] = genTuple(3, 1, 16.0 + 0.0 * I);
  tensorRes2Data[8] = genTuple(0, 2, 15.0 + 0.0 * I);
  tensorRes2Data[9] = genTuple(1, 2, 18.0 + 0.0 * I);
  tensorRes2Data[10] = genTuple(2, 2, 21.0 + 0.0 * I);
  tensorRes2Data[11] = genTuple(3, 2, 24.0 + 0.0 * I);
  tensorRes2Data[12] = genTuple(0, 3, 20.0 + 0.0 * I);
  tensorRes2Data[13] = genTuple(1, 3, 24.0 + 0.0 * I);
  tensorRes2Data[14] = genTuple(2, 3, 28.0 + 0.0 * I);
  tensorRes2Data[15] = genTuple(3, 3, 32.0 + 0.0 * I);
  tensorRes2->data = tensorRes2Data;
  tensorRes2->row = 4;
  tensorRes2->col = 4;
  tensorRes2->valsize = 16;

  rebalance_row(&tensorRes2);

  Matrix* tensorRes3 = (Matrix*)malloc(sizeof(Matrix));
  tuple** tensorRes3Data = (tuple**)malloc(sizeof(tuple*));
  tensorRes3Data[0] = genTuple(0, 0, 4.0 + 0.0 * I);
  tensorRes3->data = tensorRes3Data;
  tensorRes3->row = 1;
  tensorRes3->col = 1;
  tensorRes3->valsize = 1;


  Qubit* scalarQRes0 = (Qubit*)malloc(sizeof(Qubit));
  double complex* scalarQRes0Data = (double complex*)malloc(sizeof(double complex));
  scalarQRes0Data[0] = 0.0 + 0.0 * I;
  scalarQRes0->data = scalarQRes0Data;
  scalarQRes0->len = 1;

  Qubit* scalarQRes1 = (Qubit*)malloc(sizeof(Qubit));
  double complex* scalarQRes1Data = (double complex*)malloc(sizeof(double complex)*4);
  scalarQRes1Data[0] = 0.0 + 0.0 * I;
  scalarQRes1Data[1] = 0.0 + 0.0 * I;
  scalarQRes1Data[2] = 0.0 + 0.0 * I;
  scalarQRes1Data[3] = 0.0 + 0.0 * I;
  scalarQRes1->data = scalarQRes1Data;
  scalarQRes1->len = 4;

  Qubit* scalarQRes2 = (Qubit*)malloc(sizeof(Qubit));
  double complex* scalarQRes2Data = (double complex*)malloc(sizeof(double complex));
  scalarQRes2Data[0] = 4.0 + 0.0 * I;
  scalarQRes2->data = scalarQRes2Data;
  scalarQRes2->len = 1;

  Qubit* scalarQRes3 = (Qubit*)malloc(sizeof(Qubit));
  double complex* scalarQRes3Data = (double complex*)malloc(sizeof(double complex)*4);
  scalarQRes3Data[0] = 2.0 + 0.0 * I;
  scalarQRes3Data[1] = 4.0 + 0.0 * I;
  scalarQRes3Data[2] = 6.0 + 0.0 * I;
  scalarQRes3Data[3] = 8.0 + 0.0 * I;
  scalarQRes3->data = scalarQRes3Data;
  scalarQRes3->len = 4;

  Matrix* rebalanceRowRes0 = (Matrix*)malloc(sizeof(Matrix));
  tuple** rebalanceRowRes0Data = (tuple**)malloc(sizeof(tuple*)*4);
  rebalanceRowRes0Data[0] = genTuple(0, 0, 1.0 + 0.0 * I);
  rebalanceRowRes0Data[1] = genTuple(1, 2, 1.0 + 0.0 * I);
  rebalanceRowRes0Data[2] = genTuple(2, 3, 1.0 + 0.0 * I);
  rebalanceRowRes0Data[3] = genTuple(3, 0, 1.0 + 0.0 * I);
  rebalanceRowRes0->data = rebalanceRowRes0Data;
  rebalanceRowRes0->row = 4;
  rebalanceRowRes0->col = 4;
  rebalanceRowRes0->valsize = 4;

  Matrix* rebalanceColRes0 = (Matrix*)malloc(sizeof(Matrix));
  tuple** rebalanceColRes0Data = (tuple**)malloc(sizeof(tuple*)*4);
  rebalanceColRes0Data[0] = genTuple(0, 0, 1.0 + 0.0 * I);
  rebalanceColRes0Data[1] = genTuple(3, 0, 1.0 + 0.0 * I);
  rebalanceColRes0Data[2] = genTuple(1, 2, 1.0 + 0.0 * I);
  rebalanceColRes0Data[3] = genTuple(2, 3, 1.0 + 0.0 * I);
  rebalanceColRes0->data = rebalanceColRes0Data;
  rebalanceColRes0->row = 4;
  rebalanceColRes0->col = 4;
  rebalanceColRes0->valsize = 4;

  Matrix* controlGenRes0 = (Matrix*)malloc(sizeof(Matrix));
  tuple** controlGenRes0Data = (tuple**)malloc(sizeof(tuple*)*4);
  controlGenRes0Data[0] = genTuple(0, 0, 1.0 + 0.0 * I);
  controlGenRes0Data[0] = genTuple(1, 1, 1.0 + 0.0 * I);
  controlGenRes0Data[0] = genTuple(1, 2, 1.0 + 0.0 * I);
  controlGenRes0Data[0] = genTuple(2, 1, 1.0 + 0.0 * I);
  controlGenRes0->data = controlGenRes0Data;
  controlGenRes0->row = 4;
  controlGenRes0->col = 4;
  controlGenRes0->valsize = 4;
  
  printf("controlGen test : %s\n", matrixEqual(controlGenRes0, controlGen(0,1,notG())) ? "true" : "false");
  
  Matrix* multResCalc1 = multG(rowMatrix, colMatrix);
  Matrix* multResCalc2 = multG(colMatrix, rowMatrix);
  Matrix* multResCalc3 = multG(sparseMat1, sparseMat2);

  Matrix* plusResCalc0 = plusG(rowMatrix, negRowMatrix);

  printf("multG test0 : %s\n", matrixEqual(multResCalc1, multRes1) ? "true" : "false");
  printf("multG test1 : %s\n", matrixEqual(multResCalc2, multRes2) ? "true" : "false");
  printf("multG test2 : %s\n", matrixEqual(multResCalc3, multRes3) ? "true" : "false");
  printf("multG test3 : %s\n", multG(rowMatrix, rowMatrix) == NULL ? "true" : "false");

  printf("plusG test0 : %s\n", matrixEqual(scalarG(2.0 + 0.0 * I, multRes2), plusG(multRes2, multRes2)) ? "true" : "false");
  printf("plusG test1 : %s\n", plusG(rowMatrix, colMatrix) == NULL ? "true" : "false");
  printf("plusG test2 : %s\n", matrixEqual(scalarG(2.0 + 0.0 * I, sparseMat1), plusG(sparseMat1, sparseMat1)) ? "true" : "false");
  printf("plusG test3 : %s\n", matrixEqual(scalarG(2.0 + 0.0 * I, sparseMat2), plusG(sparseMat2, sparseMat2)) ? "true" : "false");
  printf("plusG test4 : %s\n", matrixEqual(plusResCalc0, plusRes0) ? "true" : "false");

  printf("minusG test0 : %s\n", matrixEqual(minusG(rowMatrix, negRowMatrix), scalarG(2.0 + 0.0 * I, rowMatrix)) ? "true" : "false");
  printf("minusG test1 : %s\n", matrixEqual(minusG(colMatrix, negColMatrix), scalarG(2.0 + 0.0 * I, colMatrix)) ? "true" : "false");
  printf("minusG test2 : %s\n", minusG(rowMatrix, colMatrix) == NULL ? "true" : "false");
  printf("minusG test3 : %s\n", matrixEqual(minusG(colMatrix, colMatrix), minusRes0) ? "true" : "false");
  printf("minusG test4 : %s\n", matrixEqual(minusG(rowMatrix, rowMatrix), minusRes1) ? "true" : "false");

  printf("scalarG test0 : %s\n", matrixEqual(scalarG(2.0 + 0.0 * I, rowMatrix), scalarRes0) ? "true" : "false");
  printf("scalarG test1 : %s\n", matrixEqual(scalarG(0.0 + 0.0 * I, rowMatrix), scalarRes1) ? "true" : "false");

  printf("tensor test0 : %s\n", matrixEqual(tensor(rowMatrix, rowMatrix), tensorRes0) ? "true" : "false");
  printf("tensor test1 : %s\n", matrixEqual(tensor(colMatrix, colMatrix), tensorRes1) ? "true" : "false");
  printf("tensor test2 : %s\n", matrixEqual(tensor(rowMatrix, colMatrix), tensorRes2) ? "true" : "false");
  printf("tensor test3 : %s\n", matrixEqual(tensor(singularMatrix, singularMatrix), tensorRes3) ? "true" : "false");

  printf("multBitGate test0 : %s\n", qubitEqual(multBitGate(tensor(identityG(), identityG()), qubitInit(0, 2)), qubitInit(0, 2)) ? "true" : "false");
  printf("multBitGate test1 : %s\n", qubitEqual(multBitGate(tensor(notG(), notG()), qubitInit(3, 2)), qubitInit(0, 2)) ? "true" : "false");
  printf("multBitGate test2 : %s\n", multBitGate(colMatrix, qubitInit(0, 2)) == NULL ? "true" : "false");

  printf("outerProduct test0 : %s\n", matrixEqual(outerProduct(fullQubit0, fullQubit1), scalarG(2.0 + 0.0 * I, rowMatrix)) ? "true" : "false");
  printf("outerProduct test1 : %s\n", matrixEqual(outerProduct(fullQubit0, fullQubit0), scalarG(2.0 + 0.0 * I, singularMatrix)) ? "true" : "false");

  printf("innerProduct test0 : %s\n", innerProduct(fullQubit0, fullQubit0) == 4.0 + 0.0 * I ? "true" : "false");
  printf("innerProduct test1 : %s\n", innerProduct(fullQubit1, fullQubit1) == 30.0 + 0.0 * I ? "true" : "false");

  printf("plusQ test0 : %s\n", qubitEqual(plusQ(fullQubit0, fullQubit0), scalarQ(2.0 + 0.0 * I, fullQubit0)) ? "true" : "false");
  printf("plusQ test1 : %s\n", qubitEqual(plusQ(fullQubit1, fullQubit1), scalarQ(2.0 + 0.0 * I, fullQubit1)) ? "true" : "false");
  printf("plusQ test2 : %s\n", plusQ(fullQubit0, fullQubit1) == NULL ? "true" : "false");

  printf("minusQ test0 : %s\n", qubitEqual(minusQ(fullQubit0, fullQubit0), scalarQ(0.0 + 0.0 * I, fullQubit0)) ? "true" : "false");
  printf("minusQ test1 : %s\n", qubitEqual(minusQ(fullQubit1, fullQubit1), scalarQ(0.0 + 0.0 * I, fullQubit1)) ? "true" : "false");
  printf("minusQ test2 : %s\n", minusQ(fullQubit0, fullQubit1) == NULL ? "true" : "false");

  printf("scalarQ test0 : %s\n", qubitEqual(scalarQ(2.0 + 0.0 * I, fullQubit0), scalarQRes2) ? "true" : "false");
  printf("scalarQ test1 : %s\n", qubitEqual(scalarQ(2.0 + 0.0 * I, fullQubit1), scalarQRes3) ? "true" : "false");
  printf("scalarQ test2 : %s\n", qubitEqual(scalarQ(0.0 + 0.0 * I, fullQubit0), scalarQRes0) ? "true" : "false");
  printf("scalarQ test3 : %s\n", qubitEqual(scalarQ(0.0 + 0.0 * I, fullQubit1), scalarQRes1) ? "true" : "false");

  rebalance_col(&unsortedMatrix0);
  rebalance_col(&postsortedMatrix0); //this is the singleton one
  printf("rebalance_col test0 : %s\n", matrixEqual(unsortedMatrix0, rebalanceColRes0) ? "true" : "false");
  printf("rebalance_col test1 : %s\n", matrixEqual(unsortedMatrix1, postsortedMatrix0) ? "true" : "false");
  rebalance_row(&unsortedMatrix0);
  rebalance_row(&postsortedMatrix0);
  printf("rebalance_row test0 : %s\n", matrixEqual(unsortedMatrix0, rebalanceRowRes0) ? "true" : "false");
  printf("rebalance_row test1 : %s\n", matrixEqual(unsortedMatrix1, postsortedMatrix0) ? "true" : "false");

  Matrix** calcSlotTest0 = (Matrix**)malloc(sizeof(Matrix*)*4);
  calcSlotTest0[0] = notG();
  calcSlotTest0[1] = notG();
  calcSlotTest0[2] = notG();
  calcSlotTest0[3] = notG();

  Qubit* calcSlotTest0Q = qubitInit(15, 4);

  Matrix** calcSlotTest1 = (Matrix**)malloc(sizeof(Matrix*)*3);
  calcSlotTest1[0] = notG();
  calcSlotTest1[1] = tensor(notG(), notG());
  calcSlotTest1[2] = notG();

  Qubit* calcSlotTest1Q = qubitInit(15, 4);

  Matrix** calcSlotTest2 = (Matrix**)malloc(sizeof(Matrix*)*4);
  calcSlotTest2[0] = hadamardG();
  calcSlotTest2[1] = hadamardG();
  calcSlotTest2[2] = hadamardG();
  calcSlotTest2[3] = hadamardG();

  Qubit* calcSlotTest2Q = qubitInit(15, 4);

  Matrix* bigHadamard = tensor(hadamardG(), hadamardG());
  bigHadamard = tensor(bigHadamard, hadamardG());
  bigHadamard = tensor(bigHadamard, hadamardG());

  Qubit* calcSlotTest2Res = multBitGate(bigHadamard, qubitInit(15, 4));

  calcSlot(&calcSlotTest0Q, calcSlotTest0);
  calcSlot(&calcSlotTest1Q, calcSlotTest1);
  calcSlot(&calcSlotTest2Q, calcSlotTest2);

  printf("calcSlot test0 : %s\n", qubitEqual(calcSlotTest0Q, qubitInit(0, 4)) ? "true" : "false");
  printf("calcSlot test1 : %s\n", qubitEqual(calcSlotTest1Q, qubitInit(0, 4)) ? "true" : "false");
  printf("calcSlot test2 : %s\n", qubitEqual(calcSlotTest2Q, calcSlotTest2Res) ? "true" : "false");

  return 0;
}
