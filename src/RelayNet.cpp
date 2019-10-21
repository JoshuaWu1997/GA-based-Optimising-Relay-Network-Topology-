#include "GA_BR.h"
using namespace std;

int main() {
	DWORD start,stop;

	start=GetTickCount();
	GaOperate species;
	while(species.generation<=GENERATION&&species.divers>=0.1) species.Evolve();
	stop=GetTickCount();
	cout<<stop-start<<endl;
	cout<<"--------------------------------------------------";

/*	GaOperate* species;
	ofstream testOutput("testOutput.txt",ios::app);
	for(int i=370;i<=420;i+=10){
		for(int j=1;j<=50;j++){
			start=GetTickCount();
			species=new GaOperate(i);
			while(species.generation<=GENERATION&&species->divers>0.1) species->Evolve();
			stop=GetTickCount();
			testOutput<<i<<'\t'<<species->indiv[1]->fitness<<'\t'<<stop-start<<endl;
			delete species; species=NULL;
		}
	}
	testOutput.close();
*/

	Individual* result;
	BackRoute* bRoute[MAX];	double PLMax=1e8;
	for(unsigned int i=1;species.indiv[1]->fitness==species.indiv[i]->fitness;i++){
		result=species.indiv[i];
		bRoute[i]=new BackRoute(result,species.station,species.statnum,species.stationType);
		while(stop-start<=TIMELIMIT){
			if(!bRoute[i]->ResetMap()) break;
			stop=GetTickCount();
		}
		if(bRoute[i]->PL+eps<PLMax){
			bRoute[i]->PrintSolution();
			PLMax=bRoute[i]->PL;
		}
		cout<<"Current Propagation Loss:\t"<<bRoute[i]->PL<<endl<<"Nodes change:\t"<<bRoute[i]->nodesChange<<endl;
		cout<<"The "<<i<<"th Completed.\n";
		stop=GetTickCount();
		if(stop-start>TIMELIMIT) break;
		if(result->fitness<species.indiv[i+1]->fitness) break;
	}
	cout<<stop-start<<endl;

	system("PAUSE");
	return 0;
}

