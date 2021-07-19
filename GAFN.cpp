#include "GAFN.hpp"
#include <algorithm>
#include <iostream>
#include <mutex>
#include <random>
#include <thread>
#include <vector>

int GAFN::No::limites[2];       //x
uint GAFN::No::precisao;        //x
double GAFN::No::mutacao;       //x
double GAFN::No::cruzamento;    //x
uint GAFN::No::numBits;         //x
uint GAFN::No::tamanhoTorneio;  //x

void GAFN::No::setTamanhoTorneio(uint n){
    tamanhoTorneio=n;
}
void GAFN::No::setNumBits(uint n){
    numBits=n;
}
void GAFN::No::setPrecisao(uint prec){
    precisao=prec;
}
void GAFN::No::setMutacao(double mut){
    mutacao=mut;
}
void GAFN::No::setCruzamento(double mut){
    cruzamento=mut;
}
void GAFN::No::setLimites(double inferior, double superior){
    limites[0]=inferior;
    limites[1]=superior;
}

double GAFN::No::converte(std::vector<bool> v){
    
    return converte_gray_dec(v);
}
std::vector<bool> GAFN::No::converte(double d){
    return converte_dec_gray(d);
}
void GAFN::No::setVizinhos(std::vector<GAFN::No*> v){
    vizinhos= v;
}
GAFN::No::No():Individuo(){
    mut = new std::mutex();
    rd = new std::random_device();
    gen=std::mt19937((*rd)());
    std::uniform_real_distribution<double> distribution(limites[0],limites[1]);
    double valor=distribution(gen);
    setCromossomo(converte(valor));
}
GAFN::No* GAFN::No::selecao(){
    return torneio();
}

GAFN::No* GAFN::No::torneio(){
    std::set<No*> aux;
    aux.insert(vizinhos[0]);
    aux.insert(vizinhos[1]);
    /*
    
    std::uniform_int_distribution<int> distribution(0,vizinhos.size()-1);
    while(aux.size()<tamanhoTorneio){
        int pos = distribution(gen);
        aux.insert(vizinhos.at(pos));
    }*/
    return melhor(aux);
}
GAFN::No* GAFN::No::melhor(std::set<No*>vec){
    No* melhor = *vec.begin();
    for(No* n:vec)
        if(n->getFitness()>melhor->getFitness())
            melhor=n;

    return melhor;
}
void GAFN::No::setFitness(double x){
    
    mut->lock();
    
    fitness=x;
    mut->unlock();
    
}
void GAFN::No::setCromossomo(std::vector<bool> x){
    std::cout<<"\nVou setar";
    std::cout.flush();
    mut->lock();
    std::cout<<"\nEntrei";
    std::cout.flush();
    cromossomo=x;
    mut->unlock();
    std::cout<<"\nSai";
    std::cout.flush();
}
std::vector<bool> GAFN::No::getCromossomo() const{
    std::unique_lock<std::mutex> ul(*mut);
    return this->cromossomo;
}

double GAFN::No::getFitness(){
    std::unique_lock<std::mutex> ul(*mut);
    return this->fitness;

}
bool GAFN::No::valido(std::vector<bool> v){
    return(converte(v)>=limites[0] && converte(v)<=limites[1]);
}
void GAFN::No::cruza(){
    std::uniform_real_distribution<double> distribution(0,1);
    std::uniform_int_distribution<int> dist(0,numBits-2);
    if(distribution(gen)<cruzamento){
        No* pai2 = selecao();
        std::vector<bool> genesFilho1;
        std::vector<bool> genesFilho2;

        int corte = dist(gen);
        for(int i=0;i<corte;i++){
            genesFilho1.push_back(getCromossomo()[i]);
            genesFilho2.push_back(pai2->getCromossomo()[i]);
        }
        for(int i=corte;i<numBits;i++){
            genesFilho1.push_back(getCromossomo()[i]);
            genesFilho2.push_back(pai2->getCromossomo()[i]);
        }
        if(valido(genesFilho1)&& calculaFit(genesFilho1)>getFitness()){
            setCromossomo(genesFilho1);
            calculaFit();
        }
        else if(valido(genesFilho2)&&calculaFit(genesFilho2)>getFitness()){
            setCromossomo(genesFilho2);
            calculaFit();
        }
    }
    return;
}
std::vector<double>GAFN::No::getHistoricoFit(){
    return historicoFit;
}
std::vector<double>GAFN::No::getHistoricoVal(){
    return historicoVal;
}
double GAFN::No::calculaFit(std::vector<bool> v){
    double x = converte(v);
    //return (sin(10*x*M_PI)/(2*x) + pow(x-1,4));
    return( sin(x) + cos(x*sqrt(3)));
}
void GAFN::No::calculaFit(){
    double x = converte(getCromossomo());
    //return (sin(10*x*M_PI)/(2*x) + pow(x-1,4));
    setFitness( sin(x) + cos(x*sqrt(3)));
}
void GAFN::No::muta(){
    std::uniform_real_distribution<double> distribution(0,1);
    auto vec = getCromossomo();
    std::cout<<"\nPeguei";
    std::cout.flush();
    double valor;
    do{
        std::cout<<"\ntentei";
        std::cout.flush();
        for(int i =0;i<numBits;i++)
            if(distribution(gen)<mutacao)
                vec[i] = !vec[i];
        valor = converte(vec);
    }while(valor<limites[0] || valor>limites[1]);
    std::cout<<"\nMutei";
    std::cout.flush();
    No novo;
    novo.setCromossomo(vec);
    novo.calculaFit();
    if(novo.getFitness()>this->getFitness())
        setCromossomo(vec);
    std::cout<<"\nSetei";
    std::cout.flush();
    calculaFit();
}
void GAFN::No::run(int n){
    for(int i=0;i<n;i++){
        historicoFit.push_back(getFitness());
        historicoVal.push_back(converte(getCromossomo()));
        //std::cout<<"\ncomecando ciclo";
        //std::cout.flush();
        muta();
        //std::cout<<"\nmutei";
        //std::cout.flush();
        cruza();
        //std::cout<<"\ncruzei";
        //std::cout.flush();
    }
    
}

GAFN::GAFN(uint num, uint dimensao, uint geracoes, double inferior, double superior, uint precisao, double mut, double cruz, uint tam){
    No::setLimites(inferior, superior);
    No::setPrecisao(precisao);
    No::setMutacao(mut);
    No::setCruzamento(cruz);
    No::setTamanhoTorneio(tam);
    No::setNumBits(num);
    std::vector<No*> vizinhos;
    /*
    malha = std::vector<No>(dimensao*dimensao);
    for(int i=0;i<dimensao*dimensao;i++){
        
        //Vizinho superior
        vizinhos.push_back(&(
            (i<dimensao)?
            malha[i+dimensao*(dimensao-1)]:
            malha[i-dimensao]
        ));
        
        //Vizinho inferior
        vizinhos.push_back(&(
            (i>=dimensao*(dimensao-1))?
            malha[i-dimensao*(dimensao-1)]:
            malha[i+dimensao]
        ));

        //Vizinho esquerda
        vizinhos.push_back(&(
            (i%dimensao==0)?
            malha[i+(dimensao-1)]:
            malha[i-1]
        ));

        //Vizinho direita
        vizinhos.push_back(&(
            (i%dimensao==(dimensao-1))?
            malha[i-(dimensao-1)]:
            malha[i+1]
        ));
        

        malha.at(i).setVizinhos(vizinhos);
        vizinhos.clear();
    }*/

    malha = std::vector<No>(dimensao);
    for(int i=0;i<dimensao;i++){
        

        //Vizinho esquerda
        vizinhos.push_back(&(
            (i==0)?
            malha[(dimensao-1)]:
            malha[i-1]
        ));

        //Vizinho direita
        vizinhos.push_back(&(
            (i==(dimensao-1))?
            malha[0]:
            malha[i+1]
        ));
        
        malha.at(i).setVizinhos(vizinhos);
        vizinhos.clear();
    }

}

void GAFN::run(uint geracoes){
    std::vector<std::thread> vThreads;
    for(No& n:malha){
        vThreads.push_back(
            std::thread(
                [](No* n, int g){
                    n->run(g);
                }, &n, geracoes
            )
        );
    }

    for(std::thread& t:vThreads)
        t.join();
}

void GAFN::mostraPopulacao(){
    int x=0;
    for(No ind:malha){
        std::cout<<"\nIndividuo "<<x++<<": ";
        for(bool x:ind.getCromossomo())
            std::cout<<" "<<(x?1:0);
        std::cout<<"; x="<<ind.converte(ind.getCromossomo())<<"; y="<<ind.getFitness();
    }
}