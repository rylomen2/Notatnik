#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <process.h>
#include "programy.h"
#define ID_ESC 204


    //ZMIENNE GLOBALNE ITD . . .
    LRESULT CALLBACK WindowProcedure( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam );
    LRESULT CALLBACK NewWndProc( HWND hwnd, UINT mesg, WPARAM wParam, LPARAM lParam );
    WNDPROC g_OldWndProc;
    MSG Komunikat;
    LPSTR NazwaKlasy = "Klasa Okienka";
    LPSTR Nazwa = "Nowy plik tekstowy.txt";
    LPSTR Bufor;
    HANDLE hPlik;
    DWORD dwRozmiar, dwZapisane;
    HWND pt;
    int width, height;
    RECT rect;
    char sNazwaPliku[ MAX_PATH ] = "";
    HFONT hNormalFont;
    CHOOSEFONT cfnt;
    char bufor[80];
    UINT WM_FINDREPLACE;
    FINDREPLACE fr;
    HWND hdlg = NULL;


int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
    //tworzenie obiektu klasy WNDCLASSEX i inicjowanie go
    WNDCLASSEX wc;

    wc.cbSize = sizeof( WNDCLASSEX );
    wc.style = CS_DBLCLKS;
    wc.lpfnWndProc = WindowProcedure;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon( NULL, IDI_APPLICATION );
    wc.hCursor = LoadCursor( NULL, IDC_ARROW );
    wc.hbrBackground =( HBRUSH )( COLOR_HIGHLIGHT + 1 );
    wc.lpszMenuName = NULL;
    wc.lpszClassName = NazwaKlasy;
    wc.hIconSm = LoadIcon( NULL, IDI_QUESTION);

    if( !RegisterClassEx( & wc ) )
    {
        MessageBox( NULL, "Wysoka Komisja odmawia rejestracji tego okna!", "Niestety...",
        MB_ICONEXCLAMATION | MB_OK );
        return 1;
    }


    //pobieranie czcionki
    hNormalFont =( HFONT ) GetStockObject( ANSI_VAR_FONT );

    HMENU hMenu = LoadMenu( hInstance, MAKEINTRESOURCE( 201 ) );
    FILETIME time;
    SYSTEMTIME systime;

    //okno glowne aplikacji
    HWND hwnd = CreateWindowEx( WS_EX_OVERLAPPEDWINDOW, NazwaKlasy, "Wlasny edytor tekstowy", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 700, 700, NULL, hMenu, hInstance, NULL );
    //pole tekstowe
    pt = CreateWindowEx( 0, "EDIT", NULL, WS_VISIBLE | WS_CHILD | ES_MULTILINE | WS_EX_COMPOSITED | WS_VSCROLL | WS_SIZEBOX , 0, 0, 0, 0, hwnd, (HMENU) 1001, hInstance, NULL );
    SendMessage( pt, WM_SETFONT, ( WPARAM ) hNormalFont, 0 );
    g_OldWndProc =( WNDPROC ) SetWindowLong( pt, GWL_WNDPROC,( LONG ) NewWndProc );

    HACCEL hAccel = LoadAccelerators( hInstance, MAKEINTRESOURCE( 100 ) );
    if( !hAccel )
    {
        MessageBox( hwnd, "Nie można załadować akceleratorów.", NULL, MB_ICONEXCLAMATION );
    }

    ShowWindow( hwnd, nCmdShow );
    UpdateWindow( hwnd );


    GetFileTime( hPlik, &time, NULL, NULL );
    FileTimeToSystemTime( &time, &systime );

    //wyłapuje komunikaty wywalane przez windows
    MSG msg;
    while( GetMessage( & msg, NULL, 0, 0 ) )
    {
        if( !TranslateAccelerator( hwnd, hAccel, & msg ) )
        {
            TranslateMessage( & msg );
            DispatchMessage( & msg );
        }
    }

    return Komunikat.wParam;
}

LRESULT CALLBACK NewWndProc( HWND hwnd, UINT mesg, WPARAM wParam, LPARAM lParam )
{
    switch( mesg )
    {
    case WM_KEYDOWN:
        {
            CallWindowProc( WindowProcedure, hwnd, mesg, wParam, lParam );
        }
        break;
    }

    return CallWindowProc( g_OldWndProc, hwnd, mesg, wParam, lParam );
}

LRESULT CALLBACK WindowProcedure( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )//OBSLUGUJE ZDARZENIA ZWIAZANE Z KONTROLTAMI I OKNEM
{
    switch( msg )
    {
        //MYSZ
        case WM_CLOSE://ZAMYKA OKNO
        {
            DestroyWindow( hwnd );
        };
        break;

        case WM_DESTROY://ZAMYKA APLIKACJE
        {
            PostQuitMessage( 0 );
        };
        break;

        case WM_COMMAND://OBSLUGUJE PRZYCISKI
        {
            if( LOWORD( wParam ) == 202 )//ZAPIS
            {
                OPENFILENAME ofn;

                ZeroMemory( & ofn, sizeof( ofn ) );
                ofn.lStructSize = sizeof( ofn );
                ofn.lpstrFilter = "Pliki tekstowe (*.txt)\0*.txt\0Wszystkie pliki\0*.*\0";
                ofn.nMaxFile = MAX_PATH;
                ofn.lpstrFile = sNazwaPliku;
                ofn.lpstrDefExt = "txt";
                ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_CREATEPROMPT;

                if( sNazwaPliku[0] == '\0' ){
                    if( GetOpenFileName( & ofn ) )
                    {
                        hPlik = otworz_plik(hPlik,sNazwaPliku);
                    }
                }else
                    hPlik = otworz_plik(hPlik,sNazwaPliku);


                zapis(hwnd,Bufor,hPlik,dwRozmiar,dwZapisane,pt);
                zamknij_plik(Bufor, hPlik);
            }

            if( LOWORD( wParam ) == 203 )//OTWORZ
            {
                OPENFILENAME ofn;

                ZeroMemory( & ofn, sizeof( ofn ) );
                ofn.lStructSize = sizeof( ofn );
                ofn.lpstrFilter = "Pliki tekstowe (*.txt)\0*.txt\0Wszystkie pliki\0*.*\0";
                ofn.nMaxFile = MAX_PATH;
                ofn.lpstrFile = sNazwaPliku;
                ofn.lpstrDefExt = "txt";
                ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
                if( GetOpenFileName( & ofn ) )
                {
                    hPlik = otworz_plik(hPlik,sNazwaPliku);
                }

                DWORD dwSize = GetFileSize( hPlik, NULL );
                if( dwSize == 0xFFFFFFFF ) // błąd!
                     MessageBox( NULL, "Zły rozmiar pliku!", "Błąd", MB_ICONEXCLAMATION );
                else{   //WCZYTYWANIE PLIKU
                    LPSTR Bufor;
                    DWORD dwRozmiar, dwPrzeczyt;

                    dwRozmiar = GetFileSize( hPlik, NULL );
                    if( dwRozmiar == 0xFFFFFFFF ) {
                        MessageBox( NULL, "Nieprawidłowy rozmiar pliku!", "Niedobrze...", MB_ICONEXCLAMATION );
                        PostQuitMessage( 0 ); // Zakończ program
                    }

                    Bufor =( LPSTR ) GlobalAlloc( GPTR, dwRozmiar + 1 );
                    if( Bufor == NULL ) {
                        MessageBox( NULL, "Za mało pamięci!", "Ale wiocha...", MB_ICONEXCLAMATION );
                        PostQuitMessage( 0 ); // Zakończ program
                    }

                    if( !ReadFile( hPlik, Bufor, dwRozmiar, & dwPrzeczyt, NULL ) ) {
                        MessageBox( NULL, "Błąd czytania z pliku", "Dupa blada!", MB_ICONEXCLAMATION );
                        PostQuitMessage( 0 ); // Zakończ program
                    }

                    Bufor[ dwRozmiar ] = 0; // dodaj zero na końcu stringa
                    SetWindowText( pt, Bufor ); // zrób coś z tekstem, np. wyświetl go
                    zamknij_plik(Bufor,hPlik);

                }
            }//OTWORZ

            if( LOWORD( wParam ) == 205 ){//USTAW CZCIONKE
                    LOGFONT lf;
                    ZeroMemory( & cfnt, sizeof( CHOOSEFONT ) );
                    cfnt.lStructSize = sizeof( CHOOSEFONT );
                    cfnt.hwndOwner = pt;
                    cfnt.Flags = CF_EFFECTS | CF_FORCEFONTEXIST | CF_SCREENFONTS;
                    cfnt.lpLogFont = & lf;

                    if( !ChooseFont( & cfnt ) )
                    {
                        DWORD dwErr = CommDlgExtendedError();
                        if( dwErr ) MessageBox( hwnd, "Straszliwy błąd dialogu!", NULL, MB_ICONEXCLAMATION );

                    }
                    else
                    {
                        if( hNormalFont != NULL )
                             DeleteObject( hNormalFont );

                        hNormalFont = CreateFontIndirect( cfnt.lpLogFont );
                        if( hNormalFont != NULL )
                             SendMessage( pt, WM_SETFONT,( WPARAM ) hNormalFont, MAKELPARAM( TRUE, 0 ) );

                    }
            }//USTAW CZCIONKE

            if( LOWORD( wParam ) == 207)
            {
                WM_FINDREPLACE = RegisterWindowMessage( FINDMSGSTRING );
                ZeroMemory( & fr, sizeof( fr ) );
                fr.lStructSize = sizeof( fr );
                fr.hwndOwner = hwnd;
                fr.lpstrFindWhat = bufor;
                fr.wFindWhatLen = 80;

                lstrcpy( bufor, "tekst" );
                //SendMessage(pt, EM_SETSEL, 10, 15);
                hdlg = FindText( & fr );
            }

            if( LOWORD( wParam ) == 208 )   //Ctrl + A
            {
                SendMessage(pt, EM_SETSEL, 0, -1);
            }

            break;
        }

        case WM_CTLCOLOREDIT:   //ZMIENIA KOLOR TEKSTU W POLU TEKSTOWYM
        {
            HBRUSH hbr = (HBRUSH) DefWindowProc(hwnd, msg, wParam, lParam);
                SetTextColor((HDC) wParam, cfnt.rgbColors);
                return (BOOL) hbr;
            break;
        }

        //KLAWIATURA
        case WM_KEYDOWN:
        {
            //SendMessage(pt, EM_SETSEL, 10, 15);
            switch( wParam )
            {
                case VK_ESCAPE:
                    DestroyWindow( hwnd );
                break;
            }
        }

        case WM_SIZE:
        {
            if(GetWindowRect(hwnd, &rect))
            {
                width = rect.right - rect.left;
                height = rect.bottom - rect.top;
            }
            MoveWindow(pt,0,0,width - 10,height - 50,true);
            break;
        }

        default://OBSLUGUJE CALA RESZTE ZDARZEN
            {
                if( msg == WM_FINDREPLACE )
                {
                    FINDREPLACE * fr =( FINDREPLACE * ) lParam;
                    if( FR_FINDNEXT )
                    {
                        //SendMessage(pt, EM_SETSEL, 10, 15);
                        MessageBox( hwnd, "Idziemy dalej", NULL, MB_ICONINFORMATION );
                        //TCHAR buff[1024];
                        //GetWindowTextA( pt, buff, 1024 );
                        //MessageBox( hwnd, buff, NULL, MB_ICONINFORMATION );
                        /*int z = znajdz_text("a", pt, 3);
                        if(z == 1){
                            //SendMessage(pt, EM_SETSEL, 1, 5);
                        }*/

                    }else{
                        MessageBox( hwnd, "Nic nie znaleziono.", NULL, MB_ICONINFORMATION );
                    }
                }
                else
                {
                    return DefWindowProc( hwnd, msg, wParam, lParam );
                }
            }
    }
    return 0;
}
