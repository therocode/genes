#include <dna/genecodec.hpp>
#include <dna/breeder.hpp>
#include <dna/cistron.hpp>
#include <dna/chromosome.hpp>
#include <iostream>

int main()
{
    dna::Breeder breeder;

    dna::Cistron mumColorCistron(0, 5, 4, {{255, 255}, {255, 255}, {0, 255}});
    dna::Cistron mumMovementCistron(1, 5, 4, {{20, 50}, {10, 50}});
    dna::Chromosome mumChromosome(5, 4, "AATAAT", "TTATTA", {mumColorCistron, mumMovementCistron}, 100, 200);

    dna::Cistron dadColorCistron(0, 5, 4, {{0, 255}, {255, 255}, {255, 255}});
    dna::Cistron dadMovementCistron(1, 5, 4, {{45, 50}, {20, 50}});
    dna::Chromosome dadChromosome = mumChromosome;
    dadChromosome.replaceCistronValues({dadColorCistron, dadMovementCistron});
    std::cout << "--mum--\n";
    std::cout << "colour: " << 0 << ", " << 100 << ", " << 200 << "\n";
    std::cout << "jump: " << 20 << "\n";
    std::cout << "run: " << 10 << "\n";
    std::cout << "dna: " << mumChromosome.gene() << "\n\n";
    std::cout << "--dad--\n";
    std::cout << "colour: " << 250 << ", " << 50 << ", " << 150 << "\n";
    std::cout << "jump: " << 45 << "\n";
    std::cout << "run: " << 20 << "\n";
    std::cout << "dna: " << dadChromosome.gene() << "\n\n";

    uint32_t crossedKid = 0;
    for(int32_t i = 0; i < 10000; i++)
    {
        auto breedResult = breeder.breed(mumChromosome, dadChromosome, 500);
        if(breedResult.second.crossOvers > 0)
        {
            std::cout << "this kid was affected by " << breedResult.second.crossOvers << " cross overs. Its genes and properties are:\n";
            dna::Chromosome kidChromosome = std::move(breedResult.first);
            std::cout << "--kid " << i << "--\n";
            uint32_t r, g, b = 0;
            uint32_t jump, run = 0;

            for(auto cistronIter : kidChromosome.cistrons())
            {
                const dna::Cistron& cistron = cistronIter.second;

                if(cistron.id() == 0)
                {
                    cistron.decode({{r, 255}, {g, 255}, {b, 255}});
                    std::cout << "colour: " << r << ", " << g << ", " << b << "\n";
                }
                else if(cistron.id() == 1)
                {
                    cistron.decode({{jump, 50}, {run, 50}});
                    std::cout << "jump: " << jump << "\n";
                    std::cout << "run: " << run << "\n";
                }

            }
            crossedKid++;
            std::cout << "dna: " << kidChromosome.gene() << "\n\n";
        }
    }

    std::cout << "Out of a total of " << 10000 << " breedings, " << crossedKid << " got affected by crossover. Their dna length is " << mumChromosome.gene().length() << "\n";
}
