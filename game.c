#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

char name[17];
char type = '\0';
int quit = 0;
int xp = 0;

/* ticks and tick speed */
int ts = 1;
int t = 0;

/* multipliers */
int ma = 1;
int mb = 0;
int mc = 0;
int md = 0;
int me = 0;

/* costs */
int ca = 10;
int cb = 30;
int cc = 50;
int cd = 100;
int ce = 200;

int
load(void)
{
	FILE *save = fopen("save", "r");
	if(save==NULL)
		return 1;
	fscanf(save,"%d\n",&xp);
	fscanf(save,"%d\n",&ma);
	fscanf(save,"%d\n",&mb);
	fscanf(save,"%d\n",&mc);
	fscanf(save,"%d\n",&md);
	fscanf(save,"%d\n",&me);
	fclose(save);
	return 0;
}

int
save(void)
{
	FILE *save = fopen("save", "w");
	if(save==NULL)
		return 1;
	fprintf(save,"%d\n",xp);
	fprintf(save,"%d\n",ma);
	fprintf(save,"%d\n",mb);
	fprintf(save,"%d\n",mc);
	fprintf(save,"%d\n",md);
	fprintf(save,"%d\n",me);
	fclose(save);
	return 0;
}

int
reset(void)
{
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

/* show an intro message and ask the user for their name */
int
intro(void)
{
	puts("\e[1;1H\e[2J\n"
		"      Welcome!\n  What's your name?");
	scanf("%s",name);
	return 0;
}

/* let the user pick a type to play as */
int
pick_type(void)
{
	printf("What type are you %s?\n",name);
	puts("magic(a):ranged(b):melee(c)");
	for(;;){
		scanf("%c",&type);
		switch(type){
			case 'q':
				++quit;
				return 0;
			case 'a':
				puts("You are a magic type");
				return 0;
			case 'b':
				puts("You are a ranged type");
				return 0;
			case 'c':
				puts("You are a melee type");
				return 0;
		}
	}
}

/* show the user's stats */
int
stats(void)
{
	printf("%d ",xp);
	printf("%da:",ma);
	printf("%db:",mb);
	if(mb>0)
		printf("%dc:",mc);
	if(mc>0)
		printf("%dd:",md);
	if (md>0)
		printf("%de",me);
	puts("");
	return 0;
}

/* show multiplier prices */
int
cost(void)
{
	printf("cost ");
	printf("%da:",ca);
	printf("%db:",cb);
	if(mb>0)
		printf("%dc:",cc);
	if(mc>0)
		printf("%dd:",cd);
	if (md>0)
		printf("%de",ce);
	puts("");
	return 0;
}

/* for buying multiplier upgrades */
int
up(void)
{
	char c = '\0';
	for(;;){
		int i = 0;
		c = '\0';
		scanf("%c",&c);
		switch(c){
			case '.':
				puts("Main Menu");
				return 0;
			case ',':
				stats();
				break;
			case '?':
				puts("Upgrades");
				cost();
				break;
			case 'a':
				while(xp>ca){
					++ma;
					xp -= ca;
					++i;
				}
				printf("Bought %d Layer(%c) upgrades\n",i,c);
				break;
			case 'b':
				while(xp>cb){
					++mb;
					xp -= cb;
					++i;
				}
				printf("Bought %d Layer(%c) upgrades\n",i,c);
				break;
			case 'c':
				while(xp>cc){
					++mc;
					xp -= cc;
					++i;
				}
				printf("Bought %d Layer(%c) upgrades\n",i,c);
				break;
			case 'd':
				while(xp>cd){
					++md;
					xp -= cd;
					++i;
				}
				printf("Bought %d Layer(%c) upgrades\n",i,c);
				break;
			case 'e':
				while(xp>ce){
					++me;
					xp -= ce;
					++i;
				}
				printf("Bought %d Layer(%c) upgrades\n",i,c);
				break;
		}
	}
}

/* let the user pick an action */
int
pick(void)
{
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
			puts("Main Menu");
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
			puts("Upgrades");
			up();
			break;
		default:
			return 1;
	}
	return 0;
}

/* thread that automatically gains xp */
void *
auto_xp(void *vargp)
{
	pthread_detach(pthread_self());
	for(t=0; quit<1; ++t){
		xp += ma;
		if(t % 5 == 0)
			ma += mb;
		if(t % 10 == 0)
			mb += mc;
		if(t % 20 == 0)
			mc += md;
		if(t % 30 == 0)
			md += me;
		sleep(ts);
	}
	pthread_exit(NULL);
}

int
main(void)
{
	pthread_t tid;
	load();
	intro();
	pick_type();
	pthread_create(&tid,NULL,&auto_xp,NULL);
	puts("type (?) for help");
	while(quit<1)
		pick();
	save();
	puts("Saving and exiting");
	pthread_exit(NULL);
	return 0;
}
