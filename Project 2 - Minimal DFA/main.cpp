/// UNIVERSITATEA DIN BUCURESTI, FACULTATEA DE MATEMATICA SI INFORMATICA
/// Proiect II Limbaje Formale si Automate - AFD Minimal
/// ISPAS ADRIAN, Grupa 143

#include <iostream>
#include <fstream>
#include <set>

using namespace std;

int afd[100][10], afisare[100], afisare_stare[100];

int main()
{
    fstream f ("input.txt");
    set<int> alfabet, stari_finale, stari_echivalente[10];
    set<int> :: iterator it, it2;

    int tabel_marcaje[100][100];
    int i, j, l, n, x, y, sw, stare_1, stare_2, stare_finala, ordine, nr_stari_finale, nr_elemente_alfabet, terminat;
    char simbol;


    /// BEGIN - Citire date
    f >> n >> nr_elemente_alfabet >> nr_stari_finale;

    for(i = 0; i < n; i++)
        for(j = 0; j < nr_elemente_alfabet; j++)
            afd[i][j] = -1;

    for(i = 0; i < nr_stari_finale; i++)
    {
        f >> stare_finala;
        stari_finale.insert(stare_finala);
    }

    for(i = 0; i < nr_elemente_alfabet * n; i++)
    {
        f >> stare_1 >> simbol >> stare_2;
        alfabet.insert(simbol);

        ordine = 0;
        for(it = alfabet.begin(); it != alfabet.end(); ++it, ++ordine)
            if(*it == simbol)
                break;

        afd[stare_1][ordine] = stare_2;
    }
    /// END - Citire date

    for(i = 0; i < n; i++)
        for(j = 0; j < nr_elemente_alfabet; j++)
            if(afd[i][j] == -1)
    {
        cout << "nu e complet";
        return 0;
    }

    /// BEGIN - Afisare AFD Initial
    cout << "\n\tAFD Initial\n\n";
    for(i = 0; i < n; i++)
        {
            ordine = 0;
            cout << i << ": ";
            for(it = alfabet.begin(); it != alfabet.end(); ++it, ++ordine)
                if(ordine == 0)
                    cout << (char)*it << " - > " << afd[i][ordine] << endl;
                else
                    cout << "   " << (char)*it << " - > " << afd[i][ordine] << endl;
        }
    /// END - Afisare AFD Initial

    /// BEGIN - Marcare stari 1
    for(i = 0; i < n; i++)
        for(j = 0; j < n; j++)
            if(i == j)
                tabel_marcaje[i][j] = -1;
            else
                tabel_marcaje[i][j] = 0;
     /// In acesta etapa de marcare notam pe diagonala principala cu -1 iar in rest cu 0 in prima instanta.
    /// In continuare verificam toate perechile (p,q) cu propietatea p - stare finala si q - nu e stare finala sau invers si le marcam in tabel cu 1
     for(i = 0; i < n; i++)
        for(j = 0; j < n; j++)
        {
            x = y = 0;

            for(it = stari_finale.begin(); it != stari_finale.end(); ++it)
                if(*it == i)
                {
                    x = 1;
                }
            for(it = stari_finale.begin(); it != stari_finale.end(); ++it)
                if(*it == j)
                {
                    y = 1;
                }

            if(x != y)
                tabel_marcaje[i][j] = 1;

        }
    /// END - Marcare stari 1

    /// BEGIN - Marcare stari 2
    nr_elemente_alfabet = alfabet.size();
    terminat = 1;
    /// In aceasta etapa marcam starile care depind unele de celelalte si reluam procesul atata timp cat realizam cel putin o modificare
    /// Starile marcate sunt perechiile (p,q) cu propietatea ca delta(p,a) - este stare marcata sau delta(q,a) - este stare marcata pt orice a din alfabet
    while(terminat)
    {
        terminat = 0;
        for(i = 0; i < n; i++)
            for(j = 0; j < i; j++)
                if(tabel_marcaje[i][j] == 0)
                for(l = 0; l < nr_elemente_alfabet; l++)
                    if(tabel_marcaje[afd[i][l]][afd[j][l]] == 1)
                        {
                            tabel_marcaje[i][j] = 1;
                            terminat = 1;
                            break;
                        }
    }
    /// END - Marcare stari 2


    /// BEGIN - Afisare tabel cu starile marcate si nemarcate
    cout << endl;
    cout << "\n\tTabel marcaje\n\n";
    for(i = 0; i < n; i++)
        {
            for(j = 0; j <= i; j++)
                cout << tabel_marcaje[i][j] << " ";
            cout << endl;
        }

    for(i = 0; i < n; i++)
        stari_echivalente[i].insert(i);

    cout << endl;
    for(i = 0; i < n; i++)
        for(j = 0; j <= i; j++)
            if(tabel_marcaje[i][j] == 0)
                {
                        stari_echivalente[i].insert(j);
                        stari_echivalente[j].insert(i);
                }
    /// END - Afisare tabel cu starile marcate si nemarcate

    /// BEGIN - Afisarea AFD-ului minimal
    cout << "\n\tAFD-ul minimal\n\n";
    for(i = 0; i < n; i++)
    {
        /// Pe parcursul algoritmului ne-am folosit de set-uri care initial aveau starile individuale, iar pe parcurs s-au adaugat si starile echivalente acestora.
        /// Astfel, in afisare vom parcurge tabelul initial de memorare a AFD-ului si vom afisa starile aflate in set-uri la indicii corespunzatori.
        if(stari_echivalente[i].size() == 1)
            {
                if(!afisare[i])
                    {
                        ordine = 0;
                        cout << i << ": ";
                        for(it = alfabet.begin(); it != alfabet.end(); ++it, ++ordine)
                            if(ordine == 0)
                                {
                                    cout << (char)*it << " - > ";
                                    for(it2 = stari_echivalente[afd[i][ordine]].begin(); it2 != stari_echivalente[afd[i][ordine]].end(); ++it2)
                                        cout << *it2;
                                    cout << endl;

                                }
                            else
                                {
                                    cout << "   " << (char)*it << " - > ";
                                    for(it2 = stari_echivalente[afd[i][ordine]].begin(); it2 != stari_echivalente[afd[i][ordine]].end(); ++it2)
                                        cout << *it2;
                                    cout << endl;
                                }

                        afisare[i] = 1;
                    }
            }
        else
            {
                sw = 0;
                for(it = stari_echivalente[i].begin(); it != stari_echivalente[i].end(); ++it)
                    if(!afisare[*it])
                        {
                            cout << *it;
                            afisare[*it] = 1;
                            sw = 1;
                        }
                if(sw)
                    {
                        cout << ": ";
                        ordine = 0;
                        for(it = alfabet.begin(); it != alfabet.end(); ++it, ++ordine)
                            if(!ordine)
                            {
                                cout << (char)*it << " - > ";
                                for(it2 = stari_echivalente[afd[i][ordine]].begin(); it2 != stari_echivalente[afd[i][ordine]].end(); ++it2)
                                    cout << *it2;
                                cout << endl;
                            }
                            else
                            {
                                for(j = 0; j < stari_echivalente[i].size(); j++)
                                    cout << " ";

                                cout << "  " << (char)*it << " - > ";
                                for(it2 = stari_echivalente[afd[i][ordine]].begin(); it2 != stari_echivalente[afd[i][ordine]].end(); ++it2)
                                    cout << *it2;
                                cout << endl;
                            }

                    }
            }

    }
    /// END - Afisarea AFD-ului minimal

    /// BEGIN - Afisare Stari finale
    cout << endl << "Stari finale: ";
    for(it = stari_finale.begin(); it != stari_finale.end(); ++it)
        if(!afisare_stare[*it])
            {
                for(it2 = stari_echivalente[*it].begin(); it2 != stari_echivalente[*it].end(); ++it2)
                    {
                        cout << *it2;
                        afisare_stare[*it2] = 1;
                    }

                cout << " ";
            }

    /// END - Afisare Stari finale
    cout << endl;
    return 0;
}
