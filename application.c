#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <unistd.h>

int main(void){

    FILE *fp;
	int i = 0;
	char *str;
	size_t broj_bajtova = 36;

//***************************UPISUJEMO U /dev/bram_a***************************

	fp = fopen("/dev/bram_a", "w");
	
	if(fp==NULL){
		printf("Nije moguce otvoriti /dev/bram_a.\n");
		return -1;	
	}

	for( i=0; i<1152; i++ ){
		fprintf(fp, "[%d] = 2",i);
		fflush(fp);
	}

	if(fclose(fp)){	
		printf("Problem pri zatvaranju /dev/bram_a.\n");
		return -1;	
	}

//***************************UPISUJEMO U /dev/bram_b***************************
/*
	fp = fopen("/dev/bram_b", "w");
	
	if(fp==NULL){
		printf("Nije moguce otvoriti /dev/bram_b.\n");
		return -1;	
	}

	for( i=0; i<1152; i++ ){
		fprintf(fp, "[%d] = 2",i);
		fflush(fp);
	}

	if(fclose(fp)){	
		printf("Problem pri zatvaranju /dev/bram_b.\n");
		return -1;	
	}
*/
//*************************************************************************************************************

fp = fopen("/dev/IMDCT", "w");
	
	if(fp==NULL){
		printf("Nije moguce otvoriti /dev/IMDCT.\n");
		return -1;
	}

	sleep(0.5);
	
	fputs("block_type_00 = 0",fp);
	
	if(fclose(fp)){
		printf("Problem pri zatvaranju /dev/IMDCT.\n");
		return -1;	
    }

fp = fopen("/dev/IMDCT", "w");
	
	if(fp==NULL){
		
		printf("Nije moguce otvoriti /dev/IMDCT.\n");
		return -1;
		
	}

	sleep(0.5);
	
	fputs("block_type_01 = 0",fp);
	
	if(fclose(fp)){
		
		printf("Problem pri zatvaranju /dev/IMDCT.\n");
		return -1;	
    }

fp = fopen("/dev/IMDCT", "w");
	
	if(fp==NULL){
		printf("Nije moguce otvoriti /dev/IMDCT.\n");
		return -1;
	}

	sleep(0.5);
	fputs("block_type_10 = 0",fp);
	
	if(fclose(fp)){
		printf("Problem pri zatvaranju /dev/IMDCT.\n");
		return -1;	
    }

fp = fopen("/dev/IMDCT", "w");
	
	if(fp==NULL){
		printf("Nije moguce otvoriti /dev/IMDCT.\n");
		return -1;
	}

	sleep(0.5);
	fputs("block_type_10 = 0",fp);
	
	if(fclose(fp)){
		printf("Problem pri zatvaranju /dev/IMDCT.\n");
		return -1;	
    }

fp = fopen("/dev/IMDCT", "w");
	
	if(fp==NULL){
		printf("Nije moguce otvoriti /dev/IMDCT.\n");
		return -1;
	}

	sleep(0.5);
	fputs("block_type_11 = 0",fp);
	
	if(fclose(fp)){
		printf("Problem pri zatvaranju /dev/IMDCT.\n");
		return -1;	
    }

fp = fopen("/dev/IMDCT", "w");
	
	if(fp==NULL){
		printf("Nije moguce otvoriti /dev/IMDCT.\n");
		return -1;
	}

	sleep(0.5);

	fputs("gr = 0",fp);
	
	if(fclose(fp)){
		printf("Problem pri zatvaranju /dev/IMDCT.\n");
		return -1;	
    }

fp = fopen("/dev/IMDCT", "w");
	
	if(fp==NULL){
		printf("Nije moguce otvoriti /dev/IMDCT.\n");
		return -1;
	}
	
	sleep(0.5);
	
	fputs("ch = 0",fp);
	
	if(fclose(fp)){
		printf("Problem pri zatvaranju /dev/IMDCT.\n");
		return -1;	
    }

fp = fopen("/dev/IMDCT", "w");
	
	if(fp==NULL){
		printf("Nije moguce otvoriti /dev/IMDCT.\n");
		return -1;
	}
	
	sleep(0.5);
	
	fputs("start = 1",fp);
	
	if(fclose(fp)){
		printf("Problem pri zatvaranju /dev/IMDCT.\n");
		return -1;	
	}

fp = fopen("/dev/IMDCT", "r");
	
	if(fp==NULL){
		printf("Nije moguce otvoriti /dev/IMDCT.\n");
		return -1;
	}
	
	str = (char *)malloc(broj_bajtova+1);
	getline(&str, &broj_bajtova, fp);
	
	if(fclose(fp)){
		printf("Problem pri zatvaranju /dev/IMDCT.\n");
		return -1;	
	}

	free(str);

	printf("*************************\n");
	printf("**Rezultat IMDCT**\n");
	printf("*************************\n");
	
fp = fopen("/dev/bram_a", "r");
	
	if(fp==NULL){
		printf("Nije moguce otvoriti /dev/bram_a.\n");
		return -1;
	}
	
	str = (char *)malloc(broj_bajtova+1);
	getline(&str, &broj_bajtova, fp);
	
	if(fclose(fp)){
		printf("Problem pri zatvaranju /dev/bram_a.\n");
		return -1;	
	}

	free(str);

fp = fopen("/dev/bram_b", "r");
	
	if(fp==NULL){
		printf("Nije moguce otvoriti /dev/bram_b.\n");
		return -1;
	}
	
	str = (char *)malloc(broj_bajtova+1);
	getline(&str, &broj_bajtova, fp);
	
	if(fclose(fp)){
		printf("Problem pri zatvaranju /dev/bram_b.\n");
		return -1;	
	}

	free(str);

	return 0;
}