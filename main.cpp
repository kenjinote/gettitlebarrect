#define WINVER 0x0500
#pragma comment(linker,"/opt:nowin98")
#include <windows.h>

CHAR szClassName[]="window";
#define IDU_EDIT1 101

HWND hEdit;
WNDPROC EditWndProc;

LRESULT CALLBACK EditProc1(HWND hWnd,UINT msg,WPARAM wp,LPARAM lp)
{
	switch (msg){
	case WM_CAPTURECHANGED:
	case WM_LBUTTONDBLCLK:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_MBUTTONDBLCLK:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MOUSEACTIVATE:
	case WM_MOUSEMOVE:
	case WM_NCHITTEST:
	case WM_NCLBUTTONDBLCLK:
	case WM_NCLBUTTONDOWN:
	case WM_NCLBUTTONUP:
	case WM_NCMBUTTONDBLCLK:
	case WM_NCMBUTTONDOWN:
	case WM_NCMBUTTONUP:
	case WM_NCMOUSEMOVE:
	case WM_NCRBUTTONDBLCLK:
	case WM_NCRBUTTONDOWN:
	case WM_NCRBUTTONUP:
	case WM_RBUTTONDBLCLK:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP :
	case WM_KEYDOWN :
	case WM_KEYUP:
		return 0;
	default:
		break;
    }
    return (CallWindowProc(EditWndProc,hWnd,msg,wp,lp));
}


LRESULT CALLBACK WndProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	static HFONT hFont;
    switch (msg){
	case WM_CREATE:
		hFont = CreateFont(14,0,/* フォント高さ,文字幅 */
			0,
			0,
			FW_NORMAL,/* フォント太さ(FW_LIGHT,FW_REGULARなど) */
			FALSE,/* イタリック体 */
			FALSE,/* アンダーライン */
			FALSE,/* 打ち消し線 */
			SHIFTJIS_CHARSET,
			OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS,
			PROOF_QUALITY,
			FIXED_PITCH | FF_MODERN,
			"ＭＳ ゴシック"/* 書体名 */);
		hEdit=CreateWindowEx(WS_EX_STATICEDGE,
			"EDIT",
			NULL,
			WS_CHILD|WS_VISIBLE|ES_NOHIDESEL|ES_READONLY|ES_MULTILINE,
			0,0,
			0,0,
			hWnd,
			(HMENU)IDU_EDIT1,
			((LPCREATESTRUCT)lParam)->hInstance,
			NULL);
		EditWndProc=(WNDPROC)SetWindowLong(hEdit,GWL_WNDPROC,(LONG)EditProc1);
		{
			CHAR strTitleBarRect[1024];
			CHAR *str;
			
			TITLEBARINFO pti;
			pti.cbSize = sizeof(TITLEBARINFO);
			GetTitleBarInfo(hWnd,&pti);
			
			wsprintf(strTitleBarRect,"TitleBarRect = { %d, %d, %d, %d };\r\nWidth: %d\r\nHeight: %d",pti.rcTitleBar.left,pti.rcTitleBar.top,pti.rcTitleBar.right,pti.rcTitleBar.bottom,pti.rcTitleBar.right-pti.rcTitleBar.left,pti.rcTitleBar.bottom-pti.rcTitleBar.top);
			
			SetWindowText(hEdit,strTitleBarRect);
			SendMessage(hEdit,EM_SETSEL,0,-1);
			SetFocus(hEdit);
		}
		SendMessage(hEdit,WM_SETFONT,(WPARAM)hFont,TRUE);
		break;
	case WM_SIZE:
		MoveWindow(hEdit,0,0,LOWORD(lParam),HIWORD(lParam),TRUE); 
		break; 
	case WM_DESTROY:
		DeleteObject(hFont);
		PostQuitMessage(0);
		break;
	default:
		return(DefWindowProc(hWnd,msg,wParam,lParam));
    }
    return (0L);
}

int WINAPI WinMain(HINSTANCE hinst,HINSTANCE hPreInst,
                   LPSTR pCmdLine,int nCmdShow)
{
    HWND hWnd;
    MSG msg;
    WNDCLASS wndclass;
    if(!hPreInst){
        wndclass.style=CS_HREDRAW|CS_VREDRAW;
        wndclass.lpfnWndProc=WndProc;
        wndclass.cbClsExtra=0;
        wndclass.cbWndExtra=0;
        wndclass.hInstance =hinst;
        wndclass.hIcon=NULL;
        wndclass.hCursor=LoadCursor(NULL,IDC_ARROW);
        wndclass.hbrBackground=(HBRUSH)(COLOR_WINDOW+1);
        wndclass.lpszMenuName=NULL;
        wndclass.lpszClassName=szClassName;
        if(!RegisterClass(&wndclass))
            return FALSE;
    }
    hWnd=CreateWindow(szClassName,
        "タイトルバーの領域を取得する",
        WS_CAPTION,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        NULL,
        NULL,
        hinst,
        NULL);
    ShowWindow(hWnd,nCmdShow);
    UpdateWindow(hWnd);
    while (GetMessage(&msg,NULL,0,0)){
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (msg.wParam);
}

