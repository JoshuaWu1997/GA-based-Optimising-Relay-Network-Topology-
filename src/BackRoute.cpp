#include "GA_BR.h"
using namespace std;

BackRoute::BackRoute(Individual *result,double**ss,int sn,double c[]){
	Result=result;
	statnum=sn;
	station=ss;
	loss=new double*[statnum+1];
	for(unsigned int i=1;i<=statnum;i++) s[i]=i;
	for(unsigned int i=1;i<=statnum;i++){
		loss[i]=new double[statnum+1];
		node[i]=new Node;
		for(unsigned int j=1;j<=statnum;j++)
			loss[i][j]=PropLoss(station[i][j]);
		stationType[i]=c[i];
	}
	for(unsigned int i=1;i<=statnum;i++){
		node[i]->index=i;
		node[i]->pre=node[Result->linkage[i][1]];
		if(Result->BitString[i]){
			Result->linkage[i][0]=0;
			Result->linkage[i][1]=0;
		}
		else{
			PL+=los(node[i],node[i]->pre);
			node[i]->pre->next=node[i];
		}
	}
	for(unsigned int i=1;i<=statnum;i++)
		if(!Result->BitString[i]&&Result->BitString[node[i]->pre->index]){
			Result->linkage[node[i]->pre->index][0]++;
			Result->linkage[node[i]->pre->index][1]+=length(node[i]);
		}
	PrintSolution();
	cout<<"Nodes created.\n";
}
BackRoute::~BackRoute(){
	for(unsigned int i=0;i<=statnum;i++){
		delete []station[i];
		delete []loss[i];
		delete node[i];
	}
	delete []station; delete []loss; delete Result; delete station;
}
bool BackRoute::ResetMap(){
	bool p=false;
	ReOrder();
	for(unsigned int i=1;i<=statnum;i++)if(!Result->BitString[s[i]])
		for(unsigned int j=1;j<=statnum;j++)if(i!=j){
			if(!Result->BitString[s[j]]){
				if(inlineif(node[s[i]],node[s[j]]))
					p=inlineSwitch(node[s[i]],node[s[j]])||p;
				else if(inlineif(node[s[j]],node[s[i]]))
					p=inlineSwitch(node[s[j]],node[s[i]])||p;
				else
					p=bylineSwitch(node[s[i]],node[s[j]])||p;
			}
			p=append(node[s[i]],node[s[j]])||p;
		}
	return p;
}

bool BackRoute::inlineSwitch(Node*l,Node*r){
	if(r->next){
		if(dis(l->pre,r)&&dis(l,r->next)){
			double aloss,bloss;
			aloss=los(l,l->pre)+los(r,r->next);
			bloss=los(l->pre,r)+los(l,r->next);
			if(aloss>bloss+eps){
				PL=PL-los(l->pre,l)-los(r->pre,r);
				l->pre->next=r; r->pre=l->pre;
				r->next->pre=l; l->next=r->next;
				l->pre=r; r->next=l;
				PL=PL+los(l->pre,l)+los(r->pre,r);
				nodesChange++;
				return true;
			}
		}
	}
	else{
		if(dis(l->pre,r)&&los(l,l->pre)>los(r,l->pre)+eps){
			l->pre->next=r; r->pre=l->pre;
			PL=PL-los(l->pre,l)-los(r->pre,r);
			if(l->next==r){
				l->pre=r; r->next=l; l->next=NULL;
			}
			else{
				r->next=l->next; l->next->pre=r;
				l->next->next=l; l->pre=l->next; l->next=NULL;
			}
			PL=PL+los(l->pre,l)+los(r->pre,r);
			nodesChange++;
			return true;
		}
	}
	return false;
}
bool BackRoute::bylineSwitch(Node*l,Node*r){
	if(linkable(l,r)){
		PL=PL-los(l->pre,l)-los(r->pre,r);
		Result->linkage[searchMain(l)][1]-=(length(l)-length(r));
		Result->linkage[searchMain(r)][1]-=(length(r)-length(l));
		l->pre->next=r; r->pre->next=l;
		swap(l->pre,r->pre);
		PL=PL+los(l->pre,l)+los(r->pre,r);
		nodesChange++;
		return true;
	}
	return false;
}
bool BackRoute::inlineif(Node*l,Node*r){
	if(!l->next) return false;
	else{
		if(l->next==r) return true;
		else return inlineif(l->next,r);
	}
}
bool BackRoute::appendable(Node*l,int r,int n){
	int line=Result->linkage[r][0];
	int nodes=Result->linkage[r][1];
	bool c1,c2;
	c1=stationType[r]==1&&nodes+length(l)<=6;
	c2=stationType[r]==2&&nodes+length(l)<=12;
	if(n==1){
		c1=c1&&line+1<=4;
		c2=c2&&line+1<=8;
	}
	return (c1||c2);
}
bool BackRoute::append(Node*l,Node*r){
	if(los(l,r)+eps<los(l,l->pre)&&dis(l,r)){
		int lmain=searchMain(l);int rmain=searchMain(r);bool c1,c2;
		c1=Result->BitString[r->index]&&appendable(l,r->index,1);
		c2=!Result->BitString[r->index]&&appendable(l,rmain,2);
		c2=c2&&r->next==NULL&&depth(r)+length(l)<=3;
		int rcommon=c1?r->index:rmain;
		if(c1) Result->linkage[rcommon][0]+=1;
		if(c1||c2){
			Result->linkage[rcommon][1]+=length(l);
			if(depth(l)==1) Result->linkage[lmain][0]--;
			Result->linkage[lmain][1]-=length(l);
			PL-=los(l,l->pre);
			l->pre->next=NULL; l->pre=r; r->next=l;
			PL+=los(l,l->pre);
			return true;
		}
	}
	return false;
}
void BackRoute::PrintSolution(){
	stringstream route;
	route<<"RESULT_"<<Result->fitness<<"_"<<PL<<".txt";
	ofstream out(route.str(),ios::app);
	for(unsigned int i=1;i<=statnum;i++)
		if(Result->BitString[i]) out<<i<<'\t';
		else{
			if(node[i]->index) out<<node[i]->pre->index<<'\t';
			else out<<Result->linkage[i][1]<<'\t';
		}
	out<<'\n';
	out.close();
}
double BackRoute::los(Node*a,Node*b){
	return loss[a->index][b->index];
}
bool BackRoute::dis(Node* a,Node* b){
	if(Result->BitString[a->index]){
		if(station[a->index][b->index]<=20) return true;
		else return false;
	}
	else{
		if(station[a->index][b->index]<=10) return true;
		else return false;
	}
}
int BackRoute::length(Node*s){
	if(s->next==NULL) return 1;
	else return length(s->next)+1;
}
int BackRoute::depth(Node*s){
	if(Result->BitString[s->index]) return 0;
	else return depth(s->pre)+1;
}
bool BackRoute::linkable(Node*l,Node*r){
	int lmain,rmain; bool c=true;
	lmain=searchMain(l); rmain=searchMain(r);
	c=c&&length(l)+depth(r)<=4&&length(r)+depth(l)<=4;
	c=c&&dis(l->pre,r)&&dis(r->pre,l);
	c=c&&los(l->pre,r)+los(r->pre,l)+eps<los(l->pre,l)+los(r->pre,r);
	if(stationType[lmain]==1)
		c=c&&Result->linkage[lmain][1]-length(l)+length(r)<=6;
	else
		c=c&&Result->linkage[lmain][1]-length(l)+length(r)<=12;
	if(stationType[rmain]==1)
		c=c&&Result->linkage[rmain][1]-length(r)+length(l)<=6;
	else
		c=c&&Result->linkage[rmain][1]-length(r)+length(l)<=12;
	return c;
}
int BackRoute::searchMain(Node* entrance){
	if(Result->BitString[entrance->index])
		return entrance->index;
	else return searchMain(entrance->pre);

}
