#pragma once
#include "GA.hpp"
#include <iostream>
#include <mutex>
#include <thread>

class GAMS: public GA{
private:

/**
 * @brief Quantidade de threads nos quais as cargas de trabalho serao distribuidas
 */
uint nEscravos=2;

/**
 * @brief Modo de paralelismo adotado, onde 1-paralelismo de controle e
 * 2-paralelismo de dados
 */
uint modo=1;
public:

/**
 * @brief Constroi um novo objeto da classe GAMS
 * 
 * @param numeroDeBits A quantidade de bits do genotipo
 * @param tamanhoDaPopulacao A quantidade de individuos que compoem a populacao
 * @param limiteInferior O minimo valor da regiao de operacao
 * @param limiteSuperior O maximo valor da regiao de operacao
 * @param precisao A quantidade de cadas decimais apos a virgula a serem consideradas para a parametrizacao
 * @param torneioTam A quantidade de individuos a serem comparados em um torneio
 * @param mut A probabilidade de mutacao sendo qualquer valor de 0.0(0%) a 1.0(100%)
 * @param cruzamento A probabilidade de cruzamento sendo qualquer valor de 0.0(0%) a 1.0(100%)
 * @param nescravos Quantidade maxima de escravos em execucao
 * @param modo Modo de paralelismo adotado
 */
GAMS(int numeroDeBits, int tamanhoDaPopulacao, double limiteInferior, double limiteSuperior, 
    int precisao,int torneioTam, double mut,double cruzamento, uint nescravos, uint modo=1);
//void run();

/**
 * @brief Calcula a aptidao de cada individuo da populacao, distruindo os indivíduos 
 * entre os escravos disponiveis
 */
void calculaFitPopulacao() override;

/**
 * @brief Popula o vetor populacao com tamanhoDaPopulacao novos individuos
 * distribuidos aleatoriamente dentro dos limites de busca em paralelo
 * 
 */
void geraPopulacao() override;

/**
 * @brief Aplica o operador de cruzamento em toda a populacao gerando uma nova populacao
 * 
 */
void cruzaPopulacao() override;

/**
 * @brief Aplica o operador de mutacao em toda a populacao gerando uma nova populacao
 * 
 */
void mutaPopulacao() override;

};