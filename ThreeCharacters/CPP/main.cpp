#include <iostream>
#include "PowMatrix.h"

using namespace std;

int main() {
    int n;
    cin >> n;

/*
// Method 1
    int dp[n][2];
    dp[0][0] = 3;
    dp[0][1] = 0;

    for(int i = 1; i < n; i++) {
        dp[i][0] = dp[i-1][0]*2 + dp[i-1][1]*2;
        dp[i][1] = dp[i-1][0];
    }

    int sum = dp[n-1][0] + dp[n-1][1];
*/

/*
// Method 2
    int dp0 = 3;
    int dp1 = 0;

    for(int i = 1; i < n; i++) {
        int temp = dp0;
        dp0 = 2*dp0 + 2*dp1;
        dp1 = temp;
    }

    int sum = dp0+dp1;
*/


// Method 3
    int dimension = 2;
    int matrix[2*2] = {2,2,1,0};

    PowMatrix powMatrix(dimension);
    powMatrix.setMatrix(matrix);

    int count = n-1;
    powMatrix.pow(count);
    int* result = powMatrix.getResult();

    int sum = 3 * result[0] + 3 * result[2];

    cout << sum << endl;
    return 0;
}
