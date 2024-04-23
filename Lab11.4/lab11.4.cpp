#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>

using namespace std;

#pragma pack(push, 1)
struct Note {
    char firstName[50];
    char lastName[50];
    char phoneNumber[15];
    int birthDate[3];
};
#pragma pack(pop)

void createNote(const string& filename);
void printNotes(const Note* notes, int numNotes);
void sortNotesByPhoneNumber(const string& filename);
bool searchByLastName(const string& filename, const string& lastName, Note& foundNote);
void displayFullNoteDetails(const Note& note);
void writeNotes(const string& filename, const Note* notes, int numNotes);
void readAndPrintNotes(const string& filename);

int main() {
    string filename;
    cout << "Enter the filename for operations: ";
    getline(cin, filename);

    int menuItem;
    do {
        cout << "Choose an action:" << endl;
        cout << "[1] - Enter note information" << endl;
        cout << "[2] - Display note information" << endl;
        cout << "[3] - Sort by phone numbers" << endl;
        cout << "[4] - Search for a person by last name" << endl;
        cout << "[0] - Exit the program" << endl;
        cout << "Enter: ";
        cin >> menuItem;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (menuItem) {
        case 1:
            createNote(filename);
            break;
        case 2:
            readAndPrintNotes(filename);
            break;
        case 3:
            sortNotesByPhoneNumber(filename);
            break;
        case 4:
            Note searchResult;
            string lastName;
            cout << "Enter search criteria: Last Name: ";
            getline(cin, lastName);
            if (searchByLastName(filename, lastName, searchResult)) {
                displayFullNoteDetails(searchResult);
            }
            else {
                cout << "Person with last name \"" << lastName << "\" not found." << endl;
            }
            break;
        }
    } while (menuItem != 0);

    return 0;
}

void createNote(const string& filename) {
    ofstream fileOut(filename, ios::binary | ios::app);
    if (!fileOut) {
        cerr << "Cannot open file " << filename << " for writing." << endl;
        return;
    }

    Note note;
    cout << "Enter First name: ";
    cin.getline(note.firstName, 50);
    cout << "Enter Last name: ";
    cin.getline(note.lastName, 50);
    cout << "Enter Phone number: ";
    cin.getline(note.phoneNumber, 15);

    cout << "Birth date (day month year): ";
    if (!(cin >> note.birthDate[0] >> note.birthDate[1] >> note.birthDate[2])) {
        cerr << "Invalid input. Please enter numeric values for date." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    fileOut.write(reinterpret_cast<const char*>(&note), sizeof(note));
    fileOut.close();
}

void printNotes(const Note* notes, int numNotes) {
    cout << "============================================================================================" << endl;
    cout << "|  ¹  |   Surname    |  Name  |  Phone Number  |  Day   | Month  |  Year  |" << endl;
    cout << "--------------------------------------------------------------------------------------------" << endl;

    for (int i = 0; i < numNotes; ++i) {
        const Note& note = notes[i];
        cout << "|" << setw(4) << right << i + 1 << " ";
        cout << "|" << setw(14) << left << note.lastName;
        cout << "|" << setw(8) << left << note.firstName;
        cout << "|" << setw(15) << left << note.phoneNumber;
        for (int j = 0; j < 3; j++) {
            cout << "|" << setw(7) << left << note.birthDate[j];
        }
        cout << "|" << endl;
    }
    cout << "=============================================================================================" << endl << endl;
}

void sortNotesByPhoneNumber(const string& filename) {
    fstream file(filename, ios::binary | ios::in | ios::out);
    if (!file) {
        cerr << "Cannot open file " << filename << " for sorting." << endl;
        return;
    }

    file.seekg(0, ios::end);
    size_t fileSize = file.tellg();
    size_t numRecords = fileSize / sizeof(Note);
    if (numRecords < 2) return;

    for (size_t i = 0; i < numRecords - 1; ++i) {
        size_t minIndex = i;
        Note minNote;
        Note currentNote;

        file.seekg(minIndex * sizeof(Note));
        file.read(reinterpret_cast<char*>(&minNote), sizeof(Note));

        for (size_t j = i + 1; j < numRecords; ++j) {
            file.seekg(j * sizeof(Note));
            file.read(reinterpret_cast<char*>(&currentNote), sizeof(Note));

            if (strcmp(currentNote.phoneNumber, minNote.phoneNumber) < 0) {
                minIndex = j;
                minNote = currentNote;
            }
        }

        if (minIndex != i) {
            file.seekg(i * sizeof(Note));
            file.read(reinterpret_cast<char*>(&currentNote), sizeof(Note));

            file.seekp(i * sizeof(Note));
            file.write(reinterpret_cast<const char*>(&minNote), sizeof(Note));

            file.seekp(minIndex * sizeof(Note));
            file.write(reinterpret_cast<const char*>(&currentNote), sizeof(Note));
        }
    }

    file.close();
}

bool searchByLastName(const string& filename, const string& lastName, Note& foundNote) {
    ifstream fileIn(filename, ios::binary);
    if (!fileIn) {
        cerr << "Cannot open file " << filename << " for reading." << endl;
        return false;
    }

    Note note;
    while (fileIn.read(reinterpret_cast<char*>(&note), sizeof(note))) {
        if (strcmp(note.lastName, lastName.c_str()) == 0) {
            foundNote = note;
            fileIn.close();
            return true;
        }
    }
    fileIn.close();
    return false;
}

void displayFullNoteDetails(const Note& note) {
    cout << "Person found:" << endl;
    cout << "First Name: " << note.firstName << endl;
    cout << "Last Name: " << note.lastName << endl;
    cout << "Phone Number: " << note.phoneNumber << endl;
    cout << "Birth Date: " << note.birthDate[0] << "-" << note.birthDate[1] << "-" << note.birthDate[2] << endl;
}

void writeNotes(const string& filename, const Note* notes, int numNotes) {
    ofstream file(filename, ios::binary);
    if (!file) {
        cerr << "Error opening file for writing." << endl;
        return;
    }

    for (int i = 0; i < numNotes; ++i) {
        file.write(reinterpret_cast<const char*>(&notes[i]), sizeof(Note));
    }

    file.close();
}

void readAndPrintNotes(const string& filename) {
    ifstream file(filename, ios::binary);
    if (!file) {
        cerr << "Error opening file for reading." << endl;
        return;
    }

    file.seekg(0, ios::end);
    size_t fileSize = file.tellg();
    file.seekg(0, ios::beg);

    int numNotes = fileSize / sizeof(Note);
    Note* notes = new Note[numNotes];

    file.read(reinterpret_cast<char*>(notes), sizeof(Note) * numNotes);
    printNotes(notes, numNotes);

    delete[] notes;
    file.close();
}
