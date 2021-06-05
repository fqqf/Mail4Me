#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lib/jsmn_parser.h"

#define KEYS 6
#define TOKENS 128

#define MAX_JSON_LENGTH 1024
#define MAX_KEY_LENGTH 512
#define MAX_MAIL_LENGTH 1024

#define MAIL_LOCATION "..\\core\\mail.json"

#define MAIL_KEY_ADR(MAIL_NUM, KEY_NUM) MAIL_NUM*(KEYS*2)+(MAIL_NUM+1)+(2*(KEY_NUM+1))
#define MAIL_ADR(MAIL_NUM) MAIL_NUM*(KEYS*2)+(MAIL_NUM+1)
#define MAIL_KEY_ADR_(MAIL_ADR, KEY_NUM) MAIL_ADR+(2*(KEY_NUM+1))
#define MAIL_AMOUNT(JSON_SIZE) (JSON_SIZE-1)/(KEYS*2+1)

#define ID 0
#define SENDER 1
#define RECIEVER 2
#define THEME 3
#define BODY 4
#define REPLY_TO 5

void read_json(char *string, char path[100]);

int find_adr_by_id(int id);

void get_json_value(char *dest, int address);
int find_adr_by_theme(int* dest_adrs, char* theme);

char json_string[MAX_JSON_LENGTH];
char string[1024];
char mail[MAX_MAIL_LENGTH];

int adrs[1024];
int json_size;
int mail_amount;
jsmn_parser p;
jsmntok_t t[128];

int main()
{
 read_json(json_string, MAIL_LOCATION);

 jsmn_init(&p);

 json_size = jsmn_parse(&p, json_string, strlen(json_string), t, 128);

 if (json_size < 0)
 {
  printf("Failed to parse JSON: %d\n", json_size);
  exit(1);
 }

 find_adr_by_theme(adrs, "NO_THEME");
 for (int i = 1; i <= adrs[0]; i++)
 {
  get_json_value(mail, adrs[i]);
  printf("%s\n", mail);
 }


 return 1;
}

FILE *fptr;

int find_adr_by_id(int id)
{
 read_json(json_string, MAIL_LOCATION);

 mail_amount = MAIL_AMOUNT(json_size);

 for (int i = 0, iter_id = 0; i < json_size; i++)
 {
  get_json_value(string, MAIL_KEY_ADR(i, ID));
  iter_id = atoi(string);

  if (iter_id == id)
  {
   //printf("MAIL_KEY = %d\n", MAIL_KEY_ADR(i, ID) - 2);
   //printf("MAIL = %d\n", MAIL_ADR(i));
   return MAIL_ADR(i);
  }
 }

 return -1;
}

char *iter_theme;

int find_adr_by_theme(int* dest_adrs, char* theme)
{
 read_json(json_string, MAIL_LOCATION);

 mail_amount = MAIL_AMOUNT(json_size);

 for (int i = 0,j=1; i < mail_amount; i++)
 {
  get_json_value(string, MAIL_KEY_ADR(i, THEME));
  iter_theme = string;


  if (strstr(iter_theme, theme))
  {
   dest_adrs[j] = MAIL_ADR(i);
   dest_adrs[0] += 1;
   j++;
  }
 }

 return (dest_adrs[0]!=0) ? 1 : -1;
}

 void read_json(char *dest, char path[100])
 {
  fptr = fopen(path, "r");
  fread(dest, MAX_JSON_LENGTH, 1, fptr);
  fclose(fptr);
 }

 void get_json_value(char *dest, int address)
 {
  jsmntok_t key = t[address];
  unsigned int length = key.end - key.start;

  memcpy(dest, &json_string[key.start], length);
  dest[length] = '\0';
 }