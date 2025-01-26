#include <stdio.h>

int matrix_multiplication(int a, int b, int c, int d, int m1[a][b], int m2[c][d]);
int matrix_addition(int a, int b, int m1[a][b], int m2[a][b]);
int sqrt_s(int s);
int sqrt_m(int a, int b, int m[a][b]);
int tr(int a, int b, int m[a][b]); 
int choose(int a, int b, int c, int d);
void printsep();

int matrix_multiplication(int a, int b, int c, int d, int m1[a][b], int m2[c][d]){

    int product[a][d];

    for (int i=0; i<a; i++){
        for (int j=0; j<d; j++){
            product[i][j] = 0;
        }
    }

    for (int i=0; i<a; i++){
        for (int j=0; j<d; j++){
            for (int k=0; k<c; k++){
                product[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }

    return product;
}

int matrix_addition(int a, int b, int m1[a][b], int m2[a][b]){

    int sum[a][b];

    for (int i=0; i<a; i++){
        for(int j=0; j<b; j++){
            sum[i][j] = m1[i][j] + m2[i][j];
        }
    }

    return sum;
}

int sqrt_s(int s){
    return pow(s, 0.5);
}

int sqrt_m(int a, int b, int m[a][b]){

    int res[a][b];

    for (int i=0; i<a; i++){
        for (int j=0; j<b; j++){
            res[i][j] = pow(m[i][j], 0.5);
        }
    }

    return res;
}

int tr(int a, int b, int m[a][b]){

    int transpose[a][b];

    for (int i=0; i<a; i++){
        for (int j=0; j<b; j++){
            transpose[i][j] = m[j][i];
        }
    }

    return transpose;
}

int choose(int a, int b, int c, int d){
    if (a==0)
        return b;
    if (a > 0)
        return c;
    if (a < 0)
        return d;
}

void printsep(){
    printf("----------");
}




int main(){






}
