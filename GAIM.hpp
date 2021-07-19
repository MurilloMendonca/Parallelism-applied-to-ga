#pragma once
#include "GA.hpp"
#include <thread>
#include <mutex>
#include <vector>
#include <algorithm>
#include <iostream>

class GAIM{
    private:
    std::random_device rd;
    std::mt19937 gen;
    

    public:
    class Ilha: public GA{
        private:
        uint periodo;
        uint geracoes;
        uint modo;
        
        public:
        static std::vector<Individuo> migrantes;
        static std::mutex mutex;
        Ilha(int numeroDeBits, int tamanhoDaPopulacao, double limiteInferior, double limiteSuperior,
             int precisao,int torneioTam, double mut, double cruzamento, uint periodos =5 );
        void emigracao();
        void imigracao();
        void  run(int geracoes) override;
        

    };
    std::vector<Ilha> ilhas;
    GAIM(uint nIlhas, int numeroDeBits, int tamanhoDaPopulacao, double limiteInferior, double limiteSuperior,
             int precisao,int torneioTam, double mut,double cruzamento, uint periodos, uint modo=1);
    void run (uint geracoes);
    Ilha* operator[](int x);
};