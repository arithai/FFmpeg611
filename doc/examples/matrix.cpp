#include <math.h>
#include "matrix.h"
vector_t *vector_new(uint8_t dim)
{
  if (dim == 0)
  {
    printf("Wrong number of dimensions\n\r");
    return NULL;
  }
  vector_t *v = (vector_t *)calloc(1, sizeof(*v));
  v->dim = dim;
  v->data = (double*)calloc(v->dim, sizeof(*v->data));
  return v;
}
vector_t *ivector_new(double u1,double u2)
{
  vector_t *v = (vector_t *)calloc(1, sizeof(*v));
  v->dim = 2;
  v->data = (double*)calloc(2, sizeof(*v->data));
  v->data[0]=u1; v->data[1]=u2;
  return v;
}
vector_t *ivector6_new(double x1,double x2,double x3,
                       double x4,double x5,double x6) 
{
  vector_t *v = (vector_t *)calloc(1, sizeof(*v));
  v->dim = 6;
  v->data = (double*)calloc(6, sizeof(*v->data));
  v->data[0]=x1; v->data[1]=x2; v->data[2]=x3;
  v->data[3]=x4; v->data[4]=x5; v->data[5]=x6;
  return v;
}
vector_t *ivector3_new(double x1,double x2,double x3) 
{
  vector_t *v = (vector_t *)calloc(1, sizeof(*v));
  v->dim = 3;
  v->data = (double*)calloc(3, sizeof(*v->data));
  v->data[0]=x1; v->data[1]=x2; v->data[2]=x3;
  return v;
}
matrix_t *matrix_new(uint8_t rows, uint8_t cols)
{
  if (cols == 0 || rows == 0)
  {
    printf("Invalid Parameters od Matrix\n\r");
    return NULL;
  }
  matrix_t *m = (matrix_t *)calloc(1, sizeof(*m));
  m->cols = cols;
  m->rows = rows;
  m->is_square = (cols == rows) ? 1 : 0;
  m->data = (double**) calloc(m->rows, sizeof(*m->data));
  for (uint8_t i = 0; i < m->rows; ++i)
  {
    m->data[i] = (double*)calloc(m->cols, sizeof(**m->data));
  }
  return m;
}
vector_t *vector_copy(vector_t *v)
{
  if (v == NULL)
  {
    printf("vector_copy: VECTOR IS NULL\n");
    return NULL;
  }
  vector_t *newm = vector_new(v->dim);
  for (uint8_t i = 0; i < v->dim; i++)
  {
    newm->data[i] = v->data[i];
  }
  return newm;
}
vector_t *vector_eye(uint8_t dim)
{
  vector_t *newm = vector_new(dim);
  for (uint8_t i = 0; i < dim; i++)
  {
    newm->data[i] = 1.0;
  }
  return newm;
}
matrix_t *matrix_sqr_zero(uint8_t size)
{
  matrix_t *m = matrix_new(size, size);
  return m; 
}
matrix_t *matrix_eye(uint8_t size)
{
  matrix_t *m = matrix_new(size, size);
  for (uint8_t i = 0; i < m->cols; i++)
  {
    m->data[i][i] = 1.0;
  }
  return m;
}
matrix_t *matrix_copy(matrix_t *m)
{
  matrix_t *newm = matrix_new(m->rows, m->cols);
  for (uint8_t i = 0; i < m->rows; i++)
  {
    for (uint8_t j = 0; j < m->cols; j++)
    {
      newm->data[i][j] = m->data[i][j];
    }
  }
  return newm;
}
void matrix_copy3x3(matrix_t *m,double A3x3[3][3]) {
  for (uint8_t i = 0; i < 3; i++)
  {
    for (uint8_t j = 0; j < 3; j++)
    {
      m->data[i][j] = A3x3[i][j];
    }
  }
}
matrix_t *matrix_from_arr(uint8_t rows, uint8_t cols, uint8_t dim, double *values)
{
  if (rows*cols != dim)
  {
    printf("matrix_from_arr: WRONG DIMENSIONS\n");
    return NULL;
  }
  vector_t *v = vector_from_arr(dim, values);
  matrix_t *newm = matrix_from_vec(rows, cols, v);
  return newm;
}
matrix_t *matrix_from_vec(uint8_t rows, uint8_t cols, vector_t *v)
{
  matrix_t *m = matrix_new(rows, cols);
  uint8_t idx;
  for (uint8_t i = 0; i < m->rows; i++)
  {
    for (uint8_t j = 0; j < m->cols; j++)
    {
      idx = i*m->cols + j;
      m->data[i][j] = (idx < v->dim) ? v->data[idx] : 0.0;
    }
  }
  return m;
}
vector_t *vector_from_arr(uint8_t dim, double *values)
{
  vector_t *v = vector_new(dim);
  for (uint8_t i = 0; i < dim; i++)
  {
    v->data[i] = values[i];
  }
  return v;
}
void matrix_free(matrix_t *m)
{   
  if (m == NULL)
  {
    printf("MATRIX NULL\n");
    return;
  }
  for (uint8_t i = 0; i < m->rows; ++i)
  {
    free(m->data[i]);
  }
  free(m->data);
  free(m);
}
void vector_free(vector_t *v)
{
  if (v == NULL)
  {
    printf("VECTOR NULL\n");
    return;
  }
  free(v->data);
  free(v);
}
//////////////////////////////////////////////
// HELP FUNCTIONS
uint8_t matrix_check_row(matrix_t *m, uint8_t row)
{
  return (m->rows > row);
}
uint8_t matrix_check_col(matrix_t *m, uint8_t col)
{
  return (m->cols > col);
}
uint8_t matrix_check_dim(matrix_t *m, uint8_t row, uint8_t col)
{
  return matrix_check_col(m, col) && matrix_check_row(m, row);
}
uint8_t vector_check_dim(vector_t *v, uint8_t dim)
{
  return (v->dim > dim);
}
//////////////////////////////////////////////
// MATRIX EQUALITY
uint8_t vector_eq_dim(vector_t *v1, vector_t *v2)
{
  return (v1->dim == v2->dim);
}
uint8_t matrix_eq_dim(matrix_t *m1, matrix_t *m2)
{
  return (m1->cols == m2->cols) && (m1->rows == m2->rows);
}
uint8_t vector_eq(vector_t *v1, vector_t *v2, double tolerance)
{
  if (!vector_eq_dim(v1, v2))
  {
    return 0;
  }
  for (uint8_t i = 0; i < v1->dim; i++)
  {
    if (fabs(v1->data[i] - v2->data[i]) > tolerance)
    {
      return 0;
    }
  }
  return 1;
}
uint8_t matrix_eq(matrix_t *m1, matrix_t *m2, double tolerance)
{
  if(!matrix_eq_dim(m1, m2))
  {
    return 0;
  }
  for (uint8_t i = 0; i < m1->rows; i++)
  {
    for (uint8_t j = 0; j < m1->cols; j++)
    {
      if (fabs(m1->data[i][j] - m2->data[i][j]) > tolerance)
      {
        return 0;
      }
    }
  }
  return 1;
}
//////////////////////////////////////////////
// MATRIX PRINTING
void matrix_print(matrix_t *m)
{
  printf("Matrix \n");
  if (m == NULL)
  {
    printf("MATRIX NULL\n");
    return;
  }
  for (uint8_t i = 0; i < m->rows; i++)
  {
    for (uint8_t j = 0; j < m->cols; j++)
    {
      printf("%.6f ", m->data[i][j]);
    }
    printf("\n");
  }
}
void vector_print(vector_t *v)
{
  printf("Vector: \n");
  if (v == NULL)
  {
    printf("VECTOR NULL\n");
    return;
  }
  for (uint8_t i = 0; i < v->dim; i++)
  {
    printf("%.2f ", v->data[i]);
  }
  printf("\n");
}
//////////////////////////////////////////////
// Accessing and modifying matrix elements
double vector_get_el(vector_t *v, uint8_t i)
{
  if (!vector_check_dim(v, i))
  {
    printf("vector_get_el %d dimension wrong\n", i);
    return 0.0;
  }
  return v->data[i];
}
double matrix_get_el(matrix_t *m, uint8_t i, uint8_t j)
{
  if(!matrix_check_col(m, j) && !matrix_check_row(m, i))
  {
    printf("matrix_get_el: Dimentions %d %d did not exists\n", i, j);
    return 0.0;
  }
  return m->data[i][j];
}
vector_t *matrix_get_col(matrix_t *m, uint8_t col)
{
  if (!matrix_check_col(m, col))
  {
    printf("COL %d did not exist \n", col);
    return NULL;
  }
  vector_t *v = vector_new(m->rows);
  for (uint8_t i = 0; i < m->rows; i++)
  {
    v->data[i] = m->data[i][col];
  }
  return v;
}
vector_t *matrix_get_row(matrix_t *m, uint8_t row)
{
  if (!matrix_check_row(m, row))
  {
    printf("ROW %d did not exist \n", row);
    return NULL;
  }
  vector_t *v = vector_new(m->cols);
  for (uint8_t i = 0; i < m->cols; i++)
  {
    v->data[i] = m->data[row][i];
  }
  return v;
}
uint8_t vector_set_el(vector_t *v, uint8_t i, double value)
{
  if (v == NULL || !vector_check_dim(v, i))
  {
    return 0;
  }
  v->data[i] = value;
  return 1;
}
uint8_t matrix_set_el(matrix_t *m, uint8_t i, uint8_t j, double value)
{
  if (m == NULL || !matrix_check_dim(m, i, j))
  {
    printf("matrix_set_el WRONG DIMENTIONS\n");
    return 0;
  }
  m->data[i][j] = value;
  return 1;
}
uint8_t matrix_set_all(matrix_t *m, double value)
{
  if (m == NULL)
  {
    printf("matrix_set_all Matrix is NULL\n");
    return 0;
  }

  for (uint8_t i = 0; i < m->rows; i++)
  {
    for (uint8_t j = 0; j < m->cols; j++)
    {
      m->data[i][j] = value;
    }
  }
  return 1;
}
uint8_t matrix_diag_set(matrix_t *m, double value)
{
  if (m == NULL || !m->is_square)
  {
    printf("matrix_diag_set: INCORRECT MATRIX\n");
    return 0;
  }
  for (uint8_t i = 0; i < m->cols; i++)
  {
    m->data[i][i] = value;
  }
  return 1;
}
uint8_t matrix_diag_set_vector(matrix_t *m, vector_t *v)
{
  if (m == NULL || v == NULL || !m->is_square )
  {
    printf("matrix_diag_set_vector: INCORRECT DATA\n");
    return 0;
  }
  if (m->cols != v->dim)
  {
    printf("matrix_diag_set_vector: WRONG DIMENSIONS\n");
    return 0;
  }
  for (uint8_t i = 0; i < v->dim; i++)
  {
    m->data[i][i] = v->data[i];
  }
  return 1;
}
vector_t *matrix_mult_vector(matrix_t *m, vector_t *v)
{
  if(m == NULL || v == NULL)
  {
    printf("matrix_mult_by_vector: NULL ELEMENTS\n");
    return NULL;
  }
  if(m->cols != v->dim)
  {
    printf("matrix_mult_by_vector: WRONG DIMENSIONS\n");
    return NULL;
  }
  vector_t *newm = vector_new(m->rows);
  for (uint8_t i = 0; i < m->rows; i++)
  {
    double new_val = 0;
    for (uint8_t j = 0; j < m->cols; j++)
    {
      new_val += m->data[i][j]*v->data[j];
    }
    newm->data[i] = new_val;
  }
  return newm;
}
uint8_t matrix_mult_scalar_ip(matrix_t *m, double value)
{
  if (m == NULL)
  {
    printf("matrix_mult_scalar_ip: MATRIX IS NULL\n");
    return 0;
  }
  for (uint8_t i = 0; i < m->rows; i++)
  {
    for (uint8_t j = 0; j < m->cols; j++)
    {
      m->data[i][j] *= value;
    }
  }
  return 1;
}
matrix_t *matrix_mult_scalar(matrix_t *m, double value)
{
  matrix_t *newm = matrix_copy(m);
  if (!matrix_mult_scalar_ip(m, value))
  {
    matrix_free(newm);
    return NULL;
  }
  return newm;
}
uint8_t matrix_mult_row_ip(matrix_t *m, uint8_t row, double value)
{
  if (m == NULL || m->rows <= row)
  {
    printf("matrix_mult_row_ip: WRONG PARAMETERS\n");
    return 0;
  }
  for (uint8_t i = 0; i < m->cols; i++)
  {
    m->data[row][i] *= value;
  }
  return 1;
}
matrix_t *matrix_mult_row(matrix_t *m, uint8_t row, double value)
{
  matrix_t *newm = matrix_copy(m);
#if 0
  if (!succmatrix_mult_row_ip(m, row, value))
  {
    matrix_free(newm);
    return NULL;
  }
#endif    
  return newm;
}
uint8_t matrix_mult_col_ip(matrix_t *m, uint8_t col, double value)
{
  if (m == NULL || m->cols <= col)
  {
    printf("matrix_mult_col_ip: WRONG PARAMETERS\n");
    return 0;
  }
  for (uint8_t i = 0; i < m->rows; i++)
  {
    m->data[i][col] *= value;
  }
  return 1;
}
matrix_t *matrix_mult_col(matrix_t *m, uint8_t col, double value)
{
  matrix_t *newm = matrix_copy(m);
  if (!matrix_mult_col_ip(m, col, value))
  {
    matrix_free(newm);
    return NULL;
  }
  return newm;
}
uint8_t matrix_add_row_ip(matrix_t *m, uint8_t where, uint8_t row, double multiplier)
{
  if (m == NULL || m->rows <= where || m->rows <= row)
  {
    printf ("matrix_add_row_ip: WRONG PARAMS\n");
    return 0;
  }
  for (uint8_t i = 0; i < m->cols; i++)
  {
    m->data[where][i] += multiplier*m->data[row][i];
  }
  return 1;
}
matrix_t *matrix_add_row(matrix_t *m, uint8_t where, uint8_t row, double multiplier)
{
  matrix_t *newm = matrix_copy(m);
  if (!matrix_add_row_ip(newm, where, row, multiplier))
  {
    matrix_free(newm);
    return NULL;
  }
  return newm;
}
uint8_t vector_mult_scalar_ip(vector_t *v, double value, double add)
{
  if (v == NULL)
  {
    printf("vector_mult_scalar_ip: VECTOR IS NULL\n");
    return 0;
  }
  for (uint8_t i = 0; i < v->dim; i++)
  {
    v->data[i] = v->data[i]*value + add;
  }
  return 1;
}
vector_t *vector_mult_scalar(vector_t *v, double value, double add)
{
  vector_t *newm = vector_copy(v);
  if (!vector_mult_scalar_ip(newm, value, add))
  {
    vector_free(newm);
    return NULL;
  }
  return newm;
}
uint8_t vector_add_to_el_ip(vector_t *v, uint8_t element, double value)
{
  if (v == NULL || !vector_check_dim(v, element))
  {
    printf("vector_add_to_el_ip: WRONG PARAMETERS\n");
    return 0;
  }
  v->data[element] += value;
  return 1;
}
vector_t *vector_add_to_el(vector_t *v, uint8_t element, double value)
{
  vector_t *newm = vector_copy(v);
  if (!vector_add_to_el_ip(newm, element, value))
  {
    vector_free(newm);
    return NULL;
  }
  return newm;
}
//////////////////////////////////////////////
// Modifying the matrix structure
matrix_t *matrix_rem_col(matrix_t *m, uint8_t col)
{
  if (m == NULL || m->cols <= col)
  {
    printf("matrix_rem_col: WRONG PARAMETERS\n");
    return NULL;
  }
  matrix_t *newm = matrix_new(m->rows, m->cols-1);
  uint8_t i, j, k;
  for (i = 0; i < m->rows; i++)
  {
    for(j = 0, k = 0; j < m->cols; j++)
    {
      if (j != col)
      {
        newm->data[i][k] = m->data[i][j];
        k++;
      }
    }
  }
  return newm;
}
matrix_t *matrix_rem_row(matrix_t *m, uint8_t row)
{
  if (m == NULL || m->rows <= row)
  {
    printf("matrix_rem_row: WRONG PARAMETERS\n");
    return NULL;
  }
  matrix_t *newm = matrix_new(m->rows - 1, m->cols);
  uint8_t i, j, k;
  for (i = 0, k = 0; i < m->rows; i++)
  {
    if (i != row)
    {
      for (j = 0; j < m->cols; j++)
      {
        newm->data[k][j] = m->data[i][j];
      }
      k++;
    }
  }
  return newm;
}
uint8_t matrix_col_swap_ip(matrix_t *m, uint8_t col1, uint8_t col2)
{
  if (m == NULL || m->cols <= col1 || m->cols <= col2)
  {
    printf("matrix_col_swap_ip: WRONG PARAMETERS\n");
    return 0;
  }
  double tmp;
  for (uint8_t i = 0; i < m->rows; i++)
  {
    tmp = m->data[i][col1];
    m->data[i][col1] = m->data[i][col2];
    m->data[i][col2] = tmp;
  }
  return 1;
}
matrix_t *matrix_col_swap(matrix_t *m, uint8_t col1, uint8_t col2)
{
  matrix_t *newm = matrix_copy(m);
  if (!matrix_col_swap_ip(newm, col1, col2))
  {
    matrix_free(newm);
    return NULL;
  }
  return newm;
}
uint8_t matrix_row_swap_ip(matrix_t *m, uint8_t row1, uint8_t row2)
{
  if (m == NULL || m->rows <= row1 || m->rows <= row2)
  {
    printf("matrix_row_swap_ip: WRONG PARAMETERS\n");
    return 0;
  }
  double *tmp = m->data[row1];
  m->data[row1] = m->data[row2];
  m->data[row2] = tmp;
  return 1;
}
matrix_t *matrix_row_swap(matrix_t *m, uint8_t row1, uint8_t row2)
{
  matrix_t *newm = matrix_copy(m);
  if (!matrix_row_swap_ip(newm, row1, row2))
  {
    matrix_free(newm);
    return NULL;
  }
  return newm;
}
typedef unsigned int uint;
typedef unsigned char uint8_t;
uint8_t matrix_add_ip(matrix_t *m1, matrix_t *m2)
{
  if(m1 == NULL || m2 == NULL || !matrix_eq_dim(m1, m2))
  {
    printf("matrix_add_ip: NOT EQUAL DIMENSIONS\n");
    return 0;
  }
  uint i, j;
  for (i = 0; i < m1->rows; i++)
  {
    for (j = 0; j < m1->cols; j++)
    {
      m1->data[i][j] += m2->data[i][j];
    }
  }
  return 1;
}
matrix_t *matrix_add(matrix_t *m1, matrix_t *m2)
{
  matrix_t *newm = matrix_copy(m1);
  if (!matrix_add_ip(newm, m2))
  {
    matrix_free(newm);
    return NULL;
  }
  return newm;
}
uint8_t matrix_sub_ip(matrix_t *m1, matrix_t *m2)
{
  if (m1 == NULL || m2 == NULL || !matrix_eq_dim(m1, m2))
  {
    printf("matrix_sub_ip: NOT EQUAL DIMENSIONS\n");
    return 0;
  }
  uint8_t i, j;
  for (i = 0; i < m1->rows; i++)
  {
    for (j = 0; j < m1->cols; j++)
    {
      m1->data[i][j] -= m2->data[i][j];
    }
    return 1;
  }
  return 0;
}
matrix_t *matrix_sub(matrix_t *m1, matrix_t *m2)
{
  matrix_t *newm = matrix_copy(m1);
  if (!matrix_sub_ip(newm, m2))
  {
    matrix_free(newm);
    return NULL;
  }
  return newm;
}
matrix_t *matrix_dot(matrix_t *m1, matrix_t *m2)
{
  if (m1 == NULL || m2 == NULL || m1->cols != m2->rows)
  {
    printf("matrix_dot: MATRICES CAN NOT MULTYPLIED\n");
    return NULL;
  }
  matrix_t *newm = matrix_new(m1->rows, m2->cols);
  uint8_t i, j, k;
  for (i = 0; i < newm->rows; i++)
  {
    for (j = 0; j < newm->cols; j++)
    {
      for (k = 0; k < m1->cols; k++)
      {
        newm->data[i][j] += m1->data[i][k]*m2->data[k][j];
      }
    }
  }
  return newm;
}
matrix_t *matrix_transpose(matrix_t *m)
{
  if (m == NULL)
  {
    printf("matrix_transpose: MATRIX IS NULL\n");
    return NULL;
  }
  matrix_t *newm = matrix_new(m->cols, m->rows);
  uint8_t i, j;
  for (i = 0; i < newm->rows; i++)
  {
    for (j = 0; j < newm->cols; j++)
    {
      newm->data[i][j] = m->data[j][i];
    }
  }
  return newm;
}
double matrix_trace(matrix_t *m)
{
  if (m == NULL || !m->is_square)
  {
    printf("matrix_trace: MATRIX IS WRONG\n");
    return 0.0;
  }
  double trace = 0.0;
  for (uint8_t i = 0; i < m->rows; i++)
  {
    trace += m->data[i][i];
  }
  return trace;
}
double vector_scalar_dot(vector_t *v1, vector_t *v2)
{
  if (v1 == NULL || v2 == NULL || v1->dim != v2->dim)
  {
    printf("vector_scalar_dot: WRONG PARAMETERS\n");
    return 0.0;
  }
  double result = 0.0;
  for (uint8_t i = 0; i < v1->dim; i++)
  {
    result += v1->data[i]*v2->data[i];
  }
  return result;
}
double vector_norm2(vector_t *v)
{
  double result = vector_scalar_dot(v, v);
  return result;
}
double vector_norm(vector_t *v)
{
  double result = sqrt(vector_norm2(v));
  return result;
}
uint8_t vector_normalize_ip(vector_t *v)
{
  double norm = vector_norm(v);
  if (norm == 0.0)
  {
    printf("vector_normalize: NORM IS ZERO\n");
    return 0;
  }
  for (uint8_t i = 0; i < v->dim; i++)
  {
    v->data[i] /= norm;
  }
  return 1;
}
vector_t *vector_normalize(vector_t *v)
{
  vector_t *newm = vector_copy(v);
  if (!vector_normalize_ip(newm))
  {
    vector_free(newm);
    return NULL;
  }
  return newm;
}
//////////////////////////////////////////////
// Matrix invertion
matrix_t *matrix_invert(matrix_t *m1)
{
  if (m1 == NULL || !m1->is_square)
  {
    printf("matrix_invert: WRONG MATRIX\n");
    return NULL;
  }
  matrix_t *m = matrix_copy(m1);
  matrix_t *newm = matrix_eye(m->rows);
  for (uint8_t i = 0; i < m->rows; ++i)
  {
    if (m->data[i][i] == 0.0)
    {
      uint8_t r;
      for (r = i + 1; r < m->rows; ++r)
      {
        if (m->data[r][i] != 0.0)
        {
          break;
        }
      }
      if (r == m->rows)
      {
        printf("matrix_invert: MATRIX CANNOT BE INVERT\n");
        return NULL;
      }
      matrix_row_swap_ip(m, i, r);
      matrix_row_swap_ip(newm, i, r);
    }
    double scalar = 1.0/m->data[i][i];
    matrix_mult_row_ip(m, i, scalar);
    matrix_mult_row_ip(newm, i, scalar);
    for (uint8_t j = 0; j < m->rows; ++j)
    {
      if (i==j) {
        continue;
      }
      double shear_needed = -m->data[j][i];
      matrix_add_row_ip(m, j, i, shear_needed);
      matrix_add_row_ip(newm, j, i, shear_needed);
    }
  }
  matrix_free(m);
  return newm;
}
//////////////////////////////////////////////
//2026.8.4 two points in image to 6x6 matrix
matrix_t *matrix_from_vectors(vector_t *u1,vector_t *u2)
{
  if (u1 == NULL || u2 == NULL || u1->dim!=2 || u2->dim!=2)
  {
    printf("matrix_from_vectors: WRONG INPUT VECTORS\n");
    return NULL;
  }
  matrix_t *newm = matrix_new(6, 6);
  for (uint8_t i = 0; i < newm->rows; i++)
  {
    for (uint8_t j = 0; j < newm->cols; j++)
    {
      newm->data[i][j] = 0;
    }
  }
  newm->data[0][0]=u1->data[0];
  newm->data[0][1]=u1->data[1];
  newm->data[1][2]=u1->data[0];
  newm->data[1][3]=u1->data[1];
  newm->data[2][4]=u1->data[0];
  newm->data[2][5]=u1->data[1];
  newm->data[3][0]=u2->data[0];
  newm->data[3][1]=u2->data[1];
  newm->data[4][2]=u2->data[0];
  newm->data[4][3]=u2->data[1];
  newm->data[5][4]=u2->data[0];
  newm->data[5][5]=u2->data[1];
  return newm;
}
matrix_t *SolveB_from_Ax(matrix_t *A,vector_t *x) //Ab=x
{
  if (A == NULL || x == NULL || A->rows!=6 || A->cols!=6
    ||x->dim!=6 )
  {
    printf("SolveB_from_Ax: WRONG INPUT MATRIX,VECTOR\n");
    return NULL;
  }
  double delta=A->data[0][0]*A->data[3][1]-A->data[3][0]*A->data[0][1];
  if (delta==0) {
    printf("SolveB_from_Ax: WRONG INPUT DATA VALUES\n");
    return NULL;
  }
  matrix_t *newm = matrix_new(3, 2);
  newm->data[0][0]=(x->data[0]*A->data[3][1]-x->data[3]*A->data[0][1])/
                   delta;
  newm->data[0][1]=(x->data[3]*A->data[0][0]-x->data[0]*A->data[3][0])/
                   delta;
  newm->data[1][0]=(x->data[1]*A->data[3][1]-x->data[4]*A->data[0][1])/
                   delta;
  newm->data[1][1]=(x->data[4]*A->data[0][0]-x->data[1]*A->data[3][0])/
                   delta;
  newm->data[2][0]=(x->data[2]*A->data[3][1]-x->data[5]*A->data[0][1])/
                   delta;
  newm->data[2][1]=(x->data[5]*A->data[0][0]-x->data[2]*A->data[3][0])/
                   delta;
  return newm;
}
matrix_t *matrix_from_row_perm(int D[3]) {
  matrix_t *newm = matrix_new(3, 3);
  int i;
  for (i=0;i<3;i++) {
    newm->data[D[i]][i]=1;
  }
  return newm;
}
matrix_t *matrix_from_col_perm(int D[3]) {
  matrix_t *newm = matrix_new(3, 3);
  int i;
  for (i=0;i<3;i++) {
    newm->data[i][D[i]]=1;
  }
  return newm;
}
void row_perm_vector(int D[3],vector_t *v) { //B
  double tmp[3];
  tmp[0]=v->data[0];tmp[1]=v->data[1];tmp[2]=v->data[2];
  v->data[0]=tmp[D[0]];
  v->data[1]=tmp[D[1]];
  v->data[2]=tmp[D[2]];
  return;
}
void col_perm_vector(int D[3],vector_t *v) { //E
  double tmp[3];
  tmp[0]=v->data[0];tmp[1]=v->data[1];tmp[2]=v->data[2];
  v->data[D[0]]=tmp[0];
  v->data[D[1]]=tmp[1];
  v->data[D[2]]=tmp[2];
  return;
}
////////////////////////////////////////////////
// |u00 u01 u02| |b0|  |v0|
// |    u11 u12| |b1|= |v1|
// |        u22| |b2|  |v2|
void SolveB_from_Uv(matrix_t *U,vector_t *v) {
  v->data[2]=v->data[2]/U->data[2][2];
  v->data[1]=(v->data[1]-v->data[2]*U->data[1][2])/U->data[1][1];
  v->data[0]=(v->data[0]-v->data[2]*U->data[0][2]-v->data[1]*U->data[0][1])/U->data[0][0];
}
////////////////////////////////////////////////
// |  1   0  0| |b0|  |v0|
// |l10   1  0| |b1|= |v1|
// |l20 l21  1| |b2|  |v2|
void SolveB_from_Lv(matrix_t *L,vector_t *v) {
//v->data[0]=v->data[0];
  v->data[1]=v->data[1]-v->data[0]*L->data[1][0];
  v->data[2]=v->data[2]-v->data[0]*L->data[2][0]-v->data[1]*L->data[2][1];
}
matrix_t *computeBM_from_BLUE(matrix_t *L,matrix_t *U,int *B,int *E,
  vector_t *v1,vector_t *v2,vector_t *v3) {
  vector_t *v01=(vector_t *)ivector3_new(v1->data[0],v2->data[0],v3->data[0]); 
  vector_t *v02=(vector_t *)ivector3_new(v1->data[1],v2->data[1],v3->data[1]); 
  vector_t *v03=(vector_t *)ivector3_new(v1->data[2],v2->data[2],v3->data[2]); 
  row_perm_vector(B,v01);
  SolveB_from_Lv(L,v01);
  SolveB_from_Uv(U,v01);
  col_perm_vector(E,v01);

  row_perm_vector(B,v02);
  SolveB_from_Lv(L,v02);
  SolveB_from_Uv(U,v02);
  col_perm_vector(E,v02);

  row_perm_vector(B,v03);
  SolveB_from_Lv(L,v03);
  SolveB_from_Uv(U,v03);
  col_perm_vector(E,v03);

  matrix_t *m=matrix_new(3,3);
  m->data[0][0]=v01->data[0];m->data[0][1]=v01->data[1];m->data[0][2]=v01->data[2];
  m->data[1][0]=v02->data[0];m->data[1][1]=v02->data[1];m->data[1][2]=v02->data[2];
  m->data[2][0]=v03->data[0];m->data[2][1]=v03->data[1];m->data[2][2]=v03->data[2];
  return m;
}
#if 0
#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>
#include <algorithm>
// Define structure to hold the Full Pivoting LU Decomposition result
struct FullPivLU {
    std::vector<std::vector<double>> L;
    std::vector<std::vector<double>> U;
    std::vector<int> P; // Row permutations
    std::vector<int> Q; // Column permutations
    bool isSingular;
};
#endif
//use Full Pivoting (LUPQ) decomposition (P A Q = L U)
//BLUE B:before E:End
bool computeFullPivLU(
  const matrix_t *A, matrix_t *L,matrix_t *U,
  int *B,int *E, double epsilon = 1e-12) {
  int n = A->rows; //=A=cols=3
  int tmp;
  matrix_t *LU = matrix_copy((matrix_t *)A);   
// Initialize permutation vectors
  for(int i=0;i<n;i++) {
    B[i]=i;E[i]=i;
  }
  printf("n=%d\n",n);
//printf("B=%3d,%3d,%3d\n",B[0],B[1],B[2]);
//printf("E=%3d,%3d,%3d\n",E[0],E[1],E[2]);
  bool singularFlag = false;
  for (int k = 0; k < n; ++k) {
//Step 1: Find the absolute maximum element in the remaining sub-matrix (Full Pivoting)
    double maxVal = 0.0;
    int pivotRow = k;
    int pivotCol = k;        
    for (int i = k; i < n; ++i) {
      for (int j = k; j < n; ++j) {
        if (fabs(LU->data[i][j]) > maxVal) {
          maxVal = fabs(LU->data[i][j]);
          pivotRow = i;
          pivotCol = j;
        }
      }
    }
//Step 2: Check for singularity
    if (maxVal < epsilon) {
      singularFlag = true;
      //The rest of the matrix diagonal under U is structurally zero.
      //We zero them out to prevent tiny precision noise from lingering.
      for (int i = k; i < n; ++i) {
        LU->data[i][i] = 0.0;
      }
      break; 
    }
//Step 3: Swap Rows in LU and tracking vector P
    if (pivotRow != k) {
      tmp = matrix_row_swap_ip(LU, k, pivotRow);
//    printf("LU (%d,%d) %d ",k , pivotRow, result);
//    matrix_print(LU);
      tmp=B[k];B[k]=B[pivotRow];B[pivotRow]=tmp;
    }
//Step 4: Swap Columns in LU and tracking vector Q
    if (pivotCol != k) {
      tmp=matrix_col_swap_ip(LU, k, pivotCol);
      tmp=E[k];E[k]=E[pivotCol];E[pivotCol]=tmp;
    }
//Step 5: Perform standard Gaussian elimination updates
    for (int i = k + 1; i < n; ++i) {
      LU->data[i][k] /= LU->data[k][k]; // Multiplier safely processed
      for (int j = k + 1; j < n; ++j) {
        LU->data[i][j] -= LU->data[i][k] * LU->data[k][j];
      }
    }
  }
//Step 6: Separate the combined matrix into distinct L and U structures
//std::vector<std::vector<double>> L(n, std::vector<double>(n, 0.0));
//std::vector<std::vector<double>> U(n, std::vector<double>(n, 0.0));
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      if (i > j) {
        L->data[i][j] = LU->data[i][j];
      } else if (i == j) {
        L->data[i][j] = 1.0; // L is unit lower triangular
        U->data[i][j] = LU->data[i][j];
      } else {
        U->data[i][j] = LU->data[i][j];
      }
    }
  }
//printf("B=%3d,%3d,%3d\n",B[0],B[1],B[2]);
//printf("E=%3d,%3d,%3d\n",E[0],E[1],E[2]);
  return singularFlag;
}
matrix_t *computeBM_from_UV(vector_t *u1,vector_t *u2,vector_t *u3,
  vector_t *v1,vector_t *v2,vector_t *v3) {
  double A3x3[3][3];
  A3x3[0][0]=u1->data[0];A3x3[0][1]=u1->data[1];A3x3[0][2]=u1->data[2];
  A3x3[1][0]=u2->data[0];A3x3[1][1]=u2->data[1];A3x3[1][2]=u2->data[2];
  A3x3[2][0]=u3->data[0];A3x3[2][1]=u3->data[1];A3x3[2][2]=u3->data[2];
  matrix_t *A=(matrix_t *)matrix_new(3, 3);
  matrix_t *L=(matrix_t *)matrix_new(3, 3);
  matrix_t *U=(matrix_t *)matrix_new(3, 3);
  int B[3],E[3];
  matrix_copy3x3(A,A3x3);
  bool isSingular = computeFullPivLU(A,L,U,B,E);
  if(!isSingular) {
    return computeBM_from_BLUE(L,U,B,E,v1,v2,v3);
  }
  else {
    return NULL;
  }
}
int lr_main() {
// A classic 3x3 singular matrix example (Row 3 = Row 1 + Row 2)
  matrix_t *A=(matrix_t *)matrix_new(3, 3);
  double A3x3[3][3] = {
     {1.0, 2.0, 3.0}, //<== row vector 1
     {4.0, 5.0, 6.0}, //<== row vector 2
     {1.0, 7.0, 9.0}  //<== row vector 3
    };
  matrix_t *L=(matrix_t *)matrix_new(3, 3);
  matrix_t *U=(matrix_t *)matrix_new(3, 3);
  int B[3],E[3];
  matrix_copy3x3(A,A3x3);
  matrix_print(A);
  bool isSingular = computeFullPivLU(A,L,U,B,E);
  printf("L ");matrix_print(L);
  printf("U ");matrix_print(U);
  printf("B=%3d,%3d,%3d\n",B[0],B[1],B[2]);
  printf("E=%3d,%3d,%3d\n",E[0],E[1],E[2]); 
  printf("Is Singular: %s\n",isSingular ? "YES" : "NO");  
  matrix_t *BB=(matrix_t *)matrix_from_row_perm(B);
  matrix_t *EE=(matrix_t *)matrix_from_col_perm(E);
//test
  matrix_t *LxU=(matrix_t *)matrix_dot(L,U);
//printf("LxU ");matrix_print(LxU);
//printf("EE ");matrix_print(EE);
  matrix_t *LxU1=(matrix_t *)matrix_dot(LxU,EE);
//printf("LxU1 ");matrix_print(LxU1);
  matrix_t *LxU2=(matrix_t *)matrix_dot(BB,LxU1);
  printf("LxU2 ");matrix_print(LxU2);
  matrix_t *LxU3=matrix_invert(LxU2);
  printf("LxU3 ");matrix_print(LxU3);

  vector_t *u1=ivector3_new(4.0,5.0,6.0);  ////<== row vector 1

  vector_t *v1=ivector3_new(  3,  6,1.0);  ////<== row vector 1
  vector_t *v2=ivector3_new(  4,  5,0.3);  ////<== row vector 2
  vector_t *v3=ivector3_new(  6,  1,0.2);  ////<== row vector 3
  matrix_t *BM=computeBM_from_BLUE(L,U,B,E,v1,v2,v3);
  printf("BM ");matrix_print(BM);
  vector_t *va1=matrix_mult_vector(BM,u1);
  printf("va1 ");vector_print(va1);
  return 0;
}
