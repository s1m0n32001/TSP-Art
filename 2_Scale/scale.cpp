#include<iostream>
#include<fstream>
#include<math.h>
#include<random>
#include<algorithm>
#include<vector>

using namespace std;

int N = 8962;

bool magg(double a, double b) //per ordinare le lunghezze in ordine decrescente
{
    return(a>b);
}

int main()
{
    ifstream input("guesserOutput.txt");
    ofstream output("scaleOutput.txt");
    ofstream outputE("scaleOutputEnergie.txt");

    srand(time(NULL));
    random_device rd;  
    mt19937 gen(rd()); 

    uniform_real_distribution<> disReal(0., 1.); //motore per scegliere il punto tra 0 e 1 nella probabilità
    
    double* x = (double* ) malloc ((N+1)*sizeof(double));
    double* y = (double* ) malloc ((N+1)*sizeof(double));
    int* perm = (int* ) malloc((N+1)*sizeof(int));

    double dato;
    int c=0;
    while(input>>dato)
    {
        x[c]=dato;
        input >> dato;
        y[c]=dato;
        perm[c]=c;
        c++;

    }

    perm[N]=perm[0];

    //Riscaliamo i punti in modo che le y stiano tra 0 e 1 e le x varino di conseguenza
    for(int i=0; i<N; i++)
    {
        y[i]=(y[i]-8000.)/(8000.);
        x[i]=(x[i]-6000.)/(8000.);
    }

    double energiaIniziale=0.;
    for(int i=0; i<N; i++)
    {
        energiaIniziale+=sqrt(pow(x[perm[i]]-x[perm[i+1]],2.)+pow(y[perm[i]]-y[perm[i+1]],2.));
    }
    double energiaIniziale2=energiaIniziale; //Ci servirà alla fine per confrontare

    cout << "Energia iniziale " << energiaIniziale << "\n";

    long nSteps=5e7;
    int nJobs = 200;

    double T_start=0.1;
    double T_decimal=2;
    double T_scale=pow(10,-T_decimal/nJobs);
    double T_current=T_start;

    vector<double>lunghezze;
    for(int i=0; i<N; i++)
    {
        lunghezze.push_back(sqrt(pow(x[perm[i]]-x[perm[i+1]],2.)+pow(y[perm[i]]-y[perm[i+1]],2.)));
    }

    sort(lunghezze.begin(),lunghezze.end(),magg);
    cout << lunghezze.size() << " lunghezze.size() " << endl;

    double scaleBegin = lunghezze[(int)(N/1000)];
    double scaleEnd = lunghezze[(int)(N/10)];
    double scaling = exp(log(scaleEnd/scaleBegin)/nJobs);
    double scaleCurrent = scaleBegin; 

    cout << scaleBegin << " è scale Begin\n";
    cout << scaleEnd << " è scale End\n";

    for(int m=0; m<nJobs; m++) //ogni nSteps/nJobs fa un refresh del pool
    {
        /*Nel pool sono incluse le città in x e y che all'inizio dell'iterazione
        sono vertici di "bordi lunghi". Il ciclo for deve scegliere due città nel pool, trovare
        la loro posizione nella permutazione e invertire la permutazione tra loro due. Non possiamo direttamente definire
        il pool come il vettore delle posizioni nella permutazione poiché quando invertiamo una stringa
        si incasinerebbe tutto (termine tecnico).
        */
        vector<int>pool;
        for(int i=1; i<N; i++)
        {
            if(sqrt(pow(x[perm[i]]-x[perm[i-1]],2.)+pow(y[perm[i]]-y[perm[i-1]],2.))>scaleCurrent || sqrt(pow(x[perm[i]]-x[perm[i+1]],2.)+pow(y[perm[i]]-y[perm[i+1]],2.))>scaleCurrent)
            {
                pool.push_back(perm[i]); //posizione in x,y della città interessata
            }
        }

        for(int l=0; l<nSteps/nJobs; l++) //compie un ciclo di annealing per nSteps/nJobs volte prima di ricominciare
        {
            int i,j; //Indici a caso nel pool
            i = rand()%pool.size();
            j = rand()%pool.size();

            while(i==j)
            {
                j=rand()%pool.size();
            }
            
            int indiceI,indiceJ;
            indiceI=pool[i];
            indiceJ=pool[j]; //ha trovato le città in x,y corrispondenti a quelle due nel pool


            int permI, permJ; //indici nella permutazione

            bool trovatoI=false; bool trovatoJ=false; //adesso le cerca nella permutazione
            for(int k=0; k<N+1; k++)
            {
                if(perm[k]==indiceI)
                {
                    permI=k;
                    trovatoI=true;
                }
                if(perm[k]==indiceJ)
                {
                    permJ=k;
                    trovatoJ=true;
                }
                if(trovatoI && trovatoJ)
                {
                    break;
                }
            }

            //Ora permI e permJ sono gli indici NELLA PERMUTAZIONE delle città da scambiare.
            if(permI>permJ)
            {
                int pippo=permI;
                permI=permJ;
                permJ=pippo;
            }

            //Ora sono sicuro che permI<permJ

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

            T_current*=T_scale;
        }

        scaleCurrent*=scaling;
        outputE << m << "\t" << energiaIniziale << "\n";
    }



    cout << "e INIZIALE " << energiaIniziale2 << "\t e finale " << energiaIniziale << "\n";

    for(int i=0; i<N+1; i++)
    {
        output << x[perm[i]] << "\t" << y[perm[i]] << "\n";
    }



    return 0;
}