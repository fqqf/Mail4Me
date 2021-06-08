#include "core/api.h"
#include<stdio.h>
#include<conio.h>
#define ASSETS_DIR "../lib/assets/"
#define LOGO ASSETS_DIR"logo.rsv"
#define WELCOME ASSETS_DIR"welcome.rsv"



FILE *fptr;

void clearscr ( void )
{
 for ( int i = 0; i < 50; i++ ) // 50 is arbitrary
  printf("\n");
}

int print_file(char* path)
{

 char c;
 fptr = fopen(path, "r");
 if (fptr == NULL)
 {
  printf("Error. Missing file: {%s} \n",path);
  exit(0);
 }
 c = fgetc(fptr);
 while (c != EOF) {printf ("%c", c);c = fgetc(fptr);}
 fclose(fptr);
 return 0;
}

char temp[MAX_MAIL_LENGTH];

void display_adr(int adr)
{
 load_bmp();
 load_mail();
 get_value_by_adr(temp,LETTER_KEY_ADR2(adr,SENDER));
 printf("%s",temp);
 free_mem(_pbmp);
 free_mem(_pmail);
}

void display_id(int id)
{

 load_bmp();
 load_mail();
 get_value_by_adr(temp,LETTER_KEY_ADR(id,ID));
 printf("| ID:       \t%s\n",temp);
 get_value_by_adr(temp,LETTER_KEY_ADR(id,SENDER));
 printf("| SENDER:   \t%s\n",temp);
 get_value_by_adr(temp,LETTER_KEY_ADR(id,RECEIVER));
 printf("| RECEIVER  \t%s\n",temp);
 get_value_by_adr(temp,LETTER_KEY_ADR(id,THEME));
 printf("| THEME     \t%s\n",temp);
 get_value_by_adr(temp,LETTER_KEY_ADR(id,BODY));
 printf("| BODY:     \t%s\n",temp);
 get_value_by_adr(temp,LETTER_KEY_ADR(id,REPLY_TO));
 printf("| REPLY_TO: \t %s\n",temp);
 free_mem(_pbmp);
 free_mem(_pmail);
}

void find_letter_by_id()
{

 int id;
 printf("\nPLEASE SELECT ID >:");
 scanf("%d", &id);

 print_file(LOGO);
 print_file(WELCOME);

 printf("\n\n\n:: MAIL INFO ::\n\n");

 display_id(id);
}








int main()
{
 AGAIN:

 print_file(LOGO);
 print_file(WELCOME);

 while (1)
 {
  int decision = 1;
  printf("\nf>:");
  scanf("%d", &decision);

  switch (decision)
  {
   case 1:
    find_letter_by_id();
    break;
   case 2:
    break;
   case 3:
    break;
   case 4:
    break;
   case 5:
    break;
   case 6:
    break;
   default:
    exit(1);
  }
 }

}



/*printf("Bonjour\n");
int adrs[MAX_LETTERS_AMOUNT] = {0};
load_mail();
get_adrs_from_chain(adrs, "Harry@gmail.com","Olivia@hotmail.com");
char dest[LETTER_LENGTH];
for (int i = 1; i <= adrs[0]; i++)
{
 get_value_by_adr(dest, adrs[i]);
 printf("%s\n", dest);
}
free_mem(_pmail);*/