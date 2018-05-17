// WM_PRINTCLIENTTest.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "WM_PRINTCLIENTTest.h"

#define MAX_LOADSTRING	100
#define IDC_EDITBOX1	101
#define IDC_BUTTON1		102

// グローバル変数:
HINSTANCE hInst;                                // 現在のインターフェイス
WCHAR szTitle[MAX_LOADSTRING];                  // タイトル バーのテキスト
WCHAR szWindowClass[MAX_LOADSTRING];            // メイン ウィンドウ クラス名
HFONT font;

// このコード モジュールに含まれる関数の宣言を転送します:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: ここにコードを挿入してください。

    // グローバル文字列を初期化しています。
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WM_PRINTCLIENTTEST, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // アプリケーションの初期化を実行します:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WM_PRINTCLIENTTEST));

    MSG msg;

    // メイン メッセージ ループ:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  関数: MyRegisterClass()
//
//  目的: ウィンドウ クラスを登録します。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = NULL;
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WM_PRINTCLIENTTEST);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = NULL;

    return RegisterClassExW(&wcex);
}

//
//   関数: InitInstance(HINSTANCE, int)
//
//   目的: インスタンス ハンドルを保存して、メイン ウィンドウを作成します。
//
//   コメント:
//
//        この関数で、グローバル変数でインスタンス ハンドルを保存し、
//        メイン プログラム ウィンドウを作成および表示します。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // グローバル変数にインスタンス処理を格納します。

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  関数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND  - アプリケーション メニューの処理
//  WM_PAINT    - メイン ウィンドウの描画
//  WM_DESTROY  - 中止メッセージを表示して戻る
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 選択されたメニューの解析:
            switch (wmId)
            {
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
			case IDM_FONT:
				{
					LOGFONT lf;
					CHOOSEFONT cf;
					memset(&cf, 0, sizeof(CHOOSEFONT));
					cf.lStructSize = sizeof(CHOOSEFONT);
					cf.hwndOwner = hWnd;
					cf.lpLogFont = &lf;
					cf.Flags = CF_SCREENFONTS | CF_EFFECTS;
					cf.rgbColors = RGB(0, 0, 0);
					cf.nFontType = SCREEN_FONTTYPE;
					if (ChooseFont(&cf))
					{
						HWND hwndEdit = GetDlgItem(hWnd, IDC_EDITBOX1);
						HFONT fontOld = font;
						font = CreateFontIndirect(&lf);
						SendMessage(hwndEdit, WM_SETFONT, reinterpret_cast<WPARAM>(font), TRUE);
						DeleteObject(fontOld);
						InvalidateRect(hWnd, NULL, TRUE); // コントロールが表示されていない場合、これで再描画する。
					}
				}
				break;
			case IDC_BUTTON1:
				{
					HWND hwndEdit = GetDlgItem(hWnd, IDC_EDITBOX1);
					ShowWindow(hwndEdit, IsWindowVisible(hwndEdit) ? SW_HIDE : SW_SHOW);
				}
				break;
			default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			HWND hwndEdit = GetDlgItem(hWnd, IDC_EDITBOX1);
			RECT rcEdit;
			RECT rcClient;
			HDC hdcBack;
			HBITMAP hbmpBack;
			HBITMAP hbmpOld;

			// テキストボックスの矩形を取得する。
			SendMessage(hwndEdit, EM_GETRECT, 0, reinterpret_cast<LPARAM>(&rcEdit));

			// テキストボックスのクライアント領域をビットマップに描画する。
			GetClientRect(hwndEdit, &rcClient);
			hdcBack = CreateCompatibleDC(hdc);
			hbmpBack = CreateCompatibleBitmap(hdc, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top);
			hbmpOld = reinterpret_cast<HBITMAP>(SelectObject(hdcBack, hbmpBack));
			SelectObject(hdcBack, GetStockObject(WHITE_BRUSH));
			PatBlt(hdcBack, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, PATCOPY);
			IntersectClipRect(hdcBack, rcEdit.left, rcEdit.top, rcEdit.right - rcEdit.left, rcEdit.bottom - rcEdit.top);
			SendMessage(hwndEdit, WM_PRINTCLIENT, reinterpret_cast<WPARAM>(hdcBack), PRF_CLIENT | PRF_ERASEBKGND);

			// テキストボックスのクライアント座標の原点をメインウィンドウからの相対位置に合わせるように再計算する。
			// ※WS_EX_CLIENTEDGE 等のクライアント原点が変更されるスタイルが指定される場合に必須となる。
			ClientToScreen(hwndEdit, reinterpret_cast<POINT*>(&rcClient));
			ScreenToClient(hWnd, reinterpret_cast<POINT*>(&rcClient));

			// テキストボックスの内容を転送する。
			BitBlt(hdc,
				rcClient.left,
				rcClient.top,
				rcEdit.right - rcEdit.left,
				rcEdit.bottom - rcEdit.top,
				hdcBack, 0, 0, SRCCOPY);

			// 後始末
			SelectObject(hdcBack, hbmpOld);
			DeleteObject(hbmpBack);
			DeleteDC(hdcBack);

			EndPaint(hWnd, &ps);
		}
		break;
	case WM_CREATE:
		{
			font = CreateFont(144, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Meiryo UI");
			HWND hwndEdit = CreateWindowEx(
				0,//WS_EX_CLIENTEDGE,
				WC_EDIT,
				TEXT("あいうえお\r\nかきくけこ\r\nさしすせそ"),
				WS_VISIBLE | WS_CHILD | WS_BORDER | WS_CLIPSIBLINGS | ES_MULTILINE | ES_LEFT | ES_WANTRETURN,
				10, 10, 1000, 480, hWnd,
				(HMENU)IDC_EDITBOX1,
				(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
				NULL);
			SendMessage(hwndEdit, WM_SETFONT, reinterpret_cast<WPARAM>(font), FALSE);
			CreateWindow(
				WC_BUTTON,
				TEXT("切り替え"),
				WS_VISIBLE | WS_CHILD | WS_BORDER | WS_CLIPSIBLINGS | BS_PUSHBUTTON,
				1020, 10, 120, 30, hWnd,
				(HMENU)IDC_BUTTON1,
				(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
				NULL);
		}
		break;
	case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
