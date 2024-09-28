#include <unordered_map>
#include <vector>
#include <string>
#include <sstream>
#include <random>
#include <fstream>
#include <iostream>

using namespace std;

class Embedding {
public:
    unordered_map<string, int> dictGeneral;
    unordered_map<string, vector<float>> token_embeddings; // Asociar tokens con sus embeddings

    // TOKENIZACIÓN - SEPARA EL STRING EN UN VECTOR DE STRINGS
    vector<string> tokenizar(const string& input) const {
        vector<string> tokens;
        stringstream ss(input);
        string palabra;

        while (ss >> palabra) {
            tokens.push_back(palabra);
        }
        return tokens;
    }

    // CONSTRUCCIÓN DEL VOCABULARIO
    unordered_map<string, int> vocabulario(const vector<string>& tokens) {
        unordered_map<string, int> voc;
        int id = 0;
        for (const auto& token : tokens) {
            if (voc.find(token) == voc.end()) {
                voc[token] = id;
                id++;
            }
        }
        dictGeneral = voc; // Actualizamos el vocabulario general de la clase
        return voc;
    }

    // ASIGNAR IDENTIFICADORES A LOS TOKENS UTILIZANDO EL DICCIONARIO
    vector<int> asignar_ident(const vector<string>& tokens) {
        vector<int> list;
        int id = dictGeneral.size();

        for (const string& token : tokens) {
            // Si el token no está en el diccionario, le asignamos un nuevo identificador
            if (dictGeneral.find(token) == dictGeneral.end()) {
                dictGeneral[token] = id;
                id++;
            }
            list.push_back(dictGeneral[token]);
        }
        return list;
    }

    // INICIALIZAR Y ASIGNAR EMBEDDINGS A CADA TOKEN
    void init_embeddings(int dim) {
        random_device rd;
        mt19937 gen(rd());
        uniform_real_distribution<> dist(0.0, 1.0);

        for (const auto& pair : dictGeneral) {
            const string& token = pair.first;
            vector<float> embedding(dim);
            for (int j = 0; j < dim; ++j) {
                embedding[j] = dist(gen); // Embedding aleatorio para cada dimensión
            }
            token_embeddings[token] = embedding; // Asignamos el embedding al token
        }
    }

    // GUARDAR DICCIONARIO EN UN ARCHIVO DE TEXTO
    void guardar_diccionario(const string& archivo) const {
        ofstream out(archivo);
        if (out.is_open()) {
            for (const auto& par : dictGeneral) {
                out << par.first << " " << par.second << "\n";
            }
            out.close();
        } else {
            cerr << "No se pudo abrir el archivo para escribir." << endl;
        }
    }

    // CARGAR DICCIONARIO DESDE UN ARCHIVO DE TEXTO
    void cargar_diccionario(const string& archivo) {
        ifstream in(archivo);
        if (in.is_open()) {
            string palabra;
            int id;
            dictGeneral.clear(); // Limpiamos el diccionario antes de cargar
            while (in >> palabra >> id) {
                dictGeneral[palabra] = id;
            }
            in.close();
        } else {
            cerr << "No se pudo abrir el archivo para leer." << endl;
        }
    }
};

// Ejemplo de uso
int main() {
    Embedding embed;

    // Ejemplo de creación de vocabulario
    string texto = "hola mundo mundo ejemplo de tokenización";
    vector<string> tokens = embed.tokenizar(texto);
    embed.vocabulario(tokens);

    // Inicializar embeddings
    embed.init_embeddings(5); // Dimensiones de embedding = 5

    // Mostrar los embeddings para cada token
    for (const auto& par : embed.token_embeddings) {
        cout << "Token: " << par.first << " Embedding: ";
        for (const auto& valor : par.second) {
            cout << valor << " ";
        }
        cout << endl;
    }

    return 0;
}