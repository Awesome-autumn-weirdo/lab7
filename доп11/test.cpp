#include <stdio.h>

int sum_array(int* arr, int n) {
    int sum = 0;
    for (int i = 0; i < n; ++i)
        sum += arr[i];
    return sum;
}

int main() {
    int data[5] = {1, 2, 3, 4, 5};
    int total = sum_array(data, 5);
    printf("Sum = %d\n", total);
    return 0;
}
