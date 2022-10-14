#include <stdio.h>
#include <string.h>
int main () {

    char s[] = "        label       ";
    char b[] = "a";
    sscanf (s, "%*s %s", b);
    printf ("%s %d", b, strcmp (b, "a"));
}