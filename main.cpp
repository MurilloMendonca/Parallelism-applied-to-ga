//g++ -pthread GA.cpp  GAMS.cpp Individuo.cpp GAIM.cpp GAFN.cpp main.cpp -O2 && ./a.out
#include <iostream>
#include <thread>
#include <vector>
#include "GA.hpp"
#include "GAMS.hpp"
#include "GAIM.hpp"
#include "GAFN.hpp"
#include <fstream>
#include <chrono>
#include <iomanip>
using namespace std;
using namespace chrono;

const uint geracoes= 100;
const uint popSize = 50;
const uint ntestes = 50;
const uint quantidadeDeBits = 18;
const uint maxSlaves=24;
const double cruzamento=0.8;
const double mutacao=0.01;

void testaCodificacao(GA* g, double inf, double sup, int prec){
    long range = (sup-inf)*pow(10,prec);
    for(double i=inf;i<=sup;i+=(1/pow(10,prec))){
        cout<<"\nValor: "<<i<<
        "; Codificado: ";
        for(int j=0;j<g->getGenotipo(i).size();j++)
            cout<<g->getGenotipo(i)[j];
        cout<<"; Decodificado: "<<g->getFenotipo(g->getGenotipo(i));
    }
}
void testaGA(){
    vector<double> mediaPior(geracoes);
    vector<double> mediaMedia(geracoes);
    vector<double> mediaMelhor(geracoes);
    vector<double> melhores(ntestes);
    for(int i=0;i<ntestes;i++){
        GAMS ga (18,popSize,-10,10,4,2, mutacao,cruzamento, 10,1);
        ga.run(geracoes);
        for(int j =0;j<geracoes;j++){
            mediaPior.at(j) += ga.getpiorPorGeracao().at(j).getFitness()/ntestes;
            mediaMedia.at(j) += ga.getMediaPorGeracao().at(j)/ntestes;
            mediaMelhor.at(j)+= ga.getmelhorPorGeracao().at(j).getFitness()/ntestes;
        }
        melhores.at(i) = ga.getmelhorPorGeracao().at(geracoes-1).getFitness();
        //ga.mostraPopulacao();
    }
    //GA ga (18,popSize,-10,10,4,2, mutacao,cruzamento);
    //ga.run(geracoes);
    cout<<endl<<"ger.\tpior\tMedio\tmelhor";
    for(int i=0;i<geracoes;i++){
        cout<<endl<<setprecision(5)<<i<<","<<mediaPior.at(i)
            <<","<<mediaMedia.at(i)
            <<","<<mediaMelhor.at(i);
    }
    //ga.mostraPopulacao();
    ofstream arq;
    arq.open("teste.txt");
    arq<<"ger,pior,Medio,melhor";
    for(int i=0;i<geracoes;i++){
        arq<<endl<<setprecision(5)<<i<<","<<mediaPior.at(i)
            <<","<<mediaMedia.at(i)
            <<","<<mediaMelhor.at(i);
    }
    arq.close();
    double media=0;
    for(int i=0;i<ntestes;i++) media+= melhores.at(i)/ntestes;
    double dp=0;
    for(double x:melhores){
        dp+=pow((x - media),2)/ntestes;
    }
    cout<<"\nDesvio Padrao: "<<dp;
}
void testaGAMS(){
    GAMS ga (18,popSize,-10,10,4,2, mutacao,cruzamento, 12);
    ga.run(geracoes);
    cout<<endl<<"ger.\tpior\tMedio\tmelhor\tdesv.Padr";
    for(int i=0;i<geracoes;i++){
        cout<<endl<<setprecision(5)<<i<<"\t"<<ga.getpiorPorGeracao().at(i).getFitness()
            <<"\t"<<ga.getMediaPorGeracao().at(i)
            <<"\t"<<ga.getmelhorPorGeracao().at(i).getFitness()
            <<"\t"<<ga.getDesvioPadraoPorGeracao().at(i);
    }
    ga.mostraPopulacao();
}
void comparaGAMS(){
    ofstream arq;
    arq.open("tempos.csv");
    arq<<"slaves,control,data\n";
    
    double media=0;
    for(int i=0;i<ntestes;i++){
        GA ga (18,popSize,-10,10,4,2, mutacao,cruzamento);
        auto inicio = chrono::high_resolution_clock::now();
        ga.run(geracoes);
        auto final = chrono::high_resolution_clock::now();
        auto diff = duration_cast<milliseconds>(final-inicio);
        media +=diff.count()/((double)ntestes);
    }
    cout<<"Tempo de execucao para sequencial: "<<media<<" milissegundos\n";
    for(int n=1;n<21;n++){
        media=0;
        for(int i=0;i<ntestes;i++){
            GAMS ga (18,popSize,-10,10,4,2, mutacao,cruzamento,n);
            auto inicio = chrono::high_resolution_clock::now();
            ga.run(geracoes);
            auto final = chrono::high_resolution_clock::now();
            auto diff = duration_cast<milliseconds>(final-inicio);
            media +=diff.count()/((double)ntestes);
        }
        cout<<"Tempo de execucao para "<<n<<" escravos em controle : "<<media<<" milissegundos\n";
        arq<<n<<","<<media;
        media=0;
        for(int i=0;i<ntestes;i++){
            GAMS ga (18,popSize,-10,10,4,2, mutacao,cruzamento,n,2);
            auto inicio = chrono::high_resolution_clock::now();
            ga.run(geracoes);
            auto final = chrono::high_resolution_clock::now();
            auto diff = duration_cast<milliseconds>(final-inicio);
            media +=diff.count()/((double)ntestes);
        }
        arq<<","<<media<<endl;
        cout<<"Tempo de execucao para "<<n<<" escravos em dados: "<<media<<" milissegundos\n";
    }
    arq.close();
}
void testaGAIM(){
    int nIlhas=6;
    int pop = popSize/nIlhas;
    GAIM ga (nIlhas,18,pop,-10,10,4,2, mutacao,cruzamento,10);

    ga.run(geracoes);
    for(int i=0;i<nIlhas;i++){
        cout<<"\n\nPopulacao da ilha "<<i;
        ga[i]->mostraPopulacao();
    }

    ofstream arq;
    arq.open("ilhas.txt");
    arq<<"ger,pior,Medio,melhor,dev.padr,ilha";
    for(int i=0;i<geracoes;i++){
        for(int ilha=0;ilha<nIlhas;ilha++)
            arq<<endl<<i<<","<<ga[ilha]->getpiorPorGeracao().at(i).getFitness()
                <<","<<ga[ilha]->getMediaPorGeracao().at(i)
                <<","<<ga[ilha]->getmelhorPorGeracao().at(i).getFitness()
                <<","<<ga[ilha]->getDesvioPadraoPorGeracao().at(i)
                <<","<<"ilha_"<<ilha;
    }
    arq.close();
}
void testaGAFN(){
    GAFN ga (18,20,-10,10,2,mutacao, cruzamento, 2);
    ga.run(geracoes);
    ga.mostraPopulacao();
    ofstream arq;
    arq.open("granulacaofina.txt");
    arq<<"ger,no,val,fit";
    int n=0;
    for(auto x:ga.malha){
        for(int i=0;i<geracoes;i++){
            arq<<endl<<i<<","<<n<<","<<x.getHistoricoVal().at(i)
            <<","<<x.getHistoricoFit().at(i);
        }
        n++;
    }
    arq.close();
}
void testaSpeedUp(){
    ofstream arq;
    uint maxPop=100000;
    uint minPop=10;
    uint passoPop=10;
    arq.open("tempos.csv");
    arq<<"pop,slaves,tipo,tempo,speedup\n";
    double seq[2]={0,0};
    double media=0;
    cout<<"Tempo de execucao para sequencial: "<<media<<" milissegundos\n";
    for(int pop=minPop;pop<maxPop;pop*=passoPop){
        for(int n=1;n<maxSlaves;n++){
            media=0;
            for(int i=0;i<ntestes;i++){
                GAMS ga (18,pop,-10,10,4,2, mutacao,cruzamento,n);
                auto inicio = chrono::high_resolution_clock::now();
                ga.run(geracoes);
                auto final = chrono::high_resolution_clock::now();
                auto diff = duration_cast<milliseconds>(final-inicio);
                media +=diff.count()/((double)ntestes);
            }
            if(n==1) seq[0]=media;
            cout<<"Tempo de execucao para "<<n<<" escravos em controle : "<<media<<" milissegundos\n";

            arq<<pop<<","<<n<<",control,"<<media<<","<<(seq[0]/media)<<endl;
            media=0;
            for(int i=0;i<ntestes;i++){
                GAMS ga (18,pop,-10,10,4,2, mutacao,cruzamento,n,2);
                auto inicio = chrono::high_resolution_clock::now();
                ga.run(geracoes);
                auto final = chrono::high_resolution_clock::now();
                auto diff = duration_cast<milliseconds>(final-inicio);
                media +=diff.count()/((double)ntestes);
            }
            if(n==1) seq[1]=media;
            arq<<pop<<","<<n<<",data,"<<media<<","<<(seq[1]/media)<<endl;
            cout<<"Tempo de execucao para "<<n<<" escravos e "<<pop<<" ind em dados: "<<media<<" milissegundos\n";
        }
    }
    arq.close();
}
int main(void){
    //testaGA();
    //testaGAMS();
    //testaGAIM();
    //testaGAFN();
    testaSpeedUp();
   
    return 0;
}
