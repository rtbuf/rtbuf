
#ifndef STRING7_H
#define STRING7_H

typedef char string7;

unsigned char string7_len (string7 *s);
unsigned char string7__len (string7 *s);
char string7_p (string7 *s);
char string7_eq (string7 *a, string7 *b);
char string7_lt (string7 *a, string7 *b);
char string7_lte (string7 *a, string7 *b);
char string7_gt (string7 *a, string7 *b);
char string7_gte (string7 *a, string7 *b);
char string7_compare (string7 *a, string7 *b);
char string7_char (char c, string7 *s);
char string7_string7 (string7 *t, string7 *s);
char string7_string7_len (string7 *t, string7 *s, unsigned char l);
char string7_nth (string7 *s, unsigned char n);

#endif /* STRING7_H */
