#include "GAMS.hpp"
#include <iostream>

GAMS::GAMS(int numeroDeBits, int tamanhoDaPopulacao, double limiteInferior, double limiteSuperior, int precisao,int torneioTam, double mut,double cruzamento, uint escravos, uint modo):
GA( numeroDeBits,  tamanhoDaPopulacao,  limiteInferior,  limiteSuperior,  precisao, torneioTam, mut, cruzamento){
    this->nEscravos=escravos;
    this->modo=modo;


}

void GAMS::calculaFitPopulacao(){
    std::vector<std::thread> vThreads;
    std::mutex mut;
    std::cout.flush();
    Individuo* vec = populacao.data();
    int tam = populacao.size();
    int* controle=new int;
    *controle=0;
    for(int i=0;i<this->nEscravos;i++){
        //PARALELISMO DE CONTROLE
        if(modo==1){
            vThreads.push_back(std::thread([](Individuo* vec, int* controle, int tam, std::mutex* mut,GAMS* g){
                int i;
                while(*controle<tam){
                    mut->lock();
                    i=*controle;
                    *controle=(*controle) +1;
                    mut->unlock();
                    if(i<tam)
                        vec[i].setFitness(g->calculaFit(vec[i]));
                }
            }, vec, controle,tam, &mut,this));
        }
        //PARALELISMO DE DADOS
        if(modo==2){
            vThreads.push_back(std::thread([](Individuo* vec, int tam, GAMS* g){
                for(int i=0;i<tam;i++){
                    vec[i].setFitness(g->calculaFit(vec[i]));
                }
            }, &vec[(i)*(tam/nEscravos)], (i==(nEscravos-1))?((tam/nEscravos)+(tam%nEscravos)):(tam/nEscravos), this));
        }
    }
    for(std::thread& t:vThreads)
        t.join();
}

void GAMS::geraPopulacao(){
    std::vector<std::thread> vThreads;
    std::mutex mut;
    std::cout<<"\nGerando pop";
    std::cout.flush();
    populacao=std::vector<Individuo>(tamanhoPopulacao);
    int tam = populacao.size();
    int* controle=new int;
    *controle=0;
    for(int i=0;i<this->nEscravos;i++){
        //PARALELISMO DE CONTROLE
        if(modo==1){
            vThreads.push_back(std::thread([](Individuo* vec, int* controle, int tam, std::mutex* mut,GAMS* g){
                int i;
                while(*controle<tam){
                    mut->lock();
                    i=*controle;
                    *controle=(*controle) +1;
                    mut->unlock();
                    if(i<tam)
                        vec[i]=(g->geraIndividuoValido());
                }
            }, populacao.data(), controle,tam, &mut,this));
        }
        //PARALELISMO DE DADOS
        if(modo==2){
            vThreads.push_back(std::thread([](Individuo* vec, int tam, GAMS* g){
                for(int i=0;i<tam;i++){
                    vec[i]=(g->geraIndividuoValido());
                }
            }, &populacao[(i)*(tam/nEscravos)], (i==(nEscravos-1))?((tam/nEscravos)+(tam%nEscravos)):(tam/nEscravos), this));
        }
    }
    for(std::thread& t:vThreads)
        t.join();
    std::cout<<"\nGerei pop";
    std::cout.flush();
}

void GAMS::cruzaPopulacao(){
    
    std::vector<std::thread> vThreads;
    std::mutex mut;
    std::cout.flush();
    int tam = tamanhoPopulacao-quantidadeElitismo;
    std::vector<Individuo> novapop(tam);
    int* controle=new int;
    *controle=0;
    for(int i=0;i<this->nEscravos;i++){
        //PARALELISMO DE CONTROLE
        if(modo==1){
            vThreads.push_back(std::thread([](Individuo* vec, int* controle, int tam, std::mutex* mut,GAMS* g){
                int i;
                while(*controle<tam){
                    mut->lock();
                    i=*controle;
                    *controle=(*controle) +2;
                    mut->unlock();

                    std::vector<Individuo> filhos;
                    if(i<tam){
                        filhos = g->cruza(g->torneio(), g->torneio());
                        if(g->valido(filhos[0]))
                            vec[i]=(filhos[0]);
                        if(g->valido(filhos[1])&&(i+1<tam))
                            vec[i+1]=(filhos[1]);

                    }
                }
                }, novapop.data(), controle,tam, &mut,this));
            }
        //PARALELISMO DE DADOS
        if(modo==2){
            vThreads.push_back(std::thread([](Individuo* vec, int tam, GAMS* g){
                for(int i=0;i<tam;i+=2){
                    std::vector<Individuo> filhos;
                    
                   
                    filhos = g->cruza(g->torneio(), g->torneio()); 
                    
                    if(g->valido(filhos[0]))
                        vec[i]=(filhos[0]);
                    if(g->valido(filhos[1])&&(i+1<tam))
                        vec[i+1]=(filhos[1]);
                }
            }, &novapop[(i)*(tam/nEscravos)],(i==(nEscravos-1))?((tam/nEscravos)+(tam%nEscravos)):(tam/nEscravos), this));
        }
    }
    for(std::thread& t:vThreads)
        t.join();

    populacao=novapop;
}
void GAMS::mutaPopulacao(){
    std::vector<std::thread> vThreads;
    std::mutex mut;
    std::cout.flush();
    Individuo* vec = populacao.data();
    int tam = populacao.size();
    int* controle=new int;
    *controle=0;
    for(int i=0;i<this->nEscravos;i++){
        //PARALELISMO DE CONTROLE
        if(modo==1){
            vThreads.push_back(std::thread([](Individuo* vec, int* controle, int tam, std::mutex* mut,GAMS* g){
                int i;
                while(*controle<tam){
                    mut->lock();
                    i=*controle;
                    *controle=(*controle) +1;
                    //std::cout<<"\nmeu i:"<<i;
                    //std::cout.flush();
                    mut->unlock();
                    if(i<tam)
                        g->muta(vec[i]);
                }
            }, vec, controle,tam, &mut,this));
        }
        //PARALELISMO DE DADOS
        if(modo==2){
            vThreads.push_back(std::thread([](Individuo* vec, int tam, GAMS* g){
                for(int i=0;i<tam;i++){
                    g->muta(vec[i]);
                }
            }, &vec[(i)*(tam/nEscravos)], (i==(nEscravos-1))?((tam/nEscravos)+(tam%nEscravos)):(tam/nEscravos), this));
        }
    }
    for(std::thread& t:vThreads)
        t.join();
}