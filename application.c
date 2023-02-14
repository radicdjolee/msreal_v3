#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <unistd.h>

int main(void){

    FILE *fp;
    char *text_bram_a = NULL;
    char *text_bram_b;
	int  *bram_a_txt_array, *bram_b_txt_array;
    size_t size = 0;
	int i = 0;
	char str[20];
	//char *text = "2,2,2,2,2,2,2,2,2,2,2,";
    


//***************************UPISUJEMO U /dev/bram_a***************************

	fp = fopen("/dev/bram_a", "w");
	
	if(fp==NULL){
		printf("Nije moguce otvoriti /dev/bram_a.\n");
		return -1;	
	}

	for( i=0; i<1152; i++ ){
		fprintf(fs, "[%s] = %d",i, 2);
		fflush(fs);
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

	fputs(text_bram_b,fp);

	if(fclose(fp)){	
		printf("Problem pri zatvaranju /dev/bram_b.\n");
		return -1;	
	}
*/
//*************************************************************************************************************
/*
fp = fopen("/dev/IMDCT", "w");
	
	if(fp==NULL){
		
		printf("Nije moguce otvoriti /dev/IMDCT.\n");
		return -1;
		
	}
	sleep(0.5);
	printf("	********************************\n");
	printf("	**Upisujem block_type_00=0 u /dev/IMDCT.**\n");
	printf("	********************************\n");
	
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
	printf("	********************************\n");
	printf("	**Upisujem block_type_01=0 u /dev/IMDCT.**\n");
	printf("	********************************\n");
	
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
	printf("	********************************\n");
	printf("	**Upisujem block_type_10=0 u /dev/IMDCT.**\n");
	printf("	********************************\n");
	
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
	printf("	********************************\n");
	printf("	**Upisujem block_type_10=0 u /dev/IMDCT.**\n");
	printf("	********************************\n");
	
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
	printf("	********************************\n");
	printf("	**Upisujem block_type_11=0 u /dev/IMDCT.**\n");
	printf("	********************************\n");
	
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
	printf("	********************************\n");
	printf("	**Upisujem gr=0 u /dev/IMDCT.**\n");
	printf("	********************************\n");
	
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
	printf("	********************************\n");
	printf("	**Upisujem ch=0 u /dev/IMDCT.**\n");
	printf("	********************************\n");
	
	fputs("ch = 0",fp);
	
	if(fclose(fp)){
		
		printf("Problem pri zatvaranju /dev/IMDCT.\n");
		return -1;	
    }
*/
fp = fopen("/dev/IMDCT", "w");
	
	if(fp==NULL){
		
		printf("Nije moguce otvoriti /dev/IMDCT.\n");
		return -1;
		
	}
	sleep(0.5);
	printf("	********************************\n");
	printf("	**Upisujem start=1 u /dev/IMDCT.**\n");
	printf("	********************************\n");
	
	fputs("start = 1",fp);
	
	if(fclose(fp)){
		
		printf("Problem pri zatvaranju /dev/IMDCT.\n");
		return -1;	
    }

	return 0;
}