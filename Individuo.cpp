#include "Individuo.hpp"
Individuo::Individuo(){

}
Individuo::Individuo(std::vector<bool> cromossomo ){
    this->cromossomo=cromossomo;
}
std::vector<bool> Individuo::getCromossomo() const{
    return this->cromossomo;
}

double Individuo::getFitness(){
    return this->fitness;
}
void Individuo::setFitness(double x){
    this->fitness=x;
}
void Individuo::setCromossomo(std::vector<bool> x){
    this->cromossomo=x;
}

bool operator<( Individuo &ind1,  Individuo &ind2){
    return ind1.getFitness()<ind2.getFitness();
}
bool operator==( const Individuo ind1,  const Individuo ind2){
    int s = ind1.getCromossomo().size();
    for(int i=0;i<ind1.getCromossomo().size();i++)
        if(ind1.getCromossomo()[i]!=ind2.getCromossomo()[i]) return false;
    return true;
}
