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
        /**
         * @brief Quantidade de geracoes entre migracoes
         * 
         */
        uint periodo;
        
        public:
        /**
         * @brief Individuos a espera de serem recebidos por outras ilhas
         */
        static std::vector<Individuo> migrantes;

        /**
         * @brief Dispositivo de exclusao multipla para acesso ao vetor de migrantes
         */
        static std::mutex mutex;

        /**
         * @brief Constroi um novo objeto da classe Ilha
         * 
         * @param numeroDeBits A quantidade de bits do genotipo
         * @param tamanhoDaPopulacao A quantidade de individuos que compoem a populacao
         * @param limiteInferior O minimo valor da regiao de operacao
         * @param limiteSuperior O maximo valor da regiao de operacao
         * @param precisao A quantidade de cadas decimais apos a virgula a serem consideradas para a parametrizacao
         * @param torneioTam A quantidade de individuos a serem comparados em um torneio
         * @param mut A probabilidade de mutacao sendo qualquer valor de 0.0(0%) a 1.0(100%)
         * @param cruzamento A probabilidade de cruzamento sendo qualquer valor de 0.0(0%) a 1.0(100%)
         * @param periodos Quantidade de geracoes entre migracoes
         */
        Ilha(int numeroDeBits, int tamanhoDaPopulacao, double limiteInferior, double limiteSuperior,
             int precisao,int torneioTam, double mut, double cruzamento, uint periodos =5 );
        
        /**
         * @brief Envia o melhor individuo da populacao atual para o vetor de migrantes
         */
        void emigracao();

        /**
         * @brief Recebe um individuo do vetor de migrantes, substituindo o pior individuo da populacao
         */
        void imigracao();

        /**
         * @brief Executa todas as operacoes da Ilha por geracoes geracoes
         * 
         * @param geracoes Quantidade de geracoes a serem executadas em sequencia
         */
        void  run(int geracoes) override;
        

    };

    /**
     * @brief Todas as ilhas em execucao
     */
    std::vector<Ilha> ilhas;

    /**
     * @brief Constroi um novo objeto da classe GAIM
     * 
     * @param nIlhas Quantidade de subpopulacaoes a serem executadas em paralelo
     * @param numeroDeBits A quantidade de bits do genotipo
     * @param tamanhoDaPopulacao A quantidade de individuos que compoem cada subpopulacao
     * @param limiteInferior O minimo valor da regiao de operacao
     * @param limiteSuperior O maximo valor da regiao de operacao
     * @param precisao A quantidade de cadas decimais apos a virgula a serem consideradas para a parametrizacao
     * @param torneioTam A quantidade de individuos a serem comparados em um torneio
     * @param mut A probabilidade de mutacao sendo qualquer valor de 0.0(0%) a 1.0(100%)
     * @param cruzamento A probabilidade de cruzamento sendo qualquer valor de 0.0(0%) a 1.0(100%)
     * @param periodos Quantidade de geracoes entre migracoes
     */
    GAIM(uint nIlhas, int numeroDeBits, int tamanhoDaPopulacao, double limiteInferior, double limiteSuperior,
             int precisao,int torneioTam, double mut,double cruzamento, uint periodos);
    
    /**
     * @brief Executa todas as ilhas, cada uma em uma thread
     * 
     * @param geracoes Quantidade de iteracoes que cada ilha vai realizar
     */
    void run (uint geracoes);

    Ilha* operator[](int x);
};