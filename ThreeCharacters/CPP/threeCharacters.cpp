#include<iostream>

using namespace std;

int calculateCount(int);
int main(int argc, char** argv) {
    int n;
    cin >> n;

    int accumulation = calculateCount(n);
    cout << accumulation << endl;
    return 0;
}

int calculateCount(int n) {
    int accumulation = 0;
    int dp0 = 3, dp1 = 0;

    for(int i = 1; i < n; i++) {
        int temp = dp0;
        dp0 = 2*dp0 + 2*dp1;
        dp1 = temp;
    }
    
    accumulation = dp0+dp1;

    return accumulation;
}
