#include "QLIT_lib.h"
#include "unitTest.h"

bool compMatrix(const Matrix* matrix1, const Matrix* matrix2){
  bool check = true;
  int numRows1 = matrix1->rows;
  int numCols1 = matrix1->cols;
  int numRows2 = matrix2->rows;
  int numCols2 = matrix2->cols;
  double complex **m1data = matrix1->data;
  double complex **m2data = matrix2->data;
  if(numRows1 == numRows2 && numCols1 == numCols2){
    for(int x = 0; x < numRows1; x++){
      for(int y = 0; y < numCols1; y++){
        if(m1data[x][y] != m2data[x][y]){
          check = false;
          break;
        }
      }
    }
  }else{
    check = false;
  }

  return check;
}

bool compQubit(const Qubit* qubit1, const Qubit* qubit2){
  bool check = true;
  int length1 = qubit1->len;
  int length2 = qubit2->len;
  double complex *q1data = qubit1->data;
  double complex *q2data = qubit2->data;
  if(length1 == length2){
    for(int n = 0; n < length1; n++){
      if(q1data[n] != q2data[n]){
        check = false;
        break;
      }
    }
  }else{
    check = false;
  }

  return check;
}

int unitTest() {
  double complex *multBitGateTest0Data = (double complex*)malloc(sizeof(double complex) * 1);
  multBitGateTest0Data[0] = 1.0 + 0.0 * I;

  Qubit* multBitGateTest0 = (Qubit*)malloc(sizeof (*multBitGateTest0));
  multBitGateTest0->len = 1;
  multBitGateTest0->data = multBitGateTest0Data;

  double complex *multBitGateTest1Data = (double complex*)malloc(sizeof(double complex) * 3);
  multBitGateTest1Data[0] = 1.0 + 0.0 * I;
  multBitGateTest1Data[1] = 1.0 + 0.0 * I;
  multBitGateTest1Data[2] = 1.0 + 0.0 * I;

  Qubit* multBitGateTest1 = (Qubit*)malloc(sizeof (*multBitGateTest1));
  multBitGateTest1->len = 3;
  multBitGateTest1->data = multBitGateTest1Data;

  double complex **multBitGateTest2Data = (double complex**)malloc(sizeof(double complex) * 1);
  multBitGateTest2Data[0] = (double complex*)malloc(sizeof(double complex) * 1);
  multBitGateTest2Data[0][0] = 9.0 + 0.0 * I;

  Matrix* multBitGateTest2 = (Matrix*)malloc(sizeof (*multBitGateTest2));
  multBitGateTest2->rows = 1;
  multBitGateTest2->cols = 1;
  multBitGateTest2->data = multBitGateTest2Data;

  double complex **scalarGTest0Data = (double complex**)malloc(sizeof(double complex) * 1);
  scalarGTest0Data[0] = (double complex*)malloc(sizeof(double complex) * 1);
  scalarGTest0Data[0][0] = 18.0 + 0.0 * I;

  Matrix* scalarGTest0 = (Matrix*)malloc(sizeof (*scalarGTest0));
  scalarGTest0->rows = 1;
  scalarGTest0->cols = 1;
  scalarGTest0->data = scalarGTest0Data;

  double complex **multGateBitTest0Data = (double complex**)malloc(sizeof(double complex) * 1);
  multGateBitTest0Data[0] = (double complex*)malloc(sizeof(double complex) * 3);

  multGateBitTest0Data[0][0] = 2.0 + 0.0 * I;
  multGateBitTest0Data[0][1] = 2.0 + 0.0 * I;
  multGateBitTest0Data[0][2] = 2.0 + 0.0 * I;

  Matrix* multGateBitTest0 = (Matrix*)malloc(sizeof (*multGateBitTest0));
  multGateBitTest0->cols = 3;
  multGateBitTest0->rows = 1;
  multGateBitTest0->data = multGateBitTest0Data;

  double complex **scalarGTest1Data = (double complex**)malloc(sizeof(double complex) * 1);
  scalarGTest1Data[0] = (double complex*)malloc(sizeof(double complex) * 3);

  scalarGTest1Data[0][0] = 1.0 + 0.0 * I;
  scalarGTest1Data[0][1] = 1.0 + 0.0 * I;
  scalarGTest1Data[0][2] = 1.0 + 0.0 * I;

  Matrix* scalarGTest1 = (Matrix*)malloc(sizeof (*scalarGTest1));
  scalarGTest1->cols = 3;
  scalarGTest1->rows = 1;
  scalarGTest1->data = scalarGTest1Data;

  /*-----------------------------------------------------------------------------------------------*/

  double complex **outerProductTest0DataRes = (double complex**)malloc(sizeof(double complex) * 1);
  outerProductTest0DataRes[0] = (double complex*)malloc(sizeof(double complex) * 3);

  outerProductTest0DataRes[0][0] = 2.0 + 0.0 * I;
  outerProductTest0DataRes[0][1] = 4.0 + 0.0 * I;
  outerProductTest0DataRes[0][2] = 6.0 + 0.0 * I;

  Matrix* outerProductTest0Res = (Matrix*)malloc(sizeof(*outerProductTest0Res));
  outerProductTest0Res->rows = 1;
  outerProductTest0Res->cols = 3;
  outerProductTest0Res->data = outerProductTest0DataRes;

  double complex **outerProductTest1DataRes = (double complex**)malloc(sizeof(double complex) * 3);
  outerProductTest1DataRes[0] = (double complex*)malloc(sizeof(double complex) * 1);
  outerProductTest1DataRes[1] = (double complex*)malloc(sizeof(double complex) * 1);
  outerProductTest1DataRes[2] = (double complex*)malloc(sizeof(double complex) * 1);

  outerProductTest1DataRes[0][0] = 2.0 + 0.0 * I;
  outerProductTest1DataRes[1][0] = 4.0 + 0.0 * I;
  outerProductTest1DataRes[2][0] = 6.0 + 0.0 * I;

  Matrix* outerProductTest1Res = (Matrix*)malloc(sizeof(*outerProductTest1Res));
  outerProductTest1Res->rows = 3;
  outerProductTest1Res->cols = 1;
  outerProductTest1Res->data = outerProductTest1DataRes;

  /*-----------------------------------------------------------------------------------------------*/

  double complex **multTest0Data = (double complex**)malloc(sizeof(double complex) * 2);
  multTest0Data[0] = (double complex*)malloc(sizeof(double complex) * 3);
  multTest0Data[1] = (double complex*)malloc(sizeof(double complex) * 3);

  multTest0Data[0][0] = 1.0 + 0.0 * I;
  multTest0Data[0][1] = 2.0 + 0.0 * I;
  multTest0Data[0][2] = 3.0 + 0.0 * I;
  multTest0Data[1][0] = 4.0 + 0.0 * I;
  multTest0Data[1][1] = 5.0 + 0.0 * I;
  multTest0Data[1][2] = 6.0 + 0.0 * I;

  Matrix* multTest0 = (Matrix*)malloc(sizeof(*multTest0));
  multTest0->rows = 2;
  multTest0->cols = 3;
  multTest0->data = multTest0Data;

  double complex **multTest1Data = (double complex**)malloc(sizeof(double complex) * 3);
  multTest1Data[0] = (double complex*)malloc(sizeof(double complex) * 1);
  multTest1Data[1] = (double complex*)malloc(sizeof(double complex) * 1);
  multTest1Data[2] = (double complex*)malloc(sizeof(double complex) * 1);

  multTest1Data[0][0] = 1.0 + 0.0 * I;
  multTest1Data[1][0] = 2.0 + 0.0 * I;
  multTest1Data[2][0] = 3.0 + 0.0 * I;

  Matrix* multTest1 = (Matrix*)malloc(sizeof(*multTest1));
  multTest1->rows = 3;
  multTest1->cols = 1;
  multTest1->data = multTest1Data;


  double complex **multTest2Data = (double complex**)malloc(sizeof(double complex) * 2);
  multTest2Data[0] = (double complex*)malloc(sizeof(double complex) * 1);
  multTest2Data[1] = (double complex*)malloc(sizeof(double complex) * 1);

  multTest2Data[0][0] = 1.0 + 0.0 * I;
  multTest2Data[1][0] = 2.0 + 0.0 * I;

  Matrix* multTest2 = (Matrix*)malloc(sizeof(*multTest2));
  multTest2->rows = 2;
  multTest2->cols = 1;
  multTest2->data = multTest2Data;

  double complex **multTest3Data = (double complex**)malloc(sizeof(double complex) * 1);
  multTest3Data[0] = (double complex*)malloc(sizeof(double complex) * 3);

  multTest3Data[0][0] = 1.0 + 0.0 * I;
  multTest3Data[0][1] = 2.0 + 0.0 * I;
  multTest3Data[0][2] = 3.0 + 0.0 * I;

  Matrix* multTest3 = (Matrix*)malloc(sizeof(*multTest3));
  multTest3->rows = 1;
  multTest3->cols = 3;
  multTest3->data = multTest3Data;

  double complex **multTest4Data = (double complex**)malloc(sizeof(double complex) * 2);
  multTest4Data[0] = (double complex*)malloc(sizeof(double complex) * 2);
  multTest4Data[1] = (double complex*)malloc(sizeof(double complex) * 2);

  multTest4Data[0][0] = 0.0 + 0.0 * I;
  multTest4Data[1][0] = 0.0 + 0.0 * I;
  multTest4Data[0][1] = 0.0 + 0.0 * I;
  multTest4Data[1][1] = 0.0 + 0.0 * I;

  Matrix* multTest4 = (Matrix*)malloc(sizeof(*multTest4));
  multTest4->rows = 2;
  multTest4->cols = 2;
  multTest4->data = multTest4Data;

  double complex **multTest0ResData = (double complex**)malloc(sizeof(double complex) * 2);
  multTest0ResData[0] = (double complex*)malloc(sizeof(double complex) * 1);
  multTest0ResData[1] = (double complex*)malloc(sizeof(double complex) * 1);

  multTest0ResData[0][0] = 14.0 + 0.0 * I;
  multTest0ResData[1][0] = 32.0 + 0.0 * I;

  Matrix* multTest0Res = (Matrix*)malloc(sizeof(*multTest0Res));
  multTest0Res->rows = 2;
  multTest0Res->cols = 1;
  multTest0Res->data = multTest0ResData;

  double complex **multTest1ResData = (double complex**)malloc(sizeof(double complex) * 2);
  multTest1ResData[0] = (double complex*)malloc(sizeof(double complex) * 3);
  multTest1ResData[1] = (double complex*)malloc(sizeof(double complex) * 3);

  multTest1ResData[0][0] = 1.0 + 0.0 * I;
  multTest1ResData[0][1] = 2.0 + 0.0 * I;
  multTest1ResData[0][2] = 3.0 + 0.0 * I;
  multTest1ResData[1][0] = 2.0 + 0.0 * I;
  multTest1ResData[1][1] = 4.0 + 0.0 * I;
  multTest1ResData[1][2] = 6.0 + 0.0 * I;

  Matrix* multTest1Res = (Matrix*)malloc(sizeof(*multTest1Res));
  multTest1Res->rows = 2;
  multTest1Res->cols = 3;
  multTest1Res->data = multTest1ResData;

  /*-----------------------------------------------------------------------------------------------*/

  double complex *multBitGateTest0ResData = (double complex*)malloc(sizeof(double complex) * 1);
  multBitGateTest0ResData[0] = 9.0 + 0.0 * I;

  Qubit* multBitGateTest0Res = (Qubit*)malloc(sizeof (*multBitGateTest0Res));
  multBitGateTest0Res->len = 1;
  multBitGateTest0Res->data = multBitGateTest0ResData;

  double complex *multBitGateTest1ResData = (double complex*)malloc(sizeof(double complex) * 1);
  multBitGateTest1ResData[0] = 6.0 + 0.0 * I;

  Qubit* multBitGateTest1Res = (Qubit*)malloc(sizeof (*multBitGateTest1Res));
  multBitGateTest1Res->len = 1;
  multBitGateTest1Res->data = multBitGateTest1ResData;

  /*-----------------------------------------------------------------------------------------------*/

  double complex **dataTensorTest0 = (double complex**)malloc(sizeof(double complex) * 3);
  for (int x = 0; x < 3; x++) {
    dataTensorTest0[x] = (double complex*)malloc(sizeof(double complex) * 1);
  }

  double complex **dataTensorTestRes = (double complex**)malloc(sizeof(double complex) * 3);
  for (int x = 0; x < 3; x++) {
    dataTensorTestRes[x] = (double complex*)malloc(sizeof(double complex) * 3);
  }

  double complex **dataTensorTest1 = (double complex**)malloc(sizeof(double complex) * 1);
  dataTensorTest1[0] = (double complex*)malloc(sizeof(double complex) * 3);

  dataTensorTest0[0][0] = 2.0 + 0.0 * I;
  dataTensorTest0[1][0] = 2.0 + 0.0 * I;
  dataTensorTest0[2][0] = 2.0 + 0.0 * I;

  dataTensorTest1[0][0] = 2.0 + 0.0 * I;
  dataTensorTest1[0][1] = 2.0 + 0.0 * I;
  dataTensorTest1[0][2] = 2.0 + 0.0 * I;

  dataTensorTestRes[0][0] = 4.0 + 0.0 * I;
  dataTensorTestRes[1][0] = 4.0 + 0.0 * I;
  dataTensorTestRes[2][0] = 4.0 + 0.0 * I;
  dataTensorTestRes[0][1] = 4.0 + 0.0 * I;
  dataTensorTestRes[1][1] = 4.0 + 0.0 * I;
  dataTensorTestRes[2][1] = 4.0 + 0.0 * I;
  dataTensorTestRes[0][2] = 4.0 + 0.0 * I;
  dataTensorTestRes[1][2] = 4.0 + 0.0 * I;
  dataTensorTestRes[2][2] = 4.0 + 0.0 * I;

  Matrix* tensorTestRes = (Matrix*)malloc(sizeof (*tensorTestRes));
  tensorTestRes->rows = 3;
  tensorTestRes->cols = 3;
  tensorTestRes->data = dataTensorTestRes;

  Matrix* tensorTest0 = (Matrix*)malloc(sizeof (*tensorTest0));
  tensorTest0->rows = 3;
  tensorTest0->cols = 1;
  tensorTest0->data = dataTensorTest0;

  Matrix* tensorTest1 = (Matrix*)malloc(sizeof (*tensorTest1));
  tensorTest1->rows = 1;
  tensorTest1->cols = 3;
  tensorTest1->data = dataTensorTest1;

  Matrix* tensorTest0_run = tensor(tensorTest0, tensorTest1);
  Matrix* tensorTest1_run = tensor(tensorTest1, tensorTest0);

  printf("Tensor test0 : %s\n", compMatrix(tensorTest0_run, tensorTestRes) ? "true" : "false");
  printf("Tensor test1 : %s\n", compMatrix(tensorTest1_run, tensorTestRes) ? "true" : "false");

  dataTensorTest0[0][0] = 1.0 + 0.0 * I;
  dataTensorTest0[1][0] = 2.0 + 0.0 * I;
  dataTensorTest0[2][0] = 3.0 + 0.0 * I;

  dataTensorTest1[0][0] = 1.0 + 0.0 * I;
  dataTensorTest1[0][1] = 2.0 + 0.0 * I;
  dataTensorTest1[0][2] = 3.0 + 0.0 * I;

  tensorTest0->data = dataTensorTest0;
  tensorTest1->data = dataTensorTest1;

  Matrix* tensorTest2_run = tensor(tensorTest0, tensorTest1);
  Matrix* tensorTest3_run = tensor(tensorTest1, tensorTest0);

  dataTensorTestRes[0][0] = 1.0 + 0.0 * I;
  dataTensorTestRes[1][0] = 2.0 + 0.0 * I;
  dataTensorTestRes[2][0] = 3.0 + 0.0 * I;
  dataTensorTestRes[0][1] = 2.0 + 0.0 * I;
  dataTensorTestRes[1][1] = 4.0 + 0.0 * I;
  dataTensorTestRes[2][1] = 6.0 + 0.0 * I;
  dataTensorTestRes[0][2] = 3.0 + 0.0 * I;
  dataTensorTestRes[1][2] = 6.0 + 0.0 * I;
  dataTensorTestRes[2][2] = 9.0 + 0.0 * I;
  tensorTestRes->data = dataTensorTestRes;

  printf("Tensor test2 : %s\n", compMatrix(tensorTest2_run, tensorTestRes) ? "true" : "false");
  printf("Tensor test3 : %s\n", compMatrix(tensorTest3_run, tensorTestRes) ? "true" : "false");

  Qubit* multBitGateTest0_run = multBitGate(multBitGateTest2, multBitGateTest0);
  Qubit* multBitGateTest1_run = multBitGate(tensorTest1, multBitGateTest1);

  printf("multBitGate test0 : %s\n", compQubit(multBitGateTest0_run, multBitGateTest0Res) ? "true" : "false");
  printf("multBitGate test1 : %s\n", compQubit(multBitGateTest1_run, multBitGateTest1Res) ? "true" : "false");

  Matrix* scalarGTest0_run = scalarG(multBitGateTest2, 2.0 + 0.0 * I);
  Matrix* scalarGTest1_run = scalarG(scalarGTest1, 2.0 + 0.0 * I);

  printf("scalarG test0 : %s\n", compMatrix(scalarGTest0, scalarGTest0_run) ? "true" : "false");
  printf("scalarG test1 : %s\n", compMatrix(multGateBitTest0, scalarGTest1_run) ? "true" : "false");

  multBitGateTest1Data[0] = 1.0 + 0.0 * I;
  multBitGateTest1Data[1] = 2.0 + 0.0 * I;
  multBitGateTest1Data[2] = 3.0 + 0.0 * I;

  multBitGateTest0Data[0] = 2.0 + 0.0 * I;
  multBitGateTest0->data = multBitGateTest0Data;
  multBitGateTest1->data = multBitGateTest1Data;

  Matrix* outerProductTest0_run = outerProduct(multBitGateTest0, multBitGateTest1);
  Matrix* outerProductTest1_run = outerProduct(multBitGateTest1, multBitGateTest0);

  printf("outerProduct test0 : %s\n", compMatrix(outerProductTest0_run, outerProductTest0Res) ? "true" : "false");
  printf("outerProduct test1 : %s\n", compMatrix(outerProductTest1_run, outerProductTest1Res) ? "true" : "false");

  Matrix* plusTest0_run = plusG(tensorTestRes, tensorTestRes);

  dataTensorTestRes[0][0] = 2.0 + 0.0 * I;
  dataTensorTestRes[1][0] = 4.0 + 0.0 * I;
  dataTensorTestRes[2][0] = 6.0 + 0.0 * I;
  dataTensorTestRes[0][1] = 4.0 + 0.0 * I;
  dataTensorTestRes[1][1] = 8.0 + 0.0 * I;
  dataTensorTestRes[2][1] = 12.0 + 0.0 * I;
  dataTensorTestRes[0][2] = 6.0 + 0.0 * I;
  dataTensorTestRes[1][2] = 12.0 + 0.0 * I;
  dataTensorTestRes[2][2] = 18.0 + 0.0 * I;
  tensorTestRes->data = dataTensorTestRes;

  printf("plusG test0 : %s\n", compMatrix(plusTest0_run, tensorTestRes) ? "true" : "false");
  printf("plusG test1 : %s\n", plusG(tensorTest0, tensorTestRes) == NULL ? "true" : "false");

  Matrix* minusTest0_run = minusG(tensorTestRes, tensorTestRes);

  printf("minusG test0 : %s\n", compMatrix(minusTest0_run, matrixInit(3,3)) ? "true" : "false");
  printf("minusG test1 : %s\n", minusG(tensorTest0, tensorTestRes) == NULL ? "true" : "false");

  Matrix* multTest0_run = multG(multTest0, multTest1);
  Matrix* multTest1_run = multG(multTest2, multTest3);

  printf("multG test0 : %s\n", compMatrix(multTest0_run, multTest0Res) ? "true" : "false");
  printf("multG test1 : %s\n", compMatrix(multTest1_run, multTest1Res) ? "true" : "false");
  printf("multG test2 : %s\n", multG(multTest3, multTest4) == NULL ? "true" : "false");

  /*-----------------------------------------------------------------------------------------------*/

  double complex **controlGenTestData = (double complex**)malloc(sizeof(double complex) * 4);
  controlGenTestData[0] = (double complex*)malloc(sizeof(double complex) * 4);
  controlGenTestData[1] = (double complex*)malloc(sizeof(double complex) * 4);
  controlGenTestData[2] = (double complex*)malloc(sizeof(double complex) * 4);
  controlGenTestData[3] = (double complex*)malloc(sizeof(double complex) * 4);

  controlGenTestData[0][0] = 1.0 + 0.0 * I;
  controlGenTestData[1][0] = 0.0 + 0.0 * I;
  controlGenTestData[2][0] = 0.0 + 0.0 * I;
  controlGenTestData[3][0] = 0.0 + 0.0 * I;
  controlGenTestData[0][1] = 0.0 + 0.0 * I;
  controlGenTestData[1][1] = 1.0 + 0.0 * I;
  controlGenTestData[2][1] = 0.0 + 0.0 * I;
  controlGenTestData[3][1] = 0.0 + 0.0 * I;
  controlGenTestData[0][2] = 0.0 + 0.0 * I;
  controlGenTestData[1][2] = 0.0 + 0.0 * I;
  controlGenTestData[2][2] = 0.0 + 0.0 * I;
  controlGenTestData[3][2] = 1.0 + 0.0 * I;
  controlGenTestData[0][3] = 0.0 + 0.0 * I;
  controlGenTestData[1][3] = 0.0 + 0.0 * I;
  controlGenTestData[2][3] = 1.0 + 0.0 * I;
  controlGenTestData[3][3] = 0.0 + 0.0 * I;
  Matrix* controlGenTest = (Matrix*)malloc(sizeof(Matrix*));
  controlGenTest->data = controlGenTestData;
  controlGenTest->rows = 4;
  controlGenTest->cols = 4;
  
  printf("controlGen test : %s\n", compMatrix(controlGenTest, controlGen(0,1,not)) ? "true" : "false");

  /*-----------------------------------------------------------------------------------------------*/

  Matrix* h = hadamardG();
  Qubit* observeTest0 = qubitInit(0,3);
  Matrix* observeTest0H = tensor(h, h);
  observeTest0H = tensor(observeTest0H, h);
  observeTest0 = multBitGate(observeTest0H, observeTest0);
  
  Qubit* plusTestQ0 = (Qubit*)malloc(sizeof(Qubit));
  double complex* plusTestQ0Data = (double complex*)malloc(sizeof(double complex));
  plusTestQ0Data[0] = 1.0 + 0.0 * I;
  plusTestQ0->data = plusTestQ0Data;
  plusTestQ0->len = 1;

  Qubit* plusTestQ1 = (Qubit*)malloc(sizeof(Qubit));
  double complex* plusTestQ1Data = (double complex*)malloc(sizeof(double complex) * 3);
  plusTestQ1Data[0] = 1.0 + 0.0 * I;
  plusTestQ1Data[1] = 2.0 + 0.0 * I;
  plusTestQ1Data[2] = 3.0 + 0.0 * I;
  plusTestQ1->data = plusTestQ1Data;
  plusTestQ1->len = 3;
  
  Qubit* scalarQTestRes0 = (Qubit*)malloc(sizeof(Qubit));
  double complex* scalarQTestRes0Data = (double complex*)malloc(sizeof(double complex));
  scalarQTestRes0Data[0] = 2.0 + 0.0 * I;
  scalarQTestRes0->data = scalarQTestRes0Data;
  scalarQTestRes0->len = 1;

  Qubit* scalarQTestRes1 = (Qubit*)malloc(sizeof(Qubit));
  double complex* scalarQTestRes1Data = (double complex*)malloc(sizeof(double complex) * 3);
  scalarQTestRes1Data[0] = 2.0 + 0.0 * I;
  scalarQTestRes1Data[1] = 4.0 + 0.0 * I;
  scalarQTestRes1Data[2] = 6.0 + 0.0 * I;
  scalarQTestRes1->data = scalarQTestRes1Data;
  scalarQTestRes1->len = 3;

  Qubit* scalarQTestRes2 = (Qubit*)malloc(sizeof(Qubit));
  double complex* scalarQTestRes2Data = (double complex*)malloc(sizeof(double complex));
  scalarQTestRes2Data[0] = 0.0 + 0.0 * I;
  scalarQTestRes2->data = scalarQTestRes2Data;
  scalarQTestRes2->len = 1;

  Qubit* scalarQTestRes3 = (Qubit*)malloc(sizeof(Qubit));
  double complex* scalarQTestRes3Data = (double complex*)malloc(sizeof(double complex) * 3);
  scalarQTestRes3Data[0] = 0.0 + 0.0 * I;
  scalarQTestRes3Data[1] = 0.0 + 0.0 * I;
  scalarQTestRes3Data[2] = 0.0 + 0.0 * I;
  scalarQTestRes3->data = scalarQTestRes3Data;
  scalarQTestRes3->len = 3;

  Qubit* plusTestQ0_run = plusQ(plusTestQ0, plusTestQ0);
  Qubit* plusTestQ1_run = plusQ(plusTestQ1, plusTestQ1);
  
  printf("plusQ test0 : %s\n", compQubit(plusTestQ0_run, scalarQ(plusTestQ0, 2.0 + 0.0 * I)) ? "true" : "false");
  printf("plusQ test1 : %s\n", compQubit(plusTestQ1_run, scalarQ(plusTestQ1, 2.0 + 0.0 * I)) ? "true" : "false");
  printf("plusQ test2 : %s\n", plusQ(plusTestQ1, plusTestQ0) == NULL ? "true" : "false");

  Qubit* minusTestQ0_run = minusQ(plusTestQ0, plusTestQ0);
  Qubit* minusTestQ1_run = minusQ(plusTestQ1, plusTestQ1);

  printf("minusQ test0 : %s\n", compQubit(minusTestQ0_run, scalarQ(plusTestQ0, 0.0 + 0.0 * I)) ? "true" : "false");
  printf("minusQ test1 : %s\n", compQubit(minusTestQ1_run, scalarQ(plusTestQ1, 0.0 + 0.0 * I)) ? "true" : "false");
  printf("minusQ test2 : %s\n", minusQ(plusTestQ1, plusTestQ0) == NULL ? "true" : "false");

  Qubit* scalarQTest0_run = scalarQ(plusTestQ0, 2.0 + 0.0 * I);
  Qubit* scalarQTest1_run = scalarQ(plusTestQ1, 2.0 + 0.0 * I);
  Qubit* scalarQTest2_run = scalarQ(plusTestQ0, 0.0 + 0.0 * I);
  Qubit* scalarQTest3_run = scalarQ(plusTestQ1, 0.0 + 0.0 * I);

  printf("scalarQ test0 : %s\n", compQubit(scalarQTest0_run, scalarQTestRes0) ? "true" : "false");
  printf("scalarQ test1 : %s\n", compQubit(scalarQTest1_run, scalarQTestRes1) ? "true" : "false");
  printf("scalarQ test2 : %s\n", compQubit(scalarQTest2_run, scalarQTestRes2) ? "true" : "false");
  printf("scalarQ test3 : %s\n", compQubit(scalarQTest3_run, scalarQTestRes3) ? "true" : "false");

  double complex innerProductTest0_run = innerProduct(multBitGateTest0, multBitGateTest0);
  double complex innerProductTest1_run = innerProduct(multBitGateTest1, multBitGateTest1);

  printf("innerProduct test0 : %s\n", (innerProductTest0_run == 4.0 + 0.0 * I) ? "true" : "false");
  printf("innerProduct test1 : %s\n", (innerProductTest1_run == 14.0 + 0.0 * I) ? "true" : "false");
  double complex innerProductTest2_run = innerProduct(multBitGateTest0, multBitGateTest1);
  printf("innerProduct test2 : %s\n", (innerProductTest2_run == -1.0 - 1.0 * I) ? "true" : "false");

  return 0;
}
