#include "globals.hh"
#include "GA_TSP.h"

using namespace std;

// Function to calculate the Euclidean distance between two cities
double distance(const City& city1, const City& city2) {
    return sqrt(pow(city1.x - city2.x, 2) + pow(city1.y - city2.y, 2));

}

void shuffle(vector<int>& tour){
  vector<int> v_old = tour;
  for (long unsigned int i{1}; i<NUM_CITIES; i++){
      int id1 = i;
      int id2 = rnd.Rannyu(1,NUM_CITIES);

      int save_el = tour[id1];
      tour[id1] = tour[id2];
      tour[id2] = save_el;
  }
  return;
}

// Funzione per calcolare la lunghezza del percorso (tour)
double tourLength(const Tour& tour, const vector<City>& cities) {
    double totalDistance = 0.0;
    for (unsigned int i = 0; i < NUM_CITIES; ++i) {
        totalDistance += distance(cities[tour.order[i]], cities[tour.order[i + 1]]);
    }
    totalDistance += distance(cities[tour.order[NUM_CITIES]], cities[tour.order[0]]);
    return totalDistance;
}

double L2function(const Tour& tour, const vector<City>& cities) {
    double totalDistance = 0.0;
    for (unsigned int i = 0; i < NUM_CITIES; ++i) {
        totalDistance += pow(distance(cities[tour.order[i]], cities[tour.order[i + 1]]),2);
    }
    totalDistance += pow(distance(cities[tour.order[NUM_CITIES]], cities[tour.order[0]]),2);
    return totalDistance;
}

// Funzione per generare una popolazione iniziale di tour casuali
vector<Tour> generateInitialPopulation(const vector<City>& cities) {
    vector<Tour> population(POPULATION_SIZE);

    for (unsigned int i = 0; i < POPULATION_SIZE; ++i) {
        population[i].order = vector<int>(NUM_CITIES);
        for (int j = 0; j < NUM_CITIES; ++j) {
            population[i].order[j] = j;
        }
        shuffle(population[i].order);
        population[i].distance = tourLength(population[i], cities);
    }
    return population;
}

bool CheckPopulation(vector<Tour> pop){
  // Ciclo per confrontare ogni elemento con gli altri

  for (int k = 0;k<POPULATION_SIZE;++k){
    for (unsigned int i = 0; i < NUM_CITIES - 1; ++i) {
        for (int j = i + 1; j < NUM_CITIES; ++j) {
          if (pop[k].order[i] == pop[k].order[j]){
            cout<<"Problem: Trovato un elemento duplicato in un tour della popolazione"<<endl;
            for (int l{};l<NUM_CITIES;l++)
              cout<< pop[k].order[l]<<" ";
            cout<<endl;
            return 1;
          }
        }
    }
  }
  return 0; // Nessun elemento duplicato trovato
}

//inverte due interi
void swap(int& id1, int& id2){

  int save_element = id1;

  id1 = id2;
  id2 = save_element;

  return;
}

void SwapTours(vector<Tour>& population, int in, int fi){

  Tour save_element = population[in];

  population[in] = population[fi];
  population[fi] = save_element;

  return;
}
//genetics mutation operators
void swapTwoCities(vector<int>& tour){

  int id1 = (int)rnd.Rannyu(1,NUM_CITIES);
  int id2 = (int)rnd.Rannyu(1,NUM_CITIES);
  int save_element = tour[id1];

  tour[id1] = tour[id2];
  tour[id2] = save_element;

  return;
}


//shift n contiguos cities of m steps from the second city
void shiftContiguosCities(vector<int>& tour)

{
    int n = rnd.Rannyu(0., NUM_CITIES - 1);
    int m = rnd.Rannyu(0., NUM_CITIES - 2);
    int a = rnd.Rannyu(0., NUM_CITIES - 2 - m);

    vector<int> vcopy = tour;

    vector<int> lcopy(NUM_CITIES);

    for (size_t i = 0; i < NUM_CITIES; ++i)
        lcopy[i] = i;

    lcopy.erase(lcopy.begin()); // remove the first one because it has to stay fixed

    vector<int> index(m); // create a vector of indexes that are to be relocated
    for (size_t i{}; i < index.size(); ++i)
        index[i] = (a + i + n) % (NUM_CITIES - 1);

    vector<int> copiator(m); // Vector on which temporarily copy the data
    for (size_t i{}; i < copiator.size(); ++i)
        copiator[i] = lcopy[a + i];

    for (unsigned int i{}; i < m; ++i) // eliminate the elements ranging from a to a+m
        lcopy.erase(lcopy.begin() + a);

    while (lcopy.size() < NUM_CITIES - 1)
    { // insert the deleted elements in the correct position
        for (unsigned int i{}; i < m; ++i)
        {
            if ((size_t)index[i] <= lcopy.size() && lcopy.size() < NUM_CITIES - 1)
                lcopy.emplace(lcopy.begin() + index[i], copiator[i]);
        }
    }

    // cout<<"checkpoint"<<endl;

    for (unsigned int i{1}; i < NUM_CITIES; ++i)
    {
        tour[i] = vcopy[lcopy[i - 1]];
        // cout<<i<<endl;
    }

}

void permuteContiguousCities(vector<int>& tour){

  // cout<<"Mutation 3"<<endl;
  double med = (double)NUM_CITIES / 2.;

  int m = rnd.Rannyu(1., med - 1);
  vector<int> vcopy = tour;

  int a = rnd.Rannyu(1., med - m);      //  generate the starting position of the m elements
  int b = rnd.Rannyu(med, 2 * med - m); // generate the starting position of the other m elements

  for (unsigned int i = a, j = b; i < a + m; ++i, ++j)
  {
      tour[j] = vcopy[i];
      tour[i] = vcopy[j];
  }

  return;
}

void inversionContiguousCities(vector<int>& tour)
{
    int m = rnd.Rannyu(1., NUM_CITIES - 2);
    vector<int> vcopy = tour;

    int a = rnd.Rannyu(1., NUM_CITIES - m);

    for (unsigned int i{a}, j{a + m - 1}; i < a + m; ++i, --j)
    {
        tour[i] = vcopy[j];
    }
}
bool isInVector(vector<int> v, double element){

  for (long unsigned int i = 0; i < v.size(); i++){
    if (v[i]==element){
      return 1;
    }
  }
  //cout<<"elemento NON trovato"<<endl;
  return 0;

}
void Crossover(vector<int>& parent1,vector<int>& parent2)
{
    int cutter = rnd.Rannyu(NUM_CITIES/2, NUM_CITIES-1);
    //cout<<cutter<<" "<<NUM_CITIES<<endl;
    vector<int> copy_p2 = parent2;
    vector<int> copy_p1 = parent1;
    vector<int> copy_p1_cut = copy_p1;
    vector<int> copy_p2_cut = copy_p2;

    copy_p1_cut.resize(cutter);
    copy_p2_cut.resize(cutter);

    unsigned int count{};
    for (unsigned int i{1}; i < NUM_CITIES; i++) {
      if(isInVector(copy_p2_cut,copy_p1[i])==0){
        parent2[cutter+count] = copy_p1[i];
        count++;
      }
    }

    count = 0;
    for (unsigned int i{1}; i < NUM_CITIES; i++) {
      if(isInVector(copy_p1_cut,copy_p2[i]) ==0){
        parent1[cutter+count] = copy_p2[i];
        count++;
      }
    }

    return;
}
// Funzione per selezionare il migliore tra due tour
Tour selectBestTour(const Tour tour1, const Tour tour2) {
    //cout<<endl<<tour1.distance<<" "<< tour2.distance<<endl;
    return (tour1.distance < tour2.distance) ? tour1 : tour2;
}

//return the cities ordered following the best tour
vector<City> sortCities(vector<City> unsortedCities,vector<int> sortingVec){
   vector<City> newCities(NUM_CITIES);
   for( unsigned int i{}; i< NUM_CITIES;i++){
       newCities[i].x = unsortedCities[sortingVec[i]].x;
       newCities[i].y = unsortedCities[sortingVec[i]].y;
     }
   return newCities;
}

void sortPopulation(vector<Tour>& population){

  unsigned int id{};
  for( int j{}; j< POPULATION_SIZE;j++){
    Tour bestTour = population[j];
    for( unsigned int i{j}; i< POPULATION_SIZE;i++){
      if (bestTour.distance > population[i].distance){
        bestTour = population[i];
        id = i;
      }
    }
    SwapTours(population, j, id);
  }

  return;
}

void PrintPop(vector<Tour> population){
    cout<<"Distances:"<<endl;
    for( int j{}; j< POPULATION_SIZE;j++) cout<<population[j].distance<<" ";
    cout<<endl<<endl;
}

void PrintTour(Tour tour){
    cout<<"Tour:"<<endl;
    for( int j{}; j< NUM_CITIES;j++) cout<<tour.order[j]<<" ";
    cout<<endl<<endl;
}
