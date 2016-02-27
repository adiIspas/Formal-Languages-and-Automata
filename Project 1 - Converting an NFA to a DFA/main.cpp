/// UNIVERSITATEA DIN BUCURESTI, FACULTATEA DE MATEMATICA SI INFORMATICA
/// Proiect I Limbaje Formale si Automate - Transformare AFN in AFD
/// ISPAS ADRIAN, Grupa 143

#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <algorithm>
#include <queue>
#include <cstring>

using namespace std;

int main()
{
    ifstream f ("input.txt");

    unsigned i, j, k, x, l, sw, stare, poz, dim_set;
    int in, out, stare_initiala, stare_finala, nr_stari_finale;
    int stari[100] = {0}, stari_finale2[100] = {0}, tranzitii[100] = {0};
    int nr_elemente = 0, nr_stari = 0, nr_stari_noi;
    char val, cuvant[100];

    vector<int> v1[50][50];
    vector<int> v2[50][50];

    set<int> stari_finale;
    set<int> alfabet;
    set<int> reuniune;
    set<int> reuniune2;
    set<int> stari_noi[50];
    set<int> cuvinte_acceptate;
    set<int> cuvinte_acceptate2;
    set<int>::iterator it, it2;

    queue<int> q;

    /// BEGIN - Citire date I - Citim starile initiale si finale
    f >> stare_initiala;
    f >> nr_stari_finale;

    for(i = 0; i < nr_stari_finale; i++)
        {
            f >> stare_finala;
            stari_finale.insert(stare_finala);
        }
    /// END - Citire date I

    /// BEGIN - Citire date II - Citim tranzitiile
    while(f >> in >> val >> out)
        {
            alfabet.insert(val);

            i = 0;
            for(it = alfabet.begin(); it != alfabet.end(); ++it, ++i)
                if(val == *it)
                    {
                        x = i;
                        break;
                    }

            v1[in][x].push_back(out);
            v2[in][x].push_back(out);
            stari[in] = stari[out] = 1;
        }
    /// END - Citire date II

    /// BEGIN - Determinare stari si alfabet - Aflam nr. de stari si nr. de elemente din alfabet

    nr_elemente = alfabet.size();

    for(i = 0; i < 100; i++)
        if(stari[i])
            nr_stari++;
    /// END - Determinare stari si alfabet

    /// BEGIN - Sortare stari - Sortam starile citite din fisier pentru orice eventualitate

    for(i = 0; i < nr_stari; i++)
        for(j = 0; j <= nr_elemente; j++)
            {
                sort(v1[i][j].begin(),v1[i][j].end());
                sort(v2[i][j].begin(),v2[i][j].end());
            }

    /// END - Sortare stari

    /// BEGIN - Afisare AFN initial
    cout << "AFN Initial \n\n";
    for(i = 0; i < nr_stari; i++)
        {
            cout << i << " -> ";
            for(j = 0; j < nr_elemente; j++)
                {
                    poz = 0;

                    if(j > 0)
                        {
                            for(it = alfabet.begin(); it != alfabet.end(); ++it, ++poz)
                                if(poz == j)
                                    cout << "     " << (char)*it << ": ";
                        }
                    else
                        {
                            for(it = alfabet.begin(); it != alfabet.end(); ++it, ++poz)
                                if(poz == j)
                                    cout << (char)*it << ": ";
                        }

                    for(k = 0; k < v1[i][j].size(); k++)
                        cout << v1[i][j][k] << " ";
                    cout << endl;
                }

        }
    /// END - Afisare AFN initial

    nr_stari_noi = nr_stari;  /// salvam acest indice pentru a evita o parcurgere inutila a set-urilor de la 0 la nr. de stari initiale

    /// BEGIN - Codificare stari noi
    for(i = 0; i < nr_stari; i++)
        for(j = 0; j < nr_elemente; j++)
            if(v1[i][j].size() > 1)                         /// parcurgem toate tranzitiile, iar acolo unde putem ajunge cu cel putin 2 stari prelugram informatiile dupa cum urmeaza
                {
                    for(l = 0; l < v1[i][j].size(); l++)    /// introducem multimea de stari intr-un set pentru a verifica daca este nou descoperita
                        reuniune.insert(v1[i][j][l]);

                    sw = 0;
                    for(l = nr_stari_noi; l <= nr_stari; l++)
                        if(stari_noi[l] == reuniune)            /// verificam daca multimea de stari este identica cu vreo multime descoperita inainte, in caz afirmativ salvam indicele ei
                            {
                                stare = l;
                                sw = 1;
                                break;
                            }

                    if(sw == 0)                 /// daca multimea de stari nu a mai fost intalnita pana acum prelucram informatiile dupa cum urmeaza
                        {
                          stari_noi[nr_stari] = reuniune;   /// o intruducem in set-ul starilor noi descoperite la indicele corespunzator

                          v1[i][j].clear();                 /// stergem multimea de stari si adaugam indicele aferent in set
                          v1[i][j].push_back(nr_stari);

                          for(k = 0; k < nr_elemente; k++) /// creem noile tranzitii corespunzatoare starii noi pentru fiecare litera din alfabet
                            {
                                for(it = reuniune.begin(); it != reuniune.end(); ++it)
                                    for(l = 0; l < v2[*it][k].size(); l++)
                                        reuniune2.insert(v2[*it][k][l]);


                                for(it2 = reuniune2.begin(); it2 != reuniune2.end(); ++it2)
                                    {
                                        v1[nr_stari][k].push_back(*it2);
                                        v2[nr_stari][k].push_back(*it2);
                                    }

                                reuniune2.clear();
                            }

                          nr_stari++;  /// crestem nr de stari diferite
                        }

                    if(sw == 1)     /// daca starea a mai fost intalnita, doar stergem multimea de stari si adaugam indicele corespunzator gasit in set la verificarea unicitatii
                        {
                            v1[i][j].clear();
                            v1[i][j].push_back(stare);
                        }

                    reuniune.clear();

                    }

    /// END - Codificare stari noi

    /// BEGIN - Determinare starii accesibile

    tranzitii[stare_initiala] = 1;
    q.push(stare_initiala);

    while(!q.empty())
    {
        x = q.front();
        q.pop();

        for(i = 0; i < nr_elemente; i++)
            for(k = 0; k < v1[x][i].size(); k++)
                if(tranzitii[v1[x][i][k]] == 0)
                {
                    tranzitii[v1[x][i][k]] = 1;
                    q.push(v1[x][i][k]);
                }
    }

    /// END - Determinare starii accesibile

    /// BEGIN - Afisare stari initiale si stari finale
    cout << endl;

    cout << "Stare initiala: " << stare_initiala;
    cout << endl;

    cout << "Stari finale: ";       /// determinam starile finale prin parcurgerea seturilor si verificarea daca cel putin una din starile finial initiale se intalneste, in caz afirmativ in vector-ul de frecventa se marcheaza indicele set-ului
    for(it = stari_finale.begin(); it != stari_finale.end(); ++it)
            if(tranzitii[*it])
                cout << *it << " ";

    for(i = nr_stari_noi; i <= nr_stari; i++)
        {
            for(it2 = stari_finale.begin(); it2 != stari_finale.end(); ++it2)
                {
                    for(it = stari_noi[i].begin(); it != stari_noi[i].end(); ++it)
                        if(*it == *it2)
                        {
                            stari_finale2[i] = 1;
                            break;
                        }
                }
        }

    for(i = nr_stari_noi; i <= nr_stari; i++)       /// verificam si afisam continutul seturilor de la indicii marcati anterior
        if(stari_finale2[i] && tranzitii[i])
            {
                for(it = stari_noi[i].begin(); it != stari_noi[i].end(); ++it)
                    cout << *it;
                cout << " ";
            }
    /// END - Afisare stari initiale si stari finale

    cout << endl << endl;

    /// BEGIN - Afisare AFD
    cout << "AFD Rezultat\n\n";
    for(i = 0; i < nr_stari; i++)
        if(tranzitii[i])
            {
                dim_set = stari_noi[i].size();

                if(stari_noi[i].size())
                    {
                        for(it = stari_noi[i].begin(); it != stari_noi[i].end(); ++it)
                            cout << *it;
                        cout << " -> ";
                    }
                else
                    cout << i << " -> ";

                for(j = 0; j < nr_elemente; j++)
                    {
                        poz = 0;

                    if(j > 0)
                        {
                            for(it = alfabet.begin(); it != alfabet.end(); ++it, ++poz)
                                if(poz == j)
                                {
                                    if(dim_set)
                                        for(x = 0; x < dim_set + 4; x++)
                                            cout << " ";
                                    else
                                        cout << "     ";
                                    cout << (char)*it << ": ";
                                }

                        }
                    else
                        {
                            for(it = alfabet.begin(); it != alfabet.end(); ++it, ++poz)
                                if(poz == j)
                                    cout << (char)*it << ": ";
                        }

                        for(k = 0; k < v1[i][j].size(); k++)
                            if(stari_noi[v1[i][j][k]].size())
                                for(it = stari_noi[v1[i][j][k]].begin(); it != stari_noi[v1[i][j][k]].end(); ++it)
                                    cout << *it;
                            else
                                cout << v1[i][j][k];

                    cout << endl;
                    }
            }
    /// END - Afisare AFD

    /// BEGIN - Testare cuvant

    cout << endl << "Va rugam sa introduce-ti un cuvant de testat: ";
    cin.get(cuvant,100);
    cout << endl;

    cuvinte_acceptate.insert(stare_initiala);  /// ne vom folosi de un set pentru a retine starile unde sunt intr-un moment

    for(i = 0; i < strlen(cuvant); i++)
        {
            x = 0; poz = 0;
            for(it = alfabet.begin(); it != alfabet.end(); ++it, ++x)   /// cautam indexului literei in set de alfabet
                if(cuvant[i] == *it)
                    {
                        poz = x;
                        break;
                    }

            if(x != poz)        /// daca litera nu exista in alfabet cuvantul este evident neacceptat
                    {
                        cout << "Cuvantul " << cuvant << " nu este acceptat de automat.\n";
                        return 0;
                    }

            for(it = cuvinte_acceptate.begin(); it != cuvinte_acceptate.end(); ++it)    /// inseram intr-un set temporar toate starile in care putem ajunge cu litera actuala din starea actuala
                for(j = 0; j < v2[*it][poz].size(); j++)
                    cuvinte_acceptate2.insert(v2[*it][poz][j]);

            cuvinte_acceptate.clear();                  /// inlocuim set-ul de stari cu cel temporar si repetam procesul atata timp cat inca sunt litere de citit
            cuvinte_acceptate = cuvinte_acceptate2;
            cuvinte_acceptate2.clear();
        }

    sw = 0;
    for(it = cuvinte_acceptate.begin(); it != cuvinte_acceptate.end(); ++it)    /// verificam daca in set-ul nostru a ramas cel putin o stare care este finala
        for(it2 = stari_finale.begin(); it2 != stari_finale.end(); ++it2)
            if(*it == *it2)
                {
                    sw = 1;
                    break;
                }

    if(sw)              /// in caz afirmativ cuvantul este acceptat de automat, altfel nu
        cout << "Cuvantul " << cuvant << " este acceptat de automat.\n";
    else
        cout << "Cuvantul " << cuvant << " nu este acceptat de automat.\n";

    /// END - Testare cuvant

    return 0;
}
