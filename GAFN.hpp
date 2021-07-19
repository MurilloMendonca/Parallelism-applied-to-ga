#pragma once
#include "Individuo.hpp"
#include "GA.hpp"
#include <cmath>
#include <mutex>
#include <vector>
#include <set>


class GAFN{
    protected:

    class No: public Individuo,  protected  GA{
        private:
            /**
             * @brief Vetor com as limitacoes inferior e superior da regiao de operacao
             * 
             */
            static int limites[2];
            /**
             * @brief A quantidade de cadas decimais apos a virgula a serem consideradas para a parametrizacao
             * 
             */
            static uint precisao;

            /**
             * @brief A probabilidade de mutacao sendo qualquer valor de 0.0(0%) a 1.0(100%)
             * 
             */
            static double mutacao;

            /**
             * @brief A probabilidade de cruzamento sendo qualquer valor de 0.0(0%) a 1.0(100%)
             * 
             */
            static double cruzamento;

            /**
             * @brief A quantidade de bits do genotipo
             * 
             */
            static uint numBits;

            /**
             * @brief A quantidade de individuos a serem comparados em um torneio
             * 
             */
            static uint tamanhoTorneio;

            /**
             * @brief Vizinhos relacionados a este noh
             * 
             */
            std::vector<No*> vizinhos;

            /**
             * @brief Dispositivo de exclusao multipla deste noh
             * 
             */
            std::mutex* mut;

            /**
             * @brief Realiza a mutacao deste noh 
             * 
             */
            void muta();

            /**
             * @brief Realiza o cruzamento deste noh com um de sesu vizinhos
             * 
             */
            void cruza();

            /**
             * @brief Verifica se um genoma representa um individuo valido
             * 
             * @param v genotipo a ser analisado
             * @return true caso o valor correspondente de v esteja dentro da regiao valida
             * @return false caso contrario
             */
            bool valido(std::vector<bool> v);

            /**
             * @brief Realiza a selecao do noh vizinho com o qual ocorrera o cruzamento
             * 
             * @return No* endereco do noh vizinhho selecionado
             */
            No* selecao();

            /**
             * @brief Obtem o melhor noh dentre os presentes no vetor recebido
             * 
             * @return No* endereco do noh com a maior aptidao dentre os recebidos
             */
            No* melhor(std::set<No*>vec);

            /**
             * @brief Seleciona um noh vizinho com maior aptidao dentro os sorteados
             * 
             * @return No* endereco do noh vizinhho vencedor do torneio
             */
            No* torneio();

            /**
             * @brief Calcula e define a aptidao deste noh
             * 
             */
            void calculaFit();

            /**
             * @brief Calcula a aptidao de um genotipo passado
             * 
             * @param v Genotipo que sera avaliado
             * @return double Valor da aptidao de v
             */
            double calculaFit(std::vector<bool> v);

            /**
             * @brief Historico de todos os valores de aptidao assumidos por esse noh ao decorrer da execucao
             * 
             */
            std::vector<double>historicoFit;

            /**
             * @brief Historico de todos os valores de fenotipo assumidos por esse noh ao decorrer da execucao
             * 
             */
            std::vector<double>historicoVal;
            
            /*
            double converte_bin_dec(std::vector<bool> gray);
            std::vector<bool> converte_dec_bin(double x);
            double converte_gray_dec(std::vector<bool>gray);
            std::vector<bool> converte_dec_gray(double x);*/
            std::random_device* rd;
            std::mt19937 gen;
        public:

            /**
             * @brief Constroi um novo noh
             * 
             */
            No();

            /**
             * @brief Executa todas as operacoes do Noh por n geracoes
             * 
             * @param n Quantidade de geracoes a serem executadas em sequencia
             */
            void run(int n) override;

            /**
             * @brief Define o vetor de vizinhos
             * 
             */
            void setVizinhos(std::vector<No*>);

            /**
             * @brief Define o valor da aptidao
             * 
             * @param x Valor a ser atribuido a aptidao
             */
            void setFitness(double x) override;

            /**
             * @brief Define os valores do genotipo
             * 
             * @param x Valores a serem atribuidos ao genotipo deste noh
             */
            void setCromossomo(std::vector<bool> x) override;

            /**
             * @brief Obtem o cromossomo deste noh
             * 
             * @return std::vector<bool> 
             */
            std::vector<bool> getCromossomo() const override;

            /**
             * @brief Obtem a aptidao deste noh
             * 
             * @return double 
             */
            double getFitness() override;

            /**
             * @brief Realiza a conversao de genotipo para fenotipo
             * 
             * @param v Genotipo a ser convertido
             * @return double Fenotipo retornado
             */
            double converte(std::vector<bool> v);

            /**
             * @brief Realiza a conversao de fenotipo para genotipo
             * 
             * @param d Fenotipo a ser convertido
             * @return std::vector<bool> Genotipo retornado
             */
            std::vector<bool> converte(double d);

            /**
             * @brief Obtem o historico de aptidoes que este noh assumiu
             * 
             * @return std::vector<double> Historico de aptidoes
             */
            std::vector<double> getHistoricoFit();

            /**
             * @brief btem o historico de fenotipos que este noh assumiu
             * 
             * @return std::vector<double> Historico de fenotipos
             */
            std::vector<double> getHistoricoVal();
            

            /**
             * @brief Define a precisao a ser usada na parametrizacao
             * 
             * @param prec A quantidade de cadas decimais apos a virgula a serem consideradas para a parametrizacao
             */
            static void setPrecisao(uint prec);

            /**
             * @brief Define a quantidade de bits do genotipo
             * 
             * @param n A quantidade de bits do genotipo
             */
            static void setNumBits(uint n);

            /**
             * @brief Define a quantidade de individuos a serem comparados em um torneio
             * 
             * @param n A quantidade de individuos a serem comparados em um torneio
             */
            static void setTamanhoTorneio(uint n);

            /**
             * @brief Define A probabilidade de mutacao 
             * 
             * @param mut A probabilidade de mutacao sendo qualquer valor de 0.0(0%) a 1.0(100%)
             */
            static void setMutacao(double mut);

            /**
             * @brief Define A probabilidade de cruzamento 
             * 
             * @param mut A probabilidade de cruzamento sendo qualquer valor de 0.0(0%) a 1.0(100%)
             */
            static void setCruzamento(double mut);

            /**
             * @brief Define as limitacoes inferior e superior da regiao de operacao
             * 
             * @param inferior O minimo valor da regiao de operacao
             * @param superior O maximo valor da regiao de operacao
             */
            static void setLimites(double inferior, double superior);
    };
    
    public:

    /**
     * @brief Vetor responsavel por armazenar todos os nohs 
     * 
     */
    std::vector<No> malha;

    /**
     * @brief Construct a new GAFN object
     * 
     * @param num A quantidade de bits do genotipo
     * @param dimensao A quantidade de nohs
     * @param inferior O minimo valor da regiao de operacao
     * @param superior O maximo valor da regiao de operacao
     * @param precisao A quantidade de cadas decimais apos a virgula a serem consideradas para a parametrizacao
     * @param mut A probabilidade de mutacao sendo qualquer valor de 0.0(0%) a 1.0(100%)
     * @param cruz A probabilidade de cruzamento sendo qualquer valor de 0.0(0%) a 1.0(100%)
     * @param tam A quantidade de individuos a serem comparados em um torneio
     */
    GAFN(uint num,uint dimensao, double inferior, double superior, uint precisao, double mut, double cruz, uint tam);
    
    /**
     * @brief Inicia a execucao de todos os nohs cada um em uma thread
     * 
     * @param g Quantidade de iteracoes realizadas por cada noh
     */
    void run(uint g);

    /**
     * @brief Printa no console o genotipo, fenotipo e aptidao de cada noh da malha
     * 
     */
    void mostraPopulacao();
};