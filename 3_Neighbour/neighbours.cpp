//Terza parte: k-neighbours scaling
#include<iostream>
#include<fstream>
#include<vector>
#include<algorithm>
#include<math.h>

using namespace std;

double distanza(double* x,double *y, int i, int j, int* perm)
{
    double d=sqrt(pow(x[perm[i]]-x[perm[j]],2.)+pow(y[perm[i]]-y[perm[j]],2.));
    return d;
}

bool comp(vector<double>a, vector<double>b )
{
    return a[0]<b[0];
}

int main()
{
    ifstream input("scaleOutput.txt");
    int N=;
    double dato;

    ofstream output("vicini.txt");
    double* x = (double* ) malloc((N+1)*sizeof(double));
    double* y = (double* ) malloc((N+1)*sizeof(double));
    int a=0;

    while(input >> dato)
    {
        x[a]=dato;
        input >> dato;
        y[a]=dato;
        a++;
    }

    x[N]=x[0];
    y[N]=y[0]; 

    int* perm = (int* ) malloc((N+1)*sizeof(int));

    for(int i=0; i<N+1; i++)
    {
        perm[i]=i;
    } 

    vector<vector<int>>vicini; //MATRICE CONTENENTE I VICINI
    vector<vector<vector<double>>>viciniProvvisorio; //Matriche le cui entrate sono coppie (distanza, indice)
    int kNeighMax=100; //NUMERO MASSIMO DI VICINI CHE CI INTERESSANO

    for(int i=0; i<N+1; i++)
    {
        cout << "riga numero " << i << "\n";
        vector<vector<double>>riga;
        for(int j=0; j<N+1; j++)
        {
            if(i==j)
            {
                continue;
            }
            else
            {
                vector<double>coppia;
                coppia.push_back(distanza(x,y,i,j,perm));
                coppia.push_back((double)j);
                riga.push_back(coppia);
            }

        }
        viciniProvvisorio.push_back(riga);
    }

    for(int i=0; i<N+1; i++)
    {

        cout << " sort numero " << i << "\n";
        sort(viciniProvvisorio[i].begin(),viciniProvvisorio[i].end(),comp);
    }

    for(int i=0; i<N+1; i++)
    {
        vector<int>indici;
        for(int j=0; j<viciniProvvisorio[i].size(); j++)
        {
            indici.push_back((int)viciniProvvisorio[i][j][1]);
        }
        vicini.push_back(indici);
    }

    for(int i=0; i<N+1; i++)
    {
        output << "\n";
        for(int j=0; j<kNeighMax; j++)
        {
            output << vicini[i][j] << "\t";
        }
 
        
    }


    return 0;
}