#include "GA.hpp"


GA::GA(int numeroDeBits, int tamanhoDaPopulacao, double limiteInferior, 
        double limiteSuperior, int precisao, int torneioTam, double mut,
        double cruzamento){
    rd = new std::random_device;
    gen = std::mt19937((*rd)());
    numBits=numeroDeBits;
    this->taxaMutacao=mut;
    this->taxaCruzamento=cruzamento;
    this->precisao=precisao;
    this->tamanhoPopulacao=tamanhoDaPopulacao;
    this->tamanhoTorneio=torneioTam;
    limites[0]=limiteInferior;
    limites[1]=limiteSuperior;
    geraPopulacao();
    calculaFitPopulacao();
}
void GA::mostraPopulacao(){
    int x=0;
    for(Individuo ind:populacao){
        std::cout<<"\nIndividuo "<<x++<<": ";
        for(int i=0;i<numBits;i++)
            std::cout<<" "<<ind.getCromossomo()[i];
        std::cout<<"; x="<<getFenotipo(ind.getCromossomo())<<"; y="<<ind.getFitness();
    }
}
Individuo GA::geraIndividuoValido(){
    std::uniform_real_distribution<double> distribution(limites[0],limites[1]);
    double valor=distribution(gen);
    auto convertido = getGenotipo(valor);
    Individuo novo(convertido);
    return novo;
}
void GA::run(int geracoes){
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
    }
}
 void GA::calculaFitPopulacao(){
    for(Individuo& ind:populacao)
        ind.setFitness(calculaFit(ind));
}
void GA::geraPopulacao(){
    for(int i=0;i<this->tamanhoPopulacao;i++)
        populacao.push_back(geraIndividuoValido());
}
double GA::calculaFit(Individuo ind){
    double x = getFenotipo(ind.getCromossomo());
    //return (sin(10*x*M_PI)/(2*x) + pow(x-1,4));
    return sin(x) + cos(x*sqrt(3));
}
void GA::cruzaPopulacao(){
    std::vector<Individuo> novapop;
    std::vector<Individuo> filhos;
    std::uniform_real_distribution<double> distribution(0,1);
    while (novapop.size()<(tamanhoPopulacao-quantidadeElitismo)){
        Individuo pai1 = torneio();
        Individuo pai2 = torneio();
        if(distribution(gen)>taxaCruzamento){
            filhos = cruza(pai1, pai2);
            if(valido(filhos[0]))
                novapop.push_back(filhos[0]);
            if(valido(filhos[1])&&(novapop.size()<(tamanhoPopulacao-quantidadeElitismo)))
                novapop.push_back(filhos[1]);
        }
        else{
            novapop.push_back(pai1);
            if(novapop.size()<(tamanhoPopulacao-quantidadeElitismo))
                novapop.push_back(pai2);
        }
    }
    populacao=novapop;
}
std::vector<Individuo> GA::melhores( std::vector<Individuo> vec, int n){
    std::vector<Individuo> v;
    for(int i=0;i<n;i++){
        v.push_back(melhor(vec));
        vec.erase(std::find(vec.begin(),vec.end(),v.at(i)));
    }
    return v;
}
std::vector<Individuo> GA::cruza( Individuo pai1,  Individuo pai2){
    std::uniform_int_distribution<int> distribution(0,numBits-1);

    std::vector<std::vector<bool>> genFilhos(2);
    std::vector<bool> genesFilho1;
    std::vector<bool> genesFilho2;

    int corte = distribution(gen);
    for(int i=0;i<corte;i++){
        genesFilho1.push_back(pai1.getCromossomo()[i]);
        genesFilho2.push_back(pai2.getCromossomo()[i]);
    }
    for(int i=corte;i<numBits;i++){
        genesFilho1.push_back(pai1.getCromossomo()[i]);
        genesFilho2.push_back(pai2.getCromossomo()[i]);
    }
    std::vector<Individuo>res;
    res.emplace_back(genesFilho1);
    res.emplace_back(genesFilho2);
    return res;
}
void GA::mutaPopulacao(){
    std::uniform_real_distribution<double> distribution(0,1);
    for(Individuo& ind:populacao)
        //if(distribution(gen)<taxaMutacao)
        muta(ind);
}
void GA::muta(Individuo& ind){
    std::uniform_real_distribution<double> distribution(0,1);
    auto vec = ind.getCromossomo();
    Individuo mutado(vec);
    do{
        vec = ind.getCromossomo();
        for(int i =0;i<numBits;i++)
            if(distribution(gen)<taxaMutacao)
                vec[i] = !vec[i];
        mutado.setCromossomo(vec);
    }while(!valido(mutado));
    mutado.setFitness(calculaFit(mutado));
    ind = mutado;
}
Individuo GA::torneio(){
    std::vector<Individuo> aux;
    std::uniform_int_distribution<int> distribution(0,tamanhoPopulacao-1);
    for(int i=0;i<tamanhoTorneio;i++){
        int pos = distribution(gen);
        aux.push_back(populacao.at(pos));
    }
    return melhor(aux);
}
Individuo GA::melhor(std::vector<Individuo>vec){
    return *(std::max_element(vec.begin(), vec.end()));
}
Individuo GA::pior (std::vector<Individuo>vec){
    return *(std::min_element(vec.begin(), vec.end()));;
}
Individuo GA::piordaPopulacao (){
    return pior(populacao);
}
Individuo GA::melhordaPopulacao(){
    return melhor(populacao);
}
double GA::converte_bin_dec(std::vector<bool>bin)
{

    double dec =0;
    for(int i =0, j=(numBits-1); j>=0; i++,j--)
    {
        dec += bin[j]*pow(2,i);
    }
    
    dec = dec/pow(10, precisao);
    dec+=limites[0];
        return dec;

}
std::vector<bool> GA::converte_dec_bin(double x)
{
    x=-x;
    x+=limites[0];
    float decf = x*pow(10, precisao);
    int dec = decf;
    std::vector<bool>bin(numBits);

    for(int i =(numBits-1); i>=0; i--)
    {
        bin[i] = dec%2;
        dec = dec/2;
    }
    return bin;
}
double GA::fitnessMedia(){
    double media = 0;
    for(auto ind:populacao)
        media+=ind.getFitness()/populacao.size();
    return media;
}
void GA::setPopulation(std::vector<Individuo> nova){
    this->populacao=nova;
    calculaFitPopulacao();
}
std::vector<Individuo> GA::getmelhorPorGeracao(){
    return this->melhorPorGeracao;
}
std::vector<Individuo> GA::getpiorPorGeracao(){
    return this->piorPorGeracao;
}
std::vector<double> GA::getMediaPorGeracao(){
    return this->mediaPorGeracao;
}
std::vector<double> GA::getDesvioPadraoPorGeracao(){
    return this->desvioPadraoPorGeracao;
}
double GA::desvioPadraoDaPopulacao(){
    double media = fitnessMedia();
    double dp=0;
    for(auto ind:populacao){
        dp+=pow((ind.getFitness() - media),2)/populacao.size();
    }
    return dp;
}
double GA::converte_gray_dec(std::vector<bool>gray)
{
    std::vector<bool>bin(numBits);
    int i= 1;
    bin[0]=gray[0];
    while(i<numBits)
    {
 
        bin[i]=gray[i]^bin[i-1];
        i++;
    }
    return converte_bin_dec(bin);
    

}
bool GA::valido(Individuo i){
    return(getFenotipo(i.getCromossomo())>=limites[0] && getFenotipo(i.getCromossomo())<=limites[1]);
}
std::vector<bool> GA::converte_dec_gray(double x)
{

    std::vector<bool>bin = converte_dec_bin(x);
    std::vector<bool>gray(numBits);
    int i= 1;
    gray[0]=bin[0];
    while(i<numBits)
    {
        {
            gray[i]=bin[i]^bin[i-1];
        }
        i++;
    }
    return gray;
}
double GA::getFenotipo(std::vector<bool>genotipo){
    //return converte_bin_dec(genotipo);
    return converte_gray_dec(genotipo);
}
std::vector<bool> GA::getGenotipo(double fenotipo){
    return converte_dec_gray(fenotipo);
    //return converte_dec_bin(fenotipo);
}