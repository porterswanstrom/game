#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

int quit = 0;
char name[17];
char type = '\0';
int xp = 0;

/* ticks and tick speed */
int t = 0;
int ts = 1;

/* multipliers */
int m01 = 1;
int m02 = 0;
int m03 = 0;
int m04 = 0;
int m05 = 0;

/* costs */
int c01 = 10;
int c02 = 30;
int c03 = 50;
int c04 = 100;
int c05 = 200;

/* make save file */
int
save(void){
	FILE *save = fopen("save", "w");
	if(save==NULL)
		return 1;
	fprintf(save,"%d\n",xp);
	fprintf(save,"%d\n",m01);
	fprintf(save,"%d\n",m02);
	fprintf(save,"%d\n",m03);
	fprintf(save,"%d\n",m04);
	fprintf(save,"%d\n",m05);
	fclose(save);
	return 0;
}

/* use save file */
int
load(void){
	FILE *save = fopen("save", "r");
	if(save==NULL)
		return 1;
	fscanf(save,"%d\n",&xp);
	fscanf(save,"%d\n",&m01);
	fscanf(save,"%d\n",&m02);
	fscanf(save,"%d\n",&m03);
	fscanf(save,"%d\n",&m04);
	fscanf(save,"%d\n",&m05);
	fclose(save);
	return 0;
}

/* wipe save file */
int
reset(void){
	FILE *save = fopen("save", "w");
	if(save==NULL)
		return 1;
	fputs("0\n",save);
	fputs("1\n",save);
	fputs("0\n",save);
	fputs("0\n",save);
	fputs("0\n",save);
	fputs("0\n",save);
	fclose(save);
	load();
	return 0;
}

/* show intro message asking for a username */
int
intro(void){
	puts("\e[1;1H\e[2J\n"
		"      Welcome!\n  What's your name?");
	scanf("%s",name);
	return 0;
}

/* let user pick a type to play */
int
pick_type(void){
	printf("what type are you %s? (this does nothing as of now)\n",name);
	puts("magic(a):ranged(b):melee(c)");
	for(;;){
		scanf("%c",&type);
		switch(type){
			case 'q':
				++quit;
				return 0;
			case 'a':
				puts("you're a magic type");
				return 0;
			case 'b':
				puts("you're a ranged type");
				return 0;
			case 'c':
				puts("you're a melee type");
				return 0;
		}
	}
}

/* show user stats */
int
stats(void){
	printf("%d ",xp);
	printf("%da:",m01);
	printf("%db:",m02);
	if(m02>0)
		printf("%dc:",m03);
	if(m03>0)
		printf("%dd:",m04);
	if(m04>0)
		printf("%de",m05);
	puts("");
	return 0;
}

/* show multiplier prices */
int
cost(void){
	printf("cost ");
	printf("%da:",c01);
	printf("%db:",c02);
	if(m02>0)
		printf("%dc:",c03);
	if(m03>0)
		printf("%dd:",c04);
	if (m04>0)
		printf("%de",c05);
	puts("");
	return 0;
}

/* for buying multiplier upgrades */
int
up(void){
	char c = '\0';
	for(;;){
		int i = 0;
		c = '\0';
		scanf("%c",&c);
		switch(c){
			case '.':
				puts("main menu");
				return 0;
			case ',':
				stats();
				break;
			case '?':
				puts("upgrades");
				cost();
				break;
			case 'a':
				while(xp>c01){
					++m01;
					xp -= c01;
					++i;
				}
				printf("bought %d layer(%c) upgrades\n",i,c);
				break;
			case 'b':
				while(xp>c02){
					++m02;
					xp -= c02;
					++i;
				}
				printf("bought %d layer(%c) upgrades\n",i,c);
				break;
			case 'c':
				while(xp>c03){
					++m03;
					xp -= c03;
					++i;
				}
				printf("bought %d layer(%c) upgrades\n",i,c);
				break;
			case 'd':
				while(xp>c04){
					++m04;
					xp -= c04;
					++i;
				}
				printf("bought %d layer(%c) upgrades\n",i,c);
				break;
			case 'e':
				while(xp>c05){
					++m05;
					xp -= c05;
					++i;
				}
				printf("bought %d layer(%c) upgrades\n",i,c);
				break;
		}
	}
}

/* let user pick an action */
int
pick(void){
	char c = '\0';
	scanf("%c",&c);
	switch(c){
		case '.':
			++quit;
			return 0;
		case ',':
			stats();
			break;
		case '?':
			puts("main menu");
			puts("quit(.):stats(,):save(s):load(l):reset(r):upgrades(u)");
			break;
		case 's':
			save();
			break;
		case 'l':
			load();
			break;
		case 'r':
			reset();
			break;
		case 'u':
			puts("upgrades");
			up();
			break;
		default:
			return 1;
	}
	return 0;
}

/* thread which automatically gains xp */
void *
auto_xp(void *vargp){
	pthread_detach(pthread_self());
	for(t=0; quit<1; ++t){
		xp += m01;
		if(t % 5 == 0)
			m01 += m02;
		if(t % 10 == 0)
			m02 += m03;
		if(t % 20 == 0)
			m03 += m04;
		if(t % 30 == 0)
			m04 += m05;
		sleep(ts);
	}
	pthread_exit(NULL);
}

int
main(void){
	pthread_t tid;
	load();
	intro();
	pick_type();
	pthread_create(&tid,NULL,&auto_xp,NULL);
	puts("type (?) for help");
	while(quit<1)
		pick();
	save();
	puts("saving and exiting");
	pthread_exit(NULL);
	return 0;
}
