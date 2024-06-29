#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

bool compare(const long &left, const long &right) {
    return left > right;
}

vector<long> get_divisors(long x) {
    vector<long> divisors;
    long i = 1;
    while (i * i <= x) {
        if (x % i == 0) {
            divisors.push_back(i);
            if (x / i != i) {
                divisors.push_back(x / i);
            }
        }
        i++;
    }
    return divisors;
}

bool between(long a, long b) {
    long i = 2;
    while (i * i <= (a / b)) {
        if ((a / b) % i == 0) {
            return false;
        }
        i++;
    }
    return true;
}

void display_graph(vector<long> divisors, long len) {
    cout << "graph {" << endl;
    long i = 0;
    while (i < len) {
        cout << "\t" << divisors[i] << endl;
        i++;
    }
    i = 0;
    int j;
    while (i < len) {
        j = i + 1;
        while (j < len) {
            if (divisors[i] % divisors[j] == 0 && between(divisors[i], divisors[j])) {
                cout << "\t" << divisors[i] << "--" << divisors[j] << endl;
            }
            j++;
        }
        i++;
    }
    cout << "}" << endl;
}

int main() {
    long n;
    cin >> n;
    vector<long> divisors = get_divisors(n);
    sort(divisors.begin(), divisors.end(), compare);
    display_graph(divisors, divisors.size());
    return 0;
}
