#include <iostream>

#include "../include/LSH.h"
#include "../include/hashTable.h"

using namespace std;

LSH::LSH(int k, int L, int N, Data &data, uint32_t w, uint32_t m)
    : k(k), L(L), N(N), data(data), w(w), m(m)
{
    this->M = uint32_t(pow(2, 32 / this->k));

    this->md.resize(this->data.d, 0);
    this->md[1] = this->m % this->M;

    this->tables.resize(this->L, hashTable(this->data.n / 16, this->k, this->data.d, this->w));

    cout << "m: " << this->m << " M: " << this->M << endl;
    cout << this->md.size() << " " << this->md[1] << endl;
}

LSH::~LSH() {}

int LSH::Run()
{
    hashData();

    return 0;
}

void LSH::hashData()
{
    for (int i = 0; i < this->L; i++)
    {
        for (int j = 0; j < this->data.n; j++)
        {
            uint32_t g = this->calculate_g(this->data.data[j], this->tables[i].S);

            cout << "g(x) = " << g << endl;

            // store image in HashTables[i][g]
        }
    }
}

uint32_t LSH::calculate_g(const vector<uint8_t> &x, const vector<vector<int>> &S)
{
    uint32_t g = 0;

    for (int i = 0; i < this->k; i++)
    {
        g = g << 32 / this->k;
        g = g | this->calculate_h(x, S[k]);
    }

    return g;
}

uint32_t LSH::calculate_h(const vector<uint8_t> &x, const vector<int> &s)
{
    uint32_t h = uint32_t(calculate_a(x[this->data.d - 1], s[this->data.d - 1])) % this->M;

    for (int i = this->data.d - 2; i >= 0; i--)
    {
        if (this->md[this->data.d - 1 - i] == 0)
        {
            this->md[this->data.d - 1 - i] = (this->md[this->data.d - 2 - i] * this->md[1]) % this->M;
        }

        int a = calculate_a(x[i], s[i]);
        if (a != 0) // saving some compute time
        {
            h += (((a % this->M) * this->md[this->data.d - 1 - i]) % this->M) % this->M;
        }
    }

    return h % this->M;
}

int LSH::calculate_a(const uint8_t &xi, const int &si)
{
    return floor(double((int(xi) - si)) / double(this->w));
}