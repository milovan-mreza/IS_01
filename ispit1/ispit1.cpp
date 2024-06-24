#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <cctype>
#include <fstream>
#include <filesystem>


class Konferencija {
protected:
    std::string nazivKonferencije;
    std::string drzavaOdrzavanja;
    std::string brojPrijave;
    int godinaOdrzavanja;

public:
    Konferencija(const std::string& naziv, const std::string& drzava, const std::string& broj, int godina = 2022)
        : nazivKonferencije(naziv), drzavaOdrzavanja(drzava), brojPrijave(broj), godinaOdrzavanja(godina) {}

    virtual bool brojPrijaveValidan() const = 0;

    friend std::ostream& operator<<(std::ostream& os, const Konferencija& konf) {
        os << konf.nazivKonferencije << " " << konf.drzavaOdrzavanja << " "
            << konf.brojPrijave << " " << konf.godinaOdrzavanja;
        return os;
    }

    int getGodinaOdrzavanja() const {
        return godinaOdrzavanja;
    }

    std::string getBrojPrijave() const {
        return brojPrijave;
    }
};

class CSKonferencija : public Konferencija {
public:
    CSKonferencija(const std::string& naziv, const std::string& drzava, const std::string& broj, int godina = 2022)
        : Konferencija(naziv, drzava, broj, godina) {}

    bool brojPrijaveValidan() const override {
        return std::none_of(brojPrijave.begin(), brojPrijave.end(), [](char c) {
            return !std::islower(c) && !std::isdigit(c);
            });
    }
};

class EkonomskaKonferencija : public Konferencija {
public:
    EkonomskaKonferencija(const std::string& naziv, const std::string& drzava, const std::string& broj, int godina = 2022)
        : Konferencija(naziv, drzava, broj, godina) {}

    bool brojPrijaveValidan() const override {
        return std::all_of(brojPrijave.begin(), brojPrijave.end(), [](char c) {
            return std::isupper(c) || std::isdigit(c);
            });
    }
};

int main() {
    std::ifstream inputFile("C:/Users/Diodox/Documents/Dev C++/input.txt");


    std::cout << "DAD";

    if (inputFile) {
        std::cout << "File exists and was successfully opened." << std::endl;
        // Proceed with reading from the file
    }
    else {
        std::cout << "File does not exist or could not be opened." << std::endl;
    }


    std::vector<Konferencija*> konferencije;
    char oznaka;
    std::string naziv, drzava, broj;
    int godina;

    while (inputFile >> oznaka >> naziv >> drzava >> broj >> godina) {
        if (godina == 0) {
            godina = 2022;
        }

        if (oznaka == 'C') {
            konferencije.push_back(new CSKonferencija(naziv, drzava, broj, godina));
        }
        else if (oznaka == 'E') {
            konferencije.push_back(new EkonomskaKonferencija(naziv, drzava, broj, godina));
        }
    }

    inputFile.close();

    std::stable_sort(konferencije.begin(), konferencije.end(), [](Konferencija* a, Konferencija* b) {
        return a->getGodinaOdrzavanja() > b->getGodinaOdrzavanja();
        });

    std::ofstream outputFile("C:/Users/Diodox/Pictures/output.txt");

    auto maxElemIt = std::max_element(konferencije.begin(), konferencije.end(), [](Konferencija* a, Konferencija* b) {
        return a->getGodinaOdrzavanja() < b->getGodinaOdrzavanja();
        });

    if (maxElemIt != konferencije.end()) {
        Konferencija* maxKonf = *maxElemIt;
        if (!maxKonf->brojPrijaveValidan()) {
            outputFile << "Konferencija sa najvecom godinom odrzavanja ima nevalidan broj prijave.\n";
        }
        else {
            outputFile << *maxKonf << "\n";
        }
    }

    for (Konferencija* konf : konferencije) {
        delete konf;
    }

    outputFile.close();

    return 0;
}
