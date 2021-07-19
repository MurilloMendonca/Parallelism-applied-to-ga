#pragma once

#include <vector>
#include <random>
#include "Individuo.hpp"
#include <cmath>
#include <iostream>
#include <algorithm>

class GA {
    protected:

    //Parametros

    /**
     * @brief Vetor com as limitacoes inferior e superior da regiao de operacao
     * 
     */
    double limites[2];

    /**
     * @brief A quantidade de cadas decimais apos a virgula a serem consideradas para a parametrizacao
     * 
     */
    int precisao = 2;

    /**
     * @brief A probabilidade de mutacao sendo qualquer valor de 0.0(0%) a 1.0(100%)
     * 
     */
    double taxaMutacao = 0.01;

    /**
     * @brief A probabilidade de cruzamento sendo qualquer valor de 0.0(0%) a 1.0(100%)
     * 
     */
    double taxaCruzamento = 0.8;

    /**
     * @brief A quantidade de individuos que compoem a populacao
     * 
     */
    int tamanhoPopulacao = 100;

    /**
     * @brief A quanteidade de melhores individuos a terem passagem garantida para a proxima geracao
     * 
     */
    int quantidadeElitismo=2;

    /**
     * @brief A quantidade de individuos a serem comparados em um torneio
     * 
     */
    int tamanhoTorneio=3;

    /**
     * @brief A quantidade de bits do genotipo
     * 
     */
    int numBits=15;

    //Principal

    /**
     * @brief Vetor que armazena todos os individuos da populacao atual
     * 
     */
    std::vector<Individuo> populacao;

    //Estatisticas

    /**
     * @brief Vetor que armazena uma copia do melhor individuo a cada geracao
     * 
     */
    std::vector<Individuo> melhorPorGeracao;

    /**
     * @brief Vetor que armazena uma copia do pior individuo a cada geracao
     * 
     */
    std::vector<Individuo> piorPorGeracao;

    /**
     * @brief Vetor que armazena o desvio padrao entre a populacao a cada geracao
     * 
     */
    std::vector<double>desvioPadraoPorGeracao;

    /**
     * @brief Vetor que armazena o valor medio da fitness de toda a populacao a cada geracao
     * 
     */
    std::vector<double> mediaPorGeracao;

    //Aleatorio
    std::random_device* rd;
    std::mt19937 gen;
    
    
    public:
    //Principais
    GA(){}
    /**
     * @brief Constroi um novo objeto da classe GA
     * 
     * @param numeroDeBits A quantidade de bits do genotipo
     * @param tamanhoDaPopulacao A quantidade de individuos que compoem a populacao
     * @param limiteInferior O minimo valor da regiao de operacao
     * @param limiteSuperior O maximo valor da regiao de operacao
     * @param precisao A quantidade de cadas decimais apos a virgula a serem consideradas para a parametrizacao
     * @param torneioTam A quantidade de individuos a serem comparados em um torneio
     * @param mut A probabilidade de mutacao sendo qualquer valor de 0.0(0%) a 1.0(100%)
     * @param cruzamento A probabilidade de cruzamento sendo qualquer valor de 0.0(0%) a 1.0(100%)
     */
    GA( int numeroDeBits, 
        int tamanhoDaPopulacao, 
        double limiteInferior, 
        double limiteSuperior, 
        int precisao,
        int torneioTam, 
        double mut, 
        double cruzamento);
    
    virtual ~GA(){}

    /**
     * @brief Executa todas as operacoes do GA por g geracoes
     * 
     * @param g Quantidade de geracoes a serem executadas em sequencia
     */
    virtual void run(int g);

    //helpers

    /**
     * @brief Avalia se um individuo esta dentro da regiao de operacao
     * 
     * @param i Individuo a ser analisado
     * @return true Caso o fenotipo deste individuo esteja dentro dos limites de busca
     * @return false Caso o fenotipo deste individuo esteja fora dos limites de busca
     */
    bool valido(Individuo i);

    /**
     * @brief Popula o vetor populacao com tamanhoDaPopulacao novos individuos
     * distribuidos aleatoriamente dentro dos limites de busca
     */
    virtual void geraPopulacao();

    /**
     * @brief Printa no console o genotipo, fenotipo e aptidao de cada individuo da populacao
     * 
     */
    void mostraPopulacao();

    void setPopulation(std::vector<Individuo>);

    /**
     * @brief Cria um novo individuo com genotipo aleatorio dentro da regiao de operacao
     * 
     * @return Individuo 
     */
    Individuo geraIndividuoValido();

    //Operadores evolutivos

    /**
     * @brief Calcula a aptidao de cada individuo da populacao
     */
    virtual void calculaFitPopulacao();

    /**
     * @brief Calcula a aptidao de um individuo
     * 
     * @param i Individuo a ser analisado
     * @return double aptidao calculada do individuo i
     */
    double calculaFit(Individuo i);

    /**
     * @brief Aplica o operador de cruzamento em toda a populacao gerando uma nova populacao
     * 
     */
    virtual void cruzaPopulacao();

    /**
     * @brief Realiza o cruzamento entre dois inviduos utilizando single point crossover
     * 
     * @param pai1 Primeiro individuo para o cruzamento
     * @param pai2 Segundo individuo para o cruzamento
     * @return std::vector<Individuo> Dois filhos complementares gerados a partir do cruzamento
     */
    std::vector<Individuo> cruza( Individuo pai1,  Individuo pai2);

    /**
     * @brief Aplica o operador de mutação em cada individuo da populacao
     * 
     */
    virtual void mutaPopulacao();

    /**
     * @brief Aplica a mutacao em um individuo, onde a probabilidade de mutacao eh avalidada
     * a cada bit do genotipo, invertendo este bit quando dentro da probabilidade
     * 
     * @param ind Individio a ser mutado
     */
    void muta(Individuo& ind);

    /**
     * @brief Realiza um operador de torneio em tamanhoTorneio individuos aleatoriamente 
     * sorteados da populacao
     * 
     * @return Individuo o melhor individuo dentre os avaliados
     */
    Individuo torneio();

    //Estatisticas

    /**
     * @brief Calcula a aptidao media da populaca
     * 
     * @return double a aptidao media dentro todos os individuos
     */
    double fitnessMedia();

    /**
     * @brief Obtem o melhor individuo dentre os presentes no vetor recebido
     * 
     * @return Individuo individuo com a maior aptidao dentre os recebidos
     */
    Individuo melhor(std::vector<Individuo>);

    /**
     * @brief Obtem o melhor individio dentre todos da populacao
     * 
     * @return Individuo 
     */
    Individuo melhordaPopulacao();

    /**
     * @brief Obtem o melhor individuo dentre os presentes no vetor recebido
     * 
     * @return Individuo individuo com a menor aptidao dentre os recebidos
     */
    Individuo pior (std::vector<Individuo>);

    /**
     * @brief Obtem o pior individio dentre todos da populacao
     * 
     * @return Individuo 
     */
    Individuo piordaPopulacao ();

    /**
     * @brief Obtem o vetor dos melhores individuos de cada geracao
     * 
     * @return std::vector<Individuo> vetor com o melhor individuo de cada populacao i em cada posicao i
     */
    std::vector<Individuo> getmelhorPorGeracao();

    /**
     * @brief Obtem o vetor dos piores individuos de cada geracao
     * 
     * @return std::vector<Individuo> vetor com o pior individuo de cada populacao i em cada posicao i
     */
    std::vector<Individuo> getpiorPorGeracao();

    /**
     * @brief Obtem o vetor das fitness medias de cada geracao
     * 
     * @return std::vector<double> vetor com a fitness de cada populacao i em cada posicao i
     */
    std::vector<double> getMediaPorGeracao();
    std::vector<double> getDesvioPadraoPorGeracao();

    /**
     * @brief Obtem os n melhores individuos dentre o vetor vec
     * 
     * @param vec Vetor de individuos para a fonte da busca
     * @param n Quantidade de melhores a serem retornados
     * @return std::vector<Individuo> Vetor contendo os n melhores individuos de vec
     */
    std::vector<Individuo> melhores( std::vector<Individuo> vec, int n);
    double desvioPadraoDaPopulacao();

    //Conversoes
    double converte_bin_dec(std::vector<bool> gray);
    std::vector<bool> converte_dec_bin(double x);
    double converte_gray_dec(std::vector<bool>gray);
    std::vector<bool> converte_dec_gray(double x);
    double getFenotipo(std::vector<bool>genotipo);
    std::vector<bool> getGenotipo(double fenotipo);
};

