#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <unknwn.h>
#include "lib/jsmn_parser.h"

#define KEYS 6
#define KILOBYTE 1024
#define LETTER_LENGTH 400
#define MAX_LETTERS_AMOUNT 100
#define MAX_MAIL_LENGTH MAX_LETTERS_AMOUNT*LETTER_LENGTH+KILOBYTE
#define TOKENS (MAX_LETTERS_AMOUNT*KEYS*2)+MAX_LETTERS_AMOUNT+1
#define MAIL_LOCATION "..\\core\\mail.json"
#define BITMAP_LOCATION "..\\core\\bitmap.map"
#define SUCCESS 0
#define FAIL -1

#define BMP_ACTIVE '1'
#define BMP_REMOVED '0'

#define LETTER_KEY_ADR(MAIL_NUM, KEY_NUM) MAIL_NUM*(KEYS*2)+(MAIL_NUM+1)+(2*(KEY_NUM+1))
#define LETTER_ADR(MAIL_NUM) MAIL_NUM*(KEYS*2)+(MAIL_NUM+1)
#define LETTER_KEY_ADR2(LETTER_ADR, KEY_NUM) LETTER_ADR+(2*(KEY_NUM+1))
#define LETTERS_AMOUNT(MAIL_SIZE) (MAIL_SIZE-1)/(KEYS*2+1)
#define ID 0
#define SENDER 1
#define RECEIVER 2
#define THEME 3
#define BODY 4
#define REPLY_TO 5

#define CREATE_LETTER(ID, SENDER, RECEIVER, THEME, BODY, REPLY_TO) "{\n    \"id\": " #ID ",\n    \"sender\": \"" SENDER "\",\n    \"receiver\": \"" RECEIVER "\",\n    \"theme\": \"" THEME "\",\n    \"body\": \"" BODY "\",\n    \"reply_to\": " REPLY_TO "\n  }"


void replace_letter(char *origin, char *substr, char *new_substr);

int mail_size, bmp_size, letters_amount;
int alloc_mail_len;
int alloc_bmp_len;

jsmn_parser parser;
jsmntok_t tokens[TOKENS];

char *_mail, *_bmp;
char **_pmail = &_mail, **_pbmp = &_bmp;

// TODO: make value_holder busy/not_busy etc;

int alloc_mem(char** buf, FILE* infile)
{
 if (infile == NULL) return FAIL;

 fseek(infile, 0L, SEEK_END);
 long numbytes = ftell(infile);
 fseek(infile, 0L, SEEK_SET);

 *buf = (char *) malloc(numbytes* sizeof(char)+1);

 //printf("Allocating mem: %ld\n",numbytes);
 return numbytes* sizeof(char);
}

void free_mem(char** buf)
{
 if (buf==NULL) return;
 free(*buf);
 *buf = NULL;
 //printf("Freeing mem\n");
}

int is_bmp_valid()
{
 if (bmp_size==letters_amount) return 1;
 printf("Failed to commit operation, because bitmap size(%d)!=letters amount(%d)\n", bmp_size, letters_amount); return 0;
}

int write(char *str, char* location)
{
 FILE *fp = fopen(location, "w");

 if (fp != NULL)
 {
  fputs(str, fp);
  fclose(fp);
  //printf("\nwrote to %s\n", location);
  return SUCCESS;
 }

 printf("Failed to write to: {%s}\n", location); return FAIL;
}

int read_file(char** buf, char *path) // Check if buf is null before calling !
{
 FILE *infile;
 infile = fopen(path, "r");

 long numbytes = alloc_mem(buf, infile);

 if (*buf == NULL || numbytes == FAIL) {printf("Failed to read file: {%s}",path);return FAIL;}

 fread(*buf, sizeof(char), numbytes, infile);
 fclose(infile);
 (*buf)[numbytes]='\0';

 return numbytes;
}

int parse_json(char **json_, jsmn_parser parser_, jsmntok_t *tokens_, char *path_) // Returns json
{
 alloc_mail_len = read_file(json_, path_);
 jsmn_init(&parser_);

 int json_size_ = jsmn_parse(&parser_, *json_, strlen(*json_), tokens_, TOKENS);

 if (json_size_ < 0) {printf("Failed to parse JSON: {%d\n}", json_size_);return FAIL;}
 return json_size_;
}

int load_mail()
{
 return mail_size = parse_json(_pmail, parser, tokens, MAIL_LOCATION);
} // Returns mail size

int load_bmp()
{
 return bmp_size = read_file(_pbmp, BITMAP_LOCATION);
}  // Returns bitmap size

int get_value_by_adr(char *dest, int address)
{
 jsmntok_t key = tokens[address];
 unsigned int length = key.end - key.start;
 memcpy(dest, &((*_pmail)[key.start]), length); // TODO: Free memory
 if (dest==NULL) {printf("Failed to get json value");return FAIL;}
 dest[length] = '\0';
 return SUCCESS;
}

int get_adr_by_id(int id_)
{
 letters_amount = LETTERS_AMOUNT(mail_size);
 char value_holder[MAX_MAIL_LENGTH];  /// TODO REPLACE
 for (int i = 0, iter_id_ = 0; i < letters_amount; i++)
 {
  get_value_by_adr(value_holder, LETTER_KEY_ADR(i, ID));
  iter_id_ = atoi(value_holder); // TODO: use strtool instead
  if (iter_id_ == id_) return LETTER_ADR(i);
 }
 return ERROR;
}


int change_bmp(int bmp_id, char value)
{
 letters_amount = LETTERS_AMOUNT(mail_size);
 if (!is_bmp_valid()) return FAIL;
 (*_pbmp)[bmp_id] = value;
 write(*_pbmp, BITMAP_LOCATION);
}

char new_letter[LETTER_LENGTH];

int add_letter()
{
 int i =0;
 MARK:
 letters_amount = LETTERS_AMOUNT(mail_size);

 if (!is_bmp_valid()) return FAIL;
 char value_holder[MAX_MAIL_LENGTH];  /// TODO REPLACE

 int free_index = -1;
 for (; i<bmp_size; i++) if ((*_pbmp)[i]==BMP_REMOVED) {free_index=i; break;}
 if (free_index==-1)
 {
  snprintf(new_letter, sizeof(new_letter), "{\n    \"id\": %d,\n    \"sender\": \"%s\",\n    \"receiver\": \"%s\",\n    \"theme\": \"%s\",\n    \"body\": \"%s\",\n    \"reply_to\": %s\n  }", bmp_size,"0000","0000","00","00", "-1");

  int origin_len = strlen(*_pmail); // TODO: replace strlen
  int add_len = strlen(new_letter);
  char oldmail[alloc_mail_len];
  printf("BEFORE: %d\n",alloc_mail_len);
  strcpy(oldmail,*_pmail);
  realloc(*_pmail,alloc_mail_len=(alloc_mail_len+add_len*sizeof(char))); // TODO: Conversion
  printf("AFTER: %d\n",alloc_mail_len);
  (oldmail)[origin_len-1]='\0';
  (oldmail)[origin_len-2]=',';

  snprintf(*_pmail, alloc_mail_len,"%s\n  %s\n]",oldmail, new_letter);
  printf("%s",*_pmail);
  realloc(*_pbmp,(bmp_size+2)* sizeof(char));
  snprintf(*_pbmp,(bmp_size+2)* sizeof(char),"%s%c",*_pbmp,BMP_ACTIVE);
  write(*_pmail, MAIL_LOCATION);
  write(*_pbmp, BITMAP_LOCATION);
 } else
 {
  int adr;
  adr = get_adr_by_id(free_index);
  //printf("\nFREE_INDEX: %d\n\nADDR: %d\n\n",free_index,adr);
  get_value_by_adr(value_holder,adr);
  snprintf(new_letter, sizeof(new_letter), "{\n    \"id\": %d,\n    \"sender\": \"%s\",\n    \"receiver\": \"%s\",\n    \"theme\": \"%s\",\n    \"body\": \"%s\",\n    \"reply_to\": %s\n  }", free_index,"0000","0000","00","00", "-1");

  change_bmp(free_index, BMP_ACTIVE);

  if (strcmp(value_holder, new_letter) != 0)
  {
   replace_letter(*_pmail, value_holder, new_letter);

   return write(*_pmail, MAIL_LOCATION);

  } else {i+=1;goto MARK;}
 }
}



int get_adrs_by_theme(int *dest_adrs, char *theme) // adrs[0] stores length
{
 letters_amount = LETTERS_AMOUNT(mail_size);
 char value_holder[MAX_MAIL_LENGTH];   /// TODO REPLACE

 for (int i = 0, j = 1; i < letters_amount; i++)
 {
  get_value_by_adr(value_holder, LETTER_KEY_ADR(i, THEME));
  if (strstr(value_holder, theme))
  {
   dest_adrs[j] = LETTER_ADR(i);
   dest_adrs[0] += 1;
   j++;
  }
 }

 return (dest_adrs[0] != 0) ? SUCCESS : FAIL;
}

#define GET_BY_THEME int adrs[MAX_LETTERS_AMOUNT] = {0};load_mail();get_adrs_by_theme(adrs, "Hi");char dest[LETTER_LENGTH];for (int i = 1; i <= adrs[0]; i++){get_value_by_adr(dest, adrs[i]);printf("%s\n", dest);}free_mem(_pmail);return 1;
#define CHANGE_BMP  load_mail();load_bmp();change_bmp(0,BMP_REMOVED);free_mem(_pmail);free_mem(_pbmp);
#define ADD_LETTERS_REPLACE  load_mail();load_bmp();for (int i=0; i<bmp_size; i++){add_letter();free_mem(_pmail);free_mem(_pbmp);load_mail();load_bmp();}free_mem(_pmail);free_mem(_pbmp);

int main()
{
 printf("Bonjour\n");
 load_mail();
 load_bmp();
 add_letter();

 free_mem(_pmail);
 free_mem(_pbmp);

 return 1;
}


void replace_letter(char *origin, char *substr, char *new_substr)
{
 int origin_len, substr_len, new_substr_len;
 int i, j, k;
 int flag = 0, start, end;

 origin_len = strlen(origin); // TODO: replace strlen
 substr_len = strlen(substr);
 new_substr_len = strlen(new_substr);
 realloc(origin,alloc_mail_len=((alloc_mail_len-substr_len* sizeof(char))+new_substr_len* sizeof(char)));

 for (i = 0; i < origin_len; i++)
 {
  flag = 0;
  start = i;
  for (j = 0; origin[i] == substr[j]; j++, i++) if (j == substr_len - 1) flag = 1;
  end = i;
  if (flag == 0) {i -= j; continue;}

  for (j = start; j < end; j++)
  {
   for (k = start; k < origin_len; k++) origin[k] = origin[k + 1];
   origin_len -= 1;
   i -= 1;
  }

  for (j = start; j < start + new_substr_len; j++)
  {
   for (k = origin_len; k >= j; k--) origin[k + 1] = origin[k];
   origin[j] = new_substr[j - start];
   origin_len += 1;
   i += 1;
  }
 }
}