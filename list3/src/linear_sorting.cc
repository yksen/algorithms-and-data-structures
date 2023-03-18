#include <iostream>
using namespace std;

template <class T>
T array_maximum(T t[], int n)
{
    T max = t[0];
    while (--n)
        if (t[n] > max)
            max = t[n];
    return max;
}

template <class T>
T array_minimum(T t[], int n)
{
    T min = t[0];
    while (--n)
        if (t[n] < min)
            min = t[n];
    return min;
}

template <class T>
void print(T t[], int n)
{
    for (int i = 0; i < n; i++)
        cout << t[i] << ' ';
    cout << endl;
}

template <class T>
void insertion_sort(T t[], int n)
{
    int i;
    for (int k = 1; k < n; k++) // każdy element tablicy począwszy od drugiego
    {
        T x = t[k];                             // zapamiętaj w zmiennej x
        for (i = k; i > 0 && t[i - 1] > x; i--) // poprzedzające go większe elementy
            t[i] = t[i - 1];                    // przesuń w prawo
        t[i] = x;                               // i umieść x tuż przed nimi
    }
}

template <class T, class P>
void counting_sort(T t[], int n, int m, P key)
{
    int count[m] = {0}; // tablica liczników
    T *b = new T[n];    // przydzielenie pamięci na bufor

    for (int i = 0; i < n; ++i)
        count[key(t[i])]++; // zliczanie: count[k] = ilość kluczy równych k

    for (int k = 1; k < m; ++k)
        count[k] += count[k - 1]; // akumulacja: count[k] = ilość kluczy mniejszych bądź równych k
    // więc w posortowanej tablicy elementy o kluczach równych k powinny kończyć się na pozycji count[k]-1

    for (int i = n - 1; i >= 0; --i)  // wkładanie do kolejek: idziemy po tablicy od końca
        b[--count[key(t[i])]] = t[i]; // bo kolejki wypełniamy też od końca

    for (int i = 0; i < n; ++i)
        t[i] = b[i]; // przepisanie z bufora do tablicy wyjściowej

    delete[] b; // zwolnienie bufora z pamięci
}

template <class T>
void radix_sort(T t[], int n)
{
    T max = array_maximum(t, n);
    int m = 10; // może być też np. `int m = ceil(sqrt(max)+1);` żeby były tylko 2 przebiegi
    for (int r = 1; r <= max; r *= m)
        counting_sort(t, n, m, [r, m](int x)
                      { return x / r % m; }); // sortowanie względem cyfry r-ek
}

template <class T>
void bucket_sort(T t[], int n)
{
    double min = array_minimum(t, n);
    double max = array_maximum(t, n);
    max += (max - min) / 100;
    int m = n / 3 + 1; // tak dobieramy ilość kawałków by w każdym znalazły się np 3 liczby
    counting_sort(t, n, m, [min, max, m](T x)
                  { return int((x - min) / (max - min) * m); });
    insertion_sort(t, n);
}

int main()
{
    srand(time(nullptr));
    int n = 10000;
    double *t = new double[n];
    for (int i = 0; i < n; i++)
        t[i] = random() % 1000;
    insertion_sort(t, n);
    // counting_sort(t, n, 1000, [](int x){ return x; });
    // radix_sort(t,n);
    // for (int i = 0; i < n; i++)
    //     t[i] = random() * 0.001;
    // bucket_sort(t,n);
    print(t, n);
}
