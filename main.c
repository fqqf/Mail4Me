#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "jsmn_parser.h"
#include <malloc.h>

#define KEYS 6
#define KEYS_LEN KEYS*2
#define GET(FROM, ID) FROM*KEYS_LEN+(FROM+1)+(2*(ID+1))
#define MAX_JSON_LENGTH 1000
#define MAIL_LOCATION "..\\core\\mail.json"

FILE *fptr;

void read_json(char *string, char path[100]) {
 fptr = fopen(path, "r");
 fread(string, MAX_JSON_LENGTH, 1, fptr);
 fclose(fptr);
}

int main() {
 char s[MAX_JSON_LENGTH];

 read_json(s,MAIL_LOCATION);

 jsmn_parser p;
 jsmntok_t t[128];

 jsmn_init(&p);

 int r = jsmn_parse(&p, s, strlen(s), t, 128);

 if (r < 0) {
  printf("Failed to parse JSON: %d\n", r);
  exit(1);
 }

 printf("%d\n", r);

 if (r < 1 || t[0].type != JSMN_ARRAY) {
  printf("Array expected\n");
  return 1;
 }

 jsmntok_t key = t[GET(0, 2)];
 unsigned int length = key.end - key.start;
 char keyString[length];
 memcpy(keyString, &s[key.start], length);
 keyString[length] = '\0';
 printf("Key: %s\n", keyString);
 //  }

}

void show_mail() {

}