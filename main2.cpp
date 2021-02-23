#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <math.h>

using namespace std;

double pi = 3.14159265358979323846;  

double a = 0;
double b = 2*pi;
int n = 200;
int t = 7;
double soma[7];

double f(double x){
    return 5;
}

double f2(double x){
    return sin(2*x)+cos(5*x);
}

void* computar(void *tid){

    int rank = (int)(size_t)tid;
    int local_n = n/t;
    double resto = 0;

    if(rank == t-1){
        resto = n%t;
    }
    
    double largura_trapezio = (b-a)/n;
    cout<<"Esta é a Thread "<< rank << endl;
    double local_a = a + rank*largura_trapezio*local_n;
    double local_b = local_a + largura_trapezio*local_n + largura_trapezio*resto;
    double x_i;
    double h = (local_b - local_a)/(local_n + resto);
    double area_total = (f2(local_a)+f2(local_b))/2;
    
    for(int i = 1;i<(local_n + resto);i++){
        x_i = local_a +i*h;
        area_total += f2(x_i);
    }
  
    area_total = h*area_total;
    soma[rank] = area_total;

    pthread_exit(NULL);
}

int main(int argc, char *argv[]){

    pthread_t threads[7];
    int i;
    void *thread_return;
    double somatorio = 0;

    for(i=0; i<t; i++){
        cout<<"Processo principal criando thread "<< i << endl;
        pthread_create(&threads[i], NULL, computar, (void*)(size_t) i);
    }


    for(i=0; i<t;i++){
        cout<<"Esperando thread "<< i << " finalizar "<< endl;
        pthread_join(threads[i], &thread_return);
        cout<<"Thread "<< i << " finalizada"<< endl;
    }
   
   
    for(int j = 0;j<t;j++){
        cout<<soma[j]<<endl;
        somatorio += soma[j];
    }

    cout <<"Resultado = " << somatorio << endl;
    cout<<"O processo vai finalizar"<< endl;

    return 0;
}