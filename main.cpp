#include<iostream>
#include<fstream>
#include<string.h>
#include<stdlib.h>

#define SIZE 55

using namespace std;

int globalRRN = 0;
char buffer[SIZE + 1];

class music {
    char music_id[15];
    char title[20];
    char singer[5];
    char lang[10];

public:
    void getdata();
    void putdata();
    void pack();
    void unpack();
    void insert();
    void display();
    void modify(char* key);
    void search(char* key);
    void delet(char* key);
    int displaymusic_idfound(char* keyname);
};

int cmp(const void* a, const void* b)
{
    return strcmp((char*)a, (char*)b);
}

void sortindexfile() {
    char array[20][20];
    ifstream index("index.txt");
    ofstream temp("temp.txt");
    int i = 0, n = 0;

    while (!index.eof()) {
        index.getline(array[i++], 20, '\n');
        if (index.fail())
            break;
        n++;
    }

    qsort(array, n, 20, cmp);

    for (i = 0; i < n; i++) {
        temp << array[i] << endl;
    }

    index.close();
    temp.close();

    remove("index.txt");
    rename("temp.txt", "index.txt");
}

void music::getdata() {
    cout << "Enter music_id, title, singer, language:\n";
    cin >> music_id >> title >> singer >> lang;
}

void music::putdata() {
    cout << "\t\t" << music_id << "\t\t" << title << "\t\t" << singer << "\t\t" << lang << endl;
}

void music::pack() {
    strcpy(buffer, music_id);
    strcat(buffer, "|");
    strcat(buffer, title);
    strcat(buffer, "|");
    strcat(buffer, singer);
    strcat(buffer, "|");
    strcat(buffer, lang);

    while (strlen(buffer) < SIZE - 1) {
        strcat(buffer, "#");
    }

    strcat(buffer, "\n");
}

void music::unpack() {
    char* p;
    p = strtok(buffer, "|");
    strcpy(music_id, p);
    p = strtok(NULL, "|");
    strcpy(title, p);
    p = strtok(NULL, "|");
    strcpy(singer, p);
    p = strtok(NULL, "#");
    strcpy(lang, p);
}

void music::insert() {
    ofstream index("index.txt", ios::app);
    ofstream rec("record.txt", ios::app);
    getdata();
    index << title << "|" << music_id << "|" << ++globalRRN << "\n";
    pack();
    rec << buffer;
    index.close();
    rec.close();
    sortindexfile();
}

int music::displaymusic_idfound(char* keyname) {
    ifstream index("index.txt");
    char _title[20], _music_id[15], _rrn[10];
    int found = 0;

    while (!index.eof()) {
        index.getline(_title, 20, '|');
        if (index.fail())
            break;

        index.getline(_music_id, 15, '|');
        index.getline(_rrn, 10, '\n');

        if (strcmp(_title, keyname) == 0) {
            cout << _title << "\t\t" << _music_id << endl;
            found = 1;
        }
    }

    return found;
}

void music::display() {
    ifstream fin("record.txt");

    while (!fin.eof()) {
        fin.getline(buffer, SIZE + 1, '\n');
        if (fin.fail())
            break;
        unpack();
        putdata();
    }

    fin.close();
}

void music::search(char* key) {
    ifstream index("index.txt");
    ifstream rec("record.txt");
    char _music_id[20], _title[15], _rrn[10];
    int pos, count = 0;

    while (!index.eof()) {
        index.getline(_title, 20, '|');
        if (index.fail())
            break;

        index.getline(_music_id, 15, '|');
        index.getline(_rrn, 10, '\n');

        pos = atoi(_rrn);

        if (strcmp(_music_id, key) != 0)
            continue;

        rec.seekg((pos - 1) * (SIZE + 1), ios::beg);
        rec.getline(buffer, SIZE + 1, '\n');
        unpack();

        if (strcmp(_music_id, music_id) == 0) {
            putdata();
            count++;
        }
    }

    if (count == 0)
        cout << "Not found" << endl;

    index.close();
    rec.close();
}

void music::modify(char* key) {
    ifstream fin("record.txt");
    ofstream fout("temp.txt");
    int count = 0;

    while (!fin.eof()) {
        fin.getline(buffer, SIZE + 1, '\n');
        if (fin.fail())
            break;
        unpack();

        if (strcmp(music_id, key) == 0) {
            getdata();
            count++;
        }

        pack();
        fout << buffer;
    }

    if (count == 0)
        cout << "music_id not found" << endl;
    else
        cout << "Modified." << endl;

    fin.close();
    fout.close();

    remove("record.txt");
    rename("temp.txt", "record.txt");
}

void music::delet(char* key) {
    ifstream rec("record.txt");
    ofstream temp("temp.txt");
    int count = 0;

    while (!rec.eof()) {
        rec.getline(buffer, SIZE + 1, '\n');
        if (rec.fail())
            break;
        unpack();

        if (strcmp(music_id, key) == 0) {
            strcpy(music_id, "");
            strcpy(title, "");
            strcpy(singer, "");
            strcpy(lang, "");
            count++;
        }

        pack();

        if (!rec.fail())
            temp << buffer;
    }

    if (count == 0)
        cout << "Not found" << endl;

    rec.close();
    temp.close();

    remove("record.txt");
    rename("temp.txt", "record.txt");
}

int main() {
    int choice, choice1, found;
    music m;
    char key[15], title[15], lang[15], user[20], pass[20];

    while (1) {
        cout << "\t\t ---------------- " << endl;
        cout << "\t\t|WELCOME TO MUSIC LIBRARY |\n";
        cout << "\t\t ---------------- " << endl;
        cout << "\t\t1.Admin Login\n\t\t2.User Login\n\t\tEnter your Choice\n";
        cout << "\t\t";
        cin >> choice1;

        switch (choice1) {
        case 1:
            cout << "\t\tPlease Enter Your Username :" << endl;
            cout << "\t\t";
            cin >> user;
            cout << "\t\tPlease enter your password :" << endl;
            cout << "\t\t";
            cin >> pass;

            if (strcmp(user, "greeshma") == 0 && strcmp(pass, "8321") == 0) {
                while (1) {
                    cout << "\n\t\t1.Insert a new music record\n"
                        << "\t\t2.Display All Records\n"
                        << "\t\t3.Search a Record\n"
                        << "\t\t4.Modify Records\n"
                        << "\t\t5.Delete Records\n"
                        << "\t\t6.Exit \n\t\tEnter Your choice :\n";

                    cout << "\t\t";
                    cin >> choice;

                    switch (choice) {
                    case 1:
                        m.insert();
                        cout << "\t\tDone!\n";
                        break;
                    case 2:
                        cout << "\n\t\tThe Contents are:\n" << endl;
                        cout << "\t\t" << "Music_id" << "\t\t" << "Title" << "\t\t" << "Singer" << "\t\t" << " Language" << endl;
                        m.display();
                        cout << "\n\t\tDone!\n";
                        break;
                    case 3:
                        cout << "Enter the key name:";
                        cin >> title;
                        found = m.displaymusic_idfound(title);

                        if (found) {
                            cout << "Choose a music_id:";
                            cin >> key;
                            m.search(key);
                        }
                        else {
                            cout << "Not found" << endl;
                        }

                        cout << "Done!" << endl;
                        break;
                    case 4:
                        cout << "\t\tEnter the music title to modify:" << endl;
                        cout << "\t\t";
                        cin >> title;
                        found = m.displaymusic_idfound(title);

                        if (found) {
                            cout << "Choose a music_id:";
                            cin >> key;
                            m.modify(key);
                        }
                        else {
                            cout << "Not found" << endl;
                        }

                        cout << "\t\tDone!\n";
                        break;
                    case 5:
                        cout << "Enter the name to delete:";
                        cin >> title;
                        found = m.displaymusic_idfound(title);

                        if (found) {
                            cout << "Choose a music_id";
                            cin >> key;
                            m.delet(key);
                        }
                        else {
                            cout << "Not found" << endl;
                        }

                        cout << "Done!" << endl;
                        break;
                    case 6:
                        cout << "\t\tThank You" << endl;
                    default:
                        exit(0);
                    }
                }
            }
            break;

        case 2:
            int choice;
            while (1) {
                cout << "\t\t1.Display\n";
                cout << "\t\t2.Search\n";
                cout << "\t\t3.Exit\n" << endl;
                cout << "\t\t";
                cin >> choice;

                switch (choice) {
                case 1:
                    cout << "\n\t\tThe Contents are:\n" << endl;
                    cout << "\t\t" << "Music_id" << "\t" << "Title" << "\t" << "Singer" << "\t\t" << "Language" << endl;
                    m.display();
                    cout << "\n\t\tDone!\n";
                    break;
                case 2:
                    cout << "Enter the key name:";
                    cin >> title;
                    found = m.displaymusic_idfound(title);

                    if (found) {
                        cout << "Choose a music_id:";
                        cin >> key;
                        m.search(key);
                    }
                    else {
                        cout << "Not found" << endl;
                    }

                    cout << "Done!" << endl;
                    break;
                default:
                    cout << "\t\tInvalid" << endl;
                }

                break;
            }

        default:
            cout << "\t\tInvalid...Please Try Again" << endl;
        }
    }

    return 0;
}