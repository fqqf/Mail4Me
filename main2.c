#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include "lib/jsmn_parser.h"

#define KEYS 6
#define KILOBYTE 1024
#define LETTER_LENGTH 200
#define MAX_LETTERS_AMOUNT 100
#define MAX_MAIL_LENGTH MAX_LETTERS_AMOUNT*LETTER_LENGTH+KILOBYTE
#define TOKENS (MAX_LETTERS_AMOUNT*KEYS*2)+MAX_LETTERS_AMOUNT+1
#define MAIL_LOCATION "..\\core\\mail.json"
#define BITMAP_LOCATION "..\\core\\bitmap.map"
#define SUCCESS 0
#define ERROR -1

char *_mail, *_bmp;
char *mail_buf, *btmp_buf;


int mail_size;

jsmn_parser parser;
jsmntok_t tokens[TOKENS];

int alloc_mem(char* buf, FILE* infile)
{
 if (infile == NULL) return ERROR;

 fseek(infile, 0L, SEEK_END);
 long numbytes = ftell(infile);
 fseek(infile, 0L, SEEK_SET);

 buf = (char *) calloc(numbytes, sizeof(char));
 return numbytes;
}

void free_mem(char* buf)
{
 free(buf);
 buf = NULL;
}

int read_file(char* buf, char *path) // Check if buf is null before calling !
{
 FILE *infile;
 infile = fopen(path, "r");

 long numbytes = alloc_mem(buf, infile);

 if (buf == NULL || numbytes == ERROR) return ERROR;

 fread(buf, sizeof(char), numbytes, infile);
 fclose(infile);
 return SUCCESS;
}

int parse_json(char *json_, jsmn_parser parser_, jsmntok_t *tokens_, char *path_) // Returns json
{
 read_file(json_, path_);
 jsmn_init(&parser_);

 int json_size_ = jsmn_parse(&parser_, json_, strlen(json_), tokens_, TOKENS);

 if (json_size_ < 0) {printf("Failed to parse JSON: {%d\n}", json_size_);return ERROR;}
 return json_size_;
}

int load_mail(char* mail) // Returns mail size
{
 int r = parse_json(mail, parser, tokens, MAIL_LOCATION);
 return r;
}

void foo(char* mail)
{

}

int main()
{
 printf("Bonjour\n");
 foo(NULL);
}