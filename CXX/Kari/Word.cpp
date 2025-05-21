#include <iostream>
#include <cstring>
#include <cctype>
#include <cwchar>
#include <clocale>

#ifdef _WIN32
#include <io.h>
#include <fcntl.h>
#endif

using namespace std;

void SplitIntoSentences(wchar_t* str, wchar_t**& sentences, int& count) {
    count = 0;
    sentences = nullptr;

    wchar_t* buffer = new wchar_t[wcslen(str) + 1];
    wcscpy(buffer, str);

    wchar_t* sentence = wcstok(buffer, L".");
    while (sentence != nullptr) {
        while (*sentence == L' ') sentence++;

        if (*sentence != L'\0') {
            wchar_t** temp = (wchar_t**)realloc(sentences, (count + 1) * sizeof(wchar_t*));
            sentences = temp;
            sentences[count] = new wchar_t[wcslen(sentence) + 1];
            wcscpy(sentences[count], sentence);                         // Copy sentence into allocated memory
            count++;
        }

        sentence = wcstok(nullptr, L".");
    }

    delete[] buffer;
}

void SplitIntoTokens(wchar_t* str, wchar_t**& tokens, int& count) {
    count = 0;
    tokens = nullptr;

    wchar_t* buffer = new wchar_t[wcslen(str) + 1];
    wcscpy(buffer, str);

    wchar_t* token = wcstok(buffer, L" ,.");  
    while (token != nullptr) {
        wchar_t** temp = (wchar_t**)realloc(tokens, (count + 1) * sizeof(wchar_t*));
         if (temp == nullptr && count > 0) {
                // Handle realloc failure.  Original `tokens` is still valid.
                wcerr << L"Ошибка выделения памяти!" << endl;
                // Free allocated memory before exiting or returning.
                for (int i = 0; i < count; ++i) {
                    delete[] tokens[i];
                }
                free(tokens);
                delete[] buffer;
                return; // Or exit the program, depending on error handling strategy.
            }
        tokens = temp;

        tokens[count] = new wchar_t[wcslen(token) + 1];
        wcscpy(tokens[count], token);
        count++;
        token = wcstok(nullptr, L" ,.");
    }

    delete[] buffer;  
}

wchar_t CaesarEncrypt(wchar_t ch, int shift) {
    if (!iswalpha(ch)) return ch;

    bool isUpper = iswupper(ch);
    ch = towlower(ch);

    const wstring alphabet = L"abcdefghijklmnopqrstuvwxyz";
    size_t pos = alphabet.find(ch);
    if (pos == string::npos) return ch;

    pos = (pos + shift) % 26;
    wchar_t result = alphabet[pos];
    return isUpper ? towupper(result) : result;
}

wchar_t CaesarDecrypt(wchar_t ch, int shift) {
    if (!iswalpha(ch)) return ch;

    bool isUpper = iswupper(ch);
    ch = towlower(ch);

    const wstring alphabet = L"abcdefghijklmnopqrstuvwxyz";
    size_t pos = alphabet.find(ch);
    if (pos == string::npos) return ch;

    pos = (pos - shift + 26) % 26;
    wchar_t result = alphabet[pos];
    return isUpper ? towupper(result) : result;
}

void EncryptWord(wchar_t* word, int wordCount) {
    for (int i = 0; word[i] != L'\0'; i++) {
        int shift;
        if (i == 0) {
            shift = 1;
        } else {
           shift = (wordCount % (i == 0 ? 1 : i)) + 1; // Prevent division by zero
        }
        word[i] = CaesarEncrypt(word[i], shift);
    }
}

void DecryptWord(wchar_t* word, int wordCount) {
    for (int i = 0; word[i] != L'\0'; i++) {
        int shift;
        if (i == 0) {
            shift = 1;
        } else {
            shift = (wordCount % (i == 0 ? 1 : i)) + 1; // Prevent division by zero
        }
        word[i] = CaesarDecrypt(word[i], shift);
    }
}

void ProcessSentence(wchar_t* sentence) {
    wchar_t** words = nullptr;
    int wordCount = 0;

    SplitIntoTokens(sentence, words, wordCount);

    for (int i = 0; i < wordCount; i++) {
        EncryptWord(words[i], wordCount);
    }

    wcscpy(sentence, L"");
    for (int i = 0; i < wordCount; i++) {
        if (i > 0) wcscat(sentence, L" ");
        wcscat(sentence, words[i]);
        delete[] words[i];
    }

    if(words != nullptr){
        free(words);
    }

}

void ProcessDecryptedSentence(wchar_t* sentence) {
    wchar_t** words = nullptr;
    int wordCount = 0;

    SplitIntoTokens(sentence, words, wordCount);

    for (int i = 0; i < wordCount; i++) {
        DecryptWord(words[i], wordCount);
    }

    wcscpy(sentence, L"");
    for (int i = 0; i < wordCount; i++) {
        if (i > 0) wcscat(sentence, L" ");
        wcscat(sentence, words[i]);
        delete[] words[i];
    }

    if (words != nullptr) {
        free(words);
    }
}

wchar_t** CopyText(wchar_t** originalText, int N) {
    wchar_t** newText = new wchar_t*[N];

    for (int i = 0; i < N; i++) {
        newText[i] = new wchar_t[wcslen(originalText[i]) + 1];
        wcscpy(newText[i], originalText[i]);
    }

    return newText;
}
void EncryptText(wchar_t** text, int N) {
    wcout << L"Зашифрованный текст:\n";

    for (int i = 0; i < N; i++) {
        wchar_t* textCopy = new wchar_t[wcslen(text[i]) + 1];
        wcscpy(textCopy, text[i]);

        wchar_t** sentences = nullptr;
        int sentenceCount = 0;

        SplitIntoSentences(textCopy, sentences, sentenceCount);

        text[i][0] = L'\0';

        wcout << L"Строка " << i + 1 << L": ";
        for (int j = 0; j < sentenceCount; j++) {
            ProcessSentence(sentences[j]);

            wcout << sentences[j] << L".";

            if (j > 0) wcscat(text[i], L".");
            wcscat(text[i], sentences[j]);
        }

        if (sentences != nullptr) {
           for (int k = 0; k < sentenceCount; ++k) {
                delete[] sentences[k];
            }
            free(sentences);
        }
        wcout << endl;

        delete[] textCopy;
    }
}

void DecryptText(wchar_t** text, int N) {
    wcout << L"Дешифрованный текст:\n";

    for (int i = 0; i < N; i++) {
        wchar_t* textCopy = new wchar_t[wcslen(text[i]) + 1];
        wcscpy(textCopy, text[i]);

        wchar_t** sentences = nullptr;
        int sentenceCount = 0;

        SplitIntoSentences(textCopy, sentences, sentenceCount);

        text[i][0] = L'\0';

        wcout << L"Строка " << i + 1 << L": ";
        for (int j = 0; j < sentenceCount; j++) {
            ProcessDecryptedSentence(sentences[j]);

            wcout << sentences[j] << L".";

            if (j > 0) wcscat(text[i], L".");
            wcscat(text[i], sentences[j]);
        }

       if (sentences != nullptr) {
            for (int k = 0; k < sentenceCount; ++k) {
                delete[] sentences[k];
            }
            free(sentences);
        }
        wcout << endl;

        delete[] textCopy;
    }
}

int main() {
#ifdef _WIN32
    _setmode(_fileno(stdout), _O_U8TEXT);
    _setmode(_fileno(stdin), _O_U8TEXT);
#else
    setlocale(LC_ALL, "Russian");
    wcin.imbue(locale("ru_RU.UTF-8"));
    wcout.imbue(locale("ru_RU.UTF-8"));
#endif

    int N;
    do {
        wcout << L"Введите количество строк (2-10): ";
        wcin >> N;
        wcin.ignore();
    } while (N < 2 || N > 10);

    // Создаем массив для хранения исходного текста
    wchar_t** originalText = new wchar_t*[N];
    for (int i = 0; i < N; i++) {
        originalText[i] = new wchar_t[81];
        wcout << L"Введите строку " << i + 1 << L": ";
        wcin.getline(originalText[i], 80);
    }

    wchar_t** encryptedText = CopyText(originalText, N);

    wcout << L"\n--- Результат шифрования ---\n";
    EncryptText(encryptedText, N);

    wchar_t** decryptedText = CopyText(encryptedText, N);

    wcout << L"\n--- Результат дешифрования ---\n";
    DecryptText(decryptedText, N);

    for (int i = 0; i < N; i++) {
        delete[] originalText[i];
        delete[] encryptedText[i];
        delete[] decryptedText[i];
    }
    delete[] originalText;
    delete[] encryptedText;
    delete[] decryptedText;
}
