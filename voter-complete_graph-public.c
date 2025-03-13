#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include <time.h>
#include </your_directory/mt19937-64.c>

int temp;
char filename[200];
char filename2[200];
FILE *fp;

int innov(int totn,int*sgn_pool,int*pool) {
	int i,j;
	int x=0;
	for(i=0;i<totn;i++) {
		if(sgn_pool[i]==0) {
			pool[x]=i;
			x++;
		}
	}
	j=(int)(x*genrand64_real2());
	return pool[j];
}

void Pool(int totn, int*sgn, int*sgn_pool,int *result) {
	int i;
	for(i=0;i<totn;i++) { sgn_pool[i]=0; }
	for(i=0;i<totn;i++) { sgn_pool[sgn[i]]++; }

	int max=0;
	int totop=0;
	for(i=0;i<totn;i++) { 
		if(sgn_pool[i]>max) { max=sgn_pool[i]; } 
		if(sgn_pool[i]>0) totop++; 
	}
	result[0]=max;
	result[1]=totop;
}
void PRT(int totn,int*result,int prt,char *filename) {
	if(prt==0) fp=fopen(filename,"w");
	else if(prt==1) fp=fopen(filename,"a");

	fprintf(fp,"%g %d\n",(double)result[0]/totn,result[1]);
	fclose(fp);
}

void PRTdistr(int totn,int*sgn_pool,char *filename2,int prt,int tt) {
	if(prt==0) fp=fopen(filename2,"w");
	else if(prt==1) fp=fopen(filename2,"a");
	fprintf(fp,"#ens_t %d\n",tt);

	int distr[totn+1];
	int i;
	for(i=0;i<=totn;i++) distr[i]=0;
	for(i=0;i<totn;i++) {
		if(sgn_pool[i]>0) distr[sgn_pool[i]]++;
	}

	for(i=0;i<=totn;i++) {
		if(distr[i]>0) fprintf(fp,"%d %d\n",i,distr[i]);
	}
	fclose(fp);
}

int main(int argc, char *argv[]) {  
	int i,ii,t,ni,k;

	int ln=atof(argv[1]);
	double la=atof(argv[2]);

	int totn=pow(2,ln);
	double Nalpha=pow(2,la);
	double alpha=Nalpha/(float)totn;

	time_t now;
	double seed=(time(&now)); 
	init_genrand64(seed); 

	int *sgn; sgn=(int*)malloc((size_t)(totn*sizeof(int))); 
	int *sgn_pool; sgn_pool=(int*)malloc((size_t)(totn*sizeof(int))); 
	int *pool; pool=(int*)malloc((size_t)((totn+1)*sizeof(int))); 

	int j;
	int *result; result=(int*)malloc((size_t)(2*sizeof(int))); 
	sprintf(filename,"./Com2/n2^%d-Nalpha2^%g-public",ln,la);


	//initialize//
	int times=100;
	t=2;
	for(i=0;i<totn;i++) { 
		if(genrand64_real2()<0.5) sgn[i]=0; 
		else sgn[i]=1;
	}

	Pool(totn,sgn,sgn_pool,result);
	PRT(totn,result,0,filename);
	//timeseries
	for(ii=0;ii<totn*times;ii++) {
		for(ni=0;ni<totn;ni++) {

			i=(int)(genrand64_real2()*totn);
			j=(int)(genrand64_real2()*totn);
			while(j==i) if(j==i) j=(int)(genrand64_real2()*totn);
			sgn[i]=sgn[j];	

			if(genrand64_real2()<alpha) {
				t=innov(totn,sgn_pool,pool);
				k=(int)(genrand64_real2()*totn);
				sgn[k]=t;
			}
		}

		Pool(totn,sgn,sgn_pool,result);
		PRT(totn,result,1,filename);

	}

	return 0;
}


