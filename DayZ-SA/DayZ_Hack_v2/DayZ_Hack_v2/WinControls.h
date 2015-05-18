#ifndef WINCONTROLS_H
#define WINCONTROLS_H



static HWND hButton, hCombo, hEdit, hList, hScroll, hStatic,  hListView, hListView2 ;
static HMODULE inj_moduleHandle;

extern HWND hWndList;
extern HWND hWndList2;
extern HWND hwndListWindow;
extern bool gameover;
extern D3DXVECTOR3 testCords;
extern int EntitySize ;
extern map< int, D3DXVECTOR3 > mapCoords;
extern map<int, char*> mapEntityName;

void Game_Run ( HWND window ) ;

// structures
typedef struct tagHOUSEINFO
{
    char szAddress[64];
    char szCity[16];
    int iPrice;
    int iBeds;
    int iBaths;
} HOUSEINFO;

LRESULT CALLBACK WindowProc2 ( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) ;
DWORD WINAPI Thread2 ( LPVOID lpParameter ) ;
void listBoxWindow () ;
HWND CreateListView ( HWND hWndParent ) ;
HWND CreateListViewTwo (HWND hWndParent) ;
LRESULT NotifyHandler ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) ;
int CALLBACK ListViewCompareProc ( LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort ) ;

#endif WINCONTROLS_H
