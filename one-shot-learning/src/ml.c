/* 
 * 
 * This code calculates the house price of a house by learing from
 * training data. It uses pseudo inverse of a given matrix to find the 
 * weight of different features.
 * 
 * Predicted Price : Y = W0 + W1*x1 + W2*X2 + W3*X3 + W4*X4
 * Weight Matrix : W = pseudoInv(X)*Y
 * pseudoInv(X) = inverse(transpose(X)*X) * transpose(X)  
 * 
 * weight(w) = pseudoInv(X) * Y
 * 			where	X = Input data matrix
 * 					Y = Target vector
 * 
 */
 
#include<stdio.h>
#include<stdlib.h>

// all methods declarations
double** multiplyMatrix(double **matA, double **matB, int r1, int c1, int r2, int c2);
double** transposeMatrix(double** mat, int row, int col);
double** inverseMatrix(double **matA, int dimension);

void printMatrix(double **mat, int r, int c);
void freeMat(double **mat, int r);

// main method starts here
int main(int argc, char** argv){

    // your code goes here

    FILE *fp_train;
    fp_train = fopen(argv[1], "r");

    FILE *fp_test;
    fp_test = fopen(argv[2], "r");

    if (fp_train == NULL || fp_test == NULL) {
        return 0;
    }
    
    // get matrix dimensions
    int k, n, m;
    fscanf(fp_train, "%d\n", &k);
    fscanf(fp_train, "%d\n", &n);
    fscanf(fp_test, "%d\n", &m);

    double** train_attributes = (double**) malloc(n * sizeof(double*));
    for (int i = 0; i < n; i++) { // initialize train_attributes
        train_attributes[i] = (double*) malloc((k + 1) * sizeof(double));
    }

    double** train_prices = (double**) malloc(n * sizeof(double*));
    for (int i = 0; i < n; i++) { // initialize train_prices
        train_prices[i] = (double*) malloc(sizeof(double));
    }

    double** test_attributes = (double**) malloc(m * sizeof(double*));
    for (int i = 0; i < m; i++) { // initialize test_attributes
        test_attributes[i] = (double*) malloc((k + 1) * sizeof(double));
    }

    // populate train_attributes & train_prices
    for (int i = 0; i < n; i++) {
        train_attributes[i][0] = 1;
        for (int j = 1; j < k + 1; j++) {
            fscanf(fp_train, "%lf,", &(train_attributes[i][j]));
        }
        fscanf(fp_train, "%lf\n", &(train_prices[i][0]));
    }

    // populate test_attributes
    for (int i = 0; i < m; i++) {
        test_attributes[i][0] = 1;
        for (int j = 1; j < k; j++) {
            fscanf(fp_test, "%lf,", &(test_attributes[i][j]));
        }
        fscanf(fp_test, "%lf\n", &(test_attributes[i][k]));
    }

    // **COMPUTE**
    double** train_attributes_transpose = transposeMatrix(train_attributes, n, k + 1);
    double** product = multiplyMatrix(train_attributes_transpose, train_attributes, k + 1, n, n, k + 1);
    freeMat(train_attributes, n);
    double** pseudoInv = inverseMatrix(product, k + 1);
    freeMat(product, k + 1);
    double** weights_init = multiplyMatrix(pseudoInv, train_attributes_transpose, k + 1, k + 1, k + 1, n);
    freeMat(pseudoInv, k + 1);
    freeMat(train_attributes_transpose, k + 1);
    double** weights = multiplyMatrix(weights_init, train_prices, k + 1, n, n, 1);
    freeMat(train_prices, n);
    freeMat(weights_init, k + 1);
    double** final_prices = multiplyMatrix(test_attributes, weights, m, k + 1, k + 1, 1);
    freeMat(test_attributes, m);
    freeMat(weights, k + 1);

    printMatrix(final_prices, m, 1);
	
    freeMat(final_prices, m);

	return 0;
}

double** multiplyMatrix(double **matA, double **matB, int r1, int c1, int r2, int c2)
{
    double** result=malloc(r1*sizeof(double*));

    // your code goes here

    if (r2 != c1) {
        return result;
    }
    
    // initialize result matrix
    for (int i = 0; i < r1; i++) {
        result[i] = (double*) malloc(c2 * sizeof(double));
    }

    // populate result matrix
    for (int i = 0; i < r1; i++) {
        for (int j = 0; j < c2; j++) {
            result[i][j] = 0;
            for (int k = 0; k < c1; k++) {
                result[i][j] += matA[i][k] * matB[k][j];
            }
        }
    }
    
    return result;
}


double** transposeMatrix(double** mat, int row, int col)
{
  
	double** matTran=malloc(col*sizeof(double*)); 
    
    // your code goes here

    // initialize matTran matrix
    for (int i = 0; i < col; i++) {
        matTran[i] = (double*) malloc(row * sizeof(double));
    }

    // populate matTran matrix
    for (int i = 0; i < col; i++) {
        for (int j = 0; j < row; j++) {
            matTran[i][j] = mat[j][i];
        }
    }
    
    return matTran;
}


double** inverseMatrix(double **matA, int dimension)
{

    double** matI=malloc(dimension*sizeof(double*)); 

    // your code goes here

    // initialize matI matrix
    for (int i = 0; i < dimension; i++) {
        matI[i] = malloc(dimension * sizeof(double));
    }

    // populate matI matrix
    int colCount = 0;
    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++) {
            if (j == colCount) {
                matI[i][j] = 1;
            }
            else {
                matI[i][j] = 0;
            }
        }
        colCount++;
    }

    for (int p = 0; p < dimension; p++) {
        double f = matA[p][p];
        double* matA_row_p = matA[p];
        double* matI_row_p = matI[p];

        for (int i = 0; i < dimension; i++) {
            matA_row_p[i] /= f;
            matI_row_p[i] /= f;
        }

        for (int i = p + 1; i < dimension; i++) {
            f = matA[i][p];
            double* matA_row_i = matA[i];
            double* matI_row_i = matI[i];

            for (int j = 0; j < dimension; j++) {
                matA_row_i[j] -= (matA_row_p[j] * f);
                matI_row_i[j] -= (matI_row_p[j] * f);
            }
        }
    }

    for (int p = dimension - 1; p >= 0; p--) {
        for (int i = p - 1; i >= 0; i--) {
            double f = matA[i][p];
            double* matA_row_p = matA[p];
            double* matI_row_p = matI[p];
            double* matA_row_i = matA[i];
            double* matI_row_i = matI[i];

            for (int j = 0; j < dimension; j++) {
                matA_row_i[j] -= (matA_row_p[j] * f);
                matI_row_i[j] -= (matI_row_p[j] * f);
            }
        }
    }
    
	return matI;
}


void printMatrix(double **mat, int r, int c) {
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            printf("%0.0lf", mat[i][j]);
        }
        printf("\n");
    }
}


void freeMat(double **mat, int r) {
    for (int i = 0; i < r; i++) {
        free(mat[i]);
    }
    free(mat);
}
