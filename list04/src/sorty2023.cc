#include <algorithm>
#include <cmath>
#include <cstring>
#include <iostream>
#include <sys/time.h>
#include <sys/timeb.h>

using namespace std;

const int N = 60 * 1000 * 1000 / 3;
// const int N=600*5000/3;
//  const int N=15;

// POMIAR CZASU
double stoper()
{
    static double t = 0;
    double t1 = t;
    struct timeval tv;
    gettimeofday(&tv, NULL);
    t = tv.tv_sec + 0.000001 * tv.tv_usec;
    return t - t1;
}

// INSERTION SORT (with std::swap)
void insertion_sort_swap(double t[], int n)
{
    for (int k = 1; k < n; k++)
        for (int i = k; i > 0 && t[i - 1] > t[i]; i--)
            std::swap(t[i - 1], t[i]);
}

// INSERTION SORT (STANDARD)
inline void insertion_sort(double t[], int n)
{
    int i;
    for (int k = 1; k < n; k++)
    {
        double x = t[k];
        for (i = k; i > 0 && t[i - 1] > x; i--)
            t[i] = t[i - 1];
        t[i] = x;
    }
}

// INSERTION SORT SENTINEL (pseudo)

inline void insert3(double *t, double *k)
{
    double x = *k;
    if (x < t[0]) // jeśli x mniejszy od wszystkich poprzednich elementów:
    {
        while (--k >= t) // każdy element tablicy
            k[1] = k[0]; // przesuń o 1 w prawo
        t[0] = x;        // i wstaw x na początku tablicy
    }
    else // w przeciwnym razie wstaw bez sprawdzania zakresu:
    {
        while (*--k > x) // dopóki poprzedni element jest większy od x
            k[1] = k[0]; // przesuń go w na "puste" miejsce i idź w lewo
        k[1] = x;        // postaw x na pustym miejscu
    }
}

inline void insert4(double *t, double *k)
{
    double x = *k;
    if (x < *t) // jeśli x mniejszy od wszystkich poprzednich elementów:
    {
        memmove(t + 1, t, (char *)k - (char *)t);
        *t = x; // i wstaw x na początku tablicy
    }
    else // w przeciwnym razie wstaw bez sprawdzania zakresu:
    {
        while (*(k - 1) > x) // dopóki poprzedni element jest większy od x
        {
            *k = *(k - 1); // przesuń go w na "puste" miejsce i idź w lewo
            --k;
        }
        *k = x; // postaw x na pustym miejscu
    }
}

void insertion_sort_sentinel3(double t[], int n)
{
    for (int i = 1; i < n; i++)
        insert3(t, t + i);
}

void insertion_sort_sentinel3(double *begin, double *end)
{
    for (double *i = begin + 1; i < end; ++i)
        insert3(begin, i);
}

void insertion_sort_sentinel4(double *begin, double *end)
{
    for (double *i = begin + 1; i < end; ++i)
        insert4(begin, i);
}

// INSERTION SORT SENTINEL

inline void insert_with_sentinel(double *end, double x)
{
    while (*--end > x)
        end[1] = end[0];
    end[1] = x;
}

inline void insert_with_sentinel0(double *end, double x)
{
    for (double *prev = end - 1; *prev > x; --prev, --end)
        *end = *prev;
    *end = x;
}

void insertion_sort_sentinel(double *begin, double *end, double *target = nullptr)
{
    double *j = end - 1;
    double x = *j;
    target || (target = begin); // domyślnie sortujemy w miejscu
    // ustaw najmniejszy na początku
    while (--j >= begin)
        if (*j > x)
            j[1] = *j;
        else
        {
            j[1] = x;
            x = *j;
        }
    *target = x;
    //  if(end-begin==1) return;
    // posortuj pozostałe elementy traktując target[0] jako wartownika
    j = begin;
    *++target = *++j;
    while (++j < end)
        insert_with_sentinel(++target, *j);
}

void insertion_sort_sentinel0(double *begin, double *end)
{
    double *m = begin;
    double *j = begin;
    while (++j < end)
        if (*j < *m)
            m = j;
    std::swap(*begin, *m);
    ++begin;
    while (++begin < end)
        insert_with_sentinel(begin, *begin);
}

void insertion_sort_sentinel00(double *begin, double *end)
{
    double *m = begin;
    double *j = begin;
    while (++j < end)
        if (*j < *m)
            m = j;
    std::swap(*begin, *m);
    ++begin;
    while (++begin < end)
    {
        double x = *begin;
        for (m = j = begin; *--j > x; --m)
            *m = *j;
        *m = x;
    }
}

// HEAP SORT

void sift_down(double t[], int n, int i)
{ /*
      Przesiej element t[i] w dół kopca:
      t[2*i+1] - lewe dziecko t[i]
      t[2*i+2] - prawe dziecko t[i]
      jeśli któreś z dzieci jest większe od t[i]:
        - zamień t[i] z tym dzieckiem miejscami
        - sprawdź ponownie t[i] w nowym miejscu ...
  */
    int k = i;
    double x = t[i];
    while ((((k += k + 2) < n) && (t[k - 1] < t[k] || --k < n)) && x < t[k]) // x mniejszy od większego syna
    {
        t[i] = t[k];
        i = k;
    }
    t[i] = x;
}

void heap_sort(double t[], int n)
{
    // Budowanie kopca
    for (int i = n / 2 - 1; i >= 0; i--)
        sift_down(t, n, i);

    // Opróżnianie kopca jako kolejki priorytetowej
    while (--n)
    {
        std::swap(t[0], t[n]);
        sift_down(t, n, 0);
    }
}

// heap sort na wskaźnikach (wersja prawie wg Cormena)
void sift_down2a(double *t, double *k, double *i)
{ // trochę  szybszy niż 2b
    double *s = i;
    double x = *i; // przesiewana wartość
    // s=(prawy syn, jeśli istnieje i jest większy od lewego, w przeciwnym razie lewy jeśli istnieje)
    while ((((s += s - t + 2) < k) && ((*(s - 1) < *s || --s) || --s < k)) && x < *s) // i x mniejszy od wartości s (większego z synów)
    {
        *i = *s; // przesuń klucz syna wyżej
        i = s;   // przejdź na pozycję syna
    }
    *i = x; // wstaw x na właściwe miejsce
}

void heap_sort2a(double *t, double *k)
{
    // Budowanie kopca
    double *i = t + (k - t) / 2;
    while (i != t)
        sift_down2a(t, k, --i);

    // Opróżnianie kopca jako kolejki priorytetowej
    while (--k != t)
    {
        std::swap(*t, *k);
        sift_down2a(t, k, t);
    }
}
void heap_sort2aa(double *begin, double *end)
{
    double *i = begin + (end - begin) / 2;

    // po tej pętli jedno przesiej brakuje do zbudowania kopca
    while (--i != begin)
        sift_down2a(begin, end, i);

    // pierwsze przesiej tej pętli kończy budować kopiec
    while (end != begin)
    {
        sift_down2a(begin, end, begin); // przesiej w dół element na szczycie
        std::swap(*begin, *--end);      // zamień pierwszy z ostatnim i zmniejsz kopiec
    }
    // ale nie wykonuje sie zbędne przesiej dla kopca o jednym elemencie
}

// heap sort na wskaźnikach (wersja "prawie" wg Cormena)

template <class T>
void sift_down2b(T *begin, T *end, T *i)
{             // najkrótsza procedura
    T *s = i; // będzie wskazywał na większego syna
    T x = *i; // przesiewana wartość

    while ((((s += s - begin + 2) < end) && (*(s - 1) < *s || --s < end)) // dopóki jest co najmniej jeden syn
           && x < *s)                                                     //  i x jest mniejszy od większego syna
    {
        *i = *s; // kopiuj klucz syna wyżej
        i = s;   // przejdź na pozycję syna
    }
    *i = x; // wstaw x na właściwe miejsce
}

template <class T>
void heap_sort1(T *begin, T *end, void (*sift_down)(T *, T *, T *))
{
    // Budowanie kopca
    T *i = begin + (end - begin) / 2;
    while (i != begin)
        sift_down(begin, end, --i);

    // Opróżnianie kopca jako kolejki priorytetowej
    while (--end != begin)
    {
        std::swap(*begin, *end);
        sift_down(begin, end, begin);
    }
}

template <class T>
void sift_down2c(T *begin, T *end, T *i)
{
    T *s = i; // będzie wskazywał na większego syna
    T x = *i; // przesiewana wartość
    while (true)
    {
        s += s - begin + 2; // wskaż na prawego syna
        if (s >= end)
        {                   // jeśli nie ma prawego syna
            if (--s >= end) // wskaż na lewego
                break;      // jeśli nie ma lewego syna to zakończ
        }
        else                   // jeśli jednak jest prawy syn
            if (*(s - 1) > *s) // ale lewy jest większy
                --s;           // to też wskaż na lewego

        if (x >= *s) // jeśli większy sym nie jest większy od ojca
            break;   // zakończ

        *i = *s; // kopiuj klucz większego syna wyżej
        i = s;   // przejdź na pozycję większego syna
    }
    *i = x; // wstaw x na właściwe miejsce
}

void heap_sort2bb(double *t, double *k)
{

    double *i = t + (k - t) / 2;

    // po tej pętli jedno przesiej brakuje do zbudowania kopca
    while (--i != t)
        sift_down2b(t, k, i);

    // pierwsze przesiej tej pętli kończy budować kopiec
    while (k != t)
    {
        sift_down2b(t, k--, t);
        std::swap(*t, *k);
    }
    // ale nie wykonuje sie zbędne przesiej dla kopca o jednym elemencie
}

void heap_sort2cc(double *t, double *k)
{

    double *i = t + (k - t) / 2;

    // po tej pętli jedno przesiej brakuje do zbudowania kopca
    while (--i != t)
        sift_down2c(t, k, i);

    // pierwsze przesiej tej pętli kończy budować kopiec
    while (k != t)
    {
        sift_down2c(t, k--, t);
        std::swap(*t, *k);
    }
    // ale nie wykonuje sie zbędne przesiej dla kopca o jednym elemencie
}

// MERGE SORT (simple wersion)

double b[N];

void merge(int n, int k, double t[], double b[])
{
    int i = 0, j = k, l = 0;
    while (i < k && j < n)
        if (t[i] <= t[j])
            b[l++] = t[i++];
        else
            b[l++] = t[j++];
    while (i < k)
        t[--j] = t[--k];
    for (i = 0; i < j; i++)
        t[i] = b[i];
}

void merge_sort(double t[], int n)
{
    if (n > 1)
    {
        int k = n / 2;
        merge_sort(t, k);
        merge_sort(t + k, n - k);
        merge(n, k, t, b);
    }
}

// MERGE SORT 2 (bez niepotrzebnego kopiowania)

void merge2(int n, int k, double t[], double b[])
{
    int i = 0, j = k, l = 0;
    while (i < k && j < n)
        if (t[i] <= t[j])
            b[l++] = t[i++];
        else
            b[l++] = t[j++];
    while (i < k)
        b[l++] = t[i++];
    while (j < n)
        b[l++] = t[j++];
}

void copying_merge_sort2(double t[], int n, double b[]);

void merge_sort2(double t[], int n, double b[])
{
    if (n > 1)
    {
        int k = n / 2;
        // [9, 8, 2, 3, 5, 1, 6, 4]
        // [ ,  ,  ,  ,  ,  ,  ,  ]
        copying_merge_sort2(t, k, b);
        // [ ,  ,  ,  , 5, 1, 6, 4]
        // [2, 3, 8, 9,  ,  ,  ,  ]
        copying_merge_sort2(t + k, n - k, b + k);
        // [ ,  ,  ,  ,  ,  ,  ,  ]
        // [2, 3, 8, 9| 1, 4, 5, 6]
        merge2(n, k, b, t);
        // [1, 2, 3, 4, 5, 6, 8, 9]
        // [ ,  ,  ,  ,  ,  ,  ,  ]
    }
}

void copying_merge_sort2(double t[], int n, double b[])
{
    if (n > 1)
    {
        int k = n / 2;
        // [9, 8, 2, 3, 5, 1, 6, 4]
        // [ ,  ,  ,  ,  ,  ,  ,  ]
        merge_sort2(t, k, b);
        // [2, 3, 8, 9| 5, 1, 6, 4]
        // [ ,  ,  ,  ,  ,  ,  ,  ]
        merge_sort2(t + k, n - k, b + k);
        // [2, 3, 8, 9| 1, 4, 5, 6]
        // [ ,  ,  ,  ,  ,  ,  ,  ]
        merge2(n, k, t, b);
        // [ ,  ,  ,  ,  ,  ,  ,  ]
        // [1, 2, 3, 4, 5, 6, 8, 9]
    }
    else
        b[0] = t[0];
}

// MERGE SORT based on half_copying_merge

inline void half_copying_merge(double t1[], int k1, double t[], int k)
{
    /* pomocnicza procedura HALF-COPYING MERGE
       złącza dwie posortowane tablice w jedną
       t1= [1, 3, 5, 9]              // t1 zawiera k1 liczb (rosnąco)
       t = [ ,  ,  ,  , 2, 4, 6, 8]  // t ma na początku k1 miejsc pustych a potem k-k1 liczb (rosnąco)
       ============================  // po zakończeniu procedury
       t = [1, 2, 3, 4, 5, 6, 8, 9]  // t zawiera złączone zbiory liczb (rosnąco)
    */
    int j = k1, i = 0, l = 0;
    for (;;)
    {
        if (t1[i] <= t[j])
        {
            t[l++] = t1[i++];
            if (i == k1)
                return;
        }
        else
        {
            t[l++] = t[j++];
            if (j == k)
                break;
        }
    }
    do
        t[l++] = t1[i++];
    while (i != k1);
}

void copying_merge_sort(double t[], int n, double b[]);

void inplace_merge_sort(double t[], int n, double b[])
{
    if (n > 16)
    {
        copying_merge_sort(t, n / 2, b);
        inplace_merge_sort(t + n / 2, n - n / 2, t);
        half_copying_merge(b, n / 2, t, n);
    }
    else
        insertion_sort_sentinel(t, t + n);
}

void copying_merge_sort(double t[], int n, double b[])
{
    if (n > 16)
    {
        copying_merge_sort(t + n / 2, n - n / 2, b + n / 2);
        inplace_merge_sort(t, n / 2, t + n / 2);
        half_copying_merge(t, n / 2, b, n);
    }
    else
        insertion_sort_sentinel(t, t + n, b);
}

void hc_merge_sort(double t[], int n)
{
    if (n > 1)
    {
        double *b = new double[n / 2];

        copying_merge_sort(t, n / 2, b);
        inplace_merge_sort(t + n / 2, n - n / 2, t);
        half_copying_merge(b, n / 2, t, n);

        delete b;
    }
}

// MERGE SORT based on half_copying_merge (na wskaźnikach)

inline void half_copying_merge(double *begin1, double *end1, double *begin, double *end)
{                                             /*
                                                 Procedura HALF-COPYING MERGE złącza dwie posortowane tablice w jedną
                                                                           // początkowo:
                                                 [1, 3, 5, 9]              // [begin1,end1) zawiera k1 liczb (rosnąco)
                                                 [ ,  ,  ,  , 2, 4, 6, 8]  // [begin,end) ma k1 miejsc pustych i k2 liczb (rosnąco)
                                                 ========================  // po zakończeniu:
                                                 [1, 2, 3, 4, 5, 6, 8, 9]  // [begin,end) zawiera k1+k2 liczb (rosnąco)
                                              */
    double *begin2 = begin + (end1 - begin1); // oblicz gdzie zaczyna się prawa część
    for (;;)
    {
        if (*begin1 <= *begin2) // porównujemy początkowe liczby oby tablic
        {
            *begin++ = *begin1++; // i mniejszą przepisujemy do tablicy docelowej
            if (begin1 == end1)   // gdy doszliśmy do końca lewej części
                return;           // wszystko jest już na miejscu i kończymy algorytm.
        }
        else
        {
            *begin++ = *begin2++; // gdy mniejszą przepisujemy z prawej części
            if (begin2 == end)    // i doszliśmy do końca prawej części
                break;
        }
    }
    do                        // musimy jeszcze
        *begin++ = *begin1++; // przepisać resztę lewej części do tablicy wynikowej
    while (begin1 != end1);
}

void copying_merge_sort(double *begin, double *end, double *target);

void inplace_merge_sort(double *begin, double *end, double *buffer)
{
    if (end - begin > 6)
    {
        double *middle = begin + (end - begin) / 2;
        // [9, 8, 2, 3, 5, 1, 6, 4]
        // [ ,  ,  ,  ]
        copying_merge_sort(begin, middle, buffer);
        // [ ,  ,  ,  , 5, 1, 6, 4]
        // [2, 3, 8, 9]
        inplace_merge_sort(middle, end, begin);
        // [ ,  ,  ,  , 1, 4, 5, 6]
        // [2, 3, 8, 9]
        half_copying_merge(buffer, buffer + (middle - begin), begin, end);
        // [1, 2, 3, 4, 5, 6, 8, 9]
        // [ ,  ,  ,  ]
    }
    else
        insertion_sort_sentinel(begin, end);
}

void copying_merge_sort(double *begin, double *end, double *target)
{
    if (end - begin > 6)
    {
        double *middle = begin + (end - begin) / 2;
        // [9, 8, 2, 3, 5, 1, 6, 4]
        // [ ,  ,  ,  ,  ,  ,  ,  ]
        inplace_merge_sort(begin, middle, target);
        // [2, 3, 8, 9, 5, 1, 6, 4]
        // [ ,  ,  ,  ,  ,  ,  ,  ]
        copying_merge_sort(middle, end, target + (middle - begin));
        // [2, 3, 8, 9,  ,  ,  ,  ]
        // [ ,  ,  ,  , 1, 4, 5, 6]
        half_copying_merge(begin, middle, target, target + (end - begin));
        // [ ,  ,  ,  ,  ,  ,  ,  ]
        // [1, 2, 3, 4, 5, 6, 8, 9]
    }
    else
        insertion_sort_sentinel(begin, end, target);
}

void HC_merge_sort(double *begin, double *end)
{
    if (end - begin > 6)
    {
        double *buffer = new double[(end - begin) / 2 + 1];

        double *middle = begin + (end - begin) / 2;
        // [9, 8, 2, 3, 5, 1, 6, 4]
        // [ ,  ,  ,  ]
        copying_merge_sort(begin, middle, buffer);
        // [ ,  ,  ,  , 5, 1, 6, 4]
        // [2, 3, 8, 9]
        inplace_merge_sort(middle, end, begin);
        // [ ,  ,  ,  , 1, 4, 5, 6]
        // [2, 3, 8, 9]
        half_copying_merge(buffer, buffer + (middle - begin), begin, end);
        // [1, 2, 3, 4, 5, 6, 8, 9]
        // [ ,  ,  ,  ]
        delete buffer;
    }
    else
        insertion_sort_sentinel(begin, end);
}

// QUICK SORT
int partition(double t[], int n)
{
    // Wersja wg Hoare'a
    int k = -1;
    double x = t[n / 2]; // pivot bierzemy pośrodku
    for (;;)
    {
        do
            ++k;
        while (t[k] < x);
        do
            --n;
        while (t[n] > x);
        if (k < n)
            std::swap(t[k], t[n]);
        else
            return k;
    }
}

void quick_sort(double t[], int n)
{
    if (n > 1)
    {
        int k = partition(t, n);  // podziel na dwie części
        quick_sort(t, k);         // posortuj lewą
        quick_sort(t + k, n - k); // posortuj prawą
    }
}

void quick_sort6(double t[], int n)
{
    if (n > 16)
    {
        int k = partition(t, n);   // podziel na dwie części
        quick_sort6(t, k);         // posortuj lewą
        quick_sort6(t + k, n - k); // posortuj prawą
    }
    else
        insertion_sort_sentinel3(t, n);
}

// QUICK SORT (na wskaźnikach)

inline double *partition(double *begin, double *end)
{
    // Wersja wg Hoare'a
    double x = begin[(end - begin) / 2]; // pivot bierzemy pośrodku
    for (;;)
    {
        while (*begin < x)
            ++begin;
        do
            --end;
        while (*end > x);
        if (begin < end)
            std::swap(*begin++, *end);
        else
            return begin;
    }
}

inline void quick_sort(double *begin, double *end)
{
    if (end - begin > 1)
    {
        double *middle = partition(begin, end); // podziel na dwie części
        quick_sort(begin, middle);              // posortuj lewą
        quick_sort(middle, end);                // posortuj prawą
    }
}

inline void quick_sort6(double *begin, double *end)
{
    if (end - begin > 17)
    {
        double *middle = partition(begin, end); // podziel na dwie części
        quick_sort6(begin, middle);             // posortuj lewą
        quick_sort6(middle, end);               // posortuj prawą
    }
    else
        insertion_sort_sentinel3(begin, end);
}

inline double *partitionX(double *first, double *last)
{
    double x = first[(last - first) / 2]; // pivot bierzemy pośrodku
    for (;;)
    {
        while (*first < x)
            ++first;
        while (*last > x)
            --last;
        if (first < last)
            std::swap(*first++, *last--);
        else
            return last; // ostatni element lewej części
    }
}

inline void quick_sortX(double *first, double *last)
{
    if (last > first)
    {
        double *middle = partitionX(first, last); // podziel na dwie części
        quick_sortX(first, middle);               // posortuj lewą
        quick_sortX(middle + 1, last);            // posortuj prawą
    }
}

inline void quick_sortXY(double *first, double *last)
{
    while (last > first)
    {
        double *middle = partitionX(first, last); // podziel na dwie części
        if (middle - first < last - middle)
        {
            quick_sortX(first, middle); // posortuj lewą
            first = middle + 1;
        }
        else
        {
            quick_sortX(middle + 1, last); // posortuj prawą
            last = middle;
        }
    }
}

inline void quick_sortX6a(double *first, double *last)
{
    if (last > first + 16)
    {
        double *middle = partitionX(first, last); // podziel na dwie części
        quick_sortX6a(first, middle);             // posortuj lewą
        quick_sortX6a(middle + 1, last);          // posortuj prawą
    }
    else
        insertion_sort_sentinel3(first, last + 1);
}
inline void quick_sortX6b(double *first, double *last)
{
    if (last > first + 16)
    {
        double *middle = partitionX(first, last); // podziel na dwie części
        quick_sortX6b(first, middle);             // posortuj lewą
        quick_sortX6b(middle + 1, last);          // posortuj prawą
    }
    else
        insertion_sort_sentinel00(first, last + 1);
}

// Quick sort z wikipedii ( z innym partition)
double *partitionW(double *i, double *hi)
{
    // std::swap(i[(hi-i)/2],*hi); // odkomentuj by brać za piwot jako środkowy element
    double pivot = *hi;
    for (double *j = i; j < hi; j++)
        if (*j < pivot)
            std::swap(*i++, *j);
    std::swap(*i, *hi);
    return i;
}

void quicksortW(double *lo, double *hi)
{
    if (lo < hi)
    {
        double *p = partitionW(lo, hi);
        quicksortW(lo, p - 1);
        quicksortW(p + 1, hi);
    }
}

void quicksortW6(double *lo, double *hi)
{
    if (lo + 16 < hi)
    {
        double *p = partitionW(lo, hi);
        quicksortW6(lo, p - 1);
        quicksortW6(p + 1, hi);
    }
    else
        insertion_sort_sentinel3(lo, hi - lo + 1);
}

// STOOGE SORT
/*
 T(n)= 3T(2t/3)+O(1)
 T(n)=Theta(n^(log1.5(3))) = O(n^2.70951129135) ~O(n^2.71)
*/

void stooge_sort(double t[], int n)
{
    if (n == 1)
        return;
    if (n == 2)
    {
        if (t[0] > t[1])
            std::swap(t[0], t[1]);
        return;
    }
    int K = n / 3;
    stooge_sort(t, n - K);
    stooge_sort(t + K, n - K);
    stooge_sort(t, n - K);
}

// COUNTING SORT

void counting_sort(double t[], int n, int M = 0)
{
    // T(n)=O(n+m)
    double *b = new double[n];
    int m = max(n, M); // u = maksymalna wartość w tablicy

    int *ile = new int[m]; // przydział pamięci na  liczniki

    for (int i = 0; i < n; i++)
        ile[i] = 0; // zerowanie liczników

    for (int i = 0; i < n; i++)
        ile[int(t[i])]++; // zliczanie

    for (int i = 1; i < m; i++)
        ile[i] += ile[i - 1]; // akumulacja

    for (int i = n - 1; i >= 0; i--)
        b[--ile[int(t[i])]] = t[i]; // wpis we właściwe miejsce bufora

    for (int i = 0; i < n; i++)
        t[i] = b[i]; // przepisanie

    delete[] ile; // usunięcie liczników z pamięci
    delete[] b;
}

// RADIX SORT (sortowanie pozycyjne)

void radix_sort(double t[], int n)
{
    int u = 1000;
    int max = 2;
    double *b = new double[n]; // przydział pamięci pomocniczej
    double *b_orig = b;

    for (int i = 0; i < n; i++) // szukamy maximum
        if (t[i] > max)
            max = t[i];
    u = ceil(sqrt(max)); // w układzie o u cyfrach będziemy mieli liczby 2 cyfrowe
    if (u < 100)
        u = 100;

    int *ile = new int[u]; // przydział pamięci na liczniki

    for (int r = 1; r <= max; r *= u) // dla r=1, 10,100, 1000, itd czuli r=10^p
    {
        for (int i = 0; i < u; i++)
            ile[i] = 0; // zerowanie liczników

        for (int i = 0; i < n; i++)
            ile[int(t[i]) / r % u]++; // (x / 10^p %10) == p-ta cyfra liczby x (licząc od końca)

        // TUTAJ:  ile[i] = ilość elementów tablicy których badana cyfra jest == i

        for (int i = 1; i < u; i++) // akumulacja
            ile[i] += ile[i - 1];
        // TUTAJ:  ile[i] = ilość elementów tablicy których badana cyfra jest <= i

        for (int i = n - 1; i >= 0; i--)
            b[--ile[int(t[i]) / r % u]] = t[i]; // kopiujemy do bufora na właściwe miejsce
        // TUTAJ: liczby w buforze są uporządkowane wg badanej cyfry

        std::swap(t, b); // zamiana ról bufora i tablicy wyjściowej
    }

    delete[] ile; // zwolnienie pamięci liczników

    if (t == b_orig) // jeśli była nieparzysta liczba cyfr (i nieparzysta liczba zamian)
        for (int i = 0; i < n; i++)
            b[i] = t[i]; // przepisanie z bufora do tablicy wyjściowej

    delete b_orig; // zwolnienie pamięci pomocniczej (bufora)
}

// BUCKET SORT
void bucket_sort(double t[], int n, double *b1 = nullptr)
{
    // szukamy maximum i minimum
    double max = t[0], min = t[0];
    for (int i = 1; i < n; i++)
        if (t[i] < min)
            min = t[i];
        else if (t[i] > max)
            max = t[i];

    if (max == min)
        return;

    int kub = 10; // zakładamy przeciętnie po 10 liczb w kubełku
    int u = n / kub + 2;
    double *b = b1 ? b1 : new double[n];
    double a = n / (max - min) / kub;

    // counting sort
    int *ile = new int[u];      // liczniki
    for (int i = 0; i < u; i++) // zerowanie
        ile[i] = 0;

    for (int i = 0; i < n; i++) // zliczanie
        ile[int((t[i] - min) * a) + 1]++;

    for (int i = 1; i < u; i++) // akumulacja
        ile[i] += ile[i - 1];

    for (int i = n - 1; i >= 0; i--) // wpis do bufora
        b[--ile[int((t[i] - min) * a) + 1]] = t[i];

    // insertion sort
    int i;
    t[0] = b[0];
    for (int k = 1; k < n; k++)
    {
        double x = b[k];
        for (i = k; i > 0 && t[i - 1] > x; i--)
            t[i] = t[i - 1];
        t[i] = x;
    }
    delete b;
    delete[] ile;
}

// SPRAWDZENIE

bool check1(double t[], int n, string s)
{
    for (int i = 1; i < n; i++)
        if (t[i] < t[i - 1])
        {
            cout << s << " error." << endl;
            return false;
        }

    cout << s << " ok." << endl;
    return true;
}

void kopia(double t1[], double t2[], unsigned long long n)
{
    for (unsigned long long i = 0; i < n; i++)
        t2[i] = t1[i];
}

double orig[N], t[N], sorted[N];

bool check(double t[], long long n, string s)
{
    for (long long i = 1; i < n; i++)
        if (t[i] != sorted[i])
        {
            cout << s << " error." << endl;
            return false;
        }

    cout << s << " ok." << endl;
    return true;
}

void print(double t[], int n)
{
    for (int i = 0; i < n; i++)
        cout << t[i] << " ";
    cout << endl;
}

template <class K>
void mierz(void (*f)(double *, K), double *t, K k, string name)
{
    kopia(orig, t, N);
    stoper();
    f(t, k);
    cout << stoper() << " ";
    check(t, N, name);
}

template <class K, class L>
void mierz(void (*f)(double *, K, L), double *t, K k, L l, string name)
{
    kopia(orig, t, N);
    stoper();
    f(t, k, l);
    cout << stoper() << " ";
    check(t, N, name);
}

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
    int *count = new int[m]; // tablica liczników
    for (int i = 0; i < m; i++)
        count[i] = 0;
    T *b = new T[n]; // przydzielenie pamięci na bufor

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
    delete[] count;
}

template <class T>
void radix_sort1(T t[], int n)
{
    T max = array_maximum(t, n);
    int m = ceil(sqrt(max) + 1); // żeby były tylko 2 przebiegi
    for (int r = 1; r <= max; r *= m)
        counting_sort(t, n, m, [r, m](int x)
                      { return x / r % m; }); // sortowanie względem cyfry r-ek
}

template <class T>
void bucket_sort1(T t[], int n)
{
    double min = array_minimum(t, n);
    double max = array_maximum(t, n);
    max += (max - min) / 100;
    int m = n / 10 + 1; // tak dobieramy ilość kawałków by w każdym znalazły się np 10 liczby
    counting_sort(t, n, m, [min, max, m](T x)
                  { return int((x - min) / (max - min) * m); });
    insertion_sort(t, n);
}

int main()
{
    srand(stoper() * 100);
    for (unsigned long long i = 0; i < N; i++)
        sorted[i] = orig[i] = rand() % N;
    sort(sorted, sorted + N);

    mierz(heap_sort, t, N, "heap_sort");
    mierz(heap_sort1, t, t + N, sift_down2b<double>, "heap_sort (na wskaźnikach b)");
    // mierz(heap_sort2aa,t,t+N,"heap_sort (na wskaźnikach aa)");
    // mierz(heap_sort2bb,t,t+N,"heap_sort (na wskaźnikach bb)");
    // mierz(heap_sort2cc,t,t+N,"heap_sort (na wskaźnikach cc)");
    // mierz(heap_sort2a,t,t+N,"heap_sort (na wskaźnikach a)");
    // mierz(heap_sort2b,t,t+N,"heap_sort (na wskaźnikach b)");

    mierz(merge_sort, t, N, "merge_sort");
    mierz(merge_sort2, t, N, b, "merge_sort2 (bez niepotrzebnego kopiowania)");
    // mierz(hc_merge_sort,t,N,b,"hc_merge_sort");
    mierz(stable_sort, t, t + N, "std::stable_sort");
    mierz(HC_merge_sort, t, t + N, "HC_merge_sort (na wskaźnikach)");

    // mierz(quick_sort,t,N,"quick_sort"); //
    // mierz(quick_sort6,t,N,"quick_sort6"); //
    // mierz(quick_sort,t,t+N,"quick_sort (na wskaźnikach)");
    // mierz(quick_sort6,t,t+N,"quick_sort6 (na wskaźnikach)");
    // mierz(quicksortW,t,t+N-1,"quick_sortW");
    // mierz(quicksortW6, t, t + N - 1, "quick_sortW6");
    mierz(quick_sortX, t, t + N - 1, "quick_sortX");
    mierz(quick_sortXY, t, t + N - 1, "quick_sortXY ");
    // mierz(quick_sortX6a, t, t + N - 1, "quick_sortX6a ");
    // mierz(quick_sortX6b, t, t + N - 1, "quick_sortX6b ");
    mierz(std::sort, t, t + N, "std::sort");

    // mierz(counting_sort,t,N,"counting_sort");
    mierz(bucket_sort, t, N, (double *)nullptr, "bucket_sort");
    mierz(radix_sort, t, N, "radix_sort");
    mierz(bucket_sort1, t, N, "bucket_sort1");
    mierz(radix_sort1, t, N, "radix_sort1");

    // if(N>100000)
    //   return 0;

    // mierz(insertion_sort_swap,t,N,"insertion_sort_swap");
    // mierz(insertion_sort,t,N,"insertion_sort_insert");
    // mierz(insertion_sort_sentinel3,t,N,"insertion_sort_sentinel3");
    //  mierz(insertion_sort_sentinel3,t,t+N,"insertion_sort_sentinel3a");
    //  mierz(insertion_sort_sentinel4,t,t+N,"insertion_sort_sentinel4");
    //  mierz(insertion_sort_sentinel,t,t+N,(double*)nullptr,"insertion_sort_sentinel (na wskaźnikach)");
    //  mierz(insertion_sort_sentinel0,t,t+N,"insertion_sort_sentinel0");
    //  mierz(insertion_sort_sentinel00,t,t+N,"insertion_sort_sentinel00");
    //  mierz(stooge_sort,t,N,"stooge_sort");
}
