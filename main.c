#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lib/jsmn_parser.h"

#define KEYS 6
#define TOKENS 128

#define MAX_JSON_LENGTH 1024
#define MAX_KEY_LENGTH 512
#define MAX_LETTER_LENGTH 1024

#define MAIL_LOCATION "..\\core\\mail.json"

#define LETTER_KEY_ADR(MAIL_NUM, KEY_NUM) MAIL_NUM*(KEYS*2)+(MAIL_NUM+1)+(2*(KEY_NUM+1))
#define LETTER_ADR(MAIL_NUM) MAIL_NUM*(KEYS*2)+(MAIL_NUM+1)
#define LETTER_KEY_ADR2(LETTER_ADR, KEY_NUM) LETTER_ADR+(2*(KEY_NUM+1))
#define LETTERS_AMOUNT(JSON_SIZE) (JSON_SIZE-1)/(KEYS*2+1)

#define ID 0
#define SENDER 1
#define RECIEVER 2
#define THEME 3
#define BODY 4
#define REPLY_TO 5

int parse_json(char *json_, jsmn_parser parser_, jsmntok_t *tokens_, char *path_);
int load_json();
int read_file(char *string, char path[100]);

int find_adr_by_id(int id);
int find_adr_by_theme(int *dest_adrs, char *theme);
void get_json_value(char *dest, int address);

char json[MAX_JSON_LENGTH];
int json_size;
int letters_amount;

jsmn_parser parser;
jsmntok_t tokens[TOKENS];

char temp_holder[1024];

int main()
{

 int adrs[1024];
 find_adr_by_theme(adrs, "NO_THEME");

 char letter[MAX_LETTER_LENGTH];

 for (int i = 1; i <= adrs[0]; i++)
 {
  get_json_value(letter, adrs[i]);
  printf("%s\n", letter);
 }

 return 1;
}

int find_adr_by_id(int id)
{
 load_json();

 for (int i = 0, iter_id = 0; i < json_size; i++)
 {
  get_json_value(temp_holder, LETTER_KEY_ADR(i, ID));
  iter_id = atoi(temp_holder);

  if (iter_id == id)
  {
   //printf("MAIL_KEY = %d\n", LETTER_KEY_ADR(i, ID) - 2);
   //printf("MAIL = %d\n", LETTER_ADR(i));
   return LETTER_ADR(i);
  }
 }

 return -1;
}

int find_adr_by_theme(int *dest_adrs, char *theme)
{
 load_json();

 char *iter_theme;

 letters_amount = LETTERS_AMOUNT(json_size);

 for (int i = 0, j = 1; i < letters_amount; i++)
 {
  get_json_value(temp_holder, LETTER_KEY_ADR(i, THEME));
  iter_theme = temp_holder;


  if (strstr(iter_theme, theme))
  {
   dest_adrs[j] = LETTER_ADR(i);
   dest_adrs[0] += 1;
   j++;
  }
 }

 return (dest_adrs[0] != 0) ? 1 : -1;
}

int parse_json(char *json_, jsmn_parser parser_, jsmntok_t *tokens_, char *path_) // Returns json
{
 read_file(json_, path_);
 jsmn_init(&parser_);

 int json_size_ = jsmn_parse(&parser_, json_, strlen(json), tokens_, 128);

 if (json_size_ < 0)
 {
  printf("Failed to parse JSON: {%d\n}", json_size_);
  return -1;
 }

 return json_size_;
}

int read_file(char *dest, char *path)
{
 FILE *fptr;
 fptr = fopen(path, "r");
 fread(dest, MAX_JSON_LENGTH, 1, fptr);
 fclose(fptr);

 if (fptr == NULL)
 {
  printf("Failed to read file: {%s}\n", path);
  return -1;
 }
 return 1;
}

void get_json_value(char *dest, int address)
{
 jsmntok_t key = tokens[address];
 unsigned int length = key.end - key.start;

 memcpy(dest, &json[key.start], length);
 dest[length] = '\0';
}

int load_json()
{
 json_size = parse_json(json, parser, tokens, MAIL_LOCATION);
 letters_amount = LETTERS_AMOUNT(json_size);

 if (json_size == -1) return -1;
 return 1;
}