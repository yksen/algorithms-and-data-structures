#include <iostream>
#include <cassert>
#include <cstring>
#include <sstream>
#include <fcntl.h>

#ifdef _WIN32
#include <io.h>
#endif

using namespace std;

// Operacje na drzewie BST
// Przykłady do wykładu z Algorytmów i Struktur Danych
// C. Juszczak (2023)
//                        ┌─8─┐
//                        7   9

struct node // drzewo BST bez wskaźnika na rodzica
{
	int key;	 // wartość przechowywana w węźle
	node *left;	 // adres lewego dziecka (lewe poddrzewo)
	node *right; // adres prawego dziecka (prawe poddrzewo)

	node(int key) : key(key), left(nullptr), right(nullptr)
	{
	}
};

node *find(node *t, int x) // wyszukiwanie klucza (nierekurencyjne)
{
	while (t && t->key != x) // póki drzewo nie puste, ale klucz sie nie zgadza
		if (x < t->key)		 // jeśli szukany x jest mniejszy klucza w węźle
			t = t->left;	 // idź w lewo
		else				 // w przeciwnym razie
			t = t->right;	 // idź w prawo
	return t;
}

node *find_recursive(node *t, int x) // wyszukiwanie klucza (rekurencyjne)
{
	if (!t || t->key == x) // jeśli korzeń jest nullem lub zawiera x
		return t;		   // zwróć korzeń

	if (x < t->key)							// jeśli x mniejszy od klucza w korzeniu
		return find_recursive(t->left, x);	// zwróć wynik wyszukiwania w lewym poddrzewie
	else									// w przeciwnym razie
		return find_recursive(t->right, x); // zwróć wynik wyszukiwania w prawym poddrzewie
}

void insert(node *&t, int x) // wstawianie (nierekurencyjne)
{
	node **a = &t; // a to adres wskaźnika na korzeń

	while (*a != nullptr)	 // dopóki nie znalazłeś nulla, w którym należy podczepić x
		if (x < (**a).key)	 // jeśli x jest mniejszy od klucza w korzeniu
			a = &(**a).left; // przejdź do wskaźnika na lewe poddrzewa
		else
			a = &(**a).right; // przejdź do wskaźnika na prawe poddrzewa

	*a = new node(x); // znalazłeś właściwego nulla więc podczep tam x
}

void insert_recursive(node *&t, int x) // wstawianie (rekurencyjnie)
{
	if (t == nullptr)	 // jeśli drzewo jest puste
		t = new node(x); // utwórz nowy korzeń w umieść w nim x

	else if (x < t->key)			   // jeśli x jest mniejszy od klucza w korzeniu
		insert_recursive(t->left, x);  // wstaw x do lewego poddrzewa
	else							   // w przeciwnym razie
		insert_recursive(t->right, x); // wstaw x do prawego  poddrzewa
}

void remove(node *&t, int x) // usuwanie elementu z drzewa (bez rekurencji)
{
	node **a = &t;										// a to adres wskaźnika na korzeń
	while (*a && (**a).key != x)						// póki *a nie jest nullem, ale nie zawiera x
		a = x < (**a).key ? &(**a).left : &(**a).right; // przejdź do odpowiedniego poddrzewa

	if (*a) // jeśli znaleziono x
	{
		if ((**a).right && (**a).left) // jeśli ma dwóch synów
		{
			node **b = &(**a).right; // zaczynając od prawego syna
			while ((**b).left)		 // idź w lewo póki można
				b = &(**b).left;	 // docelowo b to minimum prawego poddrzewa
			(**a).key = (**b).key;	 // nadpisz x w węźle a kluczem z węzła b
			a = b;					 // i przeznacz węzeł b do usunięcia
		}
		// tutaj *a ma już co najwyżej jednego syna
		node *son = (**a).right ? (**a).right : (**a).left;
		delete *a; // usuwamy węzeł **t1
		*a = son;  // i zastępujemy synem
	}
}

node **find_recursive2(node *&t, int x) // wyszukiwanie klucza (rekurencyjne)
{
	if (!t || t->key == x) // jeśli korzeń jest nullem lub zawiera x
		return &t;		   // zwróć korzeń

	if (x < t->key)							 // jeśli x mniejszy od klucza w korzeniu
		return find_recursive2(t->left, x);	 // zwróć wynik wyszukiwania w lewym poddrzewie
	else									 // w przeciwnym razie
		return find_recursive2(t->right, x); // zwróć wynik wyszukiwania w prawym poddrzewie
}

node **leftmost(node *&t) // węzeł zawierający najmniejszy element
{
	assert(t);
	if (t->left)				  // jeśli jest lewe poddrzewo
		return leftmost(t->left); // to tam jest element najmniejszy
	else						  // a jak nie
		return &t;				  // najmniejszy jest w tym węźle
}

void remove_recursive(node *&t, int x) // usuwanie elementu z drzewa (bez rekurencji)
{
	node **a = find_recursive2(t, x);

	if (*a) // jeśli znaleziono
	{
		if ((**a).right && (**a).left) // jeśli ma dwóch synów
		{
			node **b = leftmost((**a).right); // znajdź minimum prawego poddrzewa
			(**a).key = (*b)->key;			  // skopiuj klucz do "usuwanego" węzła
			a = b;							  // i zamiast niego przeznacz ten do usunięcia
		}

		// tutaj *t1 ma już co najwyżej jednego syna
		node *son = (**a).right ? (**a).right : (**a).left;
		delete *a; // usuwamy węzeł **t1
		*a = son;  // i zastępujemy synem
	}
}

void inOrder(node *t) // wypisanie kluczy w porządku "in order"
{
	if (t)
	{
		inOrder(t->left);			// wypisze klucze z lewego poddrzewa
		std::cout << t->key << " "; // wypisz klucz z węzła
		inOrder(t->right);			// wypisze klucze z lewego poddrzewa
	}
}

void preOrder(node *t) // wypisanie kluczy w porządku "pre order"
{
	if (t)
	{
		std::cout << t->key << " "; // najpierw wypisz klucz z węzła
		preOrder(t->left);			// a potem klucze z lewego
		preOrder(t->right);			// i prawego poddrzewa
	}
}

void postOrder(node *t) // wypisanie kluczy w porządku "post order"
{
	if (t)
	{
		postOrder(t->left);			// wypisz klucze z poddrzewa lewego,
		postOrder(t->right);		// potem z prawego,
		std::cout << t->key << " "; // a na końcu klucz z węzła
	}
}

int count(node *t) // ilość kluczy/węzłów w drzewie (rekurencyjnie)
{
	if (!t)
		return 0; // puste drzewo nie ma kluczy

	int nL = count(t->left);  // klucze w lewym poddrzewie
	int nR = count(t->right); // klucze w prawym poddrzewie
	return 1 + nL + nR;		  // korzeń + lewe + prawe
}

int N(node *t) // (to samo krócej)
{
	return !t ? 0 : 1 + N(t->left) + N(t->right);
}

int height(node *t) // wysokość drzewa BST (rekurencyjnie)
{
	if (!t)
		return 0; // puste drzewo ma wysokość 0

	int hL = height(t->left);	 // wysokość lewego poddrzewa
	int hR = height(t->right);	 // wysokość prawego poddrzewa
	return 1 + std::max(hR, hL); // większa z nich plus jeden
}

int H(node *t) // wysokość drzewa BST (to samo krócej)
{
	return !t ? 0 : 1 + std::max(H(t->left), H(t->right));
}

int min(node *t) // minimalny klucz (to ten najbardziej na lewo)
{
	assert(t);
	while (t->left)
		t = t->left;
	return t->key;
}

int min_recursive(node *t) // minimalny klucz
{
	assert(t);
	if (t->left)					   // jeśli jest lewe poddrzewo
		return min_recursive(t->left); // to tam jest element najmniejszy
	else							   // a jak nie
		return t->key;				   // najmniejszy jest w tym węźle
}

int max(node *t) // maksymalny klucz
{
	assert(t);
	while (t->right)
		t = t->right;
	return t->key;
}

int max_recursive(node *t) // maksymalny klucz
{
	assert(t);
	if (t->right)						// jeśli jet prawe poddrzewo
		return max_recursive(t->right); // to tam jest element największy
	else								// a jak nie
		return t->key;					// to największy jest tym węźle
}

void destroy(node *&t) // usunięcie drzewa i zwolnienie pamięci (rekurencyjnie)
{
	if (t)
	{
		destroy(t->left);
		destroy(t->right);
		delete t;
		t = nullptr;
	}
}

void prettyLine(node *t, int &n, int level = 0)
{ /*
   */

	if (t)
	{
		int k = n;
		prettyLine(t->left, n, level - 1);
		stringstream s;
		s << " " << t->key << " ";
		int l = s.str().length();
		n += l;
		if (level == 0)
			cout << s.str();
		else
			while (l--)
				cout << " ";
		prettyLine(t->right, n, level - 1);
	}
}

void pretty(node *t)
{
	/*
		Pokazuje drzewo w terminalu:
					 7
				 4       9
			   3  5     8   10
			 1     6           11
	*/

	int h = H(t);
	for (int i = 0; i < h; i++)
	{
		int n = 0;
		prettyLine(t, n, i);
		cout << endl;
	}
}

void prettyLine2(node *t, int &n, string c1, string c2, int level = 0)
{
	/*
		Jedna linia rysunku dla procedurki pretty2
	*/
	if (t)
	{
		int k = n;
		prettyLine2(t->left, n, " " + c1, "-" + c1, level - 1);
		wstringstream s;
		s << ' ' << t->key << ' ';
		int l = s.str().length();
		n += l;
		if (level == 0)
			wcout << "" << s.str() << "";
		else if (level == -1)
		{
			for (int i = 0; i < (l) / 2; i++)
				wcout << (c1[0] == ' ' ? L" " : L"─");

			wcout << (c1[0] == ' ' ? L"┌" : L"┐");

			for (int i = (l) / 2 + 1; i < l; i++)
				wcout << (c2[0] == ' ' ? L" " : L"─");
		}
		else
			for (int i = 0; i < l; i++)
				wcout << (level >= 0 ? L" " : c2[-level - 1] == '-' ? L"─"
																	: L" ");
		prettyLine2(t->right, n, "-" + c2, " " + c2, level - 1);
	}
}

void pretty2(node *t)
{
/*
	Pokazuje drzewo w terminalu w taki sposób:
						┌──────7────┐
					┌───4─┐     ┌───9───┐
				  ┌─3   ┌─5─┐ ┌─8     ┌─10─┐
				  1     4   6 7       9    11
*/
#ifdef _WIN32
	_setmode(_fileno(stdout), _O_U16TEXT);
#endif

	int h = H(t);
	char c = ' ';
	for (int i = 0; i < h; i++)
	{
		int n = 0;
		prettyLine2(t, n, "", "", i); // wyświetl i-tą linię rysunku
		wcout << endl;
	}
}

void interactive(node *&t)
{
	int x = 0;

	while (cout << "podaj liczbę ", cin >> x)
	{
		if (x < 0)
		{
			remove(t, -x);
			cout << "Usuwam " << -x << endl;
		}
		if (x > 0)
		{
			insert(t, x);
			cout << "Dodaję " << x << endl;
		}
		if (x == 0)
			if (cout << "Jaką liczbę wyszukać: ", cin >> x)
			{
				pretty2(find(t, x));
				cout << "============================" << endl;
			}
		pretty2(t);
	}
}

int main(int argc, char *argv[])
{
	node *t = nullptr; // tworzymy puste drzewo BST

	// dodajemy klucze
	for (int x : {20, 41, 13, 32, 12, 17, 4, 9, 8, 200, 100, 3, 8})
		insert(t, x);
	cout << "struktura:\n";
	pretty2(t);
	cout << "kolejność inOrder :";
	inOrder(t);
	cout << endl;
	cout << "kolejność preOrder :";
	preOrder(t);
	cout << endl;
	cout << "kolejność postOrder:";
	postOrder(t);
	cout << endl;
	cout << "Ilość węzłów    = " << count(t) << endl;
	cout << "Wysokość drzewa = " << height(t) << endl;
	cout << "min = " << min(t) << " max = " << max(t) << endl;
	cout << "wyszukaj 9: " << find(t, 9)->key << endl;
	cout << "usuń korzeń (" << t->key << "): \n";
	remove_recursive(t, t->key);
	pretty2(t);
	cout << "usuń 9:\n";
	remove_recursive(t, 9);
	pretty2(t);
	cout << "usuń 200:\n";
	remove_recursive(t, 200);
	pretty2(t);
	cout << "usuń 13:\n";
	remove_recursive(t, 13);
	pretty2(t);
	cout << "usuń 4:\n";
	remove_recursive(t, 4);
	pretty2(t);

	if (argc > 1)
		interactive(t);
	destroy(t);
}