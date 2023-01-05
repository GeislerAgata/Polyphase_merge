#include "Pentagon.h"
#include "File.h"
#include "Tape.h"
#include "Buffer.h"

#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <cstdio>
#include <stdio.h>
using namespace std;

#define TAPES_NUMBER 3

unsigned long long numer_of_joins = 0;
unsigned long long numer_of_splits = 0;
unsigned long long numer_of_reads = 0;
unsigned long long numer_of_writes = 0;

int dummies;
int series[2] = { 0 };

void giveDataFromFile(char* name);

void generateRecords(char* path);
void readRecords(char* path);

void saveData(char* input, char* output);
void printData(char* path);
void getFileName(char* path);

void initialDistribution(char* path);
bool devide(bool print);
bool merge(Buffer* tape_I, Buffer* tape_II, Tape* tape_III, Tape* temp);

unsigned long long fiboacciNumber(int number);
int sum();

int main() {
    bool main_loop = true, bool_display;
    char* default_path, * backup;
    char display;
    string str_obj("input.csv");
    string str_objB("backup.csv");
    default_path = &str_obj[0];
    backup = &str_objB[0];

    int option = 0;

    while (option == 0) {
        cout << "Choose data option: " << endl;
        cout << "1. Data from the file" << endl;
        cout << "2. Randomly generated data" << endl;
        cout << "3. Data from the keyborad" << endl;
        cin >> option;

        switch (option) {
        case 1:
            getFileName(default_path);
            break;
        case 2:
            generateRecords(default_path);
            break;
        case 3:
            readRecords(default_path);
            break;
        default:
            cout << "Wrong option number! Choose correct option" << endl << endl;
            option = 0;
            break;
        }
    }
    saveData(default_path, backup);

    cout << endl << "Display files after each pase (Y/N) ";
    cin >> display;

    if (display == 'Y') {
        bool_display = true;
    }
    else {
        bool_display = false;
    }

    initialDistribution(default_path);

    numer_of_splits++;
    while (main_loop) {
        numer_of_joins++;
        main_loop = devide(bool_display);
    }


    cout << endl << "Algorythm statistics" << endl;
    cout << "splits: " << numer_of_splits << endl;
    cout << "joins: " << numer_of_joins << endl;
    cout << "reads: " << numer_of_reads << endl;
    numer_of_writes = sum();
    cout << "writes: " << numer_of_writes << endl;

    return 0;
}

unsigned long long fiboacciNumber(int number) {     // funkcja zwracająca liczbę ciągu fibonacciego o zadanym numerze
    if (number <= 1) {
        return 1;
    }
    return fiboacciNumber(number - 1) + fiboacciNumber(number - 2);
}

void initialDistribution(char* path) {              // funkcja dystrybuująca plik wejściowy na trzy taśmy, z czego jedna pozostaje pusta  
    char* start, * I, * II;
    string str_obj_buffer(path);
    string str_obj_I("I.csv");
    string str_obj_II("II.csv");
    start = &str_obj_buffer[0];
    I = &str_obj_I[0];
    II = &str_obj_II[0];

    File* input = new File(start, 0);               // plik wejściowy z danymi (bokami pięciokąta) 
    File* output_I = new File(I, 0);                // plik pierwszej taśmy
    File* output_II = new File(II, 0);              // plik drugiej taśmy

    Buffer* buffer = new Buffer(input);             // bufor wejściowy - z niego wczytujemy kolejne rekordy
    Tape* tape_I = new Tape(output_I);              // pierwsza taśma
    Tape* tape_II = new Tape(output_II);            // druga taśma

    Pentagon* record = nullptr;                     // rekord pięciokąta
    double prev_record = 0;                         // poprzednia wartość obwodu, do weryfikacji scalania serii [obwód nie może być ujemny ani zerowy]
    Tape* recording_tape = tape_I;                  // taśma do zapisu - zmieniana co poziom

    int kolejne_fib = 0;
    while (1) {
        record = buffer->nextRecord();              // pobieramy rekord

        if (record != nullptr) {                    // rekord niepusty = jest elementem serii        
            if (prev_record > record->perimeter) {  // sprawdzamy czy element należy do poprzedniej serii czy nie rozpoczyna kolejnej - przełączanie taśm
                // mamy do czynienia z początkiem nowej serii
                recording_tape->series++;           // zakańczamy serię, więc zwiększamy jej licznik

                if (recording_tape->series == fiboacciNumber(kolejne_fib)) {         // jeśli numer serii jest kolejną liczbą fib. zmieniamy taśmę, jeśli nie jest dokłądamy dalej do tej
                    if (recording_tape == tape_I) {                                  // w zależności od tego na której taśmie aktualnie jesteśmy odpowienio ją zmieniamy
                        recording_tape = tape_II;
                    }
                    else {
                        recording_tape = tape_I;
                    }
                    kolejne_fib++;
                }
            }
        }
        else {
            recording_tape->series++;
            break;                                  // rekord pusty = koniec pliku
        }
        recording_tape->saveRecord(record);         // zapisujemy rekord na taśmie 
        prev_record = record->perimeter;            // zmieniamy wartość poprzedniego elementu na nowy poprzedni element
    }

    while (recording_tape->series != fiboacciNumber(kolejne_fib)) {                 // dopełniamy taśmę niebędącą liczbą fibbonacciego pustymi seriami
        recording_tape->series++;
        recording_tape->dummies++;                                                  // możemy jednak ograniczyć tą akcję do przechowania ilości pustych serii
    }

    series[0] = tape_I->series;
    series[1] = tape_II->series;

    dummies = recording_tape->dummies;

    numer_of_reads += buffer->numer_of_read;

    delete tape_I;
    delete tape_II;
}

bool devide(bool print) {
    bool end = true;

    char* I, * II, * III, * temp;
    string str_obj_I("I.csv");
    string str_obj_II("II.csv");
    string str_obj_III("III.csv");
    string str_obj_temp("temp.csv");
    I = &str_obj_I[0];
    II = &str_obj_II[0];
    III = &str_obj_III[0];
    temp = &str_obj_temp[0];

    File* output_I = new File(I, 0);                    // plik pierwszej taśmy
    File* output_II = new File(II, 0);                  // plik drugiej taśmy
    File* output_III = new File(III, 0);                // plik trzeciej taśmy
    File* output_temp = new File(temp, 0);              // plik tymczasowy do podmiany po złączeniu

    Buffer* tape_I = new Buffer(output_I);              // pierwsza taśma - bufor z którego czytamy dane
    Buffer* tape_II = new Buffer(output_II);            // druga taśma - bufor z którego czytamy dane
    Tape* tape_III = new Tape(output_III, print);       // trzecia taśma - początkowo pusta, zapisujemy tam złączone serie
    Tape* temp_tape = new Tape(output_temp, print);     // taśma tymczasowa - zapisujemy tu pozostałe serie, które nie mają z czym się połączyć

    if (series[0] <= series[1]) {                       // w zależności od liczby serii w pliku odpowiednio wywołujemy funkcję łączenia
        end = merge(tape_II, tape_I, tape_III, temp_tape);

        numer_of_reads += tape_I->numer_of_read;
        numer_of_reads += tape_II->numer_of_read;

        delete tape_I;
        delete tape_II;
        delete tape_III;
        delete temp_tape;

        remove("I.csv");
        remove("II.csv");

        rename("III.csv", "II.csv");
        rename("temp.csv", "I.csv");
    }
    else {
        end = merge(tape_I, tape_II, tape_III, temp_tape);

        numer_of_reads += tape_I->numer_of_read;
        numer_of_reads += tape_II->numer_of_read;

        delete tape_I;
        delete tape_II;
        delete tape_III;
        delete temp_tape;

        remove("I.csv");
        remove("II.csv");

        rename("III.csv", "I.csv");
        rename("temp.csv", "II.csv");
    }

    return end;
}

bool merge(Buffer* tape_I, Buffer* tape_II, Tape* tape_III, Tape* temp) {   // tape_I zawiera dummies (ma więcej serii)
    double prev_record_I = 0, prev_record_II = 0;
    Pentagon* record_I = tape_I->nextRecord();
    Pentagon* record_II = tape_II->nextRecord();

    if (record_II == nullptr) {                                             // jeśli bufor jest pusty, nie możemy już dokonać operavji łączenia
        double first, second, third, fourth, fifth;
        bool end = false;

        ifstream input("I.csv");
        if (!input.good()) {
            cout << "File error (I.csv) " << endl;
        }
        int counter = 0;

        cout << endl << "Sorted file" << endl;
        while ((counter < 512) && (!end)) {                                 // zczytujemy kolejne wartości boków pięciokąta 
            if (input >> first >> second >> third >> fourth >> fifth) {
                counter++;
                cout << "Sides = { " << first << ", " << second << ", " << third << ", " << fourth << ", " << fifth << " }" << endl;
            }
            else {
                end = true;
            }
        }
        input.close();
        return false;
    }
    while (1) {
        if (dummies != 0) {                                                                 // jeśli mamy puste serie, powinniśmy je połączyć w pierwszej kolejności
            while ((record_II != nullptr) && (record_II->perimeter >= prev_record_II)) {    // sprawdzamy czy nie skończył nam się plik i czy nie dotarliśmy do końca serii
                tape_III->saveRecord(record_II);

                prev_record_II = record_II->perimeter;
                record_II = tape_II->nextRecord();
            }
            tape_III->series++;
            prev_record_II = 0;
            dummies--;                                                                      // jeśli dummies są już połączone, przechodizmy do łączenia pozostałych wartości
        }
        else if ((record_I != nullptr) && (record_II != nullptr)) {                         // obydwie taśmy wejściowe zawierają serie, więc możemy je połączyć
            if (record_I->perimeter < prev_record_I) {                                      // warunek końca serii taśmy I
                tape_III->series++;
                while ((record_II != nullptr) && (record_II->perimeter >= prev_record_II)) { // sprawdzamy czy nie skończył nam się plik i czy nie dotarliśmy do końca serii
                    tape_III->saveRecord(record_II);

                    prev_record_II = record_II->perimeter;
                    record_II = tape_II->nextRecord();
                }
                prev_record_I = 0;
                prev_record_II = 0;
            }
            else if (record_II->perimeter < prev_record_II) {                               // warunek końca serii taśmy II
                tape_III->series++;
                while ((record_I != nullptr) && (record_I->perimeter >= prev_record_I)) {   // sprawdzamy czy nie skończył nam się plik i czy nie dotarliśmy do końca serii
                    tape_III->saveRecord(record_I);
                    prev_record_I = record_I->perimeter;
                    record_I = tape_I->nextRecord();
                }
                prev_record_I = 0;
                prev_record_II = 0;
            }
            else {                                                                          // nie mamy do czynienia z końcem serii, sortujemy rekordy i umieszczamy na taśmie III
                if (record_I->perimeter < record_II->perimeter) {
                    tape_III->saveRecord(record_I);
                    prev_record_I = record_I->perimeter;
                    record_I = tape_I->nextRecord();
                }
                else {
                    tape_III->saveRecord(record_II);
                    prev_record_II = record_II->perimeter;
                    record_II = tape_II->nextRecord();
                }
            }
        }
        else if (record_I == nullptr) {                                                     // taśma I zakończyła się, resztę rekordów z serii dopisujemy do taśmy III
            tape_III->series++;
            while ((record_II != nullptr) && (record_II->perimeter >= prev_record_II)) {
                tape_III->saveRecord(record_II);
                prev_record_II = record_II->perimeter;
                record_II = tape_II->nextRecord();
            }
            while (record_II != nullptr) {                                                  // natomiast po zakończeniu serii, reszta wędruje do tasmy temp
                if (record_II->perimeter < prev_record_II) {
                    temp->series++;
                }
                temp->saveRecord(record_II);
                prev_record_II = record_II->perimeter;
                record_II = tape_II->nextRecord();
            }
            break;
        }
        else if (record_II == nullptr) {                                                    // taśma II zakończyła się, resztę rekordów z serii dopisujemy do taśmy III
            tape_III->series++;
            while ((record_I != nullptr) && (record_I->perimeter >= prev_record_I)) {
                tape_III->saveRecord(record_I);
                prev_record_I = record_I->perimeter;
                record_I = tape_I->nextRecord();
            }
            while (record_I != nullptr) {                                                  // natomiast po zakończeniu serii, reszta wędruje do tasmy temp
                if (record_I->perimeter < prev_record_I) {
                    temp->series++;
                }
                temp->saveRecord(record_I);
                prev_record_I = record_I->perimeter;
                record_I = tape_I->nextRecord();
            }
            break;
        }
    }
    series[0] = tape_III->series;
    series[1] = temp->series;

    return true;
}

int sum() {
    int value, sum = 0;
    bool end = false;

    ifstream input("writes.csv");
    if (!input.good()) {
        cout << "File error (writes.csv) " << endl;
    }

    while (!end) {
        if (input >> value) {
            sum += value;
        }
        else {
            end = true;
        }
    }
    input.close();
    remove("writes.csv");

    return sum;
}

void giveDataFromFile(char* name) {
    ifstream input;
    input.open(name);
    double zmianna;

    while (!input.eof()) {
        input >> zmianna;
        cout << zmianna << endl;
    }
}

void saveData(char* input, char* output) {
    File* file_input = new File(input, 0);
    File* file_output = new File(output, 0);

    Buffer* buffer = new Buffer(file_input);
    Tape* kopy = new Tape(file_output);

    cout << "File without sorting: " << endl;
    while (kopy->saveRecord(buffer->record()));

    delete buffer;
    delete kopy;
}

void printData(char* path) {
    char* fib;
    string str_obj("Fib.csv");
    fib = &str_obj[0];

    File* file_input = new File(path, 0);
    File* file_output = new File(fib, 0);

    Buffer* buffer = new Buffer(file_input);
    Tape* kopy = new Tape(file_output);

    cout << "File with sorting: " << endl;
    while (kopy->saveValue(buffer->nextRecord()));

    delete buffer;
    delete kopy;

    remove(path);
}

void getFileName(char* path) {
    string new_line, files;
    cout << "File path: ";
    getline(cin, new_line);
    getline(cin, files);
    for (int i = 0; i < files.length(); i++) {
        path[i] = files[i];
    }
    path[files.length()] = '\0';
}

void generateRecords(char* path) {
    double first, second, third, fourth, fifth;
    unsigned int counter = 0;

    Pentagon* new_pentagon = nullptr;
    File* output = new File(path, 0);
    Tape* tape = new Tape(output);

    srand(time(nullptr));
    cout << "How many records to generate: ";
    cin >> counter;
    cout << endl;

    while (counter != 0) {
        first = (rand() % 1000000) + 1;
        second = (rand() % 1000000) + 1;
        third = (rand() % 1000000) + 1;
        fourth = (rand() % 1000000) + 1;
        fifth = (rand() % 1000000) + 1;

        new_pentagon = new Pentagon(first, second, third, fourth, fifth);
        tape->saveValue(new_pentagon);
        counter--;
    }
    tape->save();
    delete tape;
}

void readRecords(char* path) {
    double first, second, third, fourth, fifth;
    unsigned int counter = 0;

    Pentagon* new_pentagon = nullptr;
    File* output = new File(path, 0);
    Tape* tape = new Tape(output);

    srand(time(nullptr));
    cout << endl << "How many records to read: ";
    cin >> counter;

    while (counter != 0) {
        cout << "First side: ";
        cin >> first;
        cout << "Second side: ";
        cin >> second;
        cout << "Third side: ";
        cin >> third;
        cout << "Fourth side: ";
        cin >> fourth;
        cout << "Fifth side: ";
        cin >> fifth;
        cout << endl;

        new_pentagon = new Pentagon(first, second, third, fourth, fifth);
        tape->saveValue(new_pentagon);
        counter--;
    }
    tape->save();
    delete tape;
}
