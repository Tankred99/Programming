// Finished 

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
        wprintf(L"Введите количество строк: ");
        wscanf(L"%d", &N);
        getchar(); // to consume the newline character left-over
    } while (N < 2 || N > 10);

    char* text = (char*)calloc(N * 81, sizeof(char));
    wprintf(L"Введите 1-ую строку: ");
    fgets(text, 80, stdin);

    char* buffer = (char*)calloc(81, sizeof(char));
    for (int i = 1; i < N; i++) {
        wprintf(L"Введите %d-ую строку: ", i + 1);
        fgets(buffer, 80, stdin);
        strcat(text, "\n");
        strcat(text, buffer);
    }
    free(buffer);

    char** words = NULL;
    int wordCount = 0;
    extractSecondWords(text, words, wordCount);

    if (wordCount == 0)
        wprintf(L"Нет предложений\n");
    else {
        wprintf(L"\nПоследние слова: \n");
        sortWords(words, wordCount);
        for (int i = 0; i < wordCount; i++) {
            wprintf(L"%s\n", words[i]);
            free(words[i]);
        }
        free(words);
    }
    free(text);
    return 0;
}