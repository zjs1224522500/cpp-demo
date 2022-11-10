#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define judge(n) ((n >= 0 && n <= 255) ? 1 : 0)
int main()
{
    char s[200];
    while (1)
    {
        fgets(s, 200, stdin);
        s[strlen(s) - 1] = 0;
        if (strcmp(s, "End of file") == 0)
            break;
        char q[] = ".";

        char *a = strtok(s, q);
        char *b = strtok(NULL, q);
        char *c = strtok(NULL, q);
        char *d = strtok(NULL, q);
        int aa = atoi(a);
        int bb = atoi(b);
        int cc = atoi(c);
        int dd = atoi(d);
        if (judge(aa) && judge(bb) && judge(cc) && judge(dd))
        {
            printf("YES\n");
        }
        else
        {
            printf("NO\n");
        }
    }
    return 0;
}