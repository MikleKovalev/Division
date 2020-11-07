#include <iostream>
#include <string>
#include <vector>
#include <chrono>

using namespace std;

const int BASE = 10;

struct Division {
    vector<int> quotient;
    vector<int> remainder;
};

bool check(const string& number) {
    for (int i = 0; i < number.size(); ++i) {
        if (number[i] < '0' || number[i] > '9') {
            return false;
        }
    }
    return true;
}

void fill(const string& s, vector<int>& d) {
    d = vector<int>(s.size());
    for (int i = int(s.size() - 1); i >= 0; --i) {
        d[s.size() - i - 1] = s[i] - '0';
    }
    while (d.size() > 1 && d.back() == 0) {
        d.pop_back();
    }
}

void print(const vector<int>& data) {
    for (int i = int(data.size() - 1); i >= 0; --i) {
        cout << data[i];
    }
    cout << endl;
}

void multiply(vector<int>& u, int v) {
    int carry = 0;
    for (int & i : u) {
        int temp = i * v + carry;
        i = temp % BASE;
        carry = temp / BASE;
    }
    if (carry > 0) {
        u.push_back(carry);
    }
}

bool is_less(const vector<int>& u, const vector<int>& v) {
    if (u.size() < v.size()) {
        return true;
    }
    if (u.size() > v.size()) {
        return false;
    }
    for (int i = int(u.size() - 1); i >= 0; --i) {
        if (u[i] < v[i]) {
            return true;
        } else if (u[i] > v[i]) {
            return false;
        }
    }
    return false;
}

void subtraction(vector<int>& u, vector<int>& v, int n, int j) {
    int carry = 0;
    for (int i = 0; i < n; ++i) {
        u[i + j] = u[i + j] - (i < v.size() ? v[i] : 0) - carry;
        carry = u[i + j] < 0;
        if (carry) {
            u[i + j] += BASE;
        }
    }
}

void addition(vector<int>& u, int n, int j) {
    int carry = 0;
    for (int i = 0; i < n; ++i) {
        int current_id = i + j;
        u[current_id] = -u[current_id] - carry;
        carry = u[current_id] < 0;
        if (carry) {
            u[current_id] += BASE;
        }
    }
}

void add(vector<int>& u, const vector<int>& v, int n, int j) {
    int carry = 0;
    for (int i = 0; i < n; ++i) {
        int temp = u[i + j] + v[i] + carry;
        u[i + j] = temp % BASE;
        carry = temp / BASE;
    }
}

int division(vector<int>& u, int v) {
    int carry = 0;
    for (int i = int(u.size() - 1); i >= 0; --i) {
        long long temp = u[i] + carry * 1ll * BASE;
        u[i] = (int) (temp / v);
        carry = (int) (temp % v);
    }
    while (u.size() > 1 && u.back() == 0) {
        u.pop_back();
    }
    return carry;
}

Division division(vector<int>& u, vector<int>& v) {
    int n = v.size();
    int m = int(u.size() - v.size());
    Division result;
    result.quotient = vector<int>(m + 1);
    result.remainder = vector<int>(n);
    for (int i = 0; i < n; ++i) {
        result.remainder[i] = u[i];
    }
    int d = BASE / (v[n - 1] + 1);
    multiply(u, d);
    multiply(v, d);
    if (u.size() == n + m) {
        u.push_back(0);
    }
    for (int j = m; j >= 0; --j) {
        int temp = (u[j + n] * BASE + u[j + n - 1]);
        int q_hat = temp / v[n - 1];
        int r_hat = temp % v[n - 1];
        while (q_hat == BASE || q_hat * v[n - 2] > r_hat * BASE + u[j + n - 2]) {
            --q_hat;
            r_hat += v[n - 1];
        }
        bool negative = is_less(u, v);
        vector<int> v_copy = v;
        multiply(v_copy, q_hat);
        subtraction(u, v_copy, n, j);
        if (negative) {
            addition(u, n + 1, j);
        }
        result.quotient[j] = q_hat;
        if (negative) {
            --result.quotient[j];
            v_copy = v;
            v_copy.push_back(0);
            add(u, v_copy, n, j);
        }
    }
    division(result.remainder, d);
    return result;
}

int main() {
    while (true) {
        string a, b;
        cin >> a;
        if (a == "q") {
            break;
        }
        cin >> b;
        auto start = chrono::high_resolution_clock::now();
        if (!check(a)) {
            cout << "First input is not a number." << endl;
            continue;
        }
        if (!check(b)) {
            cout << "Second input is not a number." << endl;
            continue;
        }
        if (b.size() == 1) {
            if (b[0] == '0') {
                cout << "Division by zero." << endl;
            } else {
                vector<int> u;
                fill(a, u);
                int remainder = division(u, b[0] - '0');
                cout << "Quotient: ";
                print(u);
                cout << "Remainder: " << remainder << endl;
                auto end = chrono::high_resolution_clock::now();
                cout << "Time: " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << " ns." << endl;
            }
        } else {
            vector<int> u, v;
            fill(a, u);
            fill(b, v);
            if (is_less(u, v)) {
                cout << "Quotient: 0" << endl;
                cout << "Remainder: ";
                print(u);
                auto end = chrono::high_resolution_clock::now();
                cout << "Time: " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << " ns." << endl;
                continue;
            }
            if (u.size() == 1 && u[0] == 0) {
                cout << "Quotient: 0" << endl << "Remainder: 0" << endl;
                auto end = chrono::high_resolution_clock::now();
                cout << "Time: " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << " ns." << endl;
                continue;
            }
            Division result = division(u, v);
            cout << "Quotient: ";
            print(result.quotient);
            cout << "Remainder: ";
            print(result.remainder);
            auto end = chrono::high_resolution_clock::now();
            cout << "Time: " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << " ns." << endl;

        }
    }
    return 0;
}
