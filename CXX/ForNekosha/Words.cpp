#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
#include <cctype>
#include <locale>

using namespace std;

void extractSecondWords(char* text, char**& words, int& k) {
    int count = 0;
    char** sentences = NULL;
    char* sentence = strtok(text, ".!?");
    while (sentence) {
        sentences = (char**)realloc(sentences, (count + 1) * sizeof(char*));
        sentences[count] = sentence;
        sentence = strtok(NULL, ".!?");
        count++;
    }

    for (int i = 0; i < count; i++) {
        char* word = strtok(sentences[i], " \n,");
        char* lastword = NULL;
        while (word != NULL) {
            lastword = word;
            word = strtok(NULL, " \n,");
        }
        if (lastword != NULL) {
            words = (char**)realloc(words, (k + 1) * sizeof(char*));
            words[k] = _strdup(lastword);
            k++;
        }
    }
    free(sentences);
}

void sortWords(char** words, int wordCount) {
    for (int i = 0; i < wordCount - 1; i++)
        for (int j = 0; j < wordCount - i - 1; j++)
            if (_stricmp(words[j], words[j + 1]) > 0)
                swap(words[j], words[j + 1]);
}

int main() {
    setlocale(LC_ALL, "Russian");
    int N;
    do {
        cout << "Введите количество строк: ";
        cin >> N;
    } while (N < 2 || N > 10);
    cin.ignore();

    char* text = (char*)calloc(N * 81, sizeof(char));
    cout << "Введите 1-ую строку: ";
    cin.getline(text, 80);

    char* buffer = (char*)calloc(81, sizeof(char));
    for (int i = 1; i < N; i++) {
        cout << "Введите " << i + 1 << "-ую строку: ";
        cin.getline(buffer, 80);
        strcat(text, "\n");
        strcat(text, buffer);
    }
    free(buffer);

    char** words = NULL;
    int wordCount = 0;
    extractSecondWords(text, words, wordCount);

    if (wordCount == 0)
        cout << "Нет предложений" << endl;
    else {
        cout << endl << "Последние слова: " << endl;
        sortWords(words, wordCount);
        for (int i = 0; i < wordCount; i++) {
            cout << words[i] << endl;
            free(words[i]);
        }
        free(words);
    }
    free(text);
    return 0;
}