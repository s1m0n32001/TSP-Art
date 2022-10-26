//Greedy guesser
#include<iostream>
#include<fstream>
#include<math.h>
#include<algorithm>
#include<vector>

using namespace std;

double dist(double*x, double* y,int i, int j)
{
    double d=sqrt(pow(x[i]-x[j],2.)+pow(y[i]-y[j],2.));
    return d;
}

void stampaPunto(vector<vector<int>>coppie,int i,int j, double*x,double*y)
{
    cout << "(" << x[coppie[i][j]] << "," << y[coppie[i][j]] << ") ";
}

int trovaIndiceMinD(vector<int>indici, double* x,double* y) //[2,5,6,9,...] --> 3
{
    int index=1;
    double dMin=sqrt(pow(x[indici[0]]-x[indici[1]],2.)+pow(y[indici[0]]-y[indici[1]],2.));
    if(indici.size()>2)
    {
    for(int i=1; i<indici.size(); i++)
    {
        double d1=sqrt(pow(x[indici[0]]-x[indici[i]],2.)+pow(y[indici[0]]-y[indici[i]],2.));
        if(d1<dMin)
        {
            dMin=d1;
            index=i;
        }
    }
    }

    return index;
}

double minimum(double a,double b,double c,double d)
{
    
    double min=a;
    if(b<min)
    {
        min=b;
    }
    if(c<min)
    {
        min=c;
    }
    if(d<min)
    {
        min=d;
    }
    return min;
}

vector<double>dMin(double*x, double*y, int i, int j, vector<vector<int>>coppie) //minima distanza tra due stringhe
{
double dMin1=dist(x,y,coppie[i][0],coppie[j][0]);
double dMin2=dist(x,y,coppie[i][0],coppie[j][coppie[j].size()-1]);
double dMin3=dist(x,y,coppie[i][coppie[i].size()-1],coppie[j][0]);
double dMin4=dist(x,y,coppie[i][coppie[i].size()-1],coppie[j][coppie[j].size()-1]);

vector<double>output; //dMin,indice1,indice2

double d=minimum(dMin1,dMin2,dMin3,dMin4);
output.push_back(d);
if(d==dMin1)
{
    output.push_back(0);
    output.push_back(0);
}
else if(d==dMin2)
{
    output.push_back(0);
    output.push_back(1);   
}
else if(d==dMin3)
{
    output.push_back(1);
    output.push_back(0);
}
else
{
    output.push_back(1);
    output.push_back(1);
}

return output;
}

vector<int>accoppia(double* x,double* y, int indicePartenza, vector<vector<int>>coppie)
{
    vector<int>output;

    double d=dMin(x,y,indicePartenza,indicePartenza+1,coppie)[0];
    int indice=indicePartenza+1;
    int coda1=(int)dMin(x,y,indicePartenza,indicePartenza+1,coppie)[1];
    int coda2=(int)dMin(x,y,indicePartenza,indicePartenza+1,coppie)[2];

    if(indicePartenza+2<coppie.size())
    {
        for(int i=indicePartenza+2; i<coppie.size(); i++)
        {
            if(dMin(x,y,indicePartenza,i,coppie)[0]<d)
            {
                d=dMin(x,y,indicePartenza,i,coppie)[0];
                indice=i;
                coda1=(int)dMin(x,y,indicePartenza,i,coppie)[1];
                coda2=(int)dMin(x,y,indicePartenza,i,coppie)[2];
            }
        }
    }
    output.push_back(indice);
    output.push_back(coda1);
    output.push_back(coda2);

    return output;
}

vector<vector<int>> concatena(vector<vector<int>> coppie2, double* x, double* y) //qua devo correggere qualcosa
{
    vector<vector<int>>coppie=coppie2;
    for(int i=0; i<coppie.size()-1; i++)
    {
        if(i%100==0)
        {
            cout << i << "\n";
        }
        vector<int>a=accoppia(x,y,i,coppie);

        if(a[1]==1 && a[2]==0)
        {
            for(int j=0; j<coppie[a[0]].size();j++)
            {

                coppie[i].push_back(coppie[a[0]][j]);
            }
            coppie.erase(coppie.begin()+a[0]);
        }
        else if(a[1]==0 && a[2]==1)
        {
            for(int j=0; j<coppie[i].size(); j++)
            {

                coppie[a[0]].push_back(coppie[i][j]);
            }
            coppie[i]=coppie[a[0]];
            coppie.erase(coppie.begin()+a[0]);
        }
        else if(a[1]==0 && a[2]==0)
        {
            vector<int>b;
            for(int j=1; j<=coppie[i].size(); j++)
            {

                b.push_back(coppie[i][coppie[i].size()-j]);
            }
            for(int j=0; j<coppie[a[0]].size(); j++)
            {
                b.push_back(coppie[a[0]][j]);
            }
            coppie[i]=b;
            coppie.erase(coppie.begin()+a[0]);
        }
        else if(a[1]==1 && a[2]==1)
        {
            for(int j=1; j<=coppie[a[0]].size(); j++)
            {

                coppie[i].push_back(coppie[a[0]][coppie[a[0]].size()-j]);
            }
            coppie.erase(coppie.begin()+a[0]);
        }
        
    }

return coppie;
}




int main()
{
    ifstream input("viso.txt");
    ofstream output("guesserOutput.txt");

    int N=8962;
    double* x=(double*) malloc(N*sizeof(double));
    double* y=(double*) malloc(N*sizeof(double));

    int c=0;
    double dato;
    while(input >> dato)
    {
        x[c]=dato;
        input >> dato;
        y[c]=dato;
        c++;
    }
    
    vector<int>indici;
    for(int i=0; i<N; i++)
    {
        indici.push_back(i);
    }

    vector<vector<int>>coppie; //tutte le coppie di punti vicini

    int conta=0;
    while(indici.size()>0)
    {
        vector<int>duo;
        duo.push_back(indici[0]);
        int indice=trovaIndiceMinD(indici,x,y);
        duo.push_back(indici[indice]);
        coppie.push_back(duo);
        indici.erase(indici.begin()+indice);
        indici.erase(indici.begin());
        conta++;
    } //fin qui ok


    vector<vector<int>>nuovo=concatena(coppie,x,y);

    cout << "ora ci sono " << nuovo.size() << " gruppi " << endl;
for(int i=0; i<nuovo.size(); i++)
       {
           cout << "\n";
           for(int j=0; j<nuovo[i].size(); j++)
           {
            stampaPunto(nuovo,i,j,x,y);
            cout << "\t";
           }
       }


    while(nuovo.size()>1)
    {
       nuovo=concatena(nuovo,x,y);
       cout << "ora ci sono " << nuovo.size() << " gruppi " << endl;
       for(int i=0; i<nuovo.size(); i++)
       {
           cout << "\n";
           for(int j=0; j<nuovo[i].size(); j++)
           {
            stampaPunto(nuovo,i,j,x,y);
            cout << "\t";
           }
       }
    }

    cout << "\n\n";
       for(int i=0; i<nuovo.size(); i++)
       {
           cout << "\n";
           for(int j=0; j<nuovo[i].size(); j++)
           {
            stampaPunto(nuovo,i,j,x,y);
            cout << "\t";
           }
       }

    for(int i=0; i<nuovo.size(); i++)
    {
        for(int j=0; j<nuovo[i].size(); j++)
        {
            output << x[nuovo[i][j]] << "\t" << y[nuovo[i][j]] << "\n";
        }
    output << "\n";
    }
    
    return 0;
}

