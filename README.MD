# Parallelism applied to GA

This project seeks to study and better understand the shapes and consequences of applying a parallel implementation or modelling to a Genetic Algorithm.

## Whats a GA?

Genetic Algorithms are a class or problem solver algorithms designed based on natural and genetic behavior. A set of possible solutions (this set being called the population) to a given problem is generated and each iteration of the algorithm apply evolution based operators, such as selection, crossover and mutation, targeting to improve this solutions metrics (fitness).

This leads to a solid and time tuned manner of solving complex and heavily multi-parameters problems and producing not only one solution, but a set of good solutions after enough iterations. This not brute force approach is usually much faster and computational lighter then searching all the possible solutions.

## Why make a parallel GA?

There are many reasons to make any application parallel, the first of then is usually performance. Most of the GA operators work individually to each individual inside the population, so, with certain caution, all the operators can be paralleled to make the overall execution time much smaller.

Another way to apply the parallel concept to GAs is to have multiple instances of sub-populations running in the same time. This isolates cases of premature convergence to local minimas or maximas, causing great accuracy gains depending on the problem and implementation. This method is known as Island Model.

## The example problem

A simple multi-modal function was choose to exemplify a GA construction, with the search space confined from x=-10.0000 to x=10.0000. 
![equation](http://www.sciweavers.org/tex2img.php?eq=y%3D%5Csin%28x%29%2B%5Ccos%28x%5Ccdot%5Csqrt%7B3%7D%29&bc=White&fc=Black&im=jpg&fs=12&ff=arev&edit=0)


## Implemented GAs
### GA - Sequential

This is a simple class-based GA implementation, focusing on easy implementation and parameters reconfiguration.

An object can be simple used as:

```cpp
GA ga (bitSize,popSize,limInf,limSup,precision,tourneySize, mutationRate,crossoverRate);
ga.run(generations);
```

The final result can be listed as:
```cpp
ga.mostraPopulacao();
```

### GAMS - Master-slave

This method focus on performance improvement, the first instance of execution (the master ) divides and distributes the workload to slave process, that complete theirs tasks and return the results to the master. This is best applied to multi-node servers, or to multi-threads machines.

When talking about Master-Slaves GAs, we usually keep the main execution sequential, but distributes the operators workload, mainly the fitness assessment, as it's tends to be the most computationally expensive operation, same times involving complex simulations.

This class can be used as:

```cpp
GAMS ga (bitSize,popSize,limInf,limSup,precision,tourneySize, mutationRate,crossoverRate, slavesNum);
ga.run(generations);
ga.mostraPopulacao();
```
### GAIM - Island Model

Different from a Master-slave, the Island model does not seek less execution time, but better results and less local minima/maxima.

This class can be used as:

```cpp
GAIM ga (islandNumber,bitSize,popSize,limInf,limSup,precision,tourneySize, mutationRate,crossoverRate,exchangePeriod);
ga.run(generations);
for(int i=0;i<islandNumber;i++)
        ga[i]->mostraPopulacao();
```

### GAFN - Fine Grained Model

This model differs from others by considering all individuals in a geometrical mesh, and running each one of then simultaneously, but restricted to mate only with their geometrical neighbors.

This class can be used as:

```cpp
GAFN ga (bitSize,precision ,limInf,limSup,tourneySize,mutationRate, crossoverRate, individualsNumber);
ga.run(geracoes);
ga.mostraPopulacao();
```


## More information
More information available on the [docs](https://parallelgadocs.netlify.app/).


## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

Please make sure to update tests as appropriate.

## License
[MIT](https://choosealicense.com/licenses/mit/)