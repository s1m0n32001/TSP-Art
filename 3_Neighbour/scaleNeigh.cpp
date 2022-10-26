#include<iostream>
#include<fstream>
#include<math.h>
#include<vector>
#include<algorithm>
#include<random>
#include<ctime>

using namespace std;

int N=8962;

int main()
{
    ifstream input("scaleOutput.txt");
    ifstream inputVicini("vicini.txt");
    ofstream output("neighOutput.txt");
    ofstream outputE("neighEnergia.txt");

    srand(time(NULL));
    random_device rd;  
    mt19937 gen(rd()); 

    uniform_real_distribution<> disReal(0., 1.); //motore per scegliere il punto tra 0 e 1 nella probabilità

    double dato; int c=0;

    double* x = (double* ) malloc((N+1)*sizeof(double));
    double* y = (double* ) malloc((N+1)*sizeof(double));
    int* perm = (int* ) malloc((N+1)*sizeof(int));

    while(input >> dato)
    {
        x[c]=dato;
        input >> dato;
        y[c]=dato;
        perm[c]=c;
        c++;
    }

    vector<vector<int>>vicini; //matrice con gli indici in x,y dei vicini di ogni punto

    for(int i=0; i<N+1; i++)
    {
        vector<int>riga;
        for(int j=0; j<100; j++)
        {
            inputVicini >> dato;
            riga.push_back(dato);
        }

        vicini.push_back(riga);
    }

    /*Ora dobbiamo fare un annealing solo sui vicini, prendendo man mano un numero sempre minore di primi vicini.
    Per farlo generiamo un indice a caso da 1 a N-1, poi uno a caso da 0 a k_current-1, rintracciamo nella permutazione
    l'indice in cui compaiono quelle due città e facciamo il solito discorso.
    */

    long nSteps=5e5;

    double T_start=0.1;
    double T_decimal=2;
    double T_scale=pow(10,-T_decimal/nSteps);
    double T_current=T_start;

    double kStart = 100.;
    double kEnd = 3.;
    double kScaling = pow((kEnd/kStart),1./nSteps);
    double kCurrent=kStart;

    double energiaIniziale=0.;

    for(int i=0; i<N; i++)
    {
        energiaIniziale+=sqrt(pow(x[perm[i]]-x[perm[i+1]],2.)+pow(y[perm[i]]-y[perm[i+1]],2.));
    }

    cout << "Energia iniziale " << energiaIniziale << "\n";
    double energiaIniziale2=energiaIniziale;

    for(int m=0; m<nSteps; m++)
    {
        int i,j; 
        i=rand()%N; //Sceglie un indice i a caso da 1 a N in x,y
        while(i==0)
        {
            i=rand()%N;
        }

        j=rand()%((int)kCurrent); //a caso j da 0 a k current
        int jCity=vicini[i][j]; //indice di j in x,y
        //Ora devo trovare nella permutazione le città interessate
        int permI,permJ;
        bool trovatoI=false; bool trovatoJ=false;

        for(int k=0; k<N+1; k++)
        {
            if(perm[k]==i)
            {
                permI=k;
                trovatoI=true;
            }
            if(perm[k]==jCity)
            {
                permJ=k;
                trovatoJ=true;
            }
        }
    
        //Ora indiceI e indiceJ sono gli indici delle due città da scambiare nella permutazione
        if(permI>permJ)
        {
            int pippo=permI;
            permI=permJ;
            permJ=pippo;
        }

        //Ora ho indiceI<indiceJ
        double e1=energiaIniziale;

        e1-=sqrt(pow(x[perm[permI]]-x[perm[permI-1]],2.)+pow(y[perm[permI]]-y[perm[permI-1]],2.));
        e1-=sqrt(pow(x[perm[permJ]]-x[perm[permJ+1]],2.)+pow(y[perm[permJ]]-y[perm[permJ+1]],2.));

        e1+=sqrt(pow(x[perm[permJ]]-x[perm[permI-1]],2.)+pow(y[perm[permJ]]-y[perm[permI-1]],2.));
        e1+=sqrt(pow(x[perm[permI]]-x[perm[permJ+1]],2.)+pow(y[perm[permI]]-y[perm[permJ+1]],2.));

        if(e1<energiaIniziale)
        {
            energiaIniziale=e1;
            reverse(perm+permI,perm+permJ+1);
        }
        else
        {
            double p,s;
            s=disReal(gen);
            p=(-(e1-energiaIniziale)/T_current);
            if(p>s)
            {
                energiaIniziale=e1;
                reverse(perm+permI,perm+permJ+1);              
            }
        }
        

        kCurrent*=kScaling;
        T_current*=T_scale;
        if(m%100==0)
        {
            outputE << m << "\t" << energiaIniziale << "\n";
        }
    
    }

    cout << "energia finale " << energiaIniziale << "\n";

    for(int i=0; i<N+1; i++)
    {
        output << x[perm[i]] << "\t" << y[perm[i]] << "\n";
    }


    return 0;
}