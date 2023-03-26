# dijkstra_algorithm
1. Treść zadania:   
Należy zaimplementować oraz dokonać pomiaru czasu działania wybranego algorytmu grafowego rozwiązującego następujący problem: Wyznaczanie najkrótszej ścieżki w grafie – algorytm Dijkstry (max. liczba punktów: 15 z użyciem gotowych struktur, 20+ z użyciem samodzielnie napisanych struktur). Algorytm należy zaimplementować dla obu poniższych reprezentacji grafu w pamięci komputera: - reprezentacja macierzowa, - reprezentacja listowa.  
 
2. Podstawy teoretyczne: 
 
Dijkstra to algorytm wyznaczania najkrótszej ścieżki pomiędzy dwoma wierzchołkami w grafie skierowanym lub nieskierowanym, w którym wagę każdej krawędzi można interpretować jako odległość. Algorytm opiera się na idei użycia kolejki priorytetowej do przechowywania wierzchołków i aktualizowania odległości do nich w czasie rzeczywistym. Kroki algorytmu Dijkstry:
• Ustaw początkowy wierzchołek jako "odwiedzony" i ustaw jego odległość jako 0.
• Dodaj wszystkie sąsiednie wierzchołki do kolejki priorytetowej, a ich odległość ustaw jako
suma odległości do początkowego wierzchołka i wagi krawędzi łączącej te wierzchołki.
• Pobierz wierzchołek z kolejki priorytetowej o najniższej wartości i ustaw jako
"odwiedzony".
• Dla każdego sąsiada "nieskończonego" wierzchołka, oblicz sumę odległości do bieżącego
wierzchołka i wagi krawędzi łączącej te wierzchołki, a następnie aktualizuj odległość do
tego wierzchołka, jeśli jest ona mniejsza niż jego obecna wartość.
• Powtórz kroki 3-4, aż wszystkie wierzchołki zostaną odwiedzone.
• Gdy wszystkie wierzchołki zostały odwiedzone, odległość do wszystkich wierzchołków
jest znana i można znaleźć najkrótszą ścieżkę.
Algorytm Dijkstry jest efektywny, ponieważ korzysta z kolejki priorytetowej, co zapewnia logarytmiczny czas wyszukiwania najmniejszej wartości.  
