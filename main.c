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

void find_letter_by_id()
{
 int id;
 printf("\nPLEASE ENTER ID\n>:");
 scanf("%d", &id);

 printf("\n\n:: MAIL INFO ::\n\n");

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
 printf("\n\n");
 print_file(LOGO);
 print_file(WELCOME);
}

void remove_letter()
{
 int id;
 printf("\nPLEASE ENTER LETTER'S ID\n>:");
 scanf("%d", &id);
 CHANGE_BMP(id)
 printf("\n\n ---- ! DELETED SUCCESSFULLY ! ---- \n\n");

 print_file(LOGO);
 print_file(WELCOME);
}

void addz_letter()
{
 ADD_LETTER_APPEND
 printf("\n\n ---- ! LETTER ADDED SUCCESSFULLY ! ---- \n\n");
 print_file(LOGO);
 print_file(WELCOME);
}

void find_letters_by_subj_line()
{

 char subject[LETTER_LENGTH];
 int adrs[MAX_LETTERS_AMOUNT] = {0};
 printf("\nPLEASE ENTER SUBJECT'S NAME\n>:");
 scanf("%s", subject);

 print_file(LOGO);
 print_file(WELCOME);

 printf("\n\n\n:: MAIL INFO ::\n\n");

 load_mail();
 get_adrs_by_theme(adrs, "Hi");
 char dest[LETTER_LENGTH];
 for (int i = 1; i <= adrs[0]; i++)
 {
  printf("-----------------------------\n");
  get_value_by_adr(dest,LETTER_KEY_ADR2(adrs[i],ID));
  printf("| ID:       \t%s\n",dest);
  get_value_by_adr(dest,LETTER_KEY_ADR2(adrs[i],SENDER));
  printf("| SENDER:   \t%s\n",dest);
  get_value_by_adr(dest,LETTER_KEY_ADR2(adrs[i],RECEIVER));
  printf("| RECEIVER  \t%s\n",dest);
  get_value_by_adr(dest,LETTER_KEY_ADR2(adrs[i],THEME));
  printf("| THEME     \t%s\n",dest);
  get_value_by_adr(dest,LETTER_KEY_ADR2(adrs[i],BODY));
  printf("| BODY:     \t%s\n",dest);
  get_value_by_adr(dest,LETTER_KEY_ADR2(adrs[i],REPLY_TO));
  printf("| REPLY_TO: \t %s\n",dest);
 }
 printf("-----------------------------\n\n\n\n");
 free_mem(_pmail);

 print_file(LOGO);
 print_file(WELCOME);
}


void find_adrs_by_person()
{
 char email[LETTER_LENGTH];
 int adrs_s[MAX_LETTERS_AMOUNT] = {0};
 int adrs_r[MAX_LETTERS_AMOUNT] = {0};
 printf("\nPLEASE ENTER PERSON'S EMAIL\n>:");
 scanf("%s", email);

 load_bmp();
 load_mail();
 
 get_adrs_by_sender(adrs_s,email);
 get_adrs_by_receiver(adrs_r,email);

 char dest[LETTER_LENGTH];
 printf("\n\n   ---[RECEIVED LETTERS]---  \n\n");
 for (int i = 1; i <= adrs_r[0]; i++)
 {
  printf("-----------------------------\n");
  get_value_by_adr(dest,LETTER_KEY_ADR2(adrs_r[i],ID));
  printf("| ID:       \t%s\n",dest);
  get_value_by_adr(dest,LETTER_KEY_ADR2(adrs_r[i],SENDER));
  printf("| SENDER:   \t%s\n",dest);
  get_value_by_adr(dest,LETTER_KEY_ADR2(adrs_r[i],RECEIVER));
  printf("| RECEIVER  \t%s\n",dest);
  get_value_by_adr(dest,LETTER_KEY_ADR2(adrs_r[i],THEME));
  printf("| THEME     \t%s\n",dest);
  get_value_by_adr(dest,LETTER_KEY_ADR2(adrs_r[i],BODY));
  printf("| BODY:     \t%s\n",dest);
  get_value_by_adr(dest,LETTER_KEY_ADR2(adrs_r[i],REPLY_TO));
  printf("| REPLY_TO: \t %s\n",dest);
 }
 printf("\n\n   ---[SENT LETTERS]---   \n\n");
 for (int i = 1; i <= adrs_s[0]; i++)
 {
  printf("-----------------------------\n");
  get_value_by_adr(dest,LETTER_KEY_ADR2(adrs_s[i],ID));
  printf("| ID:       \t%s\n",dest);
  get_value_by_adr(dest,LETTER_KEY_ADR2(adrs_s[i],SENDER));
  printf("| SENDER:   \t%s\n",dest);
  get_value_by_adr(dest,LETTER_KEY_ADR2(adrs_s[i],RECEIVER));
  printf("| RECEIVER  \t%s\n",dest);
  get_value_by_adr(dest,LETTER_KEY_ADR2(adrs_s[i],THEME));
  printf("| THEME     \t%s\n",dest);
  get_value_by_adr(dest,LETTER_KEY_ADR2(adrs_s[i],BODY));
  printf("| BODY:     \t%s\n",dest);
  get_value_by_adr(dest,LETTER_KEY_ADR2(adrs_s[i],REPLY_TO));
  printf("| REPLY_TO: \t %s\n",dest);
 }

 printf("-----------------------------\n\n\n\n");
 
 free_mem(_pbmp);
 free_mem(_pmail);

 print_file(LOGO);
 print_file(WELCOME);
}

void display_mail_chain()
{
 char email[LETTER_LENGTH];
 char email_2[LETTER_LENGTH];
 char dest[LETTER_LENGTH];
 int adrs[MAX_LETTERS_AMOUNT] = {0};
 printf("\nPLEASE ENTER 2 EMAILS\n>:");
 scanf("%s %s", email,email_2);
 printf("%s <--> %s\n",email,email_2);
 load_bmp();
 load_mail();
 get_adrs_from_chain(adrs,email,email_2);

 for (int i = 1; i <= adrs[0]; i++)
 {
  printf("---------------|-------------\n");
  printf("---------------|-------------\n");
  printf("--------------\\/-------------\n");
  get_value_by_adr(dest,LETTER_KEY_ADR2(adrs[i],ID));
  printf("| ID:       \t%s\n",dest);
  get_value_by_adr(dest,LETTER_KEY_ADR2(adrs[i],SENDER));
  printf("| SENDER:   \t%s\n",dest);
  get_value_by_adr(dest,LETTER_KEY_ADR2(adrs[i],RECEIVER));
  printf("| RECEIVER  \t%s\n",dest);
  get_value_by_adr(dest,LETTER_KEY_ADR2(adrs[i],THEME));
  printf("| THEME     \t%s\n",dest);
  get_value_by_adr(dest,LETTER_KEY_ADR2(adrs[i],BODY));
  printf("| BODY:     \t%s\n",dest);
  get_value_by_adr(dest,LETTER_KEY_ADR2(adrs[i],REPLY_TO));
  printf("| REPLY_TO: \t %s\n",dest);
 }
 printf("-----------------------------\n\n\n\n");
 free_mem(_pbmp);
 free_mem(_pmail);

 print_file(LOGO);
 print_file(WELCOME);
}





int main()
{
 AGAIN:

 print_file(LOGO);
 print_file(WELCOME);

 while (1)
 {
  int decision = 1;
  printf("\n\nPLEASE SELECT AN OPTION\n>:");
  scanf("%d", &decision);

  switch (decision)
  {
   case 1:
    find_letter_by_id();
    break;
   case 2:
    find_letters_by_subj_line();
    break;
   case 3:
    remove_letter();
    break;
   case 4:
    addz_letter();
    break;
   case 5:
    find_adrs_by_person();
    break;
   case 6:
    display_mail_chain();
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