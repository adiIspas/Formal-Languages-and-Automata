#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <stack>

using namespace std;

int main()
{
    ifstream f("input.txt");

    int i, j, k, l, ok, stari_destinatie[30][30][30], poz_banda, poz_stiva, stare_curenta, stare_initiala;
    int poz_caracter_banda, poz_caracter_stiva, nr_stari, nr_stari_finale, stare_finala, stare_plecare, stare_destinatie;
    char caracter_banda, cap_stiva;

    vector<char> automat[30][30][30];
    set<char> alfabet, alfabet_stiva;
    set<int>::iterator it;
    set<int> stari_finale;

    stringstream ss;
    string s, caractere_stiva, cuvant_testare;

    stack<char> stiva;

    /// BEGIN - Citire date
    /// citim nr de stari ale automatului si nr de stari finale
    f >> nr_stari >> stare_initiala >> nr_stari_finale;

    /// citim starile finale
    for(i = 0; i < nr_stari_finale; i++)
        {
            f >> stare_finala;
            stari_finale.insert(stare_finala);
        }

    /// pentru curatare buffer
    getline(f,s);

    /// citim cat timp avem linii in fisier
    while(getline(f,s))
    {
        ss << s;

        ss >> stare_plecare;

        ss >> caracter_banda;
        alfabet.insert(caracter_banda);

        ss >> cap_stiva;
        alfabet_stiva.insert(cap_stiva);

        ss >> stare_destinatie;
        ss >> caractere_stiva;

        if(caracter_banda == '/')
            poz_caracter_banda = 0;
        else
            poz_caracter_banda = int(caracter_banda - 96);

        poz_caracter_stiva = int(cap_stiva - 64);

        stari_destinatie[stare_plecare][poz_caracter_banda][poz_caracter_stiva] = stare_destinatie;
        for(i = 0; i < caractere_stiva.size(); i++)
            {
                automat[stare_plecare][poz_caracter_banda][poz_caracter_stiva].push_back(caractere_stiva[i]);
                alfabet_stiva.insert(caractere_stiva[i]);
            }

//        cout << stare_plecare << " " << caracter_banda << "-" << poz_caracter_banda << " " << cap_stiva << "-" << poz_caracter_stiva << " " << stari_destinatie[stare_plecare][poz_caracter_banda][poz_caracter_stiva] << " ";
//        for(i = 0; i < automat[stare_plecare][poz_caracter_banda][poz_caracter_stiva].size(); i++)
//            cout << automat[stare_plecare][poz_caracter_banda][poz_caracter_stiva][i];
//        cout << endl;

        ss.clear();
        caractere_stiva.clear();
    }
    /// END - Citire date

    /// BEGIN - Afisare automat
    cout << "Automatul Push-Down";
    cout << endl << endl;
    for(i = 0; i <= nr_stari; i++)
        for(j = 0; j < 27; j++)
            for(k = 0; k < 27; k++)
                if(automat[i][j][k].size())
                {
                    if(j == 0)
                        cout << i << " " << char(j + 47) << " " << char(k + 64) << " " << stari_destinatie[i][j][k] << " ";
                    else
                    if(k == 0)
                        cout << i << " " << char(j + 96) << " " << char(k + 47) << " " << stari_destinatie[i][j][k] << " ";
                    else
                    cout << i << " " << char(j + 96) << " " << char(k + 64) << " " << stari_destinatie[i][j][k] << " ";

                    for(l = 0; l < automat[i][j][k].size(); l++)
                        cout << automat[i][j][k][l];

                    cout << endl;
                }
    /// END - Afisare automat

    /// BEGIN - Citire cuvant

    cout << endl;
    cout << "Introduceti cuvantul de testat: ";
    cin >> cuvant_testare;

    /// END - Citire cuvant

    /// BEGIN - Testare cuvant

    stiva.push('Z');
    stare_curenta = stare_initiala;

    for(i = 0; i < cuvant_testare.size(); i++)
        {
            poz_banda = int(cuvant_testare[i] - 96);

            poz_stiva = int(stiva.top() - 64);

            if(stiva.size())
                stiva.pop();
            else
            {
                cout << "Cuvantul " << cuvant_testare << " nu este acceptat!";
                return 0;
            }

//            if(i != cuvant_testare.size())
//                cout << stare_curenta << " " << char(poz_banda + 96) << " " << char(poz_stiva + 64) << endl;

            if(!automat[stare_curenta][poz_banda][poz_stiva].size())
                {
                    if(!automat[stare_curenta][0][poz_stiva].size())
                        {
                            cout << "Cuvantul " << cuvant_testare << " nu este acceptat!";
                            return 0;
                        }
                    if(automat[stare_curenta][0][poz_stiva][0] != '/')
                        for(j = automat[stare_curenta][0][poz_stiva].size() - 1; j >= 0; j--)
                            stiva.push(automat[stare_curenta][0][poz_stiva][j]);

                    stare_curenta = stari_destinatie[stare_curenta][0][poz_stiva];
                    i--;
                }
            else
                {
                    if(automat[stare_curenta][poz_banda][poz_stiva][0] == '/')
                        {
                            //stiva.pop();
                            stare_curenta = stari_destinatie[stare_curenta][poz_banda][poz_stiva];
                        }
                    else
                        {
                            for(j = automat[stare_curenta][poz_banda][poz_stiva].size() - 1; j >= 0; j--)
                                stiva.push(automat[stare_curenta][poz_banda][poz_stiva][j]);

                            stare_curenta = stari_destinatie[stare_curenta][poz_banda][poz_stiva];
                        }
                }
        }

    /// END - Testare cuvant

    while(stiva.size())
        {
            poz_stiva = int(stiva.top() - 64);

            stiva.pop();

            if(!automat[stare_curenta][0][poz_stiva].size())
                        {
                            cout << "Cuvantul " << cuvant_testare << " nu este acceptat!";
                            return 0;
                        }
            if(automat[stare_curenta][0][poz_stiva][0] != '/')
                for(j = automat[stare_curenta][0][poz_stiva].size() - 1; j >= 0; j--)
                    stiva.push(automat[stare_curenta][0][poz_stiva][j]);

            stare_curenta = stari_destinatie[stare_curenta][0][poz_stiva];
        }

    ok = 0;
    for(it = stari_finale.begin(); it != stari_finale.end(); ++it)
        if(stare_curenta == *it)
            {ok = 1; break;}

    if(ok == 1)
        cout << "Cuvantul " << cuvant_testare << " este acceptat!";
    else
        cout << "Cuvantul " << cuvant_testare << " nu este acceptat!";

    cout << endl;

    return 0;
}
