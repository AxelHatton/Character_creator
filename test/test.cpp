#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <sqlite3.h>

using namespace std;

static int callback(void* data, int argc, char** argv, char** azColName) {
    int i;
    cout << (const char*)data << ": ";

    for (i = 0; i < argc; i++) {
        cout << azColName[i] << " = " << (argv[i] ? argv[i] : "NULL") << endl;
    }
    cout << endl;
    return 0;
}

//Annonces des Structures
class Race {
public:
    int id_race;
    string race_name;
};

class Classe {
public:
    int id_carriere;
    string nom_classe;
    string nom_carriere;
};

class Stats {
public:
    int CC;
    int CT;
    int F;
    int E;
    int I;
    int Ag;
    int Dex;
    int Int;
    int FM;
    int Soc;
    int DESTIN;
    int RESILIENCE;
    int PtsBonus;
    int MOUVEMENT;
};

//Fonctions annexes
Race race_aleatoire(sqlite3* db, sqlite3_stmt*& stmt) {
    int p = rand() % 100;
    int rc;
    const char* sql = "SELECT id_race, nom_race FROM races WHERE pmin<=? AND pmax>=?";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc) {
        cout << "Probleme de connexion a la requete";
        return { -1, "" };
    }
    sqlite3_bind_int(stmt, 1, p);
    sqlite3_bind_int(stmt, 2, p);
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        int id_race = sqlite3_column_int(stmt, 0);
        const unsigned char* raceName = sqlite3_column_text(stmt, 1);
        return { id_race, reinterpret_cast<const char*>(raceName) };
    }
    if (rc != SQLITE_DONE) {
        cout << "Probleme d'execution de la requete";
    }
    return { -1, "" };
}

Classe carriere_aleatoire(sqlite3* db, sqlite3_stmt*& stmt, const char* race) {
    int p = rand() % 100;
    int rc;
    const char* sql;
    string buf = "SELECT id_carriere, nom_classe, nom_carriere FROM classes WHERE pmin_";
    buf += race;
    buf += " <= ? AND pmax_";
    buf += race; 
    buf += " >= ?";
    sql = buf.c_str();
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc) {
        cout << "Probleme de connexion a la requete";
        return { -1, "", "" };
    }
    sqlite3_bind_int(stmt, 1, p);
    sqlite3_bind_int(stmt, 2, p);
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        int id_carriere = sqlite3_column_int(stmt, 0);
        const unsigned char* nom_classe = sqlite3_column_text(stmt, 1);
        const unsigned char* nom_carriere = sqlite3_column_text(stmt, 2);
        return { id_carriere, reinterpret_cast<const char*>(nom_classe), reinterpret_cast<const char*>(nom_carriere) };
    }
    if (rc != SQLITE_DONE) {
        cout << "Probleme d'execution de la requete";
    }
    return { -1, "", "" };
}
 
Stats Stats_aleatoires() {
    int table[10];
    int carac;
    for (int i = 0; i < 10; i++) {
        carac = rand() % 10 + rand() % 10 + 2;
        table[i] = carac;
    }
    return{ table[0],table[1],table[2],table[3],table[4],table[5],table[6],table[7],table[8],table[9],0,0,0,0 };
}

//Fonction principale
int main() {
    //Connexion à la base de donnée, initialisation de la seed aléatoire et création des objets sqlite
    sqlite3* db;
    int xp = 0;
    int rc = sqlite3_open("Warhammer.db", &db);
    if (rc) {
        cout << "Probleme de connexion a la base de donnee";
        return 0;
    }
    srand(time(NULL));
    sqlite3_stmt* statement = nullptr;
    //Détermination de la race
    Race result_race = race_aleatoire(db, statement);
    int id_race = result_race.id_race;
    const char* race = result_race.race_name.c_str();
    if (race) {
        cout << "Vous etes un " << race << ". Etes-vous satisfait ?" << endl << 
            "[Y]/[N]" << endl;
        string answer;
        cin >> answer;
        if (answer == "Y" || answer == "y") {
            xp = 20;
        }
        else if (answer == "N" || answer == "n") {
            cout << "Choisissez votre race" << endl << 
                "[1] Humain" << endl << 
                "[2] Halfling" << endl << 
                "[3] Nain" << endl << 
                "[4] Haut elfe" << endl << 
                "[5] Elfe sylvain" << endl;
            cin >> id_race;
            rc = sqlite3_prepare_v2(db, "SELECT nom_race FROM races WHERE id_race=?", -1, &statement, 0);
            if (rc) {
                cout << "Probleme de connexion a la requete";
                return 0;
            }
            sqlite3_bind_int(statement, 1, id_race);
            if ((rc = sqlite3_step(statement)) == SQLITE_ROW) {
                race = reinterpret_cast<const char*>(sqlite3_column_text(statement, 0));
            }
        }
        else {
            cout << "Reponse invalide" << endl;
            return 0;
        }
    }
    //Détermination de la carrière
    const char* nom_carriere;
    nom_carriere = new char[100];
    const char* nom_classe;
    int id_classe;
    const char* nom_carriere2;
    nom_carriere2 = new char[100];
    const char* nom_classe2;
    int id_classe2;
    const char* nom_carriere3;
    nom_carriere3 = new char[100];
    const char* nom_classe3;
    int id_classe3;
    Classe result_classe = carriere_aleatoire(db, statement, race);
    id_classe = result_classe.id_carriere;
    nom_classe = (result_classe.nom_classe).c_str();
    nom_carriere = (result_classe.nom_carriere).c_str();
    if (nom_carriere) {
        cout << "Vous etes un " << nom_carriere << ". Etes-vous satisfait ?" << endl << 
            "[Y]/[N]" << endl;
        string answer;
        cin >> answer;
        if (answer == "Y" || answer == "y") {
            xp += 50;
        }
        else if (answer == "N" || answer == "n") {
            Classe result_classe2 = carriere_aleatoire(db, statement, race);
            string resultclasse2 = result_classe2.nom_classe;
            resultclasse2 = resultclasse2;
            string resultcarriere2 = result_classe2.nom_carriere;
            resultcarriere2 = resultcarriere2;
            id_classe2 = result_classe2.id_carriere;
            nom_classe2 = resultclasse2.c_str();
            nom_carriere2 = resultcarriere2.c_str();
            Classe result_classe3 = carriere_aleatoire(db, statement, race);
            id_classe3 = result_classe3.id_carriere;
            nom_classe3 = result_classe3.nom_classe.c_str();
            nom_carriere3 = result_classe3.nom_carriere.c_str();
            cout << "Vous pouvez choisir entre :" << endl << 
                "[1] " << nom_carriere << endl << 
                "[2] " << nom_carriere2 << endl << 
                "[3] " << nom_carriere3 << endl << 
                "[4] Non satisfait" << endl;
            int reponse;
            cin >> reponse;
            if (reponse == 1) {
                xp += 25;
            }
            else if (reponse == 2) {
                xp += 25;
                result_classe = result_classe2;
            }
            else if (reponse == 3) {
                xp += 25;
                result_classe = result_classe3;
            }
            else if (reponse == 4) {
                cout << "Veuillez choisir une classe" << endl << 
                    "[1] CITADINS" << endl << 
                    "[2] COURTISANS" << endl << 
                    "[3] GUERRIERS" << endl << 
                    "[4] ITINERANTS" << endl << 
                    "[5] LETTRES" << endl << 
                    "[6] RIVERAINS" << endl << 
                    "[7] ROUBLARDS" << endl << 
                    "[8] RURAUX" << endl;
                cin >> reponse;
                reponse--;
                int rcarriere;
                for (int i = 1; i < 9; i++) {
                    int iteration = 8 * reponse + i;
                    const char* sql;
                    sql = "SELECT id_carriere, nom_classe, nom_carriere, pmin_";
                    string buf(sql);
                    buf.append(string(race));
                    buf.append(string(" FROM classes WHERE id_carriere=?"));
                    sql = buf.c_str();
                    rcarriere = sqlite3_prepare_v2(db, sql, -1, &statement, 0);
                    if (rcarriere) {
                        cout << "Probleme de connexion a la requete";
                        return 0;
                    }
                    sqlite3_bind_int(statement, 1, iteration);
                    while ((rcarriere = sqlite3_step(statement)) == SQLITE_ROW) {
                        int id_carriere = sqlite3_column_int(statement, 0);
                        const unsigned char* nom_classe = sqlite3_column_text(statement, 1);
                        const unsigned char* nom_carriere = sqlite3_column_text(statement, 2);
                        int pmin = sqlite3_column_int(statement, 3);
                        Classe result = { id_carriere, reinterpret_cast<const char*>(nom_classe), reinterpret_cast<const char*>(nom_carriere) };
                        if (pmin) {
                            cout << "[" << i << "]" << nom_carriere << endl;
                        }
                    }
                    if (rcarriere != SQLITE_DONE) {
                        cout << "Probleme d'execution de la requete";
                    }
                }

                int reponse2;
                cin >> reponse2;
                int selection = 8 * reponse + reponse2;
                int rcarriere2;
                const char* sql;
                sql = "SELECT id_carriere, nom_classe, nom_carriere FROM classes WHERE id_carriere=?";
                rcarriere2 = sqlite3_prepare_v2(db, sql, -1, &statement, 0);
                if (rcarriere2) {
                    cout << "Probleme de connexion a la requete";
                }
                sqlite3_bind_int(statement, 1, selection);
                while ((rc = sqlite3_step(statement)) == SQLITE_ROW) {
                    const unsigned char* nom_carriere4;
                    nom_carriere4 = new unsigned char[200];
                    const unsigned char* nom_classe4;
                    int id_carriere4;
                    id_carriere4 = sqlite3_column_int(statement, 0);
                    nom_classe4 = sqlite3_column_text(statement, 1);
                    nom_carriere4 = sqlite3_column_text(statement, 2);
                    id_classe = id_carriere4;
                    Classe result_classe4 = { id_carriere4, reinterpret_cast<const char*>(nom_classe4), reinterpret_cast<const char*>(nom_carriere4) };
                    result_classe = result_classe4;
                }
                if (rc != SQLITE_DONE) {
                    cout << "Probleme d'execution de la requete";
                }
            }
            else {
                cout << "Reponse invalide" << endl;
                return 0;
            }
        } 
    }
    //Détermination des stats
    const char* sql = "SELECT CC, CT, F, E, I, Ag, Dex, Int, FM, SOC, DESTIN, RESILIENCE, POINTSup, MOUVEMENT FROM races WHERE id_race=?";
    rc = sqlite3_prepare_v2(db, sql, -1, &statement, 0);
    if (rc) {
        cout << "Probleme de connexion a la requete";
    }
    int bCC;
    int bCT;
    int bF;
    int bE;
    int bI;
    int bAg;
    int bDex;
    int bInt;
    int bFM;
    int bSOC;
    int bDESTIN;
    int bRESILIENCE;
    int bPOINTSup;
    int bMOUVEMENT;
    sqlite3_bind_int(statement, 1, id_race);
    while ((rc = sqlite3_step(statement)) == SQLITE_ROW) {
        bCC = sqlite3_column_int(statement, 0);
        bCT = sqlite3_column_int(statement, 1);
        bF = sqlite3_column_int(statement, 2);
        bE = sqlite3_column_int(statement, 3);
        bI = sqlite3_column_int(statement, 4);
        bAg = sqlite3_column_int(statement, 5);
        bDex = sqlite3_column_int(statement, 6);
        bInt = sqlite3_column_int(statement, 7);
        bFM = sqlite3_column_int(statement, 8);
        bSOC = sqlite3_column_int(statement, 9);
        bDESTIN = sqlite3_column_int(statement, 10);
        bRESILIENCE = sqlite3_column_int(statement, 11);
        bPOINTSup = sqlite3_column_int(statement, 12);
        bMOUVEMENT = sqlite3_column_int(statement, 13);
    }
    Stats caracBrute = Stats_aleatoires();
    Stats caracModif;
    caracModif.CC = caracBrute.CC + bCC;
    caracModif.CT = caracBrute.CT + bCT;
    caracModif.F = caracBrute.F + bF;
    caracModif.E = caracBrute.E + bE;
    caracModif.I = caracBrute.I + bI;
    caracModif.Ag = caracBrute.Ag + bAg;
    caracModif.Dex = caracBrute.Dex + bDex;
    caracModif.Int = caracBrute.Int + bInt;
    caracModif.FM = caracBrute.FM + bFM;
    caracModif.Soc = caracBrute.Soc + bSOC;
    caracModif.DESTIN = caracBrute.DESTIN + bDESTIN;
    caracModif.RESILIENCE = caracBrute.RESILIENCE + bRESILIENCE;
    caracModif.PtsBonus = caracBrute.PtsBonus + bPOINTSup;
    caracModif.MOUVEMENT = caracBrute.MOUVEMENT + bMOUVEMENT;
    if (rc != SQLITE_DONE) {
        cout << "Probleme d'execution de la requete";
    }
    cout << "Voici vos caracteristiques :" << endl <<
        "[1] Capacite de Combat : " << caracModif.CC << " (" << caracBrute.CC << ")" << endl<<
        "[2] Capacite de Tir : " << caracModif.CT << " (" << caracBrute.CT << ")" << endl<<
        "[3] Force : " << caracModif.F << " (" << caracBrute.F << ")" << endl<<
        "[4] Endurance : " << caracModif.E << " (" << caracBrute.E << ")" << endl<<
        "[5] Initiative : " << caracModif.I << " (" << caracBrute.I << ")" << endl<<
        "[6] Agilite : " << caracModif.Ag << " (" << caracBrute.Ag << ")" << endl<<
        "[7] Dexterite : " << caracModif.Dex << " (" << caracBrute.Dex << ")" << endl<<
        "[8] Intelligence : " << caracModif.Int << " (" << caracBrute.Int << ")" << endl<<
        "[9] Force Mentale : " << caracModif.FM << " (" << caracBrute.FM << ")" << endl<<
        "[10] Social : " << caracModif.Soc << " (" << caracBrute.Soc << ")" << endl<<
        "Etes-vous satisfait de la repartition de vos caracteristiques?"<<endl<<
        "[1] Satisfait"<<endl<<
        "[2] Relancer les des de caracteristiques"<<endl;
    int answer;
    cin >> answer;
    if (answer == 1) {
        xp += 50;
    }
    else if (answer == 2) {
        while (answer == 2) {
            caracBrute = Stats_aleatoires();
            caracModif;
            caracModif.CC = caracBrute.CC + bCC;
            caracModif.CT = caracBrute.CT + bCT;
            caracModif.F = caracBrute.F + bF;
            caracModif.E = caracBrute.E + bE;
            caracModif.I = caracBrute.I + bI;
            caracModif.Ag = caracBrute.Ag + bAg;
            caracModif.Dex = caracBrute.Dex + bDex;
            caracModif.Int = caracBrute.Int + bInt;
            caracModif.FM = caracBrute.FM + bFM;
            caracModif.Soc = caracBrute.Soc + bSOC;
            caracModif.DESTIN = caracBrute.DESTIN + bDESTIN;
            caracModif.RESILIENCE = caracBrute.RESILIENCE + bRESILIENCE;
            caracModif.PtsBonus = caracBrute.PtsBonus + bPOINTSup;
            caracModif.MOUVEMENT = caracBrute.MOUVEMENT + bMOUVEMENT;
            cout << "Voici vos caracteristiques :" << endl <<
                "[1] Capacite de Combat : " << caracModif.CC << " (" << caracBrute.CC << ")" << endl <<
                "[2] Capacite de Tir : " << caracModif.CT << " (" << caracBrute.CT << ")" << endl <<
                "[3] Force : " << caracModif.F << " (" << caracBrute.F << ")" << endl <<
                "[4] Endurance : " << caracModif.E << " (" << caracBrute.E << ")" << endl <<
                "[5] Initiative : " << caracModif.I << " (" << caracBrute.I << ")" << endl <<
                "[6] Agilite : " << caracModif.Ag << " (" << caracBrute.Ag << ")" << endl <<
                "[7] Dexterite : " << caracModif.Dex << " (" << caracBrute.Dex << ")" << endl <<
                "[8] Intelligence : " << caracModif.Int << " (" << caracBrute.Int << ")" << endl <<
                "[9] Force Mentale : " << caracModif.FM << " (" << caracBrute.FM << ")" << endl <<
                "[10] Social : " << caracModif.Soc << " (" << caracBrute.Soc << ")" << endl <<
                "Etes-vous satisfait de la repartition de vos caracteristiques?" << endl <<
                "[1] Satisfait" << endl <<
                "[2] Relancer les des de caracteristiques" << endl;
            cin >> answer;
        }
    }
    while (caracModif.PtsBonus > 0) {
        cout << "Vous avez " << caracModif.PtsBonus << " points supplementaires. Ou voulez vous les attribuer?" << endl <<
            "[1] Resilience : " << caracModif.RESILIENCE<< endl <<
            "[2] Destin : "<<caracModif.DESTIN << endl;
        cin >> answer;
        if (answer == 1) {
            caracModif.RESILIENCE++;
            caracModif.PtsBonus--;
        }
        else if (answer == 2) {
            caracModif.DESTIN++;
            caracModif.PtsBonus--;
        }
    }
    sqlite3_close(db);
    cout << "Vous etes un " << nom_carriere << " " << race << "." << endl <<
        "Vos caracteristiques sont :" << endl <<
        "Capacite de Combat : " << caracModif.CC << endl <<
        "Capacite de Tir : " << caracModif.CT << endl <<
        "Force : " << caracModif.F << endl <<
        "Endurance : " << caracModif.E << endl <<
        "Initiative : " << caracModif.I << endl <<
        "Agilite : " << caracModif.Ag << endl <<
        "Dexterite : " << caracModif.Dex << endl <<
        "Intelligence : " << caracModif.Int << endl <<
        "Force Mentale : " << caracModif.FM << endl <<
        "Social : " << caracModif.Soc << endl <<
        "Destin : " << caracModif.DESTIN << endl <<
        "Resilience : " << caracModif.RESILIENCE << endl <<
        "Mouvement : " << caracModif.MOUVEMENT << endl <<
        "Vous demarrez avec " << xp << " points d'experiences." << endl;
    return 0;
}