#include "GA_BR.h"
using namespace std;

GaOperate::GaOperate() {
	cout<<"Input POPULATION.\n";
	cin>>POPULATION;
	AddMap();
//	POPULATION=p;
	for(unsigned int j=1;j<=POPULATION;j++){
		int temp[MAX]={0};
		TempCreate(temp);
		SampleInput(temp);
	}
	PopulationCheck();
	cout<<"Initiation Completes with "<<POPULATION<<" individuals!\n";
}
void GaOperate::AddMap(){
	unsigned int i,j;
	ifstream map("map.txt");
	cout<<"File loaded successfully!\n";
	map>>statnum;
	station=new double*[statnum+1];
	for(i=0;i<=statnum;i++)	station[i]=new double[statnum+1];
	for(i=1;i<=statnum;i++) map>>station[0][i];
	for(i=1;i<=statnum;i++)	map>>station[i][0];
	for(i=1;i<=statnum;i++) map>>stationType[i];
	for(i=1;i<=statnum;i++)
		for(j=1;j<=statnum;j++)
			station[i][j]=distance(station[0][i],station[0][j],station[i][0],station[j][0]);

	map>>MainCost>>SubCost>>SateCost;
	map.close();
	cout<<"Map added successfully!\n";
}
GaOperate::~GaOperate(){
	for(unsigned int i=1;i<=population;i++) delete indiv[i];
	for(unsigned int i=0;i<=statnum;i++) delete []station[i];
	delete []station;
}
void GaOperate::InitialSpecies(){
	unsigned int j,up; up=statnum/8;
	for(j=1;j<=POPULATION;j++){
		int temp[MAX]={0};
		TempCreate(temp,up);
		SampleInput(temp);
		PrintIndividual(indiv[population]);
		if(up>indiv[population]->num)
			up=indiv[population]->num;
		cout<<"\r"<<j*100/POPULATION<<"%";
	}
	PopulationCheck();
	cout<<"\nInitiation Completes with "<<POPULATION<<" individuals!\n";
}
void GaOperate::CreateSpecies(){
	for(unsigned int j=1;j<=POPULATION;j++){
		int temp[MAX]={0};
		TempCreate(temp);
		SampleInput(temp);
	}
	PopulationCheck();
	ProbabilityDetermine();
	cout<<"Initiation Completes with "<<POPULATION<<" individuals!\n";
}
void GaOperate::SampleInput(int sample[]){
	population++;
	indiv[population]=new Individual;
	for (unsigned int i=1;i<=statnum;i++){
		indiv[population]->BitString[i]=sample[i];
		if(sample[i])indiv[population]->num++;
	}
}
void GaOperate::PopulationCheck(){
	for(unsigned int i=1;i<=population;i++){
		indiv[i]->num=0;
		for (unsigned int j=1;j<=statnum;j++){
			indiv[i]->linkage[j][1]=0;
			if(indiv[i]->BitString[j]){
				indiv[i]->linkage[j][0]=j;
				indiv[i]->num++;
			}
			else indiv[i]->linkage[j][0]=0;
		}
		ConstrainFit(indiv[i]);
		CalculateCost(indiv[i]);
		cout<<"\r"<<i*100/population<<"%";
	}
	ProbabilityDetermine();
	PrintGenerationResult();
	cout<<"\nEvoluation Completes with "<<population<<" individuals!\n";
}
void GaOperate::PrintIndividual(Individual *s){
	stringstream route;
	route<<s->fitness<<"_"<<s->num<<".txt";
	ofstream out(route.str(),ios::app);
	for(unsigned int i=1;i<=statnum;i++)
		if(s->BitString[i]) out<<i<<'\t';
		else out<<s->linkage[i][1]<<'\t';
	out<<'\n';
	out.close();
}
void GaOperate::PrintGenerationResult(){
	ofstream eM("CostMin.txt",ios::app);
	eM<<indiv[1]->fitness<<"\t"; eM.close();
	ofstream eA("CostAve.txt",ios::app);
	eA<<indiv[population]->fitness-fAve<<"\t"; eA.close();
	ofstream divergent("divergent.txt",ios::app);
	divergent<<divers<<"\t"; divergent.close();
	cout<<"The next generation will begin to evolve.\n";
	cout<<"Current diversity: "<<Diversity()<<endl;
}
void GaOperate::Display(){
	stringstream route;
	route<<"Generation"<<generation<<".txt";
	ofstream out(route.str(),ios::app);
	for(unsigned int i=1;i<=population;i++) out<<indiv[i]->fitness<<endl;
	out<<endl<<endl;
	out.close();
	cout<<Diversity()<<endl;
}
//---------------------------------------------------------------------
void GaOperate::CalculateCost(Individual*s){
	s->fitness=s->num*MainCost+(statnum-s->num)*SubCost;
	for (unsigned int i=1;i<=statnum;i++)
		if (s->BitString[i]&&s->linkage[i][1]==0){
			s->linkage[i][1]=i;
			Link(s,i);
			s->fitness+=ceil(MainNum/8)*SateCost;
			MainNum=0;
		}
}
int GaOperate::MaxAvail(unsigned int pos,Individual *s){
	unsigned int i,p; double d=0;
	for (i=1;i<=statnum;i++)
		if (!s->BitString[i]&&s->linkage[i][1]==0&&station[pos][i]<=20&&i!=pos){
			if (station[pos][i]>d&&s->BitString[pos]){
				d=station[pos][i];
				p=i;
			}
			else
				if(station[pos][i]<=10&&station[i][s->linkage[pos][0]]>d){
					d=station[i][s->linkage[pos][0]];
					p=i;
				}
		}
	return d==0?0:p;
}
void GaOperate::Link(Individual *s,int entrance,int depth){
	int p;
	if(depth<=2&&link<6){
		p=MaxAvail(entrance,s);
		if(p){
			s->linkage[p][0]=s->linkage[entrance][0];
			s->linkage[p][1]=entrance;
			link++;
			Link(s,p,depth+1);
		}
	}
}
void GaOperate::Link(Individual *s,int entrance){
	for(unsigned int i=1;i<=statnum;i++)
		if(s->BitString[i]&&s->linkage[i][1]==0&&station[entrance][i]<=50){
			s->linkage[i][1]=entrance;
			MainNum++;
			Link(s,i);
		}
}
void GaOperate::ConstrainFit(Individual *s){
	unsigned int i,j,k,l; int p[MAX];
	for(i=1;i<=statnum;i++) p[i]=i;
	for(i=statnum;i>=2;i--){
		int l;
		Randomise(); l=rand()%(i-1)+1;
		swap(p[l],p[i]);
	}

	for (i=1;i<=statnum;i++)
		if (s->BitString[p[i]]==1)
			for(k=1;k<=stationType[i];k++){
				link=0;
				for (j=1;j<=4;j++)
					if(link<6)Link(s,p[i],0);
			}
	for (k=1;k<=statnum;k++){
		i=p[k];
		if (s->linkage[i][0]==0){
			s->BitString[i]=1;
			s->num++;
			s->linkage[i][0]=i;
			for(j=1;j<=stationType[i];j++){
				link=0;
				for (l=1;l<=4;l++)
					if(link<6)Link(s,i,0);
			}
		}
	}
}
//--------------------------------------------------------------
void GaOperate::Select(){
	unsigned int i,j;
	double p[POOL],t=0;
	double a,b;
	a=(SelectExpVal-1)/(fMax-fAve)*fAve;
	b=(fMax-SelectExpVal*fAve)/(fMax-fAve)*fAve;
	for(i=1;i<=population;i++){
		p[i]=a*f[i]/fAve/population+b/population/fAve;
		p[i]=exp(p[i]*pow(10,SelectTension)*generation/GENERATION);
		t+=p[i];
	}
	for(i=1;i<=population;i++) p[i]/=t;
	Individual *indivNext[POOL]; unsigned int populationNext=0;
	for(i=1;i<=population;i++){
		if(populationNext<POPULATION/10+POPULATION)
			for(j=1;j<=round(POPULATION*p[i]);j++){
				populationNext++;
				indivNext[populationNext]=new Individual;
				*indivNext[populationNext]=*indiv[i];
			}
		delete indiv[i];
	}

	population=populationNext;
	for(i=1;i<=population;i++) indiv[i]=indivNext[i];
	ProbabilityDetermine();
	ChooseBest();
	cout<<"Selection pressure: "<<'\t'<<t<<"\n";
	cout<<"The "<<generation<<"th "<<"generation begins to evolve.\n";
}
void GaOperate::Crossover(){
	int l,r,t=0; double p,pc,s=0; double a,b=0;
	for(unsigned int i=1;i<=population/2;i++){
		l=Randomise(population); r=Randomise(population);
		pc=Probability(l,k1)<Probability(r,k1)?Probability(r,k1):Probability(l,k1);
		t++;
		for(unsigned int j=1;j<=statnum;j++){
			p=URandom();
			if(p<=pc){
				a=indiv[l]->BitString[j]&indiv[r]->BitString[j];
				b=indiv[l]->BitString[j]|indiv[r]->BitString[j];
				if(a!=b) s++;
				indiv[l]->BitString[j]=a;
				indiv[r]->BitString[j]=b;
			}
		}
	}
	s/=t;
	cout<<"Crossover p: "<<'\t'<<s<<"\n";
}
void GaOperate::Mutation(){
	double p,s=0; unsigned int i,j,tt=0;
	for(i=1;i<=population;i++){
		tt++;
		for(j=1;j<=statnum;j++){
			p=URandom();
			if(p<=divers*Probability(i,k2)){
				indiv[i]->BitString[j]=!indiv[i]->BitString[j];
				s++;
			}
		}
	}
	s/=tt;
	cout<<"Mutation p: "<<'\t'<<s<<"\n";
}
double GaOperate::Diversity(){
	int a0,a1; double sum=0;
	for(unsigned int i=1;i<=statnum;i++){
		a0=a1=0;
		for(unsigned int j=1;j<=population;j++){
			if(indiv[j]->BitString[i]) a1++;
			else a0++;
		}
		sum+=abs(a0-a1);
	}
	sum=1-sum/statnum/population; divers=sum;
	return sum;
}
void GaOperate::ChooseBest(){
	BestNum=population/10;
	for(unsigned int i=1;i<=BestNum;i++){
		Best[i]=new Individual;
		*Best[i]=*indiv[i];
	}
	cout<<"Best/worst:"<<Best[1]->fitness<<" "<<Best[BestNum]->fitness<<endl;
}
void GaOperate::SelectBest(){
	if(generation!=1)
		for(unsigned int i=1;i<=BestNum;i++){
			if(indiv[1]->fitness>Best[1]->fitness+eps){
				Randomise();
				int p=rand()%population+1;
				*indiv[p]=*Best[i];
			}
			delete Best[i];
		}
	ProbabilityDetermine();
}
void GaOperate::Evolve(){
	cout<<"-------------------------------------\n";
	generation++;
	Select();
	Crossover();
	Mutation();
	PopulationCheck();
	SelectBest();
	cout<<"The "<<generation<<"th Evoluation Completed.\n";
}
Individual* GaOperate::Result(){
	Individual* p=indiv[1];
	for(int i=1;indiv[i]->fitness<=indiv[1]->fitness;i++)
		if(p->num<=indiv[i]->num) p=indiv[i];
	return p;
}

