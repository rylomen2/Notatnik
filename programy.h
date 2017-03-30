//FUNKCJA ZAPISZ ZAPISUJE DANE Z POLA TEKSTOWEGO DO PLIKU
void zapis(HWND hwnd ,LPSTR Bufor ,HANDLE hPlik ,DWORD dwRozmiar, DWORD dwZapisane, HWND pt){
    dwRozmiar = GetWindowTextLength( pt );
    Bufor =( LPSTR ) GlobalAlloc( GPTR, dwRozmiar + 2 );
    GetWindowText( pt, Bufor, dwRozmiar + 1 ); // skopiuj do bufora tekst z jakiegoœ okna
    Bufor[ dwRozmiar + 1 ] = 0; // dodaj zero na koñcu stringa
    if( !WriteFile( hPlik, Bufor, dwRozmiar, & dwZapisane, NULL ) )
        MessageBox( NULL, "B³¹d zapisu do pliku", "Dupa blada!", MB_ICONEXCLAMATION );
    else
        MessageBox( hwnd, "Zapis udany", "Komunikat", 0);
}


//FUNKCJA ZAMKNIJ ZAMYKA PROGRAM
void zamknij_plik(LPSTR Bufor, HANDLE hPlik){
    GlobalFree( Bufor ); // Zwolnij bufor
    CloseHandle( hPlik ); // Zamknij plik
}

//FUNKCJA OTWORZ
HANDLE otworz_plik(HANDLE hPlik, LPCSTR Nazwa){
    hPlik = CreateFile( Nazwa, GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, hPlik );
    if( hPlik == INVALID_HANDLE_VALUE ) {
        MessageBox( NULL, "Nie mo¿na otworzyæ pliku.", "A to pech!", MB_ICONEXCLAMATION );
        PostQuitMessage( 0 ); // Zakoñcz program
    }
    return hPlik;
}

//ZNAJDZ WZORZEC W TEKSCIE
int znajdz_text( TCHAR * wz, HWND pt, DWORD dwRozmiar ){
    //SendMessage(pt, EM_SETSEL, 1, 22);
    TCHAR buff[1024];
    dwRozmiar = 1024;
    GetWindowText( pt, buff, dwRozmiar );
    if( buff[0] == wz[0] )
        return 1;
    else
        return 5;
}
