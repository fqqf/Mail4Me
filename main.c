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

#define ID 0
#define SENDER 1
#define RECIEVER 2
#define THEME 3
#define BODY 4
#define REPLY_TO 5

#define SUCCESS 0
#define ERROR -1

int parse_json(char *json_, jsmn_parser parser_, jsmntok_t *tokens_, char *path_);

int load_bitmap(char* bitmap_);
int load_mail(char* mail_);

int read_file(char *string, char path[100]);

int change_bitmap(char* mail, char* bitmap, int letter_address,char value);
int find_adr_by_id(char* mail, int id);
int find_adr_by_theme(char* mail, int *dest_adrs, char *theme);
int get_value_by_addr(char* mail, char *dest, int address);

jsmn_parser parser;
jsmntok_t tokens[TOKENS];

char key_holder[LETTER_LENGTH];
int addr[MAX_LETTERS_AMOUNT];

//get_info (ВЫЗЫВАЕТСЯ ВНУТРИ)

int main()
{
 //TODO : METHOD DISPLAY

 find_adr_by_theme(NULL,addr, "Hey");

 return 1;
}

int add_letter(char* args)
{

}

int change_bitmap(char* mail, char* bitmap, int letter_address,char value)
{
 char* mail_ = (mail==NULL) ? alloca(MAX_MAIL_LENGTH* sizeof(char)) : mail;
 char* bitmap_ = (bitmap==NULL) ? alloca(MAX_BITMAP_LENGTH* sizeof(char)) : bitmap;

 int bitmap_size = load_bitmap(bitmap_);
 int mail_size = load_mail(mail_);
 int letters_amount = LETTERS_AMOUNT(mail_size);

 if (bitmap_size!=letters_amount)
 {
  printf("Failed to change bitmap, because bitmap size!=letters amount");
  return ERROR;
 }
 bitmap_[letter_address] = value;

 FILE *fp = fopen(BITMAP_LOCATION, "w");

 if (fp != NULL)
 {

  fputs(bitmap_, fp);
  fclose(fp);
  return SUCCESS;
 }

 printf("Failed to write to bitmap in: {%s}\n", BITMAP_LOCATION);
 return ERROR;
}

int find_adr_by_id(char* mail, int id_)
{
 char* mail_ = (mail==NULL) ? alloca(MAX_MAIL_LENGTH* sizeof(char)) : mail;
 int mail_size = load_mail(mail_);
 int letters_amount = LETTERS_AMOUNT(mail_size);

 for (int i = 0, iter_id_ = 0; i < letters_amount; i++)
 {
  get_value_by_addr(mail_, key_holder, LETTER_KEY_ADR(i, ID));
  iter_id_ = atoi(key_holder); // TODO: use strtool instead

  if (iter_id_ == id_) return LETTER_ADR(i);
  //printf("MAIL_KEY = %d\n", LETTER_KEY_ADR(i, ID) - 2);
  //printf("MAIL = %d\n", LETTER_ADR(i));
 }

 return ERROR;
}



int find_adr_by_theme(char* mail, int *dest_adrs, char *theme)
{
 char* mail_ = (mail==NULL) ? alloca(MAX_MAIL_LENGTH* sizeof(char)) : mail;

 int mail_size = load_mail(mail_);
 int letters_amount = LETTERS_AMOUNT(mail_size);

 char *iter_theme;

 for (int i = 0, j = 1; i < letters_amount; i++)
 {
  get_value_by_addr(mail_, key_holder, LETTER_KEY_ADR(i, THEME));
  iter_theme = key_holder;


  if (strstr(iter_theme, theme))
  {
   dest_adrs[j] = LETTER_ADR(i);
   dest_adrs[0] += 1;
   j++;
  }

  for (int f = 1; f <= addr[0]; f++)
  {
   get_value_by_addr(mail_, key_holder, addr[f]);
   printf("%s\n", key_holder);
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
 if (mail!=NULL) goto copy_mail;
 char* mail_;
 copy_mail: mail_ = mail;

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

int load_mail(char* mail_) // Returns mail size
{
 return parse_json(mail_, parser, tokens, MAIL_LOCATION);
}

int load_bitmap(char* bitmap_) // Returns bitmap size
{
 return read_file(bitmap_, BITMAP_LOCATION);
}
