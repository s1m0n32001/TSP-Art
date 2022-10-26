#include<iostream>
#include<fstream>
#include<vector>
#include<math.h>
#include<algorithm>

using namespace std;

int N=8962;

int main()
{
    ifstream input("viso.txt");
    ofstream output("guesserOutputNuovo.txt");

    vector<double>x;
    vector<double>y;

    double dato;
    while(input >> dato)
    {
        x.push_back(dato);
        input >> dato;
        y.push_back(dato);
    }

    vector<double>xGuess;
    vector<double>yGuess;
    
    xGuess.push_back(x[0]);
    yGuess.push_back(y[0]);

    x.erase(x.begin());
    y.erase(y.begin()); //Rimuoviamo il primo elemento poiché lo abbiamo già messo nel nuovo vettore

    while(x.size()!=0)
    {
        int nMin=0;
        double distMin=sqrt(pow(xGuess[xGuess.size()-1]-x[0],2.)+pow(yGuess[yGuess.size()-1]-y[0],2.));
        for(int i=1; i<x.size(); i++)
        {
            if(sqrt(pow(xGuess[xGuess.size()-1]-x[i],2.)+pow(yGuess[yGuess.size()-1]-y[i],2.))<distMin)
            {
                nMin=i;
                distMin=sqrt(pow(xGuess[xGuess.size()-1]-x[i],2.)+pow(yGuess[yGuess.size()-1]-y[i],2.));
            }

        }

        xGuess.push_back(x[nMin]);
        yGuess.push_back(y[nMin]);

        x.erase(x.begin()+nMin);
        y.erase(y.begin()+nMin);

    }    

    for(int i=0; i<xGuess.size(); i++)
    {
        output << xGuess[i] << "\t" << yGuess[i] << "\n";
    }        

    
    
    
    return 0;
}