#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

void makeDir(folderName){

    if (!fs::exists(folderName)) {
        // La cartella "data" non esiste, quindi la creiamo
        if (fs::create_directory(folderName)) {
            std::cout << "Cartella creata: " << folderName << std::endl;
        } else {
            std::cerr << "Errore nella creazione della cartella!" << std::endl;
            return 1;
        }
    } else {
        // La cartella "data" esiste, quindi la cancelliamo e la ricreiamo
        try {
            fs::remove_all(folderName);  // Cancella ricorsivamente la cartella e il suo contenuto
            if (fs::create_directory(folderName)) {
                std::cout << "Cartella ricreata: " << folderName << std::endl;
            } else {
                std::cerr << "Errore nella ricreazione della cartella!" << std::endl;
                return 1;
            }
        } catch (const std::exception& ex) {
            std::cerr << "Errore durante la cancellazione o ricreazione della cartella: " << ex.what() << std::endl;
            return 1;
        }
    }
};
