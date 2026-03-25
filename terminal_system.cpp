#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class Directory;
class Entry;


class Entry {
protected:
    string name;
    bool isHidden;

public:
    Entry(string n) : name(n), isHidden(false) {}
    virtual ~Entry() {}

    string getName() const { return name; }
    void setName(string n) { name = n; }

    bool getIsHidden() const { return isHidden; }
    void setHidden(bool hidden) { isHidden = hidden; }

    virtual void edit() {
        cout << "Cannot edit this entity." << endl;
    }

    virtual void cat() {
        cout << "Cannot read this entity." << endl;
    }

    virtual void listAll() {
    }

    virtual Directory* asDirectory() {
        return nullptr;
    }
};

class Directory : public Entry {
private:
    vector<Entry*> children;

public:
    Directory(string n) : Entry(n) {}
    ~Directory() {
        for (auto child : children) {
            delete child;
        }
    }

    void addEntry(Entry* entry) {
        children.push_back(entry);
    }

    const vector<Entry*>& getChildren() const {
        return children;
    }

    Directory* asDirectory() override {
        return this;
    }

    void listAll() override {
        for (auto child : children) {
            cout << "  - " << child->getName() << endl;
        }
    }

    Entry* findChild(string name) {
        for (auto child : children) {
            if (child->getName() == name) {
                return child;
            }
        }
        return nullptr;
    }
};


class SingleLineFile : public Entry {
private:
    string content;

public:
    SingleLineFile(string n) : Entry(n), content("") {}

    void edit() override {
        cout << "Enter new line for file " << name << ": ";
        cin.ignore();
        getline(cin, content);
        cout << "File updated." << endl;
    }

    void cat() override {
        cout << "--- Content of file " << name << " ---" << endl;
        cout << content << endl;
        cout << "-------------------------------" << endl;
    }
};


class MultiLineFile : public Entry {
private:
    vector<string> content;

public:
    MultiLineFile(string n) : Entry(n) {}

    void edit() override {
        cout << "Enter content (empty line to finish):" << endl;
        content.clear();
        string line;
        while (true) {
            getline(cin, line);
            if (line.empty()) break;
            content.push_back(line);
        }
        cout << "File updated." << endl;
    }

    void cat() override {
        cout << "--- Content of file " << name << " ---" << endl;
        for (const auto& line : content) {
            cout << line << endl;
        }
        cout << "-------------------------------" << endl;
    }
};


class Terminal {
private:
    Directory* root;
    vector<Directory*> pathStack;

    Directory* getCurrentDir() {
        if (pathStack.empty()) return root;
        return pathStack.back();
    }

    Entry* findEntryInCurrentDir(string name) {
        return getCurrentDir()->findChild(name);
    }

public:
    Terminal() {
        root = new Directory("root");
        pathStack.push_back(root);
    }

    ~Terminal() {
        delete root;
    }

    void run() {
        cout << "Welcome to Virtual Terminal!" << endl;
        cout << "Type 'help' for list of commands." << endl;

        string command;
        while (true) {
            cout << "> ";
            cin >> command;

            if (command == "exit") {
                break;
            }
            else if (command == "help") {
                cout << "Commands: help, exit, ls, cd, cat, touch, mkdir, rm, restore, mv, edit" << endl;
            }
            else if (command == "ls") {
                Directory* current = getCurrentDir();
                cout << "Directory contents: " << current->getName() << endl;
                bool isEmpty = true;

                for (auto child : current->getChildren()) {
                    if (!child->getIsHidden()) {
                        cout << "  " << child->getName() << endl;
                        isEmpty = false;
                    }
                }
                if (isEmpty) cout << "  (empty)" << endl;

            }
            else if (command == "cd") {
                string arg; cin >> arg;
                if (arg == "..") {
                    if (pathStack.size() > 1) {
                        pathStack.pop_back();
                        cout << "Moved up." << endl;
                    }
                    else {
                        cout << "Already in root." << endl;
                    }
                }
                else {
                    Entry* entry = findEntryInCurrentDir(arg);
                    if (entry && entry->asDirectory()) {
                        pathStack.push_back(entry->asDirectory());
                        cout << "Changed directory to: " << arg << endl;
                    }
                    else {
                        cout << "Error: Directory '" << arg << "' not found." << endl;
                    }
                }
            }
            else if (command == "touch") {
                string arg; cin >> arg;
                getCurrentDir()->addEntry(new SingleLineFile(arg));
                cout << "File '" << arg << "' created." << endl;
            }
            else if (command == "mkdir") {
                string arg; cin >> arg;
                getCurrentDir()->addEntry(new Directory(arg));
                cout << "Directory '" << arg << "' created." << endl;
            }
            else if (command == "cat") {
                string arg; cin >> arg;
                Entry* entry = findEntryInCurrentDir(arg);
                if (entry && !entry->getIsHidden()) {
                    entry->cat();
                }
                else {
                    cout << "Error: File not found or hidden." << endl;
                }
            }
            else if (command == "edit") {
                string arg; cin >> arg;
                Entry* entry = findEntryInCurrentDir(arg);
                if (entry && !entry->getIsHidden()) {
                    entry->edit();
                }
                else {
                    cout << "Error: File not found or hidden." << endl;
                }
            }
            else if (command == "rm") {
                string arg; cin >> arg;
                Entry* entry = findEntryInCurrentDir(arg);
                if (entry) {
                    entry->setHidden(true);
                    cout << "Entity '" << arg << "' hidden." << endl;
                }
                else {
                    cout << "Error: Entity not found." << endl;
                }
            }
            else if (command == "restore") {
                string arg; cin >> arg;
                Entry* entry = findEntryInCurrentDir(arg);
                if (entry && entry->getIsHidden()) {
                    entry->setHidden(false);
                    cout << "Entity '" << arg << "' restored." << endl;
                }
                else {
                    cout << "Error: Entity not found or was not hidden." << endl;
                }
            }
            else if (command == "mv") {
                string oldN, newN; cin >> oldN >> newN;
                Entry* entry = findEntryInCurrentDir(oldN);
                if (entry) {
                    entry->setName(newN);
                    cout << "Renamed '" << oldN << "' to '" << newN << "'." << endl;
                }
                else {
                    cout << "Error: Entity not found." << endl;
                }
            }
            else {
                cout << "Unknown command. Type 'help'." << endl;
            }
        }
        cout << "Shutting down terminal." << endl;
    }
};