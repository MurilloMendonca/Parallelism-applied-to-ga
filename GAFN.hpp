#pragma once
#include "Individuo.hpp"
#include "GA.hpp"
#include <cmath>
#include <mutex>
#include <vector>
#include <set>


class GAFN{
    protected:

    class No: public Individuo, protected GA{
        private:
            static int limites[2];
            static uint precisao;
            static double mutacao;
            static double cruzamento;
            static uint numBits;
            static uint tamanhoTorneio;
            std::vector<No*> vizinhos;
            std::mutex* mut;
            void muta();
            void cruza();
            bool valido(std::vector<bool> v);
            No* selecao();
            No* melhor(std::set<No*>vec);
            No* torneio();
            void calculaFit();
            double calculaFit(std::vector<bool> v);
            std::vector<double>historicoFit;
            std::vector<double>historicoVal;
            
            /*
            double converte_bin_dec(std::vector<bool> gray);
            std::vector<bool> converte_dec_bin(double x);
            double converte_gray_dec(std::vector<bool>gray);
            std::vector<bool> converte_dec_gray(double x);*/
            std::random_device* rd;
            std::mt19937 gen;
        public:
            No();
            void run(int n) override;
            void setVizinhos(std::vector<No*>);
            void setFitness(double x) override;
            void setCromossomo(std::vector<bool> x) override;
            std::vector<bool> getCromossomo() const override;
            double getFitness() override;
            double converte(std::vector<bool>);
            std::vector<double>getHistoricoFit();
            std::vector<double>getHistoricoVal();
            std::vector<bool> converte(double);


            static void setPrecisao(uint prec);
            static void setNumBits(uint n);
            static void setTamanhoTorneio(uint n);
            static void setMutacao(double mut);
            static void setCruzamento(double mut);
            static void setLimites(double inferior, double superior);
    };
    
    public:
    std::vector<No> malha;
    GAFN(uint num,uint dimensao, uint geracoes, double inferior, double superior, uint precisao, double mut, double cruz, uint tam);
    void run(uint);
    void mostraPopulacao();
};