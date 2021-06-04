#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lib/jsmn_parser.h"

#define KEYS 6
#define TOKENS 128

#define MAX_JSON_LENGTH 1024
#define MAX_KEY_LENGTH 512

#define MAIL_LOCATION "..\\core\\mail.json"

#define MAIL_KEY_ADR(MAIL_NUM, KEY_NUM) MAIL_NUM*(KEYS*2)+(MAIL_NUM+1)+(2*(KEY_NUM+1))
#define MAIL_ADR(MAIL_NUM) MAIL_NUM*(KEYS*2)+(MAIL_NUM+1)
#define MAIL_KEY_ADR_(MAIL_ADR,KEY_NUM) MAIL_ADR+(2*(KEY_NUM+1))

#define ID 0
#define SENDER 1
#define RECIEVER 2
#define THEME 3
#define BODY 4
#define REPLY_TO 5

void read_json(char *string, char path[100]);
int get_json_adr_by_id(int id);
void get_json_value(char* dest, int address);

char json_string[MAX_JSON_LENGTH];
char string[1024];
jsmn_parser p;
jsmntok_t t[128];

int main() {


 read_json(json_string, MAIL_LOCATION);



 jsmn_init(&p);

 int r = jsmn_parse(&p, json_string, strlen(json_string), t, 128);

 if (r < 0) {
  printf("Failed to parse JSON: %d\n", r);
  exit(1);
 }
 char f[100];
 get_json_value(f, get_json_adr_by_id(5));
 printf("%s",f);
}

FILE *fptr;

int get_json_adr_by_id(int id)
{
 read_json(json_string, MAIL_LOCATION);

 for (int i=0, iter_id = 0; i<TOKENS/KEYS; i++)
 {
  get_json_value(string,MAIL_KEY_ADR(i,ID));
  iter_id = atoi(string);

  if (iter_id == id){
   printf("MAIL_KEY = %d\n", MAIL_KEY_ADR(i,ID)-2);
   printf("MAIL = %d\n",MAIL_ADR(i));
   return MAIL_ADR(i);
  }
 }

 return -1;
}

void read_json(char* dest, char path[100]) {
 fptr = fopen(path, "r");
 fread(dest, MAX_JSON_LENGTH, 1, fptr);
 fclose(fptr);
}

void get_json_value(char* dest, int address)
{
 jsmntok_t key = t[address];
 unsigned int length = key.end - key.start;

 memcpy(dest, &json_string[key.start], length);
 dest[length] = '\0';
}