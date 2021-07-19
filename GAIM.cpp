#include "GAIM.hpp"
#include <algorithm>
#include <mutex>
std::mutex GAIM::Ilha::mutex;
std::vector<Individuo> GAIM::Ilha::migrantes;
GAIM::Ilha::Ilha(int numeroDeBits, int tamanhoDaPopulacao, double limiteInferior, double limiteSuperior,
                int precisao,int torneioTam, double mut,double cruzamento, uint periodos ):
GA( numeroDeBits,  tamanhoDaPopulacao,  limiteInferior,  limiteSuperior,  precisao, torneioTam, mut, cruzamento)
{
this->periodo=periodos;
}

GAIM::GAIM(uint nIlhas, int numeroDeBits, int tamanhoDaPopulacao, double limiteInferior, double limiteSuperior,
             int precisao,int torneioTam, double mut,double cruzamento, uint periodos, uint modo)
{
    
    gen = std::mt19937(rd());
    ilhas = std::vector<Ilha>();
    for(int i=0;i<nIlhas;i++){
        Ilha* a = new Ilha ( numeroDeBits,  tamanhoDaPopulacao,  limiteInferior,  limiteSuperior,
              precisao, torneioTam,  mut,  periodos);
        this->ilhas.push_back(*a);
    }
    //std::vector<GAIM::Ilha>* GAIM::Ilha::arquipelago();
}

void GAIM::run (uint geracoes){
    std::vector<std::thread>vThread;
    for(Ilha& i:ilhas){
        vThread.push_back(
            std::thread(
                [](Ilha* i, int gen){
                    i->run(gen);
                },
                &i,geracoes
            )
        );
    }
    for(auto& t:vThread)
        t.join();
}
GAIM::Ilha* GAIM::operator[](int x){
    return &this->ilhas.at(x);
}

void GAIM::Ilha::emigracao(){
    mutex.lock();
    Individuo ind = melhordaPopulacao();
    migrantes.push_back(ind);
    std::cout<<"\nIndividuo enviado: ";
        for(int i=0;i<numBits;i++)
            std::cout<<" "<<ind.getCromossomo()[i];
        std::cout<<"; x="<<converte_gray_dec(ind.getCromossomo())<<"; y="<<ind.getFitness();
    mutex.unlock();
}
void GAIM::Ilha::imigracao(){
    mutex.lock();
    if(!migrantes.empty()){
        populacao.erase(std::min_element(populacao.begin(), populacao.end()));
        Individuo ind = migrantes.back();
        populacao.push_back(migrantes.back());
        std::cout<<"\nIndividuo recebido: ";
        for(int i=0;i<numBits;i++)
            std::cout<<" "<<ind.getCromossomo()[i];
        std::cout<<"; x="<<converte_gray_dec(ind.getCromossomo())<<"; y="<<ind.getFitness();
        migrantes.pop_back();
    }
    mutex.unlock();
}

void GAIM::Ilha::run(int geracoes){
for(int i=0;i<geracoes;i++){
        auto elite = melhores(populacao, quantidadeElitismo);
        cruzaPopulacao();
        mutaPopulacao();
        calculaFitPopulacao();
        for(auto ind:elite)
            populacao.push_back(ind);
        melhorPorGeracao.push_back(melhordaPopulacao());
        piorPorGeracao.push_back(piordaPopulacao());
        mediaPorGeracao.push_back(fitnessMedia());
        desvioPadraoPorGeracao.push_back(desvioPadraoDaPopulacao());
        if(i%periodo==0 && i>0){
            emigracao();
        }
        else if(i%periodo==1 && i>0){
            imigracao();
        }
        
    }
}