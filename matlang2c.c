#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

char *strsep();
int line_checker(char* line);
void add_spaces();
int is_not_char(char s);
int scalar_control();
int vector_control();
int matrix_control();
int print_control();
int printsep_control();
int comment_control();
int endfor_control();
int assignment_control();
int assign_control();
int for_control();
int* expression_control();
int* term_control();
int id_control();
int is_id(char* token);
int is_digit(char* token);
int is_exist(char* var);
int is_number(char* token);
char** sub_array(char** array, int from, int to);
char* itoa(int num, char* str, int base);

void scalar_printer(char* line);
void vector_printer(char* line);
void matrix_printer(char* line);
void assignment_printer();
void print_printer();
int expression_printer();
int term_printer();
int for_printer();

char *removed(char* myStr);
int find_index(char* var,char** array,int array_size);

char** get_token_array(char* line);

char* scalar_array[128];
int scalar_array_size = 0;
char* vector_array[128];
int vector_array_size = 0;
char* matrix_array[128];
int matrix_array_size = 0;

char real[60000];
int curr_line = 1;

int is_nestedfor = 0;

enum tokens{SCALAR=1,VECTOR=2,MATRIX=3,COMMENT=4,ASSIGNMENT=5,FOR=6,PRINT=7,PRINTSEP=8,ENDFOR=9};

void swap(char *x, char *y) {
    char t = *x; *x = *y; *y = t;
}
 
// Function to reverse `buffer[i…j]`
char* reverse(char *buffer, int i, int j)
{
    while (i < j) {
        swap(&buffer[i++], &buffer[j--]);
    }
 
    return buffer;
}
 
// Iterative function to implement `itoa()` function in C
char* itoa(int value, char* buffer, int base)
{
    // invalid input
    if (base < 2 || base > 32) {
        return buffer;
    }
 
    // consider the absolute value of the number
    int n = abs(value);
 
    int i = 0;
    while (n)
    {
        int r = n % base;
 
        if (r >= 10) {
            buffer[i++] = 65 + (r - 10);
        }
        else {
            buffer[i++] = 48 + r;
        }
 
        n = n / base;
    }
 
    // if the number is 0
    if (i == 0) {
        buffer[i++] = '0';
    }
 
    // If the base is 10 and the value is negative, the resulting string
    // is preceded with a minus sign (-)
    // With any other base, value is always considered unsigned
    if (value < 0 && base == 10) {
        buffer[i++] = '-';
    }
 
    buffer[i] = '\0'; // null terminate string
 
    // reverse the string and return it
    return reverse(buffer, 0, i - 1);
}

int main (int argc,char *argv[]) {

    real[0] = '\0';

    char str1[1000];
    str1[0] = '\0';

    strcpy(str1,"#include <stdlib.h>\n#include <stdio.h>\n#include <math.h>\nvoid printsep();\nfloat** matrix_addition(int a, int b,float** m1,float** m2);\nfloat** matrix_subtraction(int a, int b,float** m1,float** m2);\nfloat** matrix_multiplication(int a, int b, int c, int d, float** m1,float** m2);\nfloat sqrt_s(float s);\nfloat** tr(int a, int b, float **m);\nfloat** multiplication(int a, int b, float** m1, float x);\nfloat choose(float a, float b, float c, float d);\nint main() {\n");
    strcat(real,str1);

    FILE *fp;
    FILE *fwrite;

    
    
    char line[256];
    /* Open file for reading Filename is given on the command line */
    
    if (argc != 2) {

        printf("Give filename as command line argument\n") ;
        return(1);

    }
    fp = fopen(argv[1], "r");
    if(fp == NULL) {
        printf("Cannot open %s\n",argv[1]);
        return(1);
    }

    char filename[256];
    filename[0] = '\0';
    strcpy(filename,removed(argv[1]));
    strcat(filename,".c");

    fwrite = fopen(filename,"w");

    int for_count = 0;
    int endfor_count = 0;
    int last_for_index = 0;
    int is_error = 0;

    while( fgets(line,256,fp) != NULL ) {
        line[strcspn(line, "\r\n")] = 0;
   
        int line_code = 0;
        line_code = line_checker(line);
    
        if (line_code==SCALAR){
         
            if(!scalar_control(line)){
                printf("Error (Line %d)\n",curr_line);
                is_error = 1;
                break;
            }
            scalar_printer(line);
        }
        else if (line_code==VECTOR){
          
            if(!vector_control(line)){
                printf("Error (Line %d)\n",curr_line);
                is_error = 1;
                break;
            }
            vector_printer(line);
        }
        else if (line_code==MATRIX){
            if(!matrix_control(line)){
                printf("Error (Line %d)\n",curr_line);
                is_error = 1;
                break;
            }
            matrix_printer(line);
        }
        else if (line_code==COMMENT){
         
            if(!comment_control(line)){
                printf("Error (Line %d)\n",curr_line);
                is_error = 1;
                break;
            }
        }
        else if (line_code==ASSIGNMENT){
            
            if(!assignment_control(line)){
                printf("Error (Line %d)\n",curr_line);
                is_error = 1;
                break;
            }
            assignment_printer(line);
        }
        else if (line_code==FOR){
            
            if(!for_control(line)){
                printf("Error (Line %d)\n",curr_line);
                is_error = 1;
                break;
            }
            for_printer(line);
            last_for_index = curr_line;
            for_count++;
        }
        else if (line_code==PRINT){
          
            if(!print_control(line)){
                printf("Error (Line %d)\n",curr_line);
                is_error = 1;
                break;
            }
            print_printer(line);
        }
        else if (line_code==PRINTSEP){
      
            if(!printsep_control(line)){
                printf("Error (Line %d)\n",curr_line);
                is_error = 1;
                break;
            }
            strcat(real,"printsep();\n");
        }
        else if (line_code==ENDFOR){
            
            if(!endfor_control(line)){
                printf("Error (Line %d)\n",curr_line);
                is_error = 1;
                break;
            }
            endfor_count++;
            if (endfor_count > for_count) {
                printf("Error (Line %d)\n",curr_line);
                is_error = 1;
                break;
            }
            strcat(real,"}\n");
            if (is_nestedfor) {strcat(real,"}\n");is_nestedfor = 0;}
        }

        else{
            if(line[0] != '\0' && line[0] != '\n'){
                printf("Error (Line %d)\n",curr_line);
                is_error = 1;
                break;
            }
        }
        curr_line++;
    }

    if (endfor_count != for_count && !is_error) {
        printf("Error (Line %d)\n",last_for_index);
        is_error = 1;
        }

    if (!is_error) {
        strcat(real,"}\n");
        strcat(real, "void printsep(){\nprintf(\"----------\\n\");\n}\n");
        strcat(real,"float** matrix_subtraction(int a, int b,float** m1,float** m2){\nfloat** result = (float**) malloc(a*sizeof(float*));\nfor (int i=0; i<a; i++){\n\tresult[i] = (float*) calloc(b,sizeof(float));\n\tfor(int j=0; j<b; j++){\n\t\tresult[i][j] = m1[i][j] - m2[i][j];\n}\n}\nreturn result;\n}\nfloat** matrix_addition(int a, int b,float** m1,float** m2){\nfloat** result = (float**) malloc(a*sizeof(float*));\nfor (int i=0; i<a; i++){\n\tresult[i] = (float*) calloc(b,sizeof(float));\n\tfor(int j=0; j<b; j++){\n\t\tresult[i][j] = m1[i][j] + m2[i][j];\n}\n}\nreturn result;\n}\nfloat** matrix_multiplication(int a, int b, int c, int d, float** m1,float** m2){\n\tfloat** result = (float**) malloc(a*sizeof(float*));\n\tfor (int i=0; i<a; i++){\n\t\tresult[i] = (float*) calloc(d,sizeof(float));\n\t}\n\tfor (int i=0; i<a; i++){\n\t\tfor (int j=0; j<d; j++){\n\t\t\tfor (int k=0; k<c; k++){\n\t\t\t\tresult[i][j] += m1[i][k] * m2[k][j];\n\t\t\t}\n\t\t}\n\t}\n\treturn result;\n}\nfloat** multiplication(int a, int b, float** m1, float x) {\n\tfloat** result = (float**) malloc(a*sizeof(float*));\n\tfor (int i=0; i<a; i++){\n\t\tresult[i] = (float*) calloc(b,sizeof(float));\n\t\tfor (int j=0; j<b; j++){\n\t\t\tresult[i][j] = m1[i][j] * x;\n\t\t}\n\t}\n\treturn result;\n}\nfloat sqrt_s(float s){\n\treturn sqrt(s);\n}\nfloat** tr(int a, int b, float **m){\n\tfloat** result = (float**) malloc(b*sizeof(float*));\n\tfor (int i=0; i<b; i++){\n\t\tresult[i] = (float*) calloc(a,sizeof(float));\n\t\tfor (int j=0; j<a; j++){\n\t\t\tresult[i][j] = m[j][i];\n\t\t}\n\t}\n\treturn result;\n}\nfloat choose(float a, float b, float c, float d){\n\tif (a==0)\n\t\treturn b;\n\tif (a > 0)\n\t\treturn c;\n\tif (a < 0)\n\t\treturn d;\n}");
       
        fprintf(fwrite,"%s",real);
    }

    fclose(fp);

    fclose(fwrite);

    return(0);

}

int line_checker(char* line){
    //lineları kontrol ediyor

    char* pline = (char*)malloc(256);
    //char* pline = line;
    strcpy(pline, line);

    char *q ;
    char *token ;

    add_spaces(pline);
    
    q = strdup(pline);
    
    free(pline);
    while( (token = strsep(&q," ")) != NULL ){
        
        if (strcmp(token,"\n")==0){return 0;}
        else if (strcmp(token,"for")==0){return FOR;}
        else if (strcmp(token,"scalar")==0){return SCALAR;}
        else if (strcmp(token,"vector")==0)return VECTOR;
        else if (strcmp(token,"matrix")==0)return MATRIX;
        else if (strcmp(token,"#")==0)return COMMENT;
        else if (strcmp(token,"=")==0){return ASSIGNMENT;}
        else if (strcmp(token,"print")==0)return PRINT;
        else if (strcmp(token,"printsep")==0)return PRINTSEP;
        else if (strcmp(token,"}")==0)return ENDFOR;  
    }
    
    return(0);
}

char *strsep(char **stringp, const char *delim) {
    //windowsta strsep çalışmıyor sanırım o yüzden internetten bunu buldum
    char *rv = *stringp;
    if (rv) {
        *stringp += strcspn(*stringp, delim);
        if (**stringp)
            *(*stringp)++ = '\0';
        else
            *stringp = 0; }
    return rv;
}

void add_spaces(char* pline) {
    // sembollerin öncesine ve sonrasına boşluk koyuyor
    char temp[256];
    char* x = pline;
    int i = 0;
    while (*pline != '\0'){
        temp[i] = *pline;
        if (is_not_char((char)*(pline+1))== 1){temp[i+1] = ' ';i++;}
        if (is_not_char((char)*(pline))== 1){temp[i+1] = ' ';i++;}
        i++;
    
        *pline++;
    }
    temp[i] = *pline;
    int j =0;
    while (temp[j] != '\0'){
        *x = temp[j];
        *x++;
        j++;
    }
    *x = temp[j];

}

int is_not_char(char s){
    // karakterlerin sembol olmasını kontrol ediyor
    char not_chars[] = {'+','-',',','{','}',':',';','(',')','#','=','[',']','*'};
    int length = sizeof(not_chars)/sizeof(not_chars[0]);
    for (int i=0;i<length;i++){
        if (not_chars[i] == s){return 1;}
    }
    return 0;
}

int scalar_control(char* line){
    // satırda iki token bulunması gerektiği için bunu ve tokenin isimlendirmesinin doğru olup olmamasını kontrol ediyor
    char** token_array = get_token_array(line);

    int size = 0;

    while (strcmp(token_array[size],"\0")!=0){
        size++;
    }

    if (size != 2){free(token_array);return 0;}
    if (strcmp(token_array[2],"\0")!=0){free(token_array);return 0;}   
    if (!is_id(token_array[1])){free(token_array);return 0;}
    if (is_exist(token_array[1]) != 0){free(token_array);return 0;} // daha önce tanımladı mı

    scalar_array[scalar_array_size] = token_array[1];
    scalar_array_size++;

    free(token_array);
    return 1;
}

int vector_control(char* line){
    // satırda 5 token bulunmasını 'vector','id','[','int',']' ve variable adını kontrol ediyor
    char** token_array = get_token_array(line);

    int size = 0;

    while (strcmp(token_array[size],"\0")!=0){
        size++;
    }

    if (size != 5){free(token_array);return 0;} //token sayısı
    if (strcmp(token_array[5],"\0")!=0){free(token_array);return 0;}//6. token bos mu
    if (!is_id(token_array[1])){free(token_array);return 0;}// variable adi dogru mu
    if ((strcmp(token_array[2],"[")!=0) || (strcmp(token_array[4],"]")!=0)) {free(token_array);return 0;} // 3. ve 5. token koseli parantez mi
    if (!is_digit(token_array[3])){free(token_array);return 0;}//4. token integer mı
    if (is_exist(token_array[1]) != 0){free(token_array);return 0;} // daha önce tanımladı mı

    vector_array[vector_array_size] = token_array[1];
    vector_array_size++;
    vector_array[vector_array_size] = token_array[3];
    vector_array_size++;

    return 1;
}

int matrix_control(char* line) {
    char** token_array = get_token_array(line);

    int size = 0;

    while (strcmp(token_array[size],"\0")!=0){
        size++;
    }

    if (size != 7){free(token_array);return 0;} //token sayısı
    if (strcmp(token_array[7],"\0")!=0){free(token_array);return 0;}//8. token bos mu
    if (!is_id(token_array[1])){free(token_array);return 0;}// variable adi dogru mu
    if ((strcmp(token_array[2],"[")!=0) || (strcmp(token_array[4],",")!=0) || (strcmp(token_array[6],"]")!=0)) {free(token_array);return 0;} // 3. ve 5. token koseli parantez mi
    if (!is_digit(token_array[3]) || !is_digit(token_array[5])){free(token_array);return 0;}//4. ve 6. token integer mı
    if (is_exist(token_array[1]) != 0){free(token_array);return 0;} // daha önce tanımladı mı

    matrix_array[matrix_array_size] = token_array[1];
    matrix_array_size++;
    matrix_array[matrix_array_size] = token_array[3];
    matrix_array_size++;
    matrix_array[matrix_array_size] = token_array[5];
    matrix_array_size++;

    return 1;
}

int print_control(char* line) {
    char** token_array = get_token_array(line);

    int size = 0;
    while (strcmp(token_array[size],"\0")!=0){
        size++;
    }
    if (size <= 3){free(token_array);return 0;}

    if (strcmp(token_array[1],"(")!=0){free(token_array);return 0;}
    if (strcmp(token_array[size - 1],")")!=0){free(token_array);return 0;}
    if (size- 2 == 1) {free(token_array);return 0;}

    char** id_token_array = sub_array(token_array,2,size - 1);
    if (!id_control(id_token_array,size - 3)) {return 0;}

    return 1;
}

int printsep_control(char* line) {
    char** token_array = get_token_array(line);

    int size = 0;
    while (strcmp(token_array[size],"\0")!=0){
        size++;
    }
    
    if (size != 3){free(token_array);return 0;}
    if (strcmp(token_array[1],"(")!=0){free(token_array);return 0;}
    if (strcmp(token_array[2],")")!=0){free(token_array);return 0;}

    return 1;

}

int endfor_control(char* line) {
    char** token_array = get_token_array(line);

    int size = 0;
    while (strcmp(token_array[size],"\0")!=0){
        size++;
    }

    if (size != 1) {return 0;}
    if (strcmp(token_array[0],"}")!=0){free(token_array);return 0;}

    return 1;
}

int comment_control(char* line) {
    char** token_array = get_token_array(line);

    if (strcmp(token_array[0],"#")!=0){free(token_array);return 0;}

    free(token_array);

    return 1;
}

int for_control(char* line){
    char** token_array = get_token_array(line);
    int size = 0;

    while (strcmp(token_array[size],"\0")!=0){
        size++;
    }

    if (size < 11) {free(token_array);return 0;}
    if (strcmp(token_array[0],"for")!=0){free(token_array);return 0;}
    if ((strcmp(token_array[1],"(")!=0)  || (strcmp(token_array[size-2],")")!=0)  || (strcmp(token_array[size-1],"{")!=0)){free(token_array);return 0;}
    if (strcmp(token_array[3],",")==0) {
        if (strcmp(token_array[5],"in")!=0){free(token_array);return 0;}
        int clas1 = is_exist(token_array[2]);
        if (clas1 != 1){free(token_array);return 0;}
        int clas2 = is_exist(token_array[4]);
        if (clas2 != 1){free(token_array);return 0;}

        char** remaining_token_array = sub_array(token_array,6,size-2);


        int size1 = size-8 ;

        if (size1 < 11) {free(token_array);return 0;}
        int open_parantez_num = 0;
        int close_parantez_num = 0;
        int open_square_num = 0;
        int close_square_num = 0;

        int comma_num = 0;

        int ind_from = 0;
        for (int i = 0; i < size1; i++){
            if(strcmp(remaining_token_array[i],"(")==0){open_parantez_num++;}
            if(strcmp(remaining_token_array[i],")")==0){close_parantez_num++;}
            if (close_parantez_num > open_parantez_num){return 0;}
            if(strcmp(remaining_token_array[i],"[")==0){open_square_num++;}
            if(strcmp(remaining_token_array[i],"]")==0){close_square_num++;}
            if (close_square_num > open_square_num){return 0;}

            if ((strcmp(remaining_token_array[i],":")==0) && (close_parantez_num == open_parantez_num)) {
                if (ind_from == i) {return 0;}
                char** expression_token_array = sub_array(remaining_token_array,ind_from,i);
               
                int* expression = expression_control(expression_token_array,i - ind_from);
                if(expression[0] == 0) {free(token_array);return 0;}
                if(expression[3] != 1) {free(token_array);return 0;}

                ind_from = i + 1;
            }
            if ((strcmp(remaining_token_array[i],",")==0) && (close_parantez_num == open_parantez_num) && (close_square_num == open_square_num)) {
                if (comma_num == 1) {return 0;}
                if (ind_from == i) {return 0;}
                comma_num++;
                char** expression_token_array = sub_array(remaining_token_array,ind_from,i);
              
                int* expression = expression_control(expression_token_array,i - ind_from);
                if(expression[0] == 0) {free(token_array);return 0;}
                if(expression[3] != 1) {free(token_array);return 0;}

                ind_from = i + 1;
            }
        }
        if (ind_from >= size1) {return 0;}
        char** expression_token_array = sub_array(remaining_token_array,ind_from,size1);

        int* expression = expression_control(expression_token_array,size1 - ind_from);
        if(expression[0] == 0) {free(token_array);return 0;}
        if(expression[3] != 1) {free(token_array);return 0;}

    } else {
        if ((strcmp(token_array[1],"(")!=0) || (strcmp(token_array[3],"in")!=0) || (strcmp(token_array[size-2],")")!=0)  || (strcmp(token_array[size-1],"{")!=0)){free(token_array);return 0;}
        int clas = is_exist(token_array[2]);
        if (clas != 1){free(token_array);return 0;}

        char* column = ":";
        int ind_column = find_index(column,token_array,size);
        if (ind_column == -1) {free(token_array);return 0;}
        if (ind_column - 4 < 1) {free(token_array);return 0;}
        char** expression1_token_array = sub_array(token_array,4,ind_column);
 
        int* expression1 = expression_control(expression1_token_array,ind_column-4);
        if(expression1[0] == 0) {free(token_array);return 0;}
        if(expression1[3] != 1) {free(token_array);return 0;}


        char** remaining_token_array = sub_array(token_array,ind_column+1,size-2);
    
        int ind_column2 = find_index(column,remaining_token_array,size - ind_column- 3);
        if (ind_column2 == -1) {free(token_array);return 0;}
        if (ind_column2 == 0) {free(token_array);return 0;}
        char** expression2_token_array = sub_array(remaining_token_array,0,ind_column2);
   
        int* expression2 = expression_control(expression2_token_array,ind_column2);
        if(expression2[0] == 0) {free(token_array);return 0;}
        if(expression2[3] != 1) {free(token_array);return 0;}

        int size2 = size - ind_column- ind_column2 - 4;
        if (size2 < 1)  {free(token_array);return 0;}
        char** expression3_token_array = sub_array(remaining_token_array,ind_column2 + 1,size - ind_column- 3);
    
        int* expression3 = expression_control(expression3_token_array,size - ind_column- ind_column2 - 4);
        if(expression3[0] == 0) {free(token_array);return 0;}
        if(expression3[3] != 1) {free(token_array);return 0;}

    }

    return 1;

}

int assignment_control(char* line){
    char** token_array = get_token_array(line);

    int size = 0;
    while (strcmp(token_array[size],"\0")!=0){
        size++;
    }

    if (size < 3){free(token_array);return 0;}
    if (strcmp(token_array[0],"=")==0){free(token_array);return 0;}
    int clas = is_exist(token_array[0]);
    if (clas == 0){free(token_array);return 0;}


    if (strcmp(token_array[1],"=")==0){
        if (strcmp(token_array[2],"{")==0){
            if(!assign_control(token_array, size)){free(token_array);return 0;}
        } else {
            char** expression_token_array = sub_array(token_array,2,size);
         
            int* expression = expression_control(expression_token_array,size-2);
            if(!expression[0]){return 0;}

            if (clas == 2) {
                if ((expression[3] != 2 ) && (expression[3] != 3 )) {return 0;}
            }
            else if (clas != expression[3]){return 0;}
        }
    } else if(strcmp(token_array[1],"[")==0) {
        if (clas == 2) {
            if (strcmp(token_array[3],"]")!=0) {return 0;}
            if (strcmp(token_array[4],"=")!=0) {return 0;}
            char** expression_token_array = sub_array(token_array,5,size);
            
            int* expression = expression_control(expression_token_array,size-5);
            if(!expression[0]){return 0;}

            if (expression[3] != 1 ) {return 0;}

        } else if (clas == 3) {
            if (strcmp(token_array[3],",")!=0) {return 0;}
            if (strcmp(token_array[5],"]")!=0) {return 0;}
            if (strcmp(token_array[6],"=")!=0) {return 0;}
            char** expression_token_array = sub_array(token_array,7,size);
            
            int* expression = expression_control(expression_token_array,size-7);
            if(!expression[0]){return 0;}
            
            if (expression[3] != 1 ) {return 0;}
        } else {return 0;}

    } else {return 0;}


    //assignment_printer(token_array);

    return 1;
}

int assign_control(char** token_array, int size){
    char* var = token_array[0];
    int data = is_exist(var);
    int curr = 3;

    if (data == 2) {
        int index = find_index(var,vector_array,vector_array_size);
        int a = atoi(vector_array[index+1]);
        if (a+3 != size - 1)return 0;
        for (int i = 3; i < size - 1; i++) {
            if(!is_number(token_array[i])) return 0;
        }

    } else if (data == 3) {
        int index = find_index(var,matrix_array,matrix_array_size);
        int a = atoi(matrix_array[index+1]);
        int b = atoi(matrix_array[index+2]);
        if (a*b+3 != size - 1)return 0;
        for (int i = 3; i < size - 1; i++) {
            if(!is_number(token_array[i])) return 0;
        }
    }

    return 1;
}

int id_control(char** token_array,int size) {
    int clas = is_exist(token_array[0]);
    if (clas == 0) return 0;
    if (size == 1) return 1;
    if (size == 4) {
        if (clas == 1) { return 0;}
        if (strcmp(token_array[1],"[")!=0) {return 0;}
        if (strcmp(token_array[3],"]")!=0) {return 0;}
        if (!is_digit(token_array[2])) {return 0;}
        return 1;
        
    } else if (size == 6) {
        if (clas == 1) { return 0;}
        if (clas == 2) { return 0;}
        if (strcmp(token_array[1],"[")!=0) {return 0;}
        if (strcmp(token_array[3],",")!=0) {return 0;}
        if (strcmp(token_array[5],"]")!=0) {return 0;}
        if (!is_digit(token_array[2])) {return 0;}
        if (!is_digit(token_array[4])) {return 0;}
        return 1;

    } else return 0;
}

int* expression_control(char** token_array,int size) {
    //
    int open_parantez_num = 0;
    int close_parantez_num = 0;

    int row = 1;
    int column = 1;

    int* valid = (int*)malloc(512);
    int* error = (int*)malloc(512);
    error[0] = 0;
    error[1] = 0;
    error[2] = 0;
    valid[0] = 1;

    int is_term = 1;

    for (int i = 0; i < size; i++){
        if(strcmp(token_array[i],"(")==0){open_parantez_num++;}
        if(strcmp(token_array[i],")")==0){close_parantez_num++;}
        if (close_parantez_num > open_parantez_num){return error;}
        if ((strcmp(token_array[i],"+")==0) && close_parantez_num == open_parantez_num)  {
            // toplama

            if (i == 0) return error;
            char** term_token_array = sub_array(token_array,0,i);

            int* term1 = term_control(term_token_array, i);
            if (!term1[0]) {
                return error;
            }
            if (i + 1 == size) return error;
            char** expression_token_array = sub_array(token_array,i + 1,size );
            int* term2 = expression_control(expression_token_array,size - i -1);
            if (!term2[0]) {
                return error;
            }
            if (term1[1] != term2[1] || term1[2] != term2[2]) return error;
            if (term1[3] != term2[3]) return error;
            row = term1[1];
            column = term1[2];
            valid[3] = term1[3];

            is_term = 0;
            break;
        }
        if((strcmp(token_array[i],"-")==0) && close_parantez_num == open_parantez_num) {
            //cikarma
            if (i == 0) return error;
            char** term_token_array = sub_array(token_array,0,i);

            int* term1 = term_control(term_token_array, i);
            if (!term1[0]) {
                return error;
            }
            if (i + 1 == size) return error;
            char** expression_token_array = sub_array(token_array,i + 1,size );
            int* term2 = expression_control(expression_token_array,size - i -1);
            if (!term2[0]) {
                return error;
            }
            if (term1[1] != term2[1] || term1[2] != term2[2]) return error;
            if (term1[3] != term2[3]) return error;
            row = term1[1];
            column = term1[2];
            valid[3] = term1[3];

            is_term = 0;
            break;
        }
    }

    if (is_term) {
        int* term1 = term_control(token_array, size);
        if (!term1[0]) {
            return error;
        }

        row = term1[1];
        column = term1[2];
        valid[3] = term1[3];
    }

    valid[1] = row;
    valid[2] = column;

    return valid;
}

int* term_control(char** token_array,int size) {

    int open_parantez_num = 0;
    int close_parantez_num = 0;

    int is_factor = 0;

    int row = 1;
    int column = 1;
    int index = 0;
    int* valid = (int*)malloc(512);
    int* error = (int*)malloc(512);
    error[0] = 0;
    error[1] = 0;
    error[2] = 0;
    valid[0] = 1;


    if (size == 1) {
        if (isdigit(token_array[0][0])) {
            if (!is_number(token_array[0])) {return error;}
            valid[1] = row;
            valid[2] = column;
            valid[3] = 1;

        } else {
            int clas = is_exist(token_array[0]);
            if (is_exist(token_array[0]) == 0 ) {return  error;}
            if (clas == 1) {}
            else if (clas == 2) {index = find_index(token_array[0],vector_array,vector_array_size); row = atoi(vector_array[index +1]);}
            else if (clas == 3) {index = find_index(token_array[0],matrix_array,matrix_array_size); row = atoi(matrix_array[index +1]); column = atoi(matrix_array[index +2]);}

            valid[1] = row;
            valid[2] = column;
            valid[3] = clas;
        }
        return valid;
    }

    for (int i = 0; i < size; i++){
        if(strcmp(token_array[i],"(")==0){open_parantez_num++;}
        if(strcmp(token_array[i],")")==0){close_parantez_num++;}
        if (close_parantez_num > open_parantez_num){return error;}
        if ((strcmp(token_array[i],"*")==0) && close_parantez_num == open_parantez_num)  {
            if (i == 0) return error;
            char** term_token_array = sub_array(token_array,0,i);
            int* term1 = term_control(term_token_array, i);
            if (!term1[0]) {
                return error;
            }
            if (i + 1 == size) return 0;
            char** term_token_array2 = sub_array(token_array,i + 1,size );

            int* term2 = term_control(term_token_array2,size - i -1);
            if (!term2[0]) {
                return error;
            }
            if ((term1[3] != 1) && (term2[3] != 1)) {if(term1[2] != term2[1]) return error;}

            is_factor = 1;

            valid[1] = term1[1];
            valid[2] = term2[2];
            if ((term1[3] == 1) && (term2[3] == 1)) {valid[3] = 1;}
            else if ((term1[3] == 3) || (term2[3] == 3)) {valid[3] = 3;}
            else {valid[3] = 2;}
        }
    }

    if (is_factor){
        return valid;
    }

    if (strcmp(token_array[0],"(")==0) {
        if (strcmp(token_array[size - 1],")")!=0) {return error;}
        if (size == 2) {return error;}
        char** expression_token_array = sub_array(token_array,1,size - 1);
        int* expression = expression_control(expression_token_array,size-2);
        if (!expression[0]) {
            return error;
        }
        row = expression[1];
        column = expression[2];
        valid[1] = row;
        valid[2] = column;
        valid[3] = expression[3];
        return valid;

    } else if (strcmp(token_array[0],"sqrt")==0) {
        if ((strcmp(token_array[size - 1],")")!=0) || (strcmp(token_array[1],"(")!=0)){return error;}
        if (size == 3) {return error;}
        char** expression_token_array = sub_array(token_array,2,size - 1);

        int* expression = expression_control(expression_token_array,size-3);
        if (!expression[0]) {
            return error;
        }
        if ((expression[1] != 1) || (expression[2] != 1)){ return error;}
        if (expression[3] != 1){ return error;}
        valid[1] = row;
        valid[2] = column;
        valid[3] = 1;
        return valid;

    } else if (strcmp(token_array[0],"tr")==0) {
        if ((strcmp(token_array[size - 1],")")!=0) || (strcmp(token_array[1],"(")!=0)){return error;}
        if (size == 3) {return error;}
        char** expression_token_array = sub_array(token_array,2,size - 1);
       
        int* expression = expression_control(expression_token_array,size-3);
        if (!expression[0]) {
            return error;
        }
        row = expression[2];
        column = expression[1];
        valid[1] = row;
        valid[2] = column;
        if (expression[3] == 2) {
            valid[3] = 3;
        } else{
            valid[3] = expression[3];
        }   
        return valid;

    } else if (strcmp(token_array[0],"choose")==0){
        if ((strcmp(token_array[size - 1],")")!=0) || (strcmp(token_array[1],"(")!=0)){return 0;}
        if (size < 10) {return error;}

        open_parantez_num = 0;
        close_parantez_num = 0;
        int open_square_num = 0;
        int close_square_num = 0;
        int from = 2;
        int comma = 0;

        for (int i = 2; i < size - 1; i++){
            if(strcmp(token_array[i],"(")==0){open_parantez_num++;}
            if(strcmp(token_array[i],")")==0){close_parantez_num++;}
            if(strcmp(token_array[i],"[")==0){open_square_num++;}
            if(strcmp(token_array[i],"]")==0){close_square_num++;}
            if (close_parantez_num > open_parantez_num){return error;}
            if (close_square_num > open_square_num){return error;}
            if ((strcmp(token_array[i],",")==0) && close_parantez_num == open_parantez_num && close_square_num == open_square_num)  {
                if (i == from) return error;
                if (comma == 3) return error;
                char** expression_token_array = sub_array(token_array,from,i);

                int* expression = expression_control(expression_token_array, i - from);
                if (!expression[0]) {
                    return error;
                }
                if ((expression[1] != 1) || (expression[2] != 1)){ return error;}
                if (expression[3] != 1) { return error;}
                from = i+1;
                comma++;
            }
        }

        if (comma != 3) {return error;}
        else {
            if(from == size - 1){return error;}
            char** expression_token_array = sub_array(token_array,from,size - 1);

            int* expression = expression_control(expression_token_array, size - from - 1);
            if (!expression[0]) {
                return error;
            }
            if ((expression[1] != 1) || (expression[2] != 1)){ return error;}
            if (expression[3] != 1) { return error;}
        }

        valid[1] = row;
        valid[2] = column;
        valid[3] = 1;
        return valid;

    } else if (strcmp(token_array[1],"[")==0) {
        if (strcmp(token_array[size - 1],"]")!=0) {return error;}
        if (size - 2 == 1) {return error;}

        int clas = is_exist(token_array[0]);
        if (clas == 0) {return error;}
        if (clas == 1) {return error;}
        if (clas == 2) {
            char** expression_token_array = sub_array(token_array,2,size - 1);
            
            int* expression = expression_control(expression_token_array, size - 3);
            if (!expression[0]) {
                return error;
            }
            if ((expression[1] != 1) || (expression[2] != 1)){ return error;}
            valid[1] = row;
            valid[2] = column;
            valid[3] = 1;
            return valid;
        } else if (clas == 3) {
            char* index_of = ",";
            int index = find_index(index_of,token_array,size);
          
            if (index == -1) { 
                char** expression_token_array = sub_array(token_array,2,size - 1);
               
                int* expression = expression_control(expression_token_array, size - 3);
                if (!expression[0]) {
                    return error;
                }
                if ((expression[1] != 1) || (expression[2] != 1)){ return error;}
                valid[1] = row;
                valid[2] = column;
                valid[3] = 2;
                return valid;
            } else {
                if (index== 2) { return error;}
                char** expression_token_array1 = sub_array(token_array,2,index);
               
                int* expression1 = expression_control(expression_token_array1, index - 2);
                if (!expression1[0]) {
                    return error;
                }
                if ((expression1[1] != 1) || (expression1[2] != 1)){ return error;} 
                char** expression_token_array2 = sub_array(token_array,index+1,size - 1);
    
                int* expression2 = expression_control(expression_token_array2, size - index-2);
                if (!expression2[0]) {
                    return error;
                }
                if ((expression2[1] != 1) || (expression2[2] != 1)){ return error;}
            }
        }

        valid[1] = row;
        valid[2] = column;
        valid[3] = 1;
        return valid;
    }

    valid[1] = row;
    valid[2] = column;
    

    return error;
}


char** get_token_array(char* line){
    //tokenleri array haline getirip donduruyor
    char* pline = (char*)malloc(512);
    strcpy(pline, line);

    char *q ;
    char *token ;

    add_spaces(pline);
    
    q = strdup(pline);

    int i = 0;  
    char** token_array = (char**) malloc(512);

    while( (token = strsep(&q," ")) != NULL ){
        if (strcmp(token,"")==0) {
            continue;
        } else if (strcmp(token,"\n")==0) {
            continue;
        } else if (strcmp(token," ")==0) {
            continue; 
        } else {
            token_array[i] = token;
        }
        i++;
    }
    token_array[i] = "\0";
    free(pline);

    return token_array;
}

int is_id(char* token){
    //tokenin sayıyla başlamayıp alfanumerik karakterler içermesini kontrol ediyor
    int is_id = 1;
    char* str = token;

    if (!isalpha(*str) && *str != '_') {
        return 0;
    }
    str++;
    for(str; *str != '\0' ; str++) {
       
        is_id = is_id && (isalpha(*str)||isdigit(*str)|| *str=='_');
    }

    return is_id;
}

int is_digit(char* token){
    int is_digit = 1;
    char* str = token;

    for(str; *str != '\0' ; str++) {
        is_digit = is_digit && isdigit(*str);
    }

    return is_digit;
}

int is_number(char* token) {
    int is_number = 1;
    char* str = token;
    int dot_number = 0;

    if (!isdigit(*str)){return 0;}

    for(str; *str != '\0' ; str++) {
        
        if (*str == '.'){
            dot_number++;
            if (dot_number > 1) return 0;
        }
        
        is_number = is_number && (isdigit(*str) || *str == '.');
    }

    if (!isdigit(*--str)){return 0;}

    return 1;
}

int is_exist(char* var){
    // variable daha önce tanımlandı mı tanımlandıysa turu ne scalar-1 vector-2 matrix-3

    for (int i = 0; i < scalar_array_size; i++) {
        if (strcmp(scalar_array[i],var)==0) {
            return 1;
        }
    }
    for (int j = 0; j < vector_array_size; j++) {
        if (strcmp(vector_array[j],var)==0) {
            return 2;
        }
    }
    for (int k = 0; k < matrix_array_size; k++) {
        if (strcmp(matrix_array[k],var)==0) {
            return 3;
        }
    }
    return 0;
}

int find_index(char* var,char** array,int array_size){
    int index = -1;
     
    for (int i = 0; i < array_size; i++) {
        if (strcmp(array[i],var)==0) {
            index = i;
            break;
        }
    }
    return index;
}

char** sub_array(char** array, int from, int to) {
    char** sub_array = (char**)malloc(512);

    int i = 0;

    for (from;from < to; from++){
        sub_array[i] = array[from];
        
        i++;
    }

    return sub_array;
}

void scalar_printer(char* line){
    char** token_array = get_token_array(line);

    int size = 0;

    while (strcmp(token_array[size],"\0")!=0){
        size++;
    }

    char str1[1000];
    str1[0] = '\0';

    sprintf(str1,"float %s;\n",token_array[1]);

    strcat(real,str1);
}

void vector_printer(char* line){
    char** token_array = get_token_array(line);

    int size = 0;

    while (strcmp(token_array[size],"\0")!=0){
        size++;
    }

    int index = find_index(token_array[1],vector_array,vector_array_size);
    int row = atoi(vector_array[index +1]);

    char rowc[10];
    itoa(row,rowc,10);

    char str1[1000];
    str1[0] = '\0';
 
    sprintf(str1,"float** %s = (float** ) malloc(%s* sizeof(float*));\n",token_array[1],rowc);
  
    strcat(real,str1);
    
}

void matrix_printer(char* line){
    char** token_array = get_token_array(line);

    int size = 0;

    while (strcmp(token_array[size],"\0")!=0){
        size++;
    }
    int index = find_index(token_array[1],matrix_array,matrix_array_size);
    int row = atoi(matrix_array[index +1]);

    char rowc[10];
    itoa(row,rowc,10);

    char str1[1000];
    str1[0] = '\0';
    
    sprintf(str1,"float** %s = (float** ) malloc(%s* sizeof(float*));\n",token_array[1],rowc);

    strcat(real,str1);


}

void assignment_printer(char* line){
    char** token_array = get_token_array(line);

    int size = 0;

    while (strcmp(token_array[size],"\0")!=0){
        size++;
    }


    if (strcmp(token_array[2],"{")==0) {

        int clas = is_exist(token_array[0]);

        int dimen = 0;
        int row = 1;
        int column = 1;
        char rowc[10];
        char columnc[10];

        if (clas == 2) {
            int index = find_index(token_array[0],vector_array,vector_array_size);
            row = atoi(vector_array[index +1]);
            dimen = row;
        } else if (clas == 3) {
            int index = find_index(token_array[0],matrix_array,matrix_array_size);
            row = atoi(matrix_array[index +1]);
            column = atoi(matrix_array[index +2]);
            dimen = row * column;
        }
        char dimens[20];
        itoa(dimen,dimens,10);
        itoa(row,rowc,10);
        itoa(column,columnc,10);
        
        char str1[1000];
        str1[0] = '\0';
        sprintf(str1,"float _%s[%s] = {",token_array[0],dimens);
        strcat(str1,token_array[3]);
        for (int i = 0; i < dimen - 1;i++){
            strcat(str1,",");
            strcat(str1,token_array[4 + i]);
        }
        strcat(str1,"};");

        char str2[1000];
        str2[0] = '\0';
        sprintf(str2,"for (int i = 0; i< %s;i++){\n\t%s[i] = (float *) calloc(%s,sizeof(float));\n\tfor (int j = 0; j< %s; j++){\n\t\t%s[i][j]= _%s[i*%s+j];}}\n",rowc,token_array[0],columnc,columnc,token_array[0],token_array[0],columnc);

        char str3[1000];
        str3[0] = '\0';
        sprintf(str3,"%s\n%s\n",str1,str2);

        strcat(real,str3);

    } else if (strcmp(token_array[1],"[")==0) {
        int clas = is_exist(token_array[0]);
        if (clas == 2) {
            char str2[1000];
            str2[0] = '\0';
            char str3[1000];
            str3[0] = '\0';

            char** expression_token_array = sub_array(token_array,5,size);
            int* expression = expression_control(expression_token_array,size-5);

            expression_printer(expression_token_array,size-5,str2);

            sprintf(str3,"%s[%s-1][0] = %s;\n",token_array[0],token_array[2],str2);
            strcat(real,str3);

        } else if (clas == 3) {
            char str2[1000];
            str2[0] = '\0';
            char str3[1000];
            str3[0] = '\0';

            char** expression_token_array = sub_array(token_array,7,size);
            int* expression = expression_control(expression_token_array,size-7);

            expression_printer(expression_token_array,size-7,str2);

            sprintf(str3,"%s[%s-1][%s-1] = %s;\n",token_array[0],token_array[2],token_array[4],str2);
            strcat(real,str3);
        } 
    } else {
        int clas = is_exist(token_array[0]);

        char** expression_token_array = sub_array(token_array,2,size);
        int* expression = expression_control(expression_token_array,size-2);
        char str2[1000];
        str2[0] = '\0';
        char str3[1000];
        str3[0] = '\0';
        expression_printer(expression_token_array,size-2,str2);

        if (clas == 1){
            sprintf(str3,"\n%s = %s;\n",token_array[0],str2);
            strcat(real,str3);
        } else {
            int dimen = 0;
            int row = 1;
            int column = 1;
            char rowc[10];
            char columnc[10];

            if (clas == 2) {
                int index = find_index(token_array[0],vector_array,vector_array_size);
                row = atoi(vector_array[index +1]);
                dimen = row;
            } else if (clas == 3) {
                int index = find_index(token_array[0],matrix_array,matrix_array_size);
                row = atoi(matrix_array[index +1]);
                column = atoi(matrix_array[index +2]);
                dimen = row * column;
            }
            char dimens[20];
            itoa(dimen,dimens,10);
            itoa(row,rowc,10);
            itoa(column,columnc,10);

            char str4[1000];
            str4[0] = '\0';
            char str5[1000];
            str5[0] = '\0';
            sprintf(str4,"float** _%s = %s;\n",token_array[0],str2);

            sprintf(str5,"for (int i = 0; i< %s;i++){\n\t%s[i] = (float *) calloc(%s,sizeof(float));\n\tfor (int j = 0; j< %s; j++){\n\t\t%s[i][j]= _%s[i][j];}}\n",rowc,token_array[0],columnc,columnc,token_array[0],token_array[0]);

            sprintf(str3,"%s%s\n",str4,str5);
            strcat(real,str3);
        }
    }
}

void print_printer(char* line){
    char** token_array = get_token_array(line);

    int size = 0;

    while (strcmp(token_array[size],"\0")!=0){
        size++;
    }

    int clas = is_exist(token_array[2]);
    char rowc[10];
    char columnc[10];

    if (clas  == 1){
        char str1[1000];
        str1[0] = '\0';
        sprintf(str1,"printf(\"%%f\\n\",%s);\n",token_array[2]);

        strcat(real,str1);
    } else if (clas == 2){
        if (strcmp(token_array[3],"[")==0){
            char str1[1000];
            str1[0] = '\0';
            sprintf(str1,"printf(\"%%f\\n\",%s[%s-1][0]);\n",token_array[2],token_array[4]);
            strcat(real,str1);
        } else {
            char str1[1000];
            str1[0] = '\0';
            int index = find_index(token_array[2],vector_array,vector_array_size);
            int row = atoi(vector_array[index +1]);
            sprintf(str1,"for(int i=0; i < %s; i++){\nprintf(\"%%f\\n\",%s[i][0]);}\n",vector_array[index +1],token_array[2]);
            strcat(real,str1);
        }
    } else if (clas == 3){
        int index = find_index(token_array[2],matrix_array,matrix_array_size);
        int row = atoi(matrix_array[index +1]) - 1;
        int column = atoi(matrix_array[index +1]) - 1;
        
        itoa(row,rowc,10); 
        itoa(column,columnc,10);
       
        if(strcmp(token_array[3],")")==0){
            
            char str1[1000];
            str1[0] = '\0';
            sprintf(str1,"for(int i=0; i <= %s; i++){\n\tfor(int j=0; j <= %s; j++){\n\tprintf(\"%%f\\t\",%s[i][j]);}\nprintf(\"\\n\");}\n",rowc,columnc,token_array[2]);
            strcat(real,str1);

        } else if (strcmp(token_array[5],"]")==0){
            char str1[1000];
            str1[0] = '\0';
            sprintf(str1,"for(int i=0; i <= %s; i++){\nprintf(\"%%f\\t\",%s[%s-1][i]);}\n",columnc,token_array[2],token_array[4]);
            strcat(real,str1);
        } else if (strcmp(token_array[7],"]")==0){
            char str1[1000];
            str1[0] = '\0';  
            sprintf(str1,"printf(\"%%f\\n\",%s[%s-1][%s-1]);\n",token_array[2],token_array[4],token_array[6]);
            strcat(real,str1);
        }
    }
}

int expression_printer(char** token_array,int size,char* str) {

    int open_parantez_num = 0;
    int close_parantez_num = 0;

    int row = 1;
    int column = 1;
    int clas= 0;

    char rowc[10];
    char columnc[10];

    int is_term = 1;
    char str1[1000], str2[1000], str3[1000]; 
   
    str1[0] = str2[0] = str3[0] = '\0' ; 

    for (int i = size-1; i >= 0; i--){
        if(strcmp(token_array[i],"(")==0){open_parantez_num++;}
        if(strcmp(token_array[i],")")==0){close_parantez_num++;}
        
        if ((strcmp(token_array[i],"+")==0) && close_parantez_num == open_parantez_num)  {

            char** term_token_array = sub_array(token_array,i+1,size);

            int* term1 = term_control(term_token_array, size - i -1);
            if (!term1[0]) {
               
            }

            term_printer(term_token_array, size - i -1,str1);

            char** expression_token_array = sub_array(token_array,0,i);
            int* term2 = expression_control(expression_token_array, i );
            if (!term2[0]) {
                
            }
            expression_printer(expression_token_array, i,str2);

            row = term1[1];
            column = term1[2];
            clas = term1[3];

            itoa(row,rowc,10); 
            itoa(column,columnc,10);

            if (clas != 1){
                sprintf(str3,"matrix_addition(%s,%s,%s,%s)",rowc,columnc,str1,str2);
            } else {
                sprintf(str3,"%s + %s",str1,str2);
            }

            strcpy(str,str3);

            is_term = 0;
            break;
        }
        if((strcmp(token_array[i],"-")==0) && close_parantez_num == open_parantez_num) {
          
            char** term_token_array = sub_array(token_array,i+1,size);
            int* term1 = term_control(term_token_array, size - i -1);
            if (!term1[0]) {
                
            }
            term_printer(term_token_array, size - i -1,str1);

            char** expression_token_array = sub_array(token_array,0,i);
            int* term2 = expression_control(expression_token_array, i );
            if (!term2[0]) {
                
            }
            expression_printer(expression_token_array, i,str2);

            row = term1[1];
            column = term1[2];
            clas = term1[3];

            itoa(row,rowc,10); 
            itoa(column,columnc,10);

            if (clas != 1){
                sprintf(str3,"matrix_subtraction(%s,%s,%s,%s)",rowc,columnc,str2,str1);
            } else {
                sprintf(str3,"%s - %s",str2,str1);
            }

            strcpy(str,str3);

            is_term = 0;
            break;
        }
    }

    if (is_term) {
        int* term1 = term_control(token_array, size);
        if (!term1[0]) {

        }
        term_printer(token_array, size,str1);
        row = term1[1];
        column = term1[2];
        clas = term1[3];
        sprintf(str3,"%s",str1);
        strcpy(str,str3);
    }


    return 1;
}

int term_printer(char** token_array,int size, char* str) {

    int open_parantez_num = 0;
    int close_parantez_num = 0;

    int is_factor = 0;

    int row = 1;
    int column = 1;
    int index = 0;

    char rowc[10];
    char columnc[10];

    char str1[1000], str2[1000], str3[1000]; 
   
    str1[0] = str2[0] = str3[0] = '\0';



    if (size == 1) {
        if (isdigit(token_array[0][0])) {
            
            sprintf(str3,"%s",token_array[0]);
            strcpy(str,str3);

        } else {
            int clas = is_exist(token_array[0]);

            if (clas == 1) {}
            else if (clas == 2) {index = find_index(token_array[0],vector_array,vector_array_size); row = atoi(vector_array[index +1]);}
            else if (clas == 3) {index = find_index(token_array[0],matrix_array,matrix_array_size); row = atoi(matrix_array[index +1]); column = atoi(matrix_array[index +2]);}

            sprintf(str3,"%s",token_array[0]);
            strcpy(str,str3);
        }
        return 1;
    }

    for (int i = size-1; i >= 0; i--){
        if(strcmp(token_array[i],"(")==0){open_parantez_num++;}
        if(strcmp(token_array[i],")")==0){close_parantez_num++;}

        if ((strcmp(token_array[i],"*")==0) && close_parantez_num == open_parantez_num)  {
            ;
            char** term_token_array = sub_array(token_array,0,i);

            int* term1 = term_control(term_token_array, i);
            term_printer(term_token_array, i,str1);
            if (!term1[0]) {

            }
            
            char** term_token_array2 = sub_array(token_array,i + 1,size );
            int* term2 = term_control(term_token_array2,size - i -1);
            term_printer(term_token_array2,size - i -1,str2);
            if (!term2[0]) {
                
            }
            if ((term1[3] != 1) && (term2[3] != 1)) {if(term1[2] != term2[1]) return 0;}

            is_factor = 1;

            int clas1 =  term1[3];
            int clas2 = term2[3];

            if((clas1 == 1) && (clas2 == 1)){
                sprintf(str3,"%s * %s",str1,str2);
            }else if (clas1 == 1) {
                row = term2[1];
                column = term2[2];

                itoa(row,rowc,10); 
                itoa(column,columnc,10);

                sprintf(str3,"multiplication(%s,%s,%s,%s)",rowc,columnc,str2,str1);
            } else if (clas2 == 1) { 
                row = term1[1];
                column = term1[2];

                itoa(row,rowc,10); 
                itoa(column,columnc,10);

                sprintf(str3,"multiplication(%s,%s,%s,%s)",rowc,columnc,str1,str2);
            } else {
                row = term1[1];
                column = term2[2];
                int ara = term1[2];
                char arac[10];
                itoa(ara,arac,10);
                itoa(row,rowc,10); 
                itoa(column,columnc,10);

                sprintf(str3,"matrix_multiplication(%s,%s,%s,%s,%s,%s)",rowc,arac,arac,columnc,str1,str2);
            }

            strcpy(str,str3);
        }
    }

    if (is_factor){
        return 1;
    }

    if (strcmp(token_array[0],"(")==0) {
        
        char** expression_token_array = sub_array(token_array,1,size - 1);
        int* expression = expression_control(expression_token_array,size-2);
        if (!expression[0]) {
            
        }
        row = expression[1];
        column = expression[2];
        int clas = expression[3];
        expression_printer(expression_token_array,size-2,str1);
        sprintf(str3,"(%s)",str1);
        strcpy(str,str3);
        return 1;

    } else if (strcmp(token_array[0],"sqrt")==0) {
        
        char** expression_token_array = sub_array(token_array,2,size - 1);
        int* expression = expression_control(expression_token_array,size-3);
        if (!expression[0]) {
            
        }

        expression_printer(expression_token_array,size-3,str1);
        sprintf(str3,"sqrt_s(%s)",str1);
        strcpy(str,str3);
        return 1;

    } else if (strcmp(token_array[0],"tr")==0) {

        char** expression_token_array = sub_array(token_array,2,size - 1);
        int* expression = expression_control(expression_token_array,size-3);
        if (!expression[0]) {
            
        }
        row = expression[1];
        column = expression[2];
        itoa(row,rowc,10); 
        itoa(column,columnc,10);
        int clas= expression[3];
        if (clas == 1) {
            expression_printer(expression_token_array,size-3,str1);
            sprintf(str3,"(%s)",str1);
            strcpy(str,str3);
        } else{
            expression_printer(expression_token_array,size-3,str1);
            sprintf(str3,"tr(%s,%s,%s)",rowc,columnc,str1);
            strcpy(str,str3);
        }   
        return 1;

    } else if (strcmp(token_array[0],"choose")==0){
       
        open_parantez_num = 0;
        close_parantez_num = 0;
        int open_square_num = 0;
        int close_square_num = 0;
        int from = 2;
        int comma = 0;

        char str5[1000], str1[1000], str2[1000], str3[1000],str4[1000]; 
   
        str5[0] = str1[0] = str2[0] = str3[0] = str4[0] = '\0';

        for (int i = 2; i < size - 1; i++){
            if(strcmp(token_array[i],"(")==0){open_parantez_num++;}
            if(strcmp(token_array[i],")")==0){close_parantez_num++;}
            if(strcmp(token_array[i],"[")==0){open_square_num++;}
            if(strcmp(token_array[i],"]")==0){close_square_num++;}
            if ((strcmp(token_array[i],",")==0) && close_parantez_num == open_parantez_num && close_square_num == open_square_num)  {

                char** expression_token_array = sub_array(token_array,from,i);
                int* expression = expression_control(expression_token_array, i - from);
                
                if (comma == 0) {
                    expression_printer(expression_token_array, i - from,str1);
                } else if (comma == 1) {
                    expression_printer(expression_token_array, i - from,str2);
                } else if (comma == 2) {
                    expression_printer(expression_token_array, i - from,str3);
                }

                from = i+1;
                comma++;
            }
        }
 
        char** expression_token_array = sub_array(token_array,from,size - 1);
        int* expression = expression_control(expression_token_array, size - from - 1);
        expression_printer(expression_token_array, size - from - 1,str4);

        sprintf(str5,"choose(%s, %s, %s, %s)",str1,str2,str3,str4);
        strcpy(str,str5);

        return 1;

    } else if (strcmp(token_array[1],"[")==0) {

       

        int clas = is_exist(token_array[0]);

        if (clas == 2) {
            char** expression_token_array = sub_array(token_array,2,size - 1);
            
            int* expression = expression_control(expression_token_array, size - 3);
            if (!expression[0]) {
                
            }
            expression_printer(expression_token_array, size - 3,str1);
            sprintf(str3,"%s[%s-1][0]",token_array[0],str1);
            strcpy(str,str3);

            return 1;
        } else if (clas == 3) {
            char* index_of = ",";
            int index = find_index(index_of,token_array,size);
            
            if (index == -1) { 
                char** expression_token_array = sub_array(token_array,2,size - 1);
               
                int* expression = expression_control(expression_token_array, size - 3);
                if (!expression[0]) {
                   
                }
                expression_printer(expression_token_array, size - 3,str1);

                sprintf(str3,"%s[%s-1]",token_array[0],str1);
                strcpy(str,str3);
                return 1;
            } else {
                
                char** expression_token_array1 = sub_array(token_array,2,index);

                int* expression1 = expression_control(expression_token_array1, index - 2);
                if (!expression1[0]) {
                    
                }
                expression_printer(expression_token_array1, index - 2,str1);

                char** expression_token_array2 = sub_array(token_array,index+1,size - 1);
                int* expression2 = expression_control(expression_token_array2, size - index-2);
                if (!expression2[0]) {
                    
                }
                expression_printer(expression_token_array2, size - index-2,str2);
                
                sprintf(str3,"%s[%s-1][%s-1]",token_array[0],str1,str2);
                strcpy(str,str3);
                return 1;
            }
        }
    }

    return 0;
}

int for_printer(char* line){
    char** token_array = get_token_array(line);

    int size = 0;

    while (strcmp(token_array[size],"\0")!=0){
        size++;
    }

    char str[1000], str1[1000], str2[1000], str3[1000],str4[1000],str5[1000],str6[1000]; 
   
    str[0] = str1[0] = str2[0] = str3[0] = str4[0] = str5[0] = str6[0] = '\0';

    if (strcmp(token_array[3],",")==0) {

        char** remaining_token_array = sub_array(token_array,6,size-2);

        int size1 = size-8;
        int open_parantez_num = 0;
        int close_parantez_num = 0;
        int open_square_num = 0;
        int close_square_num = 0;
        int comma_num = 0;
        int ind_from = 0;
        int expr_num = 1;

        for (int i = 0; i < size1; i++){
            if(strcmp(remaining_token_array[i],"(")==0){open_parantez_num++;}
            if(strcmp(remaining_token_array[i],")")==0){close_parantez_num++;}
            
            if(strcmp(remaining_token_array[i],"[")==0){open_square_num++;}
            if(strcmp(remaining_token_array[i],"]")==0){close_square_num++;}

            if ((strcmp(remaining_token_array[i],":")==0) && (close_parantez_num == open_parantez_num)) {

                char** expression_token_array = sub_array(remaining_token_array,ind_from,i);
                int* expression = expression_control(expression_token_array,i - ind_from);

                if (expr_num == 1) {
                    expression_printer(expression_token_array,i - ind_from,str1);
                } else if (expr_num == 2) {
                    expression_printer(expression_token_array,i - ind_from,str2);
                } else if (expr_num == 3) {
                    expression_printer(expression_token_array,i - ind_from,str3);
                } else if (expr_num == 4) {
                    expression_printer(expression_token_array,i - ind_from,str4);
                } else if (expr_num == 5) {
                    expression_printer(expression_token_array,i - ind_from,str5);
                } else if (expr_num == 6) {
                    expression_printer(expression_token_array,i - ind_from,str6);
                }
                
                expr_num++;
                ind_from = i + 1;
            }
            if ((strcmp(remaining_token_array[i],",")==0) && (close_parantez_num == open_parantez_num) && (close_square_num == open_square_num)) {

                comma_num++;
                char** expression_token_array = sub_array(remaining_token_array,ind_from,i);
                int* expression = expression_control(expression_token_array,i - ind_from);

                if (expr_num == 3) {
                    expression_printer(expression_token_array,i - ind_from,str3);
                } else if (expr_num == 4) {
                    expression_printer(expression_token_array,i - ind_from,str4);
                }

                expr_num++;
                ind_from = i + 1;
            }
        }
        
        char** expression_token_array = sub_array(remaining_token_array,ind_from,size1);
        int* expression = expression_control(expression_token_array,size1 - ind_from);
        expression_printer(expression_token_array,size1 - ind_from,str6);

        sprintf(str,"for (int %s=%s; %s<=(%s) ; %s = %s + %s ) {\n\tfor (int %s=%s; %s<=(%s) ; %s = %s + %s ) {\n",token_array[2],str1,token_array[2],str2,token_array[2],token_array[2],str3,token_array[4],str4,token_array[4],str5,token_array[4],token_array[4],str6);
        strcat(real,str);
        is_nestedfor = 1;
        return 1;
    } else {
        char* column = ":";
        int ind_column = find_index(column,token_array,size);
        char** expression1_token_array = sub_array(token_array,4,ind_column);
        int* expression1 = expression_control(expression1_token_array,ind_column-4);
        expression_printer(expression1_token_array,ind_column-4,str1);

        char** remaining_token_array = sub_array(token_array,ind_column+1,size-2);
        int ind_column2 = find_index(column,remaining_token_array,size - ind_column- 3);

        char** expression2_token_array = sub_array(remaining_token_array,0,ind_column2);
        int* expression2 = expression_control(expression2_token_array,ind_column2);
        expression_printer(expression2_token_array,ind_column2,str2);

        int size2 = size - ind_column- ind_column2 - 4;

        char** expression3_token_array = sub_array(remaining_token_array,ind_column2 + 1,size - ind_column- 3);
        int* expression3 = expression_control(expression3_token_array,size - ind_column- ind_column2 - 4);
        expression_printer(expression3_token_array,size - ind_column- ind_column2 - 4,str3);

        sprintf(str,"for (int %s=%s; %s<=(%s) ; %s = %s + %s ) {\n",token_array[2],str1,token_array[2],str2,token_array[2],token_array[2],str3);
        strcat(real,str);
    }
    return 1;
}
// filename returner
char *removed(char* myStr) {
    char *retStr;
    char *lastExt;
    if (myStr == NULL) return NULL;
    if ((retStr = malloc (strlen (myStr) + 1)) == NULL) return NULL;
    strcpy (retStr, myStr);
    lastExt = strrchr (retStr, '.');
    if (lastExt != NULL)
        *lastExt = '\0';
    return retStr;
}
