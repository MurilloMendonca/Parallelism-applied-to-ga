#pragma once
#include "GA.hpp"
#include <iostream>
#include <mutex>
#include <thread>

class GAMS: public GA{
private:
uint nEscravos=2;
uint modo=1;
public:
GAMS(int numeroDeBits, int tamanhoDaPopulacao, double limiteInferior, double limiteSuperior, 
    int precisao,int torneioTam, double,double cruzamento, uint, uint modo=1);
//void run();
void calculaFitPopulacao() override;
void geraPopulacao() override;
void cruzaPopulacao() override;
void mutaPopulacao() override;

};