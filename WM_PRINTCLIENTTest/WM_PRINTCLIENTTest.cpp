// WM_PRINTCLIENTTest.cpp : �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include "WM_PRINTCLIENTTest.h"

#define MAX_LOADSTRING	100
#define IDC_EDITBOX1	101
#define IDC_BUTTON1		102

// �O���[�o���ϐ�:
HINSTANCE hInst;                                // ���݂̃C���^�[�t�F�C�X
WCHAR szTitle[MAX_LOADSTRING];                  // �^�C�g�� �o�[�̃e�L�X�g
WCHAR szWindowClass[MAX_LOADSTRING];            // ���C�� �E�B���h�E �N���X��
HFONT font;

// ���̃R�[�h ���W���[���Ɋ܂܂��֐��̐錾��]�����܂�:
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

    // TODO: �����ɃR�[�h��}�����Ă��������B

    // �O���[�o������������������Ă��܂��B
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WM_PRINTCLIENTTEST, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // �A�v���P�[�V�����̏����������s���܂�:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WM_PRINTCLIENTTEST));

    MSG msg;

    // ���C�� ���b�Z�[�W ���[�v:
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
//  �֐�: MyRegisterClass()
//
//  �ړI: �E�B���h�E �N���X��o�^���܂��B
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
//   �֐�: InitInstance(HINSTANCE, int)
//
//   �ړI: �C���X�^���X �n���h����ۑ����āA���C�� �E�B���h�E���쐬���܂��B
//
//   �R�����g:
//
//        ���̊֐��ŁA�O���[�o���ϐ��ŃC���X�^���X �n���h����ۑ����A
//        ���C�� �v���O���� �E�B���h�E���쐬����ѕ\�����܂��B
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // �O���[�o���ϐ��ɃC���X�^���X�������i�[���܂��B

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
//  �֐�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  �ړI:    ���C�� �E�B���h�E�̃��b�Z�[�W���������܂��B
//
//  WM_COMMAND  - �A�v���P�[�V���� ���j���[�̏���
//  WM_PAINT    - ���C�� �E�B���h�E�̕`��
//  WM_DESTROY  - ���~���b�Z�[�W��\�����Ė߂�
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // �I�����ꂽ���j���[�̉��:
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
						InvalidateRect(hWnd, NULL, TRUE); // �R���g���[�����\������Ă��Ȃ��ꍇ�A����ōĕ`�悷��B
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

			// �e�L�X�g�{�b�N�X�̋�`���擾����B
			SendMessage(hwndEdit, EM_GETRECT, 0, reinterpret_cast<LPARAM>(&rcEdit));

			// �e�L�X�g�{�b�N�X�̃N���C�A���g�̈���r�b�g�}�b�v�ɕ`�悷��B
			GetClientRect(hwndEdit, &rcClient);
			hdcBack = CreateCompatibleDC(hdc);
			hbmpBack = CreateCompatibleBitmap(hdc, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top);
			hbmpOld = reinterpret_cast<HBITMAP>(SelectObject(hdcBack, hbmpBack));
			SelectObject(hdcBack, GetStockObject(WHITE_BRUSH));
			PatBlt(hdcBack, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, PATCOPY);
			IntersectClipRect(hdcBack, rcEdit.left, rcEdit.top, rcEdit.right - rcEdit.left, rcEdit.bottom - rcEdit.top);
			SendMessage(hwndEdit, WM_PRINTCLIENT, reinterpret_cast<WPARAM>(hdcBack), PRF_CLIENT | PRF_ERASEBKGND);

			// �e�L�X�g�{�b�N�X�̃N���C�A���g���W�̌��_�����C���E�B���h�E����̑��Έʒu�ɍ��킹��悤�ɍČv�Z����B
			// ��WS_EX_CLIENTEDGE ���̃N���C�A���g���_���ύX�����X�^�C�����w�肳���ꍇ�ɕK�{�ƂȂ�B
			ClientToScreen(hwndEdit, reinterpret_cast<POINT*>(&rcClient));
			ScreenToClient(hWnd, reinterpret_cast<POINT*>(&rcClient));

			// �e�L�X�g�{�b�N�X�̓��e��]������B
			BitBlt(hdc,
				rcClient.left,
				rcClient.top,
				rcEdit.right - rcEdit.left,
				rcEdit.bottom - rcEdit.top,
				hdcBack, 0, 0, SRCCOPY);

			// ��n��
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
				TEXT("����������\r\n����������\r\n����������"),
				WS_VISIBLE | WS_CHILD | WS_BORDER | WS_CLIPSIBLINGS | ES_MULTILINE | ES_LEFT | ES_WANTRETURN,
				10, 10, 1000, 480, hWnd,
				(HMENU)IDC_EDITBOX1,
				(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
				NULL);
			SendMessage(hwndEdit, WM_SETFONT, reinterpret_cast<WPARAM>(font), FALSE);
			CreateWindow(
				WC_BUTTON,
				TEXT("�؂�ւ�"),
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
