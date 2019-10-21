#include "GA_BR.h"

void Randomise(){
	LARGE_INTEGER nFrequency;
	if(::QueryPerformanceFrequency(&nFrequency)){
		LARGE_INTEGER nStartCounter;
		::QueryPerformanceCounter(&nStartCounter);
		::srand((unsigned)nStartCounter.LowPart);
	}
}
double Randomise(int m){
	Randomise();
	return rand()%m+1;
}

double URandom(){
	double pp;
	pp=rand()%10000; pp/=10000;
	return pp;
}

double distance(double a1,double a2,double b1,double b2){
	a1=a1/360*2*PI;
	a2=a2/360*2*PI;
	b1=b1/360*2*PI;
	b2=b2/360*2*PI;
	return R*acos(cos(b1)*cos(b2)*cos(a1-a2)+sin(b1)*sin(b2));
}

double PropLoss(double D,double F){
	double t; t=32.5+20*log(D)/log(10)+20*log(F)/log(10);
	if (t>0) return t;
	else return 0;
}
