#include "GA_BR.h"
using namespace std;

double GaOperate::Probability(int i,double k){
	if (f[i]>=fAve) return k*(fMax-f[i])/(fMax-fAve);
	else return k;
}
void GaOperate::ProbabilityDetermine(){
	Sort();
	for(unsigned int i=1;i<=population;i++){
		f[i]=indiv[population]->fitness-indiv[i]->fitness;
		fAve+=f[i];
	}
	fAve/=population; fMax=indiv[population]->fitness;
	pCommon=(fMax-fAve)/fMax;
}
void GaOperate::TempCreate(int temp[],int up){
	int r,p,i; i=1;
	Randomise();
	r=rand()%up+1;
	while (i<=r){
		Randomise();
		p=rand()%statnum+1;
		if(temp[p]==0){
			i++;
			temp[p]=1;
		}
	}
}
void GaOperate::TempCreate(int temp[]){
	for(unsigned int i=1;i<=statnum;i++){
		Randomise();
		temp[i]=rand()%2;
	}
}
void GaOperate::Sort(){
	for(unsigned int i=1;i<=population-1;i++)
		for(unsigned int j=i+1;j<=population;j++)
			if((indiv[i]->fitness)>(indiv[j]->fitness))
				swap(indiv[i],indiv[j]);
}
void BackRoute::ReOrder(){
	for(int i=statnum;i>=2;i--){
		int l;
		Randomise(); l=rand()%(i-1)+1;
		swap(s[l],s[i]);
	}
}
