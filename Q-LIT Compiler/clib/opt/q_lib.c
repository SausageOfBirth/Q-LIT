#include "q_lib.h"
#include <sys/time.h>

const double complex one = 1.0 + 0.0 * I;
const double complex zero = 0.0 + 0.0 * I;

Qubit* qubitInit(int value, int length){
#ifdef RUNTIME
  struct timeval t1, t2;
  double elapsedTime;

  /* start timer */
  gettimeofday(&t1, NULL);
#endif
  if(length == 0 || value >= (int)pow(2, length)){
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
  /* stop timer */
  gettimeofday(&t2, NULL);

  /* compute and print the elapsed time in millisec */
  elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
  elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
  printf("delta time for qubitInit %f ms.\n", elapsedTime);
#endif
  return q;
}

tuple* genTuple(int x, int y, double complex val){
  tuple* tmpRes = (tuple*)malloc(sizeof(tuple));
  tmpRes->x = x;
  tmpRes->y = y;
  tmpRes->val = val;
  return tmpRes;
}

void printQ(Qubit* q) {
  if(q == NULL || q->data == NULL){
    printf("Qubit not print-able\n");
    return;
  }
  int len = q->len;
  double complex* data = q->data;
  for (int n = 0; n < len; n++) {
    printf("%.2f + %.2fi\n", creal(data[n]), cimag(data[n]));
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
  
/* prints the coordinates of all non-zero values in a matrix */
void printTuple(Matrix* m){
  for(int n = 0; n < m->valsize; n++){
    printf("(%d,%d)  ", m->data[n]->x, m->data[n]->y);
  }
  printf("\n");
}

/* prints all the actual non-zero values in a matrix */
void printVal(Matrix* m){
  for(int n = 0; n < m->valsize; n++){
    printf("%.2f + %.2fi\t", creal(m->data[n]->val), cimag(m->data[n]->val));
  }
  printf("\n");
}

int freeMatrix(Matrix* m){
  for(int n = 0; n < m->valsize; n++){
    free(m->data[n]);
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

/* recursive innards of rebalance, based around, hell, exactly quicksort.
 * sub_start and sub_end are the variables needed to sort through the
 * sub-arrays quicksort creates, using the wall and pivot. */
void rebalance_col_inner(Matrix** m, int sub_start, int sub_end){
  if(sub_end - sub_start < 2){
    return;
  }

  int pivot = sub_end - 1;
  int wall = sub_start-1;
  tuple* buff;
  for(int n = sub_start; n < sub_end; n++){
    if((*m)->data[n]->y < (*m)->data[pivot]->y){
      buff = (*m)->data[n];
      (*m)->data[n] = (*m)->data[wall+1];
      (*m)->data[wall+1] = buff;
      wall += 1;

    }else if ((*m)->data[n]->y == (*m)->data[pivot]->y){
      if((*m)->data[n]->x < (*m)->data[pivot]->x){
        buff = (*m)->data[n];
        (*m)->data[n] = (*m)->data[wall+1];
        (*m)->data[wall+1] = buff;
        wall += 1;
      }
    }
  }

  buff = (*m)->data[pivot];
  (*m)->data[pivot] = (*m)->data[wall+1];
  (*m)->data[wall+1] = buff;
  wall += 1;
  rebalance_col_inner(&(*m), sub_start, wall);
  rebalance_col_inner(&(*m), wall, sub_end);
}

/* this does what the above function does, but as you can see, we're
 * more concerned with matching on, and sorting around x than y,
 * which is what makes this row sorted, and above column sorted */
void rebalance_row_inner(Matrix** m, int sub_start, int sub_end){
  if(sub_end - sub_start < 2){
    return;
  }

  int pivot = sub_end - 1;
  int wall = sub_start-1;
  tuple* buff;
  for(int n = sub_start; n < sub_end; n++){
    if((*m)->data[n]->x < (*m)->data[pivot]->x){
      buff = (*m)->data[n];
      (*m)->data[n] = (*m)->data[wall+1];
      (*m)->data[wall+1] = buff;
      wall += 1;
    }else if ((*m)->data[n]->x == (*m)->data[pivot]->x){
      if((*m)->data[n]->y < (*m)->data[pivot]->y){
        buff = (*m)->data[n];
        (*m)->data[n] = (*m)->data[wall+1];
        (*m)->data[wall+1] = buff;
        wall += 1;
      }
    }
  }
  buff = (*m)->data[pivot];
  (*m)->data[pivot] = (*m)->data[wall+1];
  (*m)->data[wall+1] = buff;
  wall += 1;
  rebalance_row_inner(&(*m), sub_start, wall);
  rebalance_row_inner(&(*m), wall, sub_end);
}

/* nice wrappers for rebalance, so we don't have to set inital
 * values for the recursive innards. */
void rebalance_col(Matrix** m){
  struct timeval t1, t2;
  double elapsedTime;

  /* start timer */
  gettimeofday(&t1, NULL);

  rebalance_col_inner(&(*m), 0, (*m)->valsize);

  /* stop timer */
  gettimeofday(&t2, NULL);

  /* compute and print the elapsed time in millisec */
  elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
  elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
  printf("delta time for rebalance_col %f ms.\n", elapsedTime);
}

void rebalance_row(Matrix** m){
#ifdef RUNTIME
  struct timeval t1, t2;
  double elapsedTime;

  /* start timer */
  gettimeofday(&t1, NULL);
#endif

  rebalance_row_inner(&(*m), 0, (*m)->valsize);
#ifdef RUNTIME
  /* stop timer */
  gettimeofday(&t2, NULL);

  /* compute and print the elapsed time in millisec */
  elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
  elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
  printf("delta time for rebalance_row %f ms.\n", elapsedTime);
#endif
}

Qubit* multBitGate(Matrix* m, Qubit* q) {
#ifdef RUNTIME
  struct timeval t1, t2;
  double elapsedTime;

  /* start timer */
  gettimeofday(&t1, NULL);
#endif
  if(q->len == m->col){
    int n = 0;
    int x = 0;
    int y = 0;
    int currentX = 0;
    double complex buff = zero;
    double complex *data = (double complex*)malloc(sizeof(double complex) * q->len);
    while(n < q->len && x < m->valsize){
      while(n < q->len && x < m->valsize && m->data[x]->x == currentX){
        if(y < m->data[x]->y){
          y += 1;
        }else if(y > m->data[x]->y){
          x += 1;
        }else{
          buff = buff + q->data[y] * m->data[x]->val;
          x += 1;
          y += 1;
        }
      }
      data[n] = buff;
      buff = zero;
      n += 1;
      currentX += 1;
      y = 0;
    }
    Qubit* res = (Qubit*)malloc(sizeof (*res));
    res->len = n;
    res->data = data;
#ifdef RUNTIME
    /* stop timer */
    gettimeofday(&t2, NULL);

    /* compute and print the elapsed time in millisec */
    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
    printf("delta time for multBitGate %f ms.\n", elapsedTime);
#endif
    return res;
  }else{
#ifdef RUNTIME
    /* stop timer */
    gettimeofday(&t2, NULL);

    /* compute and print the elapsed time in millisec */
    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
    printf("delta time for multBitGate %f ms.\n", elapsedTime);
#endif
    return NULL;
  }
}

void calcSlot(Qubit** q, Matrix** mArray){
#ifdef RUNTIME
  struct timeval t1, t2;
  double elapsedTime;

  /* start timer */
  gettimeofday(&t1, NULL);
#endif

  double complex buff = zero;
  double complex* tempQ = (double complex*)malloc(sizeof(double complex) * (*q)->len);
  for(int x = 0; x < (*q)->len; x++){
    buff = calcRow(q, mArray, 0, x, 0, (*q)->len/mArray[0]->row, one);
    tempQ[x] = buff;
    buff = zero;
  }
  (*q)->data = memcpy((*q)->data, tempQ, sizeof(double complex)*(*q)->len);
  free(tempQ);

#ifdef RUNTIME
  /* stop timer */
  gettimeofday(&t2, NULL);

  /* compute and print the elapsed time in millisec */
  elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
  elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms

  printf("delta time for calcSlot %f ms.\n", elapsedTime);
#endif
}

/* Calculates a row using a sparse method. Recursively goes through values
 * for row, filtering the ones it needs at the moment while keeping tabs with
 * the qubit index it needs to multiply with. */
double complex calcRow(Qubit** q, Matrix** mArray, int gateNum, int qubitIndex, int relQ, int blockSize, double complex tensorRes) {
  /* Narrow the given index down to one of the blocks */
  int relativeIndex = qubitIndex / blockSize;
  double complex buff = zero;

  for (int i = 0; i < mArray[gateNum]->valsize; i++){
    if (mArray[gateNum]->data[i]->x == relativeIndex) {

      /* Update relative qubit index for the row value that was found */
      if (blockSize == 1){
        /* We are at the last gate, resolve calculation */
        buff += (tensorRes * mArray[gateNum]->data[i]->val) * 
                (*q)->data[relQ + mArray[gateNum]->data[i]->y];
      } else {
        /* Recalibrate qubitIndex to fit nested block */
        int nQubitIndex = qubitIndex % blockSize;

        /* relQ is set based on which block we enter and block size,
         * next block size is set from the dimensions of NEXT block */
        buff += calcRow(q, mArray, gateNum+1, nQubitIndex, 
                        relQ+(blockSize*mArray[gateNum]->data[i]->y), 
                        blockSize / mArray[gateNum+1]->col, 
                        tensorRes * mArray[gateNum]->data[i]->val);
      }
    }
  }
  return buff;
}

/* Calculates inner product between two qubit vectors  */
double complex innerProduct(Qubit* qubit1, Qubit* qubit2){
#ifdef RUNTIME
  struct timeval t1, t2;
  double elapsedTime;

  /* start timer */
  gettimeofday(&t1, NULL);
#endif
  if(qubit1->len == qubit2->len){
    double complex res = zero;
    for(int n = 0; n < qubit1->len; n++){
      res = res + (qubit1->data[n] * qubit2->data[n]);
    }

#ifdef RUNTIME
    /* stop timer */
    gettimeofday(&t2, NULL);

    /* compute and print the elapsed time in millisec */
    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
    printf("delta time for innerProduct %f ms.\n", elapsedTime);
#endif
    return res;
  }
#ifdef RUNTIME
  /* stop timer */
  gettimeofday(&t2, NULL);

  /* compute and print the elapsed time in millisec */
  elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
  elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
  printf("delta time for innerProduct %f ms.\n", elapsedTime);
#endif
  fprintf(stderr, "Incompatible qubits for inner product! Aborting...\n");
  exit(EXIT_FAILURE);
}

Matrix* outerProduct(Qubit* qubit1, Qubit* qubit2){
#ifdef RUNTIME
  struct timeval t1, t2;
  double elapsedTime;

  /* start timer */
  gettimeofday(&t1, NULL);
#endif

  tuple** resdata = (tuple**)malloc(sizeof(tuple*) * (qubit1->len + qubit2->len));
  int n = 0;
  for(int x = 0; x < qubit1->len; x++){
    for(int y = 0; y < qubit2->len; y++){
      if(qubit1->data[x] * qubit2->data[y] != zero){
        resdata[n] = genTuple(x, y, qubit1->data[x] * qubit2->data[y]);
        n += 1;
      }
    }
  }
  tuple** resShort = (tuple**)malloc(sizeof(tuple*) * n);
  resShort = memcpy(resShort, resdata, sizeof(tuple*) * n);
  free(resdata);
  Matrix* m = (Matrix*)malloc(sizeof (*m));
  m->col = qubit2->len;
  m->row = qubit1->len;
  m->valsize = n;
  m->data = resShort;

#ifdef RUNTIME
  /* stop timer */
  gettimeofday(&t2, NULL);

  /* compute and print the elapsed time in millisec */
  elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
  elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
  printf("delta time for outerProduct %f ms.\n", elapsedTime);
#endif
  return m;
}

Matrix* scalarG(Matrix* m, double complex s){
#ifdef RUNTIME
  struct timeval t1, t2;
  double elapsedTime;

  /* start timer */
  gettimeofday(&t1, NULL);
#endif
  
  if(s == zero){
    tuple** resdata = (tuple**)malloc(sizeof(tuple*));
    resdata[0] = genTuple(0, 0, zero);
    Matrix* res = (Matrix*)malloc(sizeof (*m));
    res->col = m->col;
    res->row = m->row;
    res->valsize = 1;
    res->data = resdata;

#ifdef RUNTIME
    /* stop timer */
    gettimeofday(&t2, NULL);
    /* compute and print the elapsed time in millisec */
    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
    printf("delta time for scalarG %f ms.\n", elapsedTime);
#endif
    return res;
  }

  tuple** resdata = (tuple**)malloc(sizeof(tuple*) * m->valsize);
  for(int n = 0; n<m->valsize; n++){
    tuple* tmpRes = (tuple*)malloc(sizeof(tuple));
    tmpRes->x = m->data[n]->x;
    tmpRes->y = m->data[n]->y;
    tmpRes->val = m->data[n]->val * s;
    resdata[n] = tmpRes;
  }
  Matrix* res = (Matrix*)malloc(sizeof(Matrix));
  res->col = m->col;
  res->row = m->row;
  res->valsize = m->valsize;
  res->data = resdata;

#ifdef RUNTIME
  /* stop timer */
  gettimeofday(&t2, NULL);

  /* compute and print the elapsed time in millisec */
  elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
  elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
  printf("delta time for scalarG %f ms.\n", elapsedTime);
#endif

  return res;
}

Qubit* scalarQ(Qubit* q, double complex s){
#ifdef RUNTIME
  struct timeval t1, t2;
  double elapsedTime;

  /* start timer */
  gettimeofday(&t1, NULL);
#endif

  double complex *qdata = (double complex*)malloc(sizeof(double complex) * q->len);

  for(int n = 0; n<q->len; n++){
    qdata[n] = q->data[n] * s;
  }
  Qubit* res = (Qubit*)malloc(sizeof(Qubit));
  res->len = q->len;
  res->data = qdata;

#ifdef RUNTIME
  /* stop timer */
  gettimeofday(&t2, NULL);

  /* compute and print the elapsed time in millisec */
  elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
  elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
  printf("delta time for scalarQ %f ms.\n", elapsedTime);
#endif
  return res;
}

Qubit* plusQ(Qubit* q1, Qubit* q2){
#ifdef RUNTIME
  struct timeval t1, t2;
  double elapsedTime;

  /* start timer */
  gettimeofday(&t1, NULL);
#endif
  if(q1->len != q2->len){

#ifdef RUNTIME
    /* stop timer */
    gettimeofday(&t2, NULL);

    /* compute and print the elapsed time in millisec */
    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
    printf("delta time for plusQ %f ms.\n", elapsedTime);
#endif
    return NULL;
  }
  double complex *res = (double complex*)malloc(sizeof(double complex) * q1->len);

  for(int n = 0; n < q1->len; n++){
    res[n] = q1->data[n] + q2->data[n];
  }
  Qubit* q = (Qubit*)malloc (sizeof (*q));;
  q->len = q1->len;
  q->data = res;

#ifdef RUNTIME
  /* stop timer */
  gettimeofday(&t2, NULL);

  /* compute and print the elapsed time in millisec */
  elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
  elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
  printf("delta time for plusQ %f ms.\n", elapsedTime);
#endif
  return q;
}

Qubit* minusQ(Qubit* q1, Qubit* q2){
  struct timeval t1, t2;
  double elapsedTime;

#ifdef RUNTIME
  /* start timer */
  gettimeofday(&t1, NULL);
#endif
  if(q1->len != q2->len){

#ifdef RUNTIME
    /* stop timer */
    gettimeofday(&t2, NULL);

    /* compute and print the elapsed time in millisec */
    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
    printf("delta time for minusQ %f ms.\n", elapsedTime);
#endif
    return NULL;
  }

  double complex *res = malloc(sizeof(double complex) * q1->len);

  for(int n = 0; n < q1->len; n++){
    res[n] = q1->data[n] - q2->data[n];
  }
  Qubit* q = (Qubit*)malloc (sizeof (*q));;
  q->len = q1->len;
  q->data = res;

#ifdef RUNTIME
  /* stop timer */
  gettimeofday(&t2, NULL);

  /* compute and print the elapsed time in millisec */
  elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
  elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
  printf("delta time for minusQ %f ms.\n", elapsedTime);
#endif
  return q;
}

Matrix* multG(Matrix* m1, Matrix* m2){
  struct timeval t1, t2;
  double elapsedTime;

#ifdef RUNTIME
  /* start timer */
  gettimeofday(&t1, NULL);
#endif
  if(m1->col == m2->row){
    tuple** resdata = (tuple**)malloc(sizeof(tuple*) * (m1->valsize * m2->valsize));
    /* Indexes for getting values in matrix 1 and 2 */
    int x = 0;
    int y = 0;
    /* The current row (xWall) and column (yWall) */
    int xWall = 0;
    int yWall = 0;
    /* Counter for going back to start of row */
    int xBottom = 0;
    /* Number element in result matrix */
    int n = 0;
    rebalance_row(&m1);
    rebalance_col(&m2);
    /* Last of rows and cols with actual values */
    int lastRow = m1->data[m1->valsize-1]->x;
    int lastCol = m2->data[m2->valsize-1]->y;
    double complex buff = zero;

    /* Needed for checking bounds when we are at end of value list */
    bool xbound = false;
    bool ybound = false;

    /* Keep going until all rows and all columns are handled */
    while(xWall <= lastRow && yWall <= lastCol){
      /* printf("do we ever loop?\n"); */
      /* Check if we are still in bounds of the row and column number */
      while(x < m1->valsize &&
            y < m2->valsize &&
            m1->data[x]->x == xWall &&
            m2->data[y]->y == yWall){
        /* Check if coordinates match, else try to find two that do */
        if(m1->data[x]->y > m2->data[y]->x){
          /* printf("the y value was behind!\n"); */
          y += 1;
        }else if(m1->data[x]->y < m2->data[y]->x){
          x += 1;
        }else{
          /*
          printf("we multiply %.2f+%.2f*i with %.2f+%.2f*i\nat x %d and y %d\n",
                 creal(m1->data[x]->val), cimag(m1->data[x]->val), 
                 creal(m2->data[y]->val), cimag(m2->data[y]->val), x, y); */
          buff = buff + (m1->data[x]->val * m2->data[y]->val);
          x += 1;
          y += 1;
        }
      }

      /* Stop looping immediately when we have hit last element */
      if(x == m1->valsize && y == m2->valsize){
        tuple* tmpRes = genTuple(xWall,yWall,buff);
        resdata[n] = tmpRes;
        n += 1;
        break;
      }

      /* restep back to last element if we went over (for indexing) */
      if(x >= m1->valsize){
        x = m1->valsize-1;
        xbound = true;
      }
      if(y >= m2->valsize){
        y = m2->valsize-1;
        ybound = true;
      }

      /* Have we found an x which is outside the bounds? */
      if(xWall < m1->data[x]->x || xbound){
        if (buff != zero){
          /* Create object and insert */
          tuple* tmpRes = genTuple(xWall,yWall,buff);
          resdata[n] = tmpRes;
          n += 1;
          buff = zero;
        }
        xbound = false;
        /* Go back to start of row for next column iteration if end of row */
        if(yWall == lastCol){
          yWall = 0;
          y = 0;
          xBottom = x;
          xWall += 1;
        }else{
          /* Go to next row iteration */
          yWall += 1;
          /* all y after that point are unnecessary, fast forward */
          if(m2->data[y]->y != yWall && yWall <= lastCol){
            while(m2->data[y]->y < yWall){
              y += 1;
            }
          }
          x = xBottom;
        }
      /* Have we found a y which is outside the bounds? */
      }else if(yWall < m2->data[y]->y || ybound){
        if(buff != zero){
          tuple* tmpRes = genTuple(xWall,yWall,buff);
          resdata[n] = tmpRes;
          n += 1;
          buff = zero;
        }
        ybound = false;
        /* Go back to start of row for next column iteration if end of row */
        if(yWall == lastCol){
          yWall = 0;
          y = 0;
          xWall += 1;
          /* all x after that point are unnecessary, fast forward */
          if(m1->data[x]->x != xWall && xWall <= lastRow){
            while(m1->data[x]->x < xWall){
              x += 1;
            }
          }
          xBottom = x;
        }else{
          /* Go to next row iteration */
          yWall += 1;
          x = xBottom;
        }
      }
    }
    rebalance_row(&m2);
    tuple** resShort = (tuple**)malloc(sizeof(tuple*) * n);
    resShort = (tuple**)memcpy(resShort, resdata, sizeof(tuple*) * n);
    /* free(resdata); */
    Matrix* res = (Matrix*)malloc(sizeof(Matrix));
    res->col = m2->col;
    res->row = m1->row;
    res->valsize = n;
    res->data = resShort;

#ifdef RUNTIME
    /* stop timer */
    gettimeofday(&t2, NULL);

    /* compute and print the elapsed time in millisec */
    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
    printf("delta time for multG %f ms.\n", elapsedTime);
#endif
    return res;
  }else{

#ifdef RUNTIME
    /* stop timer */
    gettimeofday(&t2, NULL);

    /* compute and print the elapsed time in millisec */
    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
    printf("delta time for multG %f ms.\n", elapsedTime);
#endif
    return NULL;
  }
}

Matrix* plusG(Matrix* m1, Matrix* m2){
#ifdef RUNTIME
  struct timeval t1, t2;
  double elapsedTime;

  /* start timer */
  gettimeofday(&t1, NULL);
#endif
  if(m1->row == m2->row && m1->col == m2->col){
    tuple** resdata = (tuple**)malloc(sizeof(tuple*) * (m1->valsize + m2->valsize));
    int x = 0;
    int y = 0;
    int n = 0;
    while(x < m1->valsize && y < m2->valsize){
      if(m1->data[x]->x > m2->data[y]->x || m1->data[x]->y > m2->data[y]->y){
        resdata[n] = m2->data[y];
        y += 1;
        n += 1;
      }else if (m1->data[x]->x < m2->data[y]->x || m1->data[x]->y < m2->data[y]->y){
        resdata[n] = m1->data[x];
        x += 1;
        n += 1;
      }else{
        if((m1->data[x]->val + m2->data[y]->val) == zero){
          x += 1;
          y += 1;
        }else{
          tuple* tmpRes = (tuple*)malloc(sizeof(tuple));
          tmpRes->x = m1->data[x]->x;
          tmpRes->y = m1->data[x]->y;
          tmpRes->val = m1->data[x]->val + m2->data[y]->val;
          resdata[n] = tmpRes;
          x += 1;
          y += 1;
          n += 1;
        }
      }
    }
    while(x < m1->valsize){
      resdata[n] = m1->data[x];
      n += 1;
      x += 1;
    }
    while(y < m2->valsize){
      resdata[n] = m2->data[y];
      n += 1;
      y += 1;
    }
    tuple** resShort;
    if(n != 0){
      resShort = (tuple**)malloc(sizeof(tuple*) * n);
      resShort = (tuple**)memcpy(resShort, resdata, sizeof(tuple*) * n);
    }else{
      resShort = (tuple**)malloc(sizeof(tuple*));
      resShort[0] = genTuple(0, 0, zero);
      n = 1;
    }
    free(resdata);
    Matrix* res = (Matrix*)malloc(sizeof(Matrix));
    res->col = m1->col;
    res->row = m1->row;
    res->valsize = n;
    res->data = resShort;

#ifdef RUNTIME
    /* stop timer */
    gettimeofday(&t2, NULL);

    /* compute and print the elapsed time in millisec */
    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
    printf("delta time for plusG %f ms.\n", elapsedTime);
#endif
    return res;
  }else{

#ifdef RUNTIME
    /* stop timer */
    gettimeofday(&t2, NULL);

    /* compute and print the elapsed time in millisec */
    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
    printf("delta time for plusG %f ms.\n", elapsedTime);
#endif
    return NULL;
  }
}

Matrix* minusG(Matrix* m1, Matrix* m2){
#ifdef RUNTIME
  struct timeval t1, t2;
  double elapsedTime;

  /* start timer */
  gettimeofday(&t1, NULL);
#endif
  if(m1->row == m2->row && m1->col == m2->col){
    tuple** resdata = (tuple**)malloc(sizeof(tuple*) * (m1->valsize + m2->valsize));
    int x = 0;
    int y = 0;
    int n = 0;
    while(x < m1->valsize && y < m2->valsize){
      if(m1->data[x]->x > m2->data[y]->x || m1->data[x]->y > m2->data[y]->y){
        resdata[n] = m2->data[y];
        resdata[n]->val = -1 * creal(m2->data[y]->val) + cimag(m2->data[y]->val) * I;
        y += 1;
        n += 1;
      }else if (m1->data[x]->x < m2->data[y]->x || m1->data[x]->y < m2->data[y]->y){
        resdata[n] = m1->data[x];
        x += 1;
        n += 1;
      }else{
        if((m1->data[x]->val - m2->data[y]->val) == zero){
          x += 1;
          y += 1;
        }else{
          tuple* tmpRes = (tuple*)malloc(sizeof(tuple));
          tmpRes->x = m1->data[x]->x;
          tmpRes->y = m1->data[x]->y;
          tmpRes->val = m1->data[x]->val - m2->data[y]->val;
          resdata[n] = tmpRes;
          x += 1;
          y += 1;
          n += 1;
        }
      }
    }
    while(x < m1->valsize){
      resdata[n] = m1->data[x];
      n += 1;
      x += 1;
    }
    while(y < m2->valsize){
      resdata[n] = m2->data[y];
      resdata[n]->val = -1 * creal(m2->data[y]->val) + cimag(m2->data[y]->val) * I;
      n += 1;
      y += 1;
    }
    tuple** resShort;
    if(n != 0){
      resShort = (tuple**)malloc(sizeof(tuple*) * n);
      resShort = (tuple**)memcpy(resShort, resdata, sizeof(tuple*) * n);
    }else{
      resShort = (tuple**)malloc(sizeof(tuple*));
      resShort[0] = genTuple(0, 0, zero);
      n = 1;
    }
    free(resdata);
    Matrix* res = (Matrix*)malloc(sizeof(Matrix));
    res->col = m1->col;
    res->row = m1->row;
    res->valsize = n;
    res->data = resShort;

#ifdef RUNTIME
    /* stop timer */
    gettimeofday(&t2, NULL);

    /* compute and print the elapsed time in millisec */
    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
    printf("delta time for minusG %f ms.\n", elapsedTime);
#endif
    return res;
  }else{

#ifdef RUNTIME
    /* stop timer */
    gettimeofday(&t2, NULL);

    /* compute and print the elapsed time in millisec */
    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
    printf("delta time for minusG %f ms.\n", elapsedTime);
#endif
    return NULL;
  }
}

Matrix* tensor(Matrix* m1, Matrix* m2){
  struct timeval t1, t2;
  double elapsedTime;

#ifdef RUNTIME
  /* start timer */
  gettimeofday(&t1, NULL);
#endif
  /* since tensor only ever multiplies, zero is our only problem, 
     and since we don't include zeroes, we have no need to copy 
     memory. */
  int rN = 0;
  tuple** resdata = (tuple**)malloc(sizeof(tuple*) * m1->valsize * m2->valsize);
  for(int n = 0; n < m1->valsize; n++){
    for(int m = 0; m < m2->valsize; m++){
      tuple* resTmp = genTuple(0, 0, zero);
      resTmp->x = m2->data[m]->x + (m2->row * m1->data[n]->x);
      resTmp->y = m2->data[m]->y + (m2->col * m1->data[n]->y);
      resTmp->val = m1->data[n]->val * m2->data[m]->val;
      resdata[rN] = resTmp;
      rN += 1;
    }
  }
  Matrix* res = (Matrix*)malloc(sizeof(Matrix));
  res->col = m1->col * m2->col;
  res->row = m1->row * m2->row;
  res->valsize = rN;
  res->data = resdata;
  rebalance_row(&res);

#ifdef RUNTIME
  /* stop timer */
  gettimeofday(&t2, NULL);

  /* compute and print the elapsed time in millisec */
  elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
  elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
  printf("delta time for tensor %f ms.\n", elapsedTime);
#endif
  return res;
}

/* measures directly into the qubit given, it returns nothing
   because it directly alters the input. Remember to
   de-associate the qubit with & when using this. */
void measurement(Qubit** qubit, int bitSig){
#ifdef RUNTIME
  struct timeval t1, t2;
  double elapsedTime;

  /* start timer */
  gettimeofday(&t1, NULL);
#endif
  int length = (*qubit)->len;

  if(fmod(log2(length), 1.0) != 0.0){
#ifdef RUTIME
    /* stop timer */
    gettimeofday(&t2, NULL);

    /* compute and print the elapsed time in millisec */
    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
    printf("delta time for measurement %f ms.\n", elapsedTime);
#endif
    return;
  }

  srand(time(NULL));
  double pick = (double)rand() / (double)RAND_MAX;

  for(int n = 0; n < length; n++){
    pick -= pow(sqrt(pow(creal((*qubit)->data[n]),2)+pow(cimag((*qubit)->data[n]),2)),2);

    if(pow(sqrt(pow(creal((*qubit)->data[n]),2)+pow(cimag((*qubit)->data[n]),2)),2) >= pick){
      bool k = ((n % (int)pow(2, bitSig))+1) > ((int)pow(2, bitSig)/2);

      for(int m = 0; m < length; m++){
        if(k == (((m % (int)pow(2, bitSig))+1) > ((int)pow(2, bitSig)/2))){
          if((*qubit)->data[m] != zero){
            (*qubit)->data[m] = ((*qubit)->data[m]/(*qubit)->data[m]) * pow((1/sqrt(2)), log2(length)-1);
          }

          if(cimag((*qubit)->data[m]) == 0.0 * I){
            (*qubit)->data[m] = creal((*qubit)->data[m]) + 0.0 * I;
          }
        }else{
          (*qubit)->data[m] = zero;
        }
      }
      break;
    }
  }

#ifdef RUNTIME
  /* stop timer */
  gettimeofday(&t2, NULL);

  /* compute and print the elapsed time in millisec */
  elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
  elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
  printf("delta time for measurement %f ms.\n", elapsedTime);
#endif
}

Matrix* controlGen(int control, int target, Matrix* op){
#ifdef RUNTIME
  struct timeval t1, t2;
  double elapsedTime;

  /* start timer */
  gettimeofday(&t1, NULL);
#endif
  double complex fill[4] = {0.0 + 0.0 * I,
                            0.0 + 0.0 * I,
                            0.0 + 0.0 * I,
                            0.0 + 0.0 * I};
  for (int i = 0; i < op->valsize; i++){
    fill[(op->data[i]->x * 2) + op->data[i]->y] = op->data[i]->val; 
  }
  int s = pow(2,1+target-control);
  int block_size = s / 2;
  tuple** resdata = (tuple**)malloc(sizeof(tuple*) * (s * 2));
  
  int n = 0;
  for (int x = 0; x < s; x++){
    if (x < block_size) {
      resdata[n] = genTuple(x,x,one);
      n++;
    }
    else if (x % 2 == 0){
      if (fill[0] != 0.0 + 0.0 * I) {
        resdata[n] = genTuple(x,x,fill[0]);
        n++;
      }
      if (fill[1] != 0.0 + 0.0 * I) {
        resdata[n] = genTuple(x,x+1,fill[1]);
        n++;
      }
    }
    else {
      if (fill[2] != 0.0 + 0.0 * I) {
        resdata[n] = genTuple(x,x-1,fill[2]);
        n++;
      }
      if (fill[3] != 0.0 + 0.0 * I) {
        resdata[n] = genTuple(x,x,fill[3]);
        n++;
      }
    }
  }
  Matrix* res = (Matrix*)malloc(sizeof(Matrix));
  res->col = s;
  res->row = s;
  res->valsize = n;
  res->data = resdata;
#ifdef RUNTIME
  /* stop timer */
  gettimeofday(&t2, NULL);

  /* compute and print the elapsed time in millisec */
  elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
  elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
  printf("delta time for controlGen %f ms.\n", elapsedTime);
#endif
  return res;
}
