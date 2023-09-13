#include <mpi.h>
#include "globals.hh"
#include "main.h"
#include "GA_TSP.h"
#include "../Libs/random/random.h"
#include "../Libs/block/block.h"
using namespace std;

unsigned int GENERATION_MIGRATION;
unsigned int NUM_CITIES;
unsigned int POPULATION_SIZE;
unsigned int MAX_GENERATIONS;
double M_RATE;
double C_RATE;
double CONVENIENT_EXPONENT;
int seed[4];
int RANK;
int SIZE;
Random rnd;
int Nmigr = 50;

int main(int argc, char* argv[]) {

  Input();

  MPI_Init(&argc,&argv);
  MPI_Comm_size(MPI_COMM_WORLD, &SIZE);
  MPI_Comm_rank(MPI_COMM_WORLD, &RANK);

  StartGenerator();

  //cout<<"PROVA "<<pow(rnd.Rannyu(),CONVENIENT_EXPONENT)<<endl;
  vector<double> bestL;
  vector<City> cities(NUM_CITIES);
  // Inizializza le città con coordinate casuali

  ifstream inCities;
  inCities.open("American_capitals.dat");
  for (unsigned int i = 0; i < NUM_CITIES; ++i) {
      inCities >> cities[i].x;
      inCities >> cities[i].y;
      // cout<<cities[i].x<<" "<<cities[i].x<<endl;
  }
  inCities.close();

  vector<Tour> population = generateInitialPopulation(cities);
  //CheckPopulation(population);

  Tour bestTour = population[0];
  ofstream out_L;
  string str_out_L_ = "./out_L_N" + std::to_string(RANK) + ".dat";
  out_L.open(str_out_L_,ios::app);


  double tstart = MPI_Wtime();
  if(RANK == 0) cout<<"\nStarting of GA for TSP problem...\n"<<endl;
  for (unsigned int generation = 1; generation < MAX_GENERATIONS+1; ++generation) {

      if(generation % (int)(MAX_GENERATIONS/10) == 0 && RANK == 0) cout<<generation/(int)(MAX_GENERATIONS/10)*10<<"%"<<endl;

      vector<Tour> newPopulation(POPULATION_SIZE);

      // Crossover e mutazione per generare la nuova popolazione

      for (unsigned int i = 0; i < POPULATION_SIZE; i++) {
          sortPopulation(population);
          int p1 = ((int)(POPULATION_SIZE*pow(rnd.Rannyu(),CONVENIENT_EXPONENT)));
          int p2 = ((int)(POPULATION_SIZE*pow(rnd.Rannyu(),CONVENIENT_EXPONENT)));

          //fill the vector of the half lower L values and then I apply the average block for blocks of POPULATION_SIZE/2 size
          if (i < POPULATION_SIZE/2)  bestL.push_back(population[i].distance);

          //cout<<" fraction of generation: "<<i<< "of" << POPULATION_SIZE<<endl;
          if (rnd.Rannyu()<M_RATE){
            //cout<<"-----------start mutation 1-----------"<<endl;
            //PrintTour(population[p1]);
            swapTwoCities(population[p1].order);
            //CheckPopulation(population);

            //PrintTour(population[p1]);
            //cout<<"-----------end mutation-----------"<<endl;
            population[p1].distance = tourLength(population[p1], cities);
          }

          if (rnd.Rannyu()<M_RATE){
            //cout<<"-----------start mutation 2-----------"<<endl;
            //PrintTour(population[p1]);
            shiftContiguosCities(population[p1].order);
            //PrintTour(population[p1]);
            //CheckPopulation(population);
            population[p1].distance = tourLength(population[p1], cities);

            //cout<<"-----------end mutation-----------"<<endl<<endl;
          }


          if (rnd.Rannyu()<M_RATE){
            //cout<<"-----------start mutation 3-----------"<<endl;
            //PrintTour(population[p1]);
            permuteContiguousCities(population[p1].order);
            //PrintTour(population[p1]);
            //CheckPopulation(population);
            population[p1].distance = tourLength(population[p1], cities);

            //cout<<"-----------end mutation-----------"<<endl<<endl;
          }
          if (rnd.Rannyu()<M_RATE){
            //cout<<"-----------start mutation 4-----------"<<endl;
            //PrintTour(population[p1]);
            inversionContiguousCities(population[p1].order);
            //PrintTour(population[p1]);
            //CheckPopulation(population);
            population[p1].distance = tourLength(population[p1], cities);

            //cout<<"-----------end mutation-----------"<<endl<<endl;
          }
          //PrintTour(population[p1]);

          if (rnd.Rannyu()<C_RATE){
            //cout<<"-----------start Crossover-----------"<<endl;
            //PrintTour(population[p1]);
            //PrintTour(population[p2]);
            Crossover(population[p1].order, population[p2].order);
            //CheckPopulation(population);
            //PrintTour(population[p1]);
            //PrintTour(population[p2]);
            population[p1].distance = tourLength(population[p1], cities);
            population[p2].distance = tourLength(population[p2], cities);
            //cout<<"-----------end crossover-----------"<<endl<<endl;
          }

          CheckPopulation(population);
          for (unsigned int j = 0; j < POPULATION_SIZE; ++j) {
              bestTour = selectBestTour(bestTour, population[j]);
          }

          newPopulation[i].order = vector<int>(NUM_CITIES);
          newPopulation[i] = bestTour;
          //newPopulation[i].distance = bestTour.distance;
      }

      sortPopulation(newPopulation);

      if(generation % GENERATION_MIGRATION == 0){
        for(int nodo = 0; nodo < SIZE ; ++nodo){
          for (unsigned int i = 0; i < POPULATION_SIZE; ++i) {
              bestTour = selectBestTour(bestTour, newPopulation[i]);
          }
          MPI_Bcast(&bestTour.order.front(),NUM_CITIES,MPI_INTEGER,nodo, MPI_COMM_WORLD);
          newPopulation[POPULATION_SIZE-nodo-1]= bestTour;
          //cout<<newPopulation[POPULATION_SIZE-nodo-1].order.size()<<"  "<<bestTour.order.size()<<endl;
          //newPopulation[POPULATION_SIZE-nodo-1].distance = bestTour.distance;
        }
      }
      //cout<<population[0].order.size()<< "  "<<newPopulation[0].order.size()<<endl;
      population = newPopulation;

      for (unsigned int i = 0; i < POPULATION_SIZE; ++i) {
          bestTour = selectBestTour(bestTour, population[i]);
      }

      out_L<<generation<<setw(15)<<bestTour.distance<<endl;
  }

  out_L.close();

  vector<City> sortedCities(NUM_CITIES);

  sortedCities = sortCities(cities,bestTour.order);
  // Output del miglior tour e della sua lunghezza

  //cout << "Best Tour Order: ";
  ofstream out_cities;
  string str_out_order = "out_order_N" + std::to_string(RANK) + ".dat";
  out_cities.open(str_out_order,ios::app);

  for (unsigned int i = 0; i < NUM_CITIES; ++i) {
      //cout << bestTour.order[i] << " ";
      out_cities<<sortedCities[i].x<<setw(15)<<sortedCities[i].y<<endl;
  }
  out_cities.close();
  cout << endl;

  //cout << "Best Tour Length: " << bestTour.distance << endl;

  double dt = MPI_Wtime() - tstart;

  cout<<"------ NODO "<<RANK<<" ------"<<endl;
  cout<<"Execution time = "<<dt<<endl;
  cout << "Best Tour Length: " << bestTour.distance << endl;

  string out_L_Ave_N = "./out_L_Ave_N" + std::to_string(RANK) + ".dat";
  Block L_average(MAX_GENERATIONS,bestL);//100 blocchi
  L_average.Average();
  L_average.Average_cumulative(out_L_Ave_N);
  cout<<"---------------------------"<<endl;

  MPI_Finalize();
  return 0;
}

void StartGenerator(void){
  ifstream Primes, Seed;
  int p1, p2, p_dummy;
  Primes.open("Primes");
  for (int i = 0; i < RANK; i++) {
    Primes>> p_dummy >> p_dummy;
  }
  Primes >> p1 >> p2 ;
  Primes.close();

  Seed.open("seed.in");

  Seed >> seed[0] >> seed[1] >> seed[2] >> seed[3];
  rnd.SetRandom(seed,p1,p2);
  Seed.close();
}
void Input(void)
{
  ifstream ReadInput;
  //Read input informations
  ReadInput.open("input.in");

  ReadInput >> NUM_CITIES;

  ReadInput >> POPULATION_SIZE;

  ReadInput >> MAX_GENERATIONS;

  ReadInput >> M_RATE;

  ReadInput >> C_RATE;

  ReadInput >> CONVENIENT_EXPONENT;

  ReadInput >> GENERATION_MIGRATION;

  ReadInput.close();
}
