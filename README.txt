
Kompilować za pomocą flagi "-std=c++1z"

Instrukcja:

1. Pierwszym argumentem w wywołaniu musi być nazwa pliku z kodem PMC.
2. Uzucie flagi "-p <arg>" ustawia PC na warość zawartą w argumencie.
   W przypadku braku flagi PC = 0.
3. Uzycie flagi "-d" uruchamia tryb debugowania i będzie wypisaywać komórki
   PMC po kazdym kroku. W przypadku braku flagi wypisywany jest stan maszyny
   po zakończeniu programu.

Przykład wywołania:
    $ PMC kod.txt -p 6 -d
