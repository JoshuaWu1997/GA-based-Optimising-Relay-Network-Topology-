#define eps 1e-8
#define MAX 1100
#define POOL 3000
#define R 6378
#define Rural 1
#define Butterfly 2
#define SelectExpVal 1.5
#define SelectTension 4
#define k1 1
#define k2 0.075
#define FREQUENCY 900
#define TIMELIMIT 99999999
#define PI 3.14159265
#define GENERATION 100

#include <cmath>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <windows.h>

void Randomise();
double Randomise(int);
double URandom();
double distance(double,double,double,double);
double PropLoss(double,double F=FREQUENCY);

struct Individual{
	unsigned int num;
	bool BitString[MAX];
	double fitness;
	unsigned int linkage[MAX][2];
};
typedef struct node{
	node* pre=NULL;
	node* next=NULL;
	unsigned int index=0;
}Node;

class GaOperate {
public:
	Individual *indiv[POOL],*Best[POOL];
	double **station; double stationType[MAX],divers=1;
	unsigned int generation=0,statnum,population=0,link=0;
	unsigned int POPULATION;

	GaOperate();
	void SampleInput(int Sample[]);
	void PopulationCheck();
	void InitialSpecies();
	void CreateSpecies();

	void PrintIndividual(Individual*);
	void PrintGenerationResult();
	void Display();

	void Select();
	void SelectBest();
	void Crossover();
	void Mutation();
	double Diversity();

	void Evolve();
	Individual* Result();
	~GaOperate();
private:
	unsigned int BestNum=0,map[MAX][2]={0},MainNum=0;
	double MainCost,SubCost,SateCost;
	double fMax=0,fAve=0,f[POOL],pCommon=0;

	void AddMap();
	void TempCreate(int temp[],int);
	void TempCreate(int temp[]);

	int MaxAvail(unsigned int,Individual*);
	void Link(Individual*,int,int);
	void Link(Individual*,int);
	void CalculateCost(Individual*);
	void ConstrainFit(Individual*);

	double Probability(int,double);
	void ProbabilityDetermine();
	void Sort();
	void ChooseBest();

};
class BackRoute {
public:
	double **station,**loss,PL=0;
	double stationType[MAX];
	unsigned int statnum,nodesChange=0;
	Individual *Result; Node* node[MAX];
	BackRoute(Individual*,double**,int,double[]);
	bool ResetMap();
	void PrintSolution();
	~BackRoute();
private:
	int s[MAX];
	bool inlineSwitch(Node*,Node*);
	bool bylineSwitch(Node*,Node*);
	bool inlineif(Node*,Node*);
	bool linkable(Node*,Node*);
	bool append(Node*,Node*);
	bool appendable(Node*,int,int);
	int searchMain(Node*);
	void ReOrder();
	bool dis(Node*,Node*);
	double los(Node*,Node*);
	int length(Node*);
	int depth(Node*);
};




