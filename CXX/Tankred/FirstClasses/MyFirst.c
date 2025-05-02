#include <stdio.h>
#include <locale.h>
#include <wchar.h>

int main(){
    setlocale(LC_ALL, ""); // This will use the user's default locale
    wprintf(L"Letters 'Мее'\n"); // Using wprintf instead of printf to handle wide characters
    return 0;
}
// Output: Letters 'МЕЕЕЕЕЕЕЕЕЕЕЕЕЕЕЕЕЕЕ'