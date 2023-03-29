#include "MainWindow.hpp"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

    auto points = new Points();
    MainWindow window(points);
    HWND hWnd;

    TCHAR szClassName[] = TEXT("MyClass");
    WNDCLASSEX wc{ sizeof(wc), CS_HREDRAW | CS_VREDRAW, MainWindow::WndProcWrapper, 0, 0, hInstance, LoadIcon(NULL, IDI_APPLICATION), LoadCursor(NULL, IDC_ARROW), (HBRUSH)GetStockObject(WHITE_BRUSH), NULL, szClassName, LoadIcon(NULL, IDI_APPLICATION) };

    if (!RegisterClassEx(&wc))
        return 0;
  

    hWnd = CreateWindow(szClassName, TEXT("AI1"), WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MAXIMIZE, 0, 0, 1920, 1080, (HWND)NULL, (HMENU)NULL, (HINSTANCE)hInstance, &window);
    ShowWindow(hWnd, SW_SHOWMAXIMIZED);
    UpdateWindow(hWnd);
    MSG msg;

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}


