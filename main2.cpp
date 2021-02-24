#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <math.h>
#include <iomanip>

#define MAX 100
#define PI 3.14159265358979323846

using namespace std;

double a = 0;
double b;
int n;
int t;
double soma[MAX];

double f(double x){
    return 5;
}

double f2(double x){
    return sin(2*x)+cos(5*x);
}

void* computarf(void *tid){

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
    double area_total = (f(local_a)+f(local_b))/2;
    
    for(int i = 1;i<(local_n + resto);i++){
        x_i = local_a +i*h;
        area_total += f(x_i);
    }
  
    area_total = h*area_total;
    soma[rank] = area_total;

    pthread_exit(NULL);
}

void* computarf2(void *tid){

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

    pthread_t threads[MAX];
    int i;
    void *thread_return;
    double somatorio = 0;
    int optfuncao;

    cout<<"Insira o número de trapézios (n > 0)"<< endl;
    cin>> n;
    while(n<=0){
        cout<<"número de trapézios inválido, tente novamente (n > 0)"<<endl;
        cin>> n;
    }

    cout<<"Insira o número de threads (t > 1)"<< endl;
    cin>> t;
    while(t<=1){
        cout<<"número de threads inválido, tente novamente (t > 1)"<<endl;
        cin>> t;
    }

    cout<<"Escolha a função a ser utilizada (1 ou 2)" <<endl;
    cin >> optfuncao;
    while(optfuncao < 1 || optfuncao > 2){
        cout<<"Opção fora do intervalo disponível, tente novamente (1 ou 2)"<<endl;
        cin>> optfuncao;
    }

    if(optfuncao == 1){
        b = 10;
        cout<< fixed << setprecision(1);
    }else {
        b = 2*PI;
        cout << scientific << setprecision(2);
    }

    for(i=0; i<t; i++){
        cout<<"Processo principal criando thread "<< i << endl;
        if(optfuncao == 1){
            pthread_create(&threads[i], NULL, computarf, (void*)(size_t) i);
        }else{
            pthread_create(&threads[i], NULL, computarf2, (void*)(size_t) i);
        }
    }

    for(i=0; i<t;i++){
        cout<<"Esperando thread "<< i << " finalizar "<< endl;
        pthread_join(threads[i], &thread_return);
        cout<<"Thread "<< i << " finalizada"<< endl;
    }
    
    cout << endl;

    for(int j = 0;j<t;j++){
        cout<<"Parcela "<< j+1 <<": "<<soma[j]<<endl;
        somatorio += soma[j];
    }

    cout << endl;

    cout <<"Resultado = " << somatorio << endl;
    
    cout << endl;
    
    cout<<"O processo vai finalizar"<< endl;

    return 0;
}