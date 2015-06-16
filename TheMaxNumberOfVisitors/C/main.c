#include <stdio.h>
#include <stdlib.h>

#define MAXPEOPLE 10000
#define MAXTIME 10000
int main()
{
    int come[MAXPEOPLE] = {0}, leave[MAXPEOPLE] = {0};
    int count = 0;

    int i;
    for(i = 0; ; i++) {
        scanf("%d", &come[i]);
        scanf("%d", &leave[i]);
        if(come[i] == 0 && leave[i] == 0 || i >= 10000) {
            break;
        }
        count++;
    }

    int hashCome[MAXTIME] = {0}, hashLeave[MAXTIME] = {0};
    for(i = 0; i < count; i++) {
        hashCome[ come[i] ]++;
        hashLeave[ leave[i] ]++;
    }

    int max = hashLeave[0]-hashCome[0];
    int sum = 0;
    for(i = 0; i < MAXTIME; i++) {
        sum += hashLeave[i]-hashCome[i];
        if( sum > max ) {
            max = sum;
        }
        if(sum < 0) {
            sum = 0;
        }
    }

    printf("%d\n", sum);
    return 0;
}
