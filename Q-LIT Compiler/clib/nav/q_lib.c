#include "q_lib.h"
#include <sys/time.h>

const double complex one = 1.0 + 0.0 * I;
const double complex zero = 0.0 + 0.0 * I;

Qubit* qubitInit(int value, int length){
#ifdef RUNTIME
  struct timeval t1, t2;
  double elapsedTime;

  // start timer
  gettimeofday(&t1, NULL);
#endif
  if(length == 0){

#ifdef RUNTIME
    // stop timer
    gettimeofday(&t2, NULL);

    // compute and print the elapsed time in millisec
    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
    printf("delta time for qubitInit %f ms.\n", elapsedTime);
#endif
    return NULL;
  }

  int size = (int)pow(2, length);
  double complex *res = (double complex*)malloc(sizeof(double complex) * size);
  for(int n = 0; n < size; n++){
    res[n] = zero;
  }
  res[value] = one;
  Qubit* q = (Qubit*)malloc(sizeof (*q));
  q->len = size;
  q->data = res;

#ifdef RUNTIME
  // stop timer
  gettimeofday(&t2, NULL);

  // compute and print the elapsed time in millisec
  elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
  elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
  printf("delta time for qubitInit %f ms.\n", elapsedTime);
#endif
  return q;
} 

Matrix* matrixInit(int col, int row) {
#ifdef RUNTIME
  struct timeval t1, t2;
  double elapsedTime;

  // start timer
  gettimeofday(&t1, NULL);
#endif
  if(col == 0 || row == 0){
#ifdef RUNTIME
    // stop timer
    gettimeofday(&t2, NULL);

    // compute and print the elapsed time in millisec
    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
    printf("delta time for matrixInit %f ms.\n", elapsedTime);
#endif
    return NULL;
  }
  
  double complex **data = (double complex**)malloc(sizeof(double complex) * row);
  for (int x = 0; x < row; x++) {
    data[x] = (double complex*)malloc(sizeof(double complex) * col);
  }
  for(int x = 0; x < row; x++){
    for(int y = 0; y < col; y++){
      data[x][y] = zero;
    }
  }
  Matrix* m = (Matrix*)malloc(sizeof (*m));
  m->rows = row;
  m->cols = col;
  m->data = data;
#ifdef RUNTIME
  // stop timer
  gettimeofday(&t2, NULL);

  // compute and print the elapsed time in millisec
  elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
  elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
  printf("delta time for matrixInit %f ms.\n", elapsedTime);
#endif
  return m;
}

int freeMatrix(Matrix* m) {
  for (int x = 0; x < m->rows; x++) {
    free(m->data[x]);
  }
  free(m->data);
  free(m);
  return 0;
}

int freeQubit(Qubit* q) {
  free(q->data);
  free(q);
  return 0;
}

void printM(const Matrix* m) {
  int cols = m->cols;
  int rows = m->rows;
  double complex** data = m->data;
  for (int x = 0; x < rows; x++) {
    for (int y = 0; y < cols; y++) {
      printf("%.2f + %.2fi\t", creal(data[x][y]), cimag(data[x][y]));
    }
    printf("\n");
  }
}

void printQ(const Qubit* q) {
  int len = q->len;
  double complex* data = q->data;
  for (int n = 0; n < len; n++) {
    printf("%.2f + %.2fi\t", creal(data[n]), cimag(data[n]));
    printf("\n");
  }
}

void outputQ(char* name, const Qubit* q) {
  FILE* fp;

  fp = fopen(name,"w+");
  fprintf(fp,"Result qubit vector:\n");
  int len = q->len;
  double complex* data = q->data;
  for (int n = 0; n < len; n++) {
    fprintf(fp,"%.2f + %.2fi\n", creal(data[n]), cimag(data[n]));
  }
  fclose(fp);
}

Matrix* tensor(const Matrix* gate1, const Matrix* gate2) {
#ifdef RUNTIME
  struct timeval t1, t2;
  double elapsedTime;

  // start timer
  gettimeofday(&t1, NULL);
#endif
  int numRows1 = gate1->rows;
  int numCols1 = gate1->cols;
  int numRows2 = gate2->rows;
  int numCols2 = gate2->cols;
  int rows = numRows1 * numRows2;
  int cols = numCols1 * numCols2;
  double complex **g1data = gate1->data;
  double complex **g2data = gate2->data;

  double complex **res = (double complex**)malloc(sizeof(double complex) * rows);
  for(int n = 0; n < rows; n++){
    res[n] = (double complex*)malloc(sizeof(double complex) * cols);
  }
  for(int g1x = 0; g1x < numRows1; g1x++){
    for(int g1y = 0; g1y < numCols1; g1y++){
      for(int g2x = 0; g2x < numRows2; g2x++){
        for(int g2y = 0; g2y < numCols2; g2y++){
          res[g2x+(numRows2*g1x)][g2y+(numCols2*g1y)] = 
            g1data[g1x][g1y] * g2data[g2x][g2y];
        }
      }
    }
  }
  Matrix* m = (Matrix*)malloc(sizeof (*m));
  m->cols = cols;
  m->rows = rows;
  m->data = res;
#ifdef RUNTIME
  // stop timer
  gettimeofday(&t2, NULL);

  // compute and print the elapsed time in millisec
  elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
  elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
  printf("delta time for tensor %f ms.\n", elapsedTime);
#endif
  return m;
}

Qubit* multBitGate(const Matrix* gate, const Qubit* qubit) {
#ifdef RUNTIME
  struct timeval t1, t2;
  double elapsedTime;

  // start timer
  gettimeofday(&t1, NULL);
#endif
  int numRows = gate->rows;
  int qubitLen = qubit->len;
  double complex **gdata = gate->data;
  double complex *qdata = qubit->data;
  double complex *res = (double complex*)malloc(sizeof(double complex) * qubitLen);

  for(int x = 0; x < numRows; x++){
    for(int y = 0; y < qubitLen; y++){
      res[x] = res[x] + (qdata[y] * gdata[x][y]);
    }
  }

  Qubit* q = (Qubit*)malloc (sizeof (*q));;
  q->len = gate->rows;
  q->data = res;
#ifdef RUNTIME
  // stop timer
  gettimeofday(&t2, NULL);

  // compute and print the elapsed time in millisec
  elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
  elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
  printf("delta time for multBitGate %f ms.\n", elapsedTime);
#endif
  return q;
}

double complex innerProduct(const Qubit* qubit1, const Qubit* qubit2) {
#ifdef RUNTIME
  struct timeval t1, t2;
  double elapsedTime;

  // start timer
  gettimeofday(&t1, NULL);
#endif
  int qubitLen1 = qubit1->len;
  int qubitLen2 = qubit2->len;
  double complex *qdata1 = qubit1->data;
  double complex *qdata2 = qubit2->data;
  
  if(qubitLen1 == qubitLen2){
    double complex res = zero;
    for(int n = 0; n < qubitLen1; n++){
      res = res + (qdata1[n] * qdata2[n]);
    }
#ifdef RUNTIME
    // stop timer
    gettimeofday(&t2, NULL);

    // compute and print the elapsed time in millisec
    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
    printf("delta time for innerProduct %f ms.\n", elapsedTime);
#endif
    return res;
  }
#ifdef RUNTIME
  // stop timer
  gettimeofday(&t2, NULL);

  // compute and print the elapsed time in millisec
  elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
  elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
  printf("delta time for innerProduct %f ms.\n", elapsedTime);
#endif
  fprintf(stderr, "Incompatible qubits for inner product! Aborting...\n");
  exit(EXIT_FAILURE);
}

Matrix* outerProduct(const Qubit* qubit1, const Qubit* qubit2) {
#ifdef RUNTIME
  struct timeval t1, t2;
  double elapsedTime;

  // start timer
  gettimeofday(&t1, NULL);
#endif
  int qubitLen1 = qubit1->len;
  int qubitLen2 = qubit2->len;
  double complex *qdata1 = qubit1->data;
  double complex *qdata2 = qubit2->data;
  
  double complex **res = (double complex**)malloc(sizeof(double complex) * qubitLen1);
  for(int n = 0; n < qubitLen1; n++){
    res[n] = (double complex*)malloc(sizeof(double complex) * qubitLen2);
  }

  for(int x = 0; x < qubitLen1; x++){
    for(int y = 0; y < qubitLen2; y++){
      res[x][y] = qdata1[x] * qdata2[y];
    }
  }
  Matrix* m = (Matrix*)malloc(sizeof (*m));
  m->cols = qubitLen2;
  m->rows = qubitLen1;
  m->data = res;
#ifdef RUNTIME
  // stop timer
  gettimeofday(&t2, NULL);

  // compute and print the elapsed time in millisec
  elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
  elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
  printf("delta time for outerProduct %f ms.\n", elapsedTime);
#endif
  return m;
}

Matrix* scalarG(Matrix* gate, double complex scalar){
#ifdef RUNTIME
  struct timeval t1, t2;
  double elapsedTime;

  // start timer
  gettimeofday(&t1, NULL);
#endif
  int numRows = gate->rows;
  int numCols = gate->cols;
  Matrix* m = matrixInit(numCols, numRows);
  double complex **gdata = m->data;

  for(int x = 0; x<numRows; x++){
    for(int y = 0; y<numCols; y++){
      gdata[x][y] = gate->data[x][y] * scalar;
    }
  }

  m->data = gdata;
#ifdef RUNTIME
  // stop timer
  gettimeofday(&t2, NULL);

  // compute and print the elapsed time in millisec
  elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
  elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
  printf("delta time for scalarG %f ms.\n", elapsedTime);
#endif
  return m;
}


Qubit* scalarQ(Qubit* qubit, double complex scalar){
#ifdef RUNTIME
  struct timeval t1, t2;
  double elapsedTime;

  // start timer
  gettimeofday(&t1, NULL);
#endif
  int length = qubit->len;
  double complex *qdata = (double complex*)malloc(sizeof(double complex)*length);

  for(int n = 0; n<length; n++){
    qdata[n] = qubit->data[n] * scalar;
  }
  
  Qubit* q = (Qubit*)malloc(sizeof (*q));
  q->len = length;
  q->data = qdata;
#ifdef RUNTIME
  // stop timer
  gettimeofday(&t2, NULL);

  // compute and print the elapsed time in millisec
  elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
  elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
  printf("delta time for scalarQ %f ms.\n", elapsedTime);
#endif
  return q;
}

Matrix* plusG(const Matrix* gate1, const Matrix* gate2) {
#ifdef RUNTIME
  struct timeval t1, t2;
  double elapsedTime;

  // start timer
  gettimeofday(&t1, NULL);
#endif
  int numRows = gate1->rows;
  int numCols = gate1->cols;
  double complex **g1data = gate1->data;
  double complex **g2data = gate2->data;
  double complex **res = (double complex**)malloc(sizeof(double complex) * numRows);
  
  if(numRows != gate2->rows || numCols != gate2->cols){
#ifdef RUNTIME
    // stop timer
    gettimeofday(&t2, NULL);

    // compute and print the elapsed time in millisec
    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
    printf("delta time for plusG %f ms.\n", elapsedTime);
#endif
    return NULL;
  }

  for(int n = 0; n < numRows; n++){
    res[n] = (double complex*)malloc(sizeof(double complex) * numCols);
  }
  for(int x = 0; x < numRows; x++){
    for(int y = 0; y < numCols; y++){
      res[x][y] = g1data[x][y] + g2data[x][y];
    }
  }
  Matrix* m = (Matrix*)malloc(sizeof (*m));
  m->cols = numRows;
  m->rows = numCols;
  m->data = res;
#ifdef RUNTIME
  // stop timer
  gettimeofday(&t2, NULL);

  // compute and print the elapsed time in millisec
  elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
  elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
  printf("delta time for plusG %f ms.\n", elapsedTime);
#endif
  return m;
}

Matrix* minusG(const Matrix* gate1, const Matrix* gate2) {
#ifdef RUNTIME
  struct timeval t1, t2;
  double elapsedTime;

  // start timer
  gettimeofday(&t1, NULL);
#endif
  int numRows = gate1->rows;
  int numCols = gate1->cols;
  double complex **g1data = gate1->data;
  double complex **g2data = gate2->data;
  double complex **res = (double complex**)malloc(sizeof(double complex) * numRows);
  
  if(numRows != gate2->rows || numCols != gate2->cols){
#ifdef RUNTIME
    // stop timer
    gettimeofday(&t2, NULL);

    // compute and print the elapsed time in millisec
    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
    printf("delta time for minusG %f ms.\n", elapsedTime);
#endif
    return NULL;
  }

  for(int n = 0; n < numRows; n++){
    res[n] = (double complex*)malloc(sizeof(double complex) * numCols);
  }
  for(int x = 0; x < numRows; x++){
    for(int y = 0; y < numCols; y++){
      res[x][y] = g1data[x][y] - g2data[x][y];
    }
  }
  Matrix* m = (Matrix*)malloc(sizeof (*m));
  m->cols = numRows;
  m->rows = numCols;
  m->data = res;
#ifdef RUNTIME
  // stop timer
  gettimeofday(&t2, NULL);

  // compute and print the elapsed time in millisec
  elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
  elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
  printf("delta time for minusG %f ms.\n", elapsedTime);
#endif
  return m;
}

Matrix* multG(const Matrix* gate1, const Matrix* gate2) {
#ifdef RUNTIME
  struct timeval t1, t2;
  double elapsedTime;

  // start timer
  gettimeofday(&t1, NULL);
#endif
  int numRows1 = gate1->rows;
  int numCols1 = gate1->cols;
  double complex **g1data = gate1->data;
  int numRows2= gate2->rows;
  int numCols2 = gate2->cols;
  double complex **g2data = gate2->data;

  if (numRows2 == numCols1){
    double complex **res = (double complex**)malloc(sizeof(double complex) * (numRows1));
    for(int n = 0; n < numRows1; n++){
      res[n] = (double complex*)malloc(sizeof(double complex) * (numCols2));;
    }
    for (int x = 0; x < numRows1; x++){
      for (int y = 0; y < numCols2; y++){
        double complex sum = zero;
        for (int n = 0; n < numCols1; n++){
          sum = sum + (g1data[x][n] * g2data[n][y]);
        }
        res[x][y] = sum;
      }
    }
    Matrix* m = (Matrix*)malloc (sizeof (*m));;
    m->rows = numRows1;
    m->cols = numCols2;
    m->data = res;
#ifdef RUNTIME
    // stop timer
    gettimeofday(&t2, NULL);

    // compute and print the elapsed time in millisec
    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
    printf("delta time for multG %f ms.\n", elapsedTime);
#endif
    return m;
  }
#ifdef RUNTIME
  // stop timer
  gettimeofday(&t2, NULL);

  // compute and print the elapsed time in millisec
  elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
  elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
  printf("delta time for multG %f ms.\n", elapsedTime);
#endif
  return NULL;
}

Qubit* plusQ(const Qubit* qubit1, const Qubit* qubit2){
#ifdef RUNTIME
  struct timeval t1, t2;
  double elapsedTime;

  // start timer
  gettimeofday(&t1, NULL);
#endif
  int len1 = qubit1->len;
  int len2 = qubit2->len;
  if(len1 != len2){
#ifdef RUNTIME
    // stop timer
    gettimeofday(&t2, NULL);

    // compute and print the elapsed time in millisec
    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
    printf("delta time for plusQ %f ms.\n", elapsedTime);
#endif
    return NULL;
  }

  double complex *data1 = qubit1->data;
  double complex *data2 = qubit2->data;
  double complex *res = malloc(sizeof(double complex) * len1);

  for(int n = 0; n < len1; n++){
    res[n] = data1[n] + data2[n];
  }
  Qubit* q = (Qubit*)malloc (sizeof (*q));;
  q->len = len1;
  q->data = res;
#ifdef RUNTIME
  // stop timer
  gettimeofday(&t2, NULL);

  // compute and print the elapsed time in millisec
  elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
  elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
  printf("delta time for plusQ %f ms.\n", elapsedTime);
#endif
  return q;
}

Qubit* minusQ(const Qubit* qubit1, const Qubit* qubit2){
#ifdef RUNTIME
  struct timeval t1, t2;
  double elapsedTime;

  // start timer
  gettimeofday(&t1, NULL);
#endif
  int len1 = qubit1->len;
  int len2 = qubit2->len;
  if(len1 != len2){
#ifdef RUNTIME
    // stop timer
    gettimeofday(&t2, NULL);

    // compute and print the elapsed time in millisec
    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
    printf("delta time for minusQ %f ms.\n", elapsedTime);
#endif
    return NULL;
  }

  double complex *data1 = qubit1->data;
  double complex *data2 = qubit2->data;
  double complex *res = malloc(sizeof(double complex) * len1);

  for(int n = 0; n < len1; n++){
    res[n] = data1[n] - data2[n];
  }
  Qubit* q = (Qubit*)malloc (sizeof (*q));;
  q->len = len1;
  q->data = res;
#ifdef RUNTIME
  // stop timer
  gettimeofday(&t2, NULL);

  // compute and print the elapsed time in millisec
  elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
  elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
  printf("delta time for minusQ %f ms.\n", elapsedTime);
#endif
  return q;
}

Qubit* measurement(const Qubit* qubit, int bitSig){
#ifdef RUNTIME
  struct timeval t1, t2;
  double elapsedTime;

  // start timer
  gettimeofday(&t1, NULL);
#endif
  int length = qubit->len;
  double complex *content = qubit->data;
  double *buff = malloc(sizeof(double) * length);

  if(fmod(log2(length), 1.0) != 0.0){
#ifdef RUNTIME
    // stop timer
    gettimeofday(&t2, NULL);

    // compute and print the elapsed time in millisec
    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
    printf("delta time for measurement %f ms.\n", elapsedTime);
#endif
    return NULL;
  }

  for(int n = 0; n < length; n++){
    if(creal(content[n]) == 1.0){
#ifdef RUNTIME
      // stop timer
      gettimeofday(&t2, NULL);

      // compute and print the elapsed time in millisec
      elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
      elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
      printf("delta time for measurement %f ms.\n", elapsedTime);
#endif
      return NULL;
    }
    buff[n] = pow(sqrt(pow(creal(content[n]),2)+pow(cimag(content[n]),2)),2);
  }

  srand(time(NULL));
  double pick = (double)rand() / (double)RAND_MAX;

  for(int n = 0; n < length; n++){
    if(buff[n] >= pick){
      bool k = ((n % (int)pow(2, bitSig))+1) > ((int)pow(2, bitSig)/2);
      for(int m = 0; m < length; m++){
        if(k == (((m % (int)pow(2, bitSig))+1) > ((int)pow(2, bitSig)/2))){
          if(content[m] != zero){
            content[m] = (content[m]/content[m]) * pow((1/sqrt(2)), log2(length)-1);
          }

          if(cimag(content[m]) == 0.0 * I){
            content[m] = creal(content[m]) + 0.0 * I;
          }
        }else{
          content[m] = zero;
        }
      }
      break;
    }else{
      pick -= buff[n];
    }
  }
  
  Qubit* q = (Qubit*)malloc(sizeof (*q));
  q->len = length;
  q->data = content;
#ifdef RUNTIME
  // stop timer
  gettimeofday(&t2, NULL);

  // compute and print the elapsed time in millisec
  elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
  elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
  printf("delta time for measurement %f ms.\n", elapsedTime);
#endif
  return q;
}

Matrix* controlGen(int target, int control, Matrix* op){
#ifdef RUNTIME
  struct timeval t1, t2;
  double elapsedTime;

  // start timer
  gettimeofday(&t1, NULL);
#endif
  int s = pow(2,1+control-target);
  double complex **res = malloc(sizeof(double complex)* s);
  for (int x = 0; x < s; x++) {
    res[x] = (double complex*)malloc(sizeof(double complex) * s);
  }
  int block_size = s / 2;
  for (int x = 0; x < s; x++){
    for (int y = 0; y < s; y++){
      if (x == y && x < block_size){
        res[x][y] = 1.0 + 0.0 * I;
      }
      else if (y >= block_size && x >= block_size && y == x && x % 2 == 0){
        res[x][y] = op->data[0][0];
        res[x][y+1] = op->data[0][1];
        y++;
      }
      else if (y >= block_size && x >= block_size && y == x && x % 2 == 1){
        res[x][y] = op->data[1][1];
        res[x][y-1] = op->data[1][0];
      }
    }
  }
  Matrix* m = (Matrix*)malloc(sizeof (*m));
  m->rows = s;
  m->cols = s;
  m->data = res;
#ifdef RUNTIME
  // stop timer
  gettimeofday(&t2, NULL);

  // compute and print the elapsed time in millisec
  elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
  elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
  printf("delta time for controlGen %f ms.\n", elapsedTime);
#endif
  return m;
}
