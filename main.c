#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include "lib/jsmn_parser.h"

#define KEYS 6

#define KILOBYTE 1024

#define LETTER_LENGTH 300
#define MAX_LETTERS_AMOUNT 100

#define TOKENS (MAX_LETTERS_AMOUNT*KEYS*2)+MAX_LETTERS_AMOUNT+1

#define MAX_MAIL_LENGTH MAX_LETTERS_AMOUNT*LETTER_LENGTH+KILOBYTE
#define MAX_BITMAP_LENGTH 512

#define MAIL_LOCATION "..\\core\\mail.json"
#define BITMAP_LOCATION "..\\core\\bitmap.map"

#define LETTER_KEY_ADR(MAIL_NUM, KEY_NUM) MAIL_NUM*(KEYS*2)+(MAIL_NUM+1)+(2*(KEY_NUM+1))
#define LETTER_ADR(MAIL_NUM) MAIL_NUM*(KEYS*2)+(MAIL_NUM+1)
#define LETTER_KEY_ADR2(LETTER_ADR, KEY_NUM) LETTER_ADR+(2*(KEY_NUM+1))
#define LETTERS_AMOUNT(MAIL_SIZE) (MAIL_SIZE-1)/(KEYS*2+1)

#define CREATE_LETTER(ID, SENDER, RECEIVER, THEME, BODY, REPLY_TO) "{\n    \"id\": " ID ",\n    \"sender\": \"" SENDER "\",\n    \"receiver\": \"" RECEIVER "\",\n    \"theme\": \"" THEME "\",\n    \"body\": \"" BODY "\",\n    \"reply_to\": " REPLY_TO "\n  }"

#define ID 0
#define SENDER 1
#define RECIEVER 2
#define THEME 3
#define BODY 4
#define REPLY_TO 5

#define SUCCESS 0
#define ERROR -1

#define BMP_ACTIVE '1'
#define BMP_REMOVED '0'

int parse_json(char *json_, jsmn_parser parser_, jsmntok_t *tokens_, char *path_);

int load_bitmap(char* bitmap_);
int load_mail(char* mail_);

int read_file(char *buffer, char *path);

int change_bmp(char* mail, char* bitmap, int bmp_id, char value);
int find_adr_by_id(char* mail, int id);
int find_adr_by_theme(char* mail, int *dest_adrs, char *theme);
int get_value_by_addr(char* mail, char *dest, int address);

int add_letter(char* mail, char* bitmap, char* args);

jsmn_parser parser;
jsmntok_t tokens[TOKENS];

char value_holder[LETTER_LENGTH];
int addr[MAX_LETTERS_AMOUNT];

int bmp_size, mail_size, letters_amount;

//get_info (ВЫЗЫВАЕТСЯ ВНУТРИ)

int main()
{
 //TODO : METHOD DISPLAY
 char* d;
 add_letter(NULL, NULL, d);
 return 1;
}

int is_bmp_valid()
{
 if (bmp_size!=letters_amount)
 {
  printf("Failed to commit operation, because bitmap size!=letters amount");
  return 0;
 }
 return 1;
}


void replace_substr(char *origin, char *substr, char *new_substr)
{
 int origin_len, substr_len, new_substr_len;
 int i, j, k;
 int flag = 0, start, end;

 origin_len = strlen(origin);
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

 printf("Failed to write to: {%s}\n", location);
 return ERROR;
}

int add_letter(char* mail, char* bitmap, char* args)
{
 char* mail_;
 char* bitmap_;

 if (mail==NULL){ mail_ = malloc(MAX_MAIL_LENGTH* sizeof(char)); mail_size = load_mail(mail_); } else mail_ = mail;
 if (bitmap==NULL){ bitmap_ = malloc(MAX_BITMAP_LENGTH* sizeof(char)); bmp_size = load_bitmap(bitmap_); } else bitmap_ = bitmap;

 letters_amount = LETTERS_AMOUNT(mail_size);

 if (!is_bmp_valid()) return ERROR;

 int free_index = -1;
 for (int i=0; i<bmp_size; i++) if (bitmap_[i]==BMP_REMOVED) {free_index=i; break;}

 if (free_index==-1)
 {

 } else
 {
  int adr;
  adr = find_adr_by_id(mail_,free_index);
  get_value_by_addr(mail_, value_holder,adr);

  replace_substr(mail_,value_holder,CREATE_LETTER("00","0000","0000","00","00", "-1"));
  return write(mail_,MAIL_LOCATION);
  //change_bmp(mail_,bitmap_,free_index,BMP_ACTIVE);
 }


}

int change_bmp(char* mail, char* bitmap, int bmp_id, char value)
{
 char* mail_;
 char* bitmap_;

 if (mail==NULL){ mail_ = malloc(MAX_MAIL_LENGTH* sizeof(char)); mail_size = load_mail(mail_); } else mail_ = mail;
 if (bitmap==NULL){ bitmap_ = malloc(MAX_BITMAP_LENGTH* sizeof(char)); bmp_size = load_bitmap(bitmap_); } else bitmap_ = bitmap;

 letters_amount = LETTERS_AMOUNT(mail_size);

 if (!is_bmp_valid()) return ERROR;

 bitmap_[bmp_id] = value;

 write(bitmap_, BITMAP_LOCATION);
}

int find_adr_by_id(char* mail, int id_)
{
 char* mail_;

 if (mail==NULL){ mail_ = malloc(MAX_MAIL_LENGTH* sizeof(char)); mail_size = load_mail(mail_); } else mail_ = mail;

 letters_amount = LETTERS_AMOUNT(mail_size);

 for (int i = 0, iter_id_ = 0; i < letters_amount; i++)
 {
  get_value_by_addr(mail_, value_holder, LETTER_KEY_ADR(i, ID));
  iter_id_ = atoi(value_holder); // TODO: use strtool instead

  if (iter_id_ == id_) return LETTER_ADR(i);
  //printf("MAIL_KEY = %d\n", LETTER_KEY_ADR(i, ID) - 2);
  //printf("MAIL = %d\n", LETTER_ADR(i));
 }

 return ERROR;
}



int find_adr_by_theme(char* mail, int *dest_adrs, char *theme)
{
 char* mail_;

 if (mail==NULL){ mail_ = malloc(MAX_MAIL_LENGTH* sizeof(char)); mail_size = load_mail(mail_); } else mail_ = mail;

 letters_amount = LETTERS_AMOUNT(mail_size);

 char *iter_theme;

 for (int i = 0, j = 1; i < letters_amount; i++)
 {
  get_value_by_addr(mail_, value_holder, LETTER_KEY_ADR(i, THEME));
  iter_theme = value_holder;


  if (strstr(iter_theme, theme))
  {
   dest_adrs[j] = LETTER_ADR(i);
   dest_adrs[0] += 1;
   j++;
  }

  for (int f = 1; f <= addr[0]; f++)
  {
   get_value_by_addr(mail_, value_holder, addr[f]);
   printf("%s\n", value_holder);
  }
 }

 return (dest_adrs[0] != 0) ? SUCCESS : ERROR;
}

int parse_json(char *json_, jsmn_parser parser_, jsmntok_t *tokens_, char *path_) // Returns json
{
 read_file(json_, path_);

 jsmn_init(&parser_);

 int json_size_ = jsmn_parse(&parser_, json_, strlen(json_), tokens_, TOKENS);

 if (json_size_ < 0)
 {
  printf("Failed to parse JSON: {%d\n}", json_size_);
  return ERROR;
 }



 return json_size_;
}

int read_file(char *buffer, char *path)
{
 long length=0;
 FILE * fp_ = fopen(path, "rb");

 if (fp_)
 {
  fseek (fp_, 0, SEEK_END);
  length = ftell (fp_);
  fseek (fp_, 0, SEEK_SET);

  if (buffer)
  {
   fread (buffer, 1, length, fp_);
  }
  fclose (fp_);
 }

 // TODO: RECHECK if (buffer) {}

 //printf("%s\n-----------\n",buffer);

 if (buffer == NULL)
 {
  printf("Failed to read file: {%s}\n", path);
  return ERROR;
 }
 return length;
}

int get_value_by_addr(char* mail, char *dest, int address)
{
 char* mail_;

 if (mail==NULL){ mail_ = malloc(MAX_MAIL_LENGTH* sizeof(char)); mail_size = load_mail(mail_); } else mail_ = mail;

 jsmntok_t key = tokens[address];
 unsigned int length = key.end - key.start;

 memcpy(dest, &mail_[key.start], length);

 if (dest==NULL)
 {
  printf("Failed to get json value");
  return ERROR;
 }

 dest[length] = '\0';
 return SUCCESS;
}

int load_mail(char* mail) // Returns mail size
{
 printf("%s\n\n","LOADING MAILz");
 int r = parse_json(mail, parser, tokens, MAIL_LOCATION);
 printf("MAIL AMOUNT IS : %s\n",mail);
 return r;
}

int load_bitmap(char* bitmap) // Returns bitmap size
{
 return read_file(bitmap, BITMAP_LOCATION);
}
