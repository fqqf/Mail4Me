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
#define LETTER_KEY_ADR(MAIL_NUM, KEY_NUM) MAIL_NUM*(KEYS*2)+(MAIL_NUM+1)+(2*(KEY_NUM+1))
#define LETTER_ADR(MAIL_NUM) MAIL_NUM*(KEYS*2)+(MAIL_NUM+1)
#define LETTER_KEY_ADR2(LETTER_ADR, KEY_NUM) LETTER_ADR+(2*(KEY_NUM+1))
#define LETTERS_AMOUNT(MAIL_SIZE) (MAIL_SIZE-1)/(KEYS*2+1)
#define ID 0
#define SENDER 1
#define RECIEVER 2
#define THEME 3
#define BODY 4
#define REPLY_TO 5

void replace_substr(char *origin, char *substr, char *new_substr);

int mail_size, bmp_size, letters_amount;

jsmn_parser parser;
jsmntok_t tokens[TOKENS];

char* mail;
char* bmp;

int alloc_mem(char** buf, FILE* infile)
{
 if (infile == NULL) return FAIL;

 fseek(infile, 0L, SEEK_END);
 long numbytes = ftell(infile);
 fseek(infile, 0L, SEEK_SET);

 *buf = (char *) calloc(numbytes, sizeof(char));
 printf("BUF == NULL : %d\n",buf==NULL);
 return numbytes;
}

void free_mem(char** buf)
{
 if (buf==NULL) return;
 free(*buf);
 *buf = NULL;
}

int is_bmp_valid()
{
 if (bmp_size==letters_amount) return 1;
 printf("Failed to commit operation, because bitmap size!=letters amount"); return 0;
}

int write(char *str, char* location)
{
 FILE *fp = fopen(location, "w");

 if (fp != NULL)
 {
  fputs(str, fp);
  fclose(fp);
  printf("\nwrote to %s\n", location);
  return SUCCESS;
 }

 printf("Failed to write to: {%s}\n", location); return FAIL;
}

int read_file(char** buf, char *path) // Check if buf is null before calling !
{
 FILE *infile;
 infile = fopen(path, "r");

 long numbytes = alloc_mem(buf, infile);
 printf("BUF == NULL : %d\n",buf==NULL);
 if (buf == NULL || numbytes == FAIL) {printf("Failed to read file: {%s}",path);return FAIL;}

 fread(*buf, sizeof(char), numbytes, infile);
 fclose(infile);
 return SUCCESS;
}

int parse_json(char *json_, jsmn_parser parser_, jsmntok_t *tokens_, char *path_) // Returns json
{
 read_file(&json_, path_);
 printf("%s",json_);
 jsmn_init(&parser_);

 int json_size_ = jsmn_parse(&parser_, json_, strlen(json_), tokens_, TOKENS);

 if (json_size_ < 0) {printf("Failed to parse JSON: {%d\n}", json_size_);return FAIL;}
 return json_size_;
}

int load_mail() {return mail_size = parse_json(mail, parser, tokens, MAIL_LOCATION);} // Returns mail size

int load_bmp() {return bmp_size = read_file(bmp, BITMAP_LOCATION);}  // Returns bitmap size

int get_value_by_adr(char *dest, int address)
{
 short free_mail=0;
 if (mail==NULL) {load_mail(); free_mail = TRUE;}
 jsmntok_t key = tokens[address];
 unsigned int length = key.end - key.start;

 memcpy(dest, &mail[key.start], length); // TODO: Free memory

 if (dest==NULL) {printf("Failed to get json value");if (free_mail) free_mem(&mail);return FAIL;}

 dest[length] = '\0';
 if (free_mail) free_mem(&mail);
 return SUCCESS;
}

int get_adrs_by_theme(int *dest_adrs, char *theme) // adrs[0] stores length
{
 short free_mail=0;
 if (mail==NULL) {load_mail(); free_mail = TRUE;}

 letters_amount = LETTERS_AMOUNT(mail_size);
 char iter_theme[LETTER_LENGTH]; // TODO: Use key_len (currently THEME-key len)

 for (int i = 0, j = 1; i < letters_amount; i++)
 {
  get_value_by_adr(iter_theme, LETTER_KEY_ADR(i, THEME));
  if (strstr(iter_theme, theme))
  {
   dest_adrs[j] = LETTER_ADR(i);
   dest_adrs[0] += 1;
   j++;
  }
 }

 if (free_mail) free_mem(&mail);
 return (dest_adrs[0] != 0) ? SUCCESS : FAIL;
}

int main()
{
 printf("Bonjour\n");

 int adrs[MAX_LETTERS_AMOUNT];
 get_adrs_by_theme(adrs, "Hi");

 char dest[LETTER_LENGTH];

 for (int i = 1; i <= adrs[0]; i++)
 {
  get_value_by_adr(dest, adrs[i]);
  printf("%s\n", dest);
 }
}

void replace_substr(char *origin, char *substr, char *new_substr)
{
 int origin_len, substr_len, new_substr_len;
 int i, j, k;
 int flag = 0, start, end;

 origin_len = strlen(origin); // TODO: replace strlen
 substr_len = strlen(substr);
 new_substr_len = strlen(new_substr);

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