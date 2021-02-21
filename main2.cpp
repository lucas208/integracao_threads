#include <iostream>
#include <pthread.h>
#include <unistd.h>

using namespace std;

double a = 0;
double b = 10;
double n = 120;
int t = 6;
double soma[6];

double f(double x){
    return 5;
}

void* computar(void *tid){

    int rank = (int)(size_t)tid;
    double local_n = n/t;
    double largura_trapezio = (b-a)/n;
    cout<<"Esta é a Thread "<< rank << endl;
    double local_a = a + rank*largura_trapezio*local_n;
    double local_b = local_a + largura_trapezio*local_n;
    double x_i;
    double h = (local_b - local_a)/local_n;
    double area_total = (f(local_a)+f(local_b))/2;
    
    for(int i = 1;i<local_n;i++){
        x_i = local_a +i*h;
        area_total += f(x_i);
    }

    area_total = h*area_total;
    soma[rank] = area_total;

    pthread_exit(NULL);
}

int main(int argc, char *argv[]){

    pthread_t threads[t];
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