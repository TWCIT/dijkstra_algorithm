#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>

using namespace std;

// Struktury danych
struct Krawedz {
    int poczatek;
    int koniec;
    int waga;
};

struct GrafMacierz {
    int wierzcholki;
    int krawedzie;
    vector<vector<int>> macierz;
};

struct GrafLista {
    int wierzcholki;
    int krawedzie;
    vector<vector<Krawedz>> lista;
};

// Funkcje do obsługi strumienia wejsciowego oraz generowania grafów
GrafMacierz wczytajGrafMacierz(string nazwaPliku) {
    GrafMacierz graf;
    ifstream plik;
    plik.open(nazwaPliku);
    
    if (plik.is_open()) {
        plik >> graf.wierzcholki >> graf.krawedzie;
        graf.macierz.resize(graf.wierzcholki, vector<int>(graf.wierzcholki));
        
        for (int i = 0; i < graf.krawedzie; i++) {
            int poczatek, koniec, waga;
            plik >> poczatek >> koniec >> waga;
            graf.macierz[poczatek][koniec] = waga;
            graf.macierz[koniec][poczatek] = waga;
        }
        
        plik.close();
    }
    
    return graf;
}

GrafLista wczytajGrafLista(string nazwaPliku) {
    GrafLista graf;
    ifstream plik;
    plik.open(nazwaPliku);
    
    if (plik.is_open()) {
        plik >> graf.wierzcholki >> graf.krawedzie;
        graf.lista.resize(graf.wierzcholki);
        
        for (int i = 0; i < graf.krawedzie; i++) {
            int poczatek, koniec, waga;
            plik >> poczatek >> koniec >> waga;
            Krawedz krawedz;
            krawedz.poczatek = poczatek;
            krawedz.koniec = koniec;
            krawedz.waga = waga;
            graf.lista[poczatek].push_back(krawedz);
            graf.lista[koniec].push_back(krawedz);
        }
        
        plik.close();
    }
    
    return graf;
}

GrafMacierz wygenerujGrafMacierz(int wierzcholki, int gęstość) {
    GrafMacierz graf;
    graf.wierzcholki = wierzcholki;
    graf.krawedzie = (gęstość/100.0) * wierzcholki * (wierzcholki-1)/2;
    graf.macierz.resize(graf.wierzcholki, vector<int>(graf.wierzcholki));
    
    int iloscKrawedzi = 0; // Zmienna na liczbę dodanych krawędzi
    while (iloscKrawedzi < graf.krawedzie) {
        int poczatek = rand() % graf.wierzcholki; // Losowanie wierzchołka startowego
        int koniec = rand() % graf.wierzcholki; // Losowanie wierzchołka końcowego
        if (poczatek != koniec && graf.macierz[poczatek][koniec] == 0) { // Sprawdzenie, czy krawędź już nie istnieje i czy wierzchołki są różne
            graf.macierz[poczatek][koniec] = 1; // Dodanie krawędzi w obie strony
            graf.macierz[koniec][poczatek] = 1;
            iloscKrawedzi++; // Zwiększenie liczby dodanych krawędzi
        }
    }
    
    return graf; // Zwrócenie stworzonego grafu
}

GrafLista wygenerujGrafLista(int wierzcholki, int gęstość) {
    GrafLista graf;
    graf.wierzcholki = wierzcholki;
    graf.krawedzie = (gęstość/100.0) * wierzcholki * (wierzcholki-1)/2;
    graf.lista.resize(graf.wierzcholki);
    
    int iloscKrawedzi = 0;
    while (iloscKrawedzi < graf.krawedzie) {
        int poczatek = rand() % graf.wierzcholki;
        int koniec = rand() % graf.wierzcholki;
        if (poczatek != koniec) {
            bool istnieje = false;
            for (auto krawedz : graf.lista[poczatek]) {
                if (krawedz.koniec == koniec) {
                    istnieje = true;
                    break;
                }
            }
            if (!istnieje) {
                Krawedz krawedz;
                krawedz.poczatek = poczatek;
                krawedz.koniec = koniec;
                krawedz.waga = 1;
                graf.lista[poczatek].push_back(krawedz);
                graf.lista[koniec].push_back(krawedz);
                iloscKrawedzi++;
            }
        }
    }
    
    return graf;
}

// Algorytmy
vector<int> dijkstraMacierz(GrafMacierz graf, int poczatek, int koniec) {
    vector<int> odleglosci(graf.wierzcholki, INT_MAX);
    vector<int> poprzedniki(graf.wierzcholki, -1);
    vector<bool> odwiedzone(graf.wierzcholki, false);
    
    // ustawienie odległości od wierzchołka początkowego na 0
    odleglosci[poczatek] = 0;
    
    // główna pętla algorytmu, w każdym kroku znajdujemy najbliższy nieodwiedzony wierzchołek
    for (int i = 0; i < graf.wierzcholki; i++) {
        int aktualny = -1;
        // wyszukanie najbliższego nieodwiedzonego wierzchołka
        for (int j = 0; j < graf.wierzcholki; j++) {
            if (!odwiedzone[j] && (aktualny == -1 || odleglosci[j] < odleglosci[aktualny])) {
                aktualny = j;
            }
        }
        
        // jeśli nie ma już nieodwiedzonych wierzchołków, zakończ pętlę
        if (odleglosci[aktualny] == INT_MAX) {
            break;
        }
        
        // oznacz aktualny wierzchołek jako odwiedzony
        odwiedzone[aktualny] = true;
        
        // aktualizacja odległości do sąsiednich wierzchołków
        for (int j = 0; j < graf.wierzcholki; j++) {
            if (graf.macierz[aktualny][j] > 0) {
                int odleglosc = odleglosci[aktualny] + graf.macierz[aktualny][j];
                if (odleglosci[j] > odleglosc) {
                    odleglosci[j] = odleglosc;
                    poprzedniki[j] = aktualny;
                }
            }
        }
    }
    //Dobra kolejność wierzchołków
    //Tworzenie wektora sciezka, który będzie przechowywał kolejne wierzchołki na znalezionej ścieżce
    vector<int> sciezka;
    
    //Zmienna aktualny jest ustawiona na koniec, ponieważ ścieżka będzie budowana od końca do początku
    int aktualny = koniec;
    
    //Pętla while będzie działać, dopóki aktualny nie będzie równy -1
    while (aktualny != -1) {
        //Dodawanie aktualnego wierzchołka do wektora sciezka
        sciezka.push_back(aktualny);
        //Aktualizacja aktualnego wierzchołka na poprzednik wierzchołka aktualnego
        aktualny = poprzedniki[aktualny];
    }
    
    //Odwrócenie kolejności elementów w wektorze sciezka, aby były one odpowiednio ułożone od początku do końca
    reverse(sciezka.begin(), sciezka.end());
    
    //Zwracanie wektora sciezka jako wynik funkcji
    return sciezka;
}

vector<int> dijkstraLista(GrafLista graf, int poczatek, int koniec) {
    vector<int> odleglosci(graf.wierzcholki, INT_MAX);
    vector<int> poprzedniki(graf.wierzcholki, -1);
    vector<bool> odwiedzone(graf.wierzcholki, false);
    
    odleglosci[poczatek] = 0;
    
    for (int i = 0; i < graf.wierzcholki; i++) {
        int aktualny = -1;
        for (int j = 0; j < graf.wierzcholki; j++) {
            if (!odwiedzone[j] && (aktualny == -1 || odleglosci[j] < odleglosci[aktualny])) {
                aktualny = j;
            }
        }
        
        if (odleglosci[aktualny] == INT_MAX) {
            break;
        }
        
        odwiedzone[aktualny] = true;
        
        for (auto krawedz : graf.lista[aktualny]) {
            int odleglosc = odleglosci[aktualny] + krawedz.waga;
            if (odleglosci[krawedz.koniec] > odleglosc) {
                odleglosci[krawedz.koniec] = odleglosc;
                poprzedniki[krawedz.koniec] = aktualny;
            }
        }
    }
    
    // Odwrocenie sciezki
    vector<int> sciezka;
    int aktualny = koniec;
    while (aktualny != -1) {
        sciezka.push_back(aktualny);
        aktualny = poprzedniki[aktualny];
    }
    reverse(sciezka.begin(), sciezka.end());
    
    return sciezka;
}


int main() {
    // Wybieranie źródła danych
    int wybor;
    vector<int> rozmiary = {100, 150, 200, 300,500};
    vector<int> gestosci = {25, 50, 75, 90};
    int iloscInstancji = 50;
    cout << "Wybierz sposob wczytania danych:" << endl;
    cout << "1. Wczytanie danych z pliku" << endl;
    cout << "2. Losowe wygenerowanie danych" << endl;
    cin >> wybor;
    GrafMacierz grafMacierz;
    GrafLista grafLista;
    
    if (wybor == 1) {
        string nazwaPliku;
        cout << "Podaj nazwe pliku: ";
        cin >> nazwaPliku;
        
        grafMacierz = wczytajGrafMacierz(nazwaPliku);
        grafLista = wczytajGrafLista(nazwaPliku);
        // Wczytanie wierzchołka początkowego i końcowego
        int wierzcholekPoczatkowy, wierzcholekKoncowy;
        cout << "Podaj wierzcholek poczatkowy: ";
        cin >> wierzcholekPoczatkowy;
        cout << "Podaj wierzcholek koncowy: ";
        cin >> wierzcholekKoncowy;
        auto startMacierz = chrono::high_resolution_clock::now();
        dijkstraMacierz(grafMacierz,wierzcholekPoczatkowy,wierzcholekKoncowy);
        auto stopMacierz = chrono::high_resolution_clock::now();
        auto czasMacierz = chrono::duration_cast<chrono::nanoseconds>(stopMacierz - startMacierz);
        
        long long czasMacierz2 = czasMacierz.count();
        cout<< "Czas dla macierzy z plik: "<< czasMacierz2 << "ns";
        
        auto startLista = chrono::high_resolution_clock::now();
        dijkstraLista(grafLista,wierzcholekPoczatkowy,wierzcholekKoncowy);
        auto stopLista = chrono::high_resolution_clock::now();
        auto czasLista = chrono::duration_cast<chrono::nanoseconds>(stopLista - startLista);
        long long czasLista2 = czasLista.count();
        cout<< "Czas dla macierzy z plik: "<< czasLista2 << "ns";
    }
    else if (wybor == 2) {
        for (int r : rozmiary) {
            for (int g : gestosci) {
                int sumaCzasuMacierz = 0, sumaCzasuLista = 0;
                for (int i = 0; i < iloscInstancji; i++) {
                    GrafMacierz grafMacierz = wygenerujGrafMacierz(r, g);
                    GrafLista grafLista = wygenerujGrafLista(r, g);
                    
                    auto startMacierz = chrono::high_resolution_clock::now();
                    dijkstraMacierz(grafMacierz, 0, r-1);
                    auto stopMacierz = chrono::high_resolution_clock::now();
                    auto czasMacierz = chrono::duration_cast<chrono::nanoseconds>(stopMacierz - startMacierz);
                    sumaCzasuMacierz += czasMacierz.count();
                    
                    auto startLista = chrono::high_resolution_clock::now();
                    dijkstraLista(grafLista, 0, r-1);
                    auto stopLista = chrono::high_resolution_clock::now();
                    auto czasLista = chrono::duration_cast<chrono::nanoseconds>(stopLista - startLista);
                    sumaCzasuLista += czasLista.count();
                }
                cout << "Dla rozmiaru grafu " << r << " i gestosci " << g << "%:" << endl;
                cout << "Sredni czas dla macierzowej reprezentacji grafu: " << sumaCzasuMacierz / iloscInstancji << " ns" << endl;
                cout << "Sredni czas dla listowej reprezentacji grafu: " << sumaCzasuLista / iloscInstancji << " ns" << endl;
                cout << endl;
            }
        }
    }
    else {
        cout << "Niepoprawny wybor." << endl;
        return 0;
    }
    
}

//using namespace std;
//using namespace std::chrono;
//
//const int MAX = 1000;
//
//
//int odleglosci[MAX];
//int poprzednik[MAX];
//int n, m;
//vector<pair<int, int>> lista_sasiadow[MAX];
//int macierz[MAX][MAX];
//
//void wczytaj_z_pliku(string nazwa_pliku) {
//    ifstream plik(nazwa_pliku);
//    if (!plik.is_open()) {
//        cout << "Blad otwarcia pliku" << endl;
//        exit(1);
//    }
//    plik >> m >> n;
//    int a, b, w;
//    while (plik >> a >> b >> w) {
//        lista_sasiadow[a].push_back({b, w});
//        macierz[a][b] = w;
//    }
//    plik.close();
//}
//void losuj_graf(int ilosc_wierzcholkow, int gestosc) {
//    n = ilosc_wierzcholkow;
//    default_random_engine generator(chrono::system_clock::now().time_since_epoch().count());
//    uniform_int_distribution<int> distribution(1, 100);
//    m = (gestosc / 100.0) * (n * (n - 1)) / 2;
//    int a, b, w;
//    for (int i = 0; i < n; i++) {
//        lista_sasiadow[i].clear();
//        for (int j = 0; j < n; j++) {
//            macierz[i][j] = 0;
//        }
//    }
//    for (int i = 0; i < m; i++) {
//        a = distribution(generator) % n;
//        b = distribution(generator) % n;
//        while (a == b || macierz[a][b]) {
//            a = distribution(generator) % n;
//            b = distribution(generator) % n;
//        }
//        w = distribution(generator);
//        lista_sasiadow[a].push_back({b, w});
//        macierz[a][b] = w;
//    }
//}
////int wagaKrawedziLista(int wezelPoczatkowy, int wezelKoncowy, vector<vector<pair<int, int>>>& listaSasiedztwa) {
////    for (auto krawedz : listaSasiedztwa[wezelPoczatkowy]) {
////       if (krawedz.first == wezelKoncowy) {
////            return krawedz.second;
////        }
////    }
////    return -1;
////}
////int wagaKrawedziMacierz(int wezelPoczatkowy, int wezelKoncowy, vector<vector<int>>& macierzSasiedztwa) {
////    return macierzSasiedztwa[wezelPoczatkowy][wezelKoncowy];
////}
//void relaksMacierz(int aktualny, int nastepny, int waga, priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>>& kolejka) {
//    if(odleglosci[nastepny] > odleglosci[aktualny] + waga) {
//        odleglosci[nastepny] = odleglosci[aktualny] + waga;
//        poprzednik[nastepny] = aktualny;
//        kolejka.push({odleglosci[nastepny], nastepny});
//    }
//}
//void relaksLista(int aktualny, int nastepny, int waga, priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>>& kolejka) {
//    if(odleglosci[nastepny] > odleglosci[aktualny] + waga) {
//        odleglosci[nastepny] = odleglosci[aktualny] + waga;
//        poprzednik[nastepny] = aktualny;
//        kolejka.push({odleglosci[nastepny], nastepny});
//    }
//}
//
//void dijkstraMacierzowa(int start,int macierzSasiedztwa[MAX][MAX]) {
//    for(int wierzcholek = 0; wierzcholek < MAX; wierzcholek++) {
//        odleglosci[wierzcholek] = INT_MAX;
//        poprzednik[wierzcholek] = -1;
//    }
//    odleglosci[start] = 0;
//
//    set<int> zbiorOdwiedzonych;
//    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> kolejka;
//    kolejka.push({0, start});
//
//    while(!kolejka.empty()) {
//        int aktualny = kolejka.top().second;
//        kolejka.pop();
//
//        if(zbiorOdwiedzonych.find(aktualny) != zbiorOdwiedzonych.end()) continue;
//        zbiorOdwiedzonych.insert(aktualny);
//
//        for(int nastepny = 0; nastepny < MAX; nastepny++) {
//            if(macierzSasiedztwa[aktualny][nastepny] == 0) continue;
//            relaksMacierz(aktualny, nastepny, macierzSasiedztwa[aktualny][nastepny], kolejka);
//        }
//    }
//}
//
//void dijkstraListowa(int start,vector<pair<int, int>> listaSasiedztwa[MAX]) {
//    for(int wierzcholek = 0; wierzcholek < MAX; wierzcholek++) {
//        odleglosci[wierzcholek] = INT_MAX;
//        poprzednik[wierzcholek] = -1;
//    }
//    odleglosci[start] = 0;
//
//    set<int> zbiorOdwiedzonych;
//    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> kolejka;
//    kolejka.push({0, start});
//
//    while(!kolejka.empty()) {
//        int aktualny = kolejka.top().second;
//        kolejka.pop();
//
//        if(zbiorOdwiedzonych.find(aktualny) != zbiorOdwiedzonych.end()) continue;
//        zbiorOdwiedzonych.insert(aktualny);
//
//        for(auto nastepny : listaSasiedztwa[aktualny]) {
//            relaksLista(aktualny, nastepny.first, nastepny.second, kolejka);
//        }
//    }
//}
//
//
//
//
//
//
//
//int main() {
//    vector<pair<int, int>> listaSasiedztwa[MAX] = {};
//    int macierzSasiedztwa[MAX][MAX];
//    for(int i = 0; i < MAX; i++) {
//        for(int j = 0; j < MAX; j++) {
//            macierzSasiedztwa[i][j] = 0;
//        }
//    }
//    macierzSasiedztwa[0][1] = 2;
//    macierzSasiedztwa[0][3] = 5;
//    macierzSasiedztwa[1][2] = 1;
//    macierzSasiedztwa[2][3] = 3;
//
//    listaSasiedztwa[0].push_back({1, 2});
//    listaSasiedztwa[0].push_back({3, 5});
//    listaSasiedztwa[1].push_back({2, 1});
//    listaSasiedztwa[2].push_back({3, 3});
//
//    dijkstraMacierzowa(0,macierzSasiedztwa);
//    for(int i = 0; i < MAX; i++) {
//        cout << odleglosci[i] << " ";
//    }
//    cout << endl;
//    for(int i = 0; i < MAX; i++) {
//        cout << poprzednik[i] << " ";
//    }
//    cout << endl;
//
//    dijkstraListowa(0,listaSasiedztwa);
//    for(int i = 0; i < MAX; i++) {
//        cout << odleglosci[i] << " ";
//    }
//    cout << endl;
//    for(int i = 0; i < MAX; i++) {
//        cout << poprzednik[i] << " ";
//    }
//    cout << endl;
//
//    int start = 0;
//
//    high_resolution_clock::time_point t1 = high_resolution_clock::now();
//    dijkstraMacierzowa(0,macierzSasiedztwa);
//    high_resolution_clock::time_point t2 = high_resolution_clock::now();
//    auto duration = duration_cast<microseconds>( t2 - t1).count();
//    printf("Czas dzialania algorytmu Dijkstry dla reprezentacji macierzowej: %ld mikrosekund\n", duration);
//    high_resolution_clock::time_point t3 = high_resolution_clock::now();
//    dijkstraListowa(start,listaSasiedztwa);
//    high_resolution_clock::time_point t4 = high_resolution_clock::now();
//    auto duration2 = duration_cast<microseconds>( t4 - t3 ).count();
//    printf("Czas dzialania algorytmu Dijkstry dla reprezentacji listowej: %ld mikrosekund\n", duration2);
//
//    return 0;
//}
