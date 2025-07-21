#include "Main.h"

#include "SimpleInput.h"

HWND WindowHanled;
WNDCLASS wc;

LRESULT CALLBACK WndProc(HWND hWnd,
                        UINT msg,
                        WPARAM wParam,
                        LPARAM lParam);

void InitWindow(const char* title, HINSTANCE instanceHanled, int show)
{

    RECT rcWorkArea;
    SystemParametersInfo(SPI_GETWORKAREA, 0, &rcWorkArea, 0);
    
    int x = (rcWorkArea.right - rcWorkArea.left) / 2 - WINDOW_WIDTH / 2;
    int y = (rcWorkArea.bottom - rcWorkArea.top) / 2 - WINDOW_HEIGHT /2;

    wc.style = CS_HREDRAW |CS_VREDRAW ;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = instanceHanled;
    wc.hIcon = static_cast<HICON>(LoadImage(
    instanceHanled,
    "MDPIV.ico", 
    IMAGE_ICON, GetSystemMetrics(SM_CXICON),
    GetSystemMetrics(SM_CYICON), LR_LOADFROMFILE | LR_DEFAULTSIZE));//wc.hIcon = LoadIcon(0, IDI_APPLICATION);
    wc.hCursor = LoadCursor(0, IDC_ARROW);
    wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
    wc.lpszMenuName = 0;
    wc.lpszClassName = "MDPIV-Class";

    RegisterClass(&wc);

    WindowHanled = CreateWindowA(
        "MDPIV-Class",
        title,
        WS_OVERLAPPEDWINDOW,
        x,
        y,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        0,
        0,
        instanceHanled,
        0
    );

    if(!WindowHanled)
    {
        ErrorMessage_1("Create Window Failed!");
    }

    ShowWindow(WindowHanled,show);
    UpdateWindow(WindowHanled);

    Input_Initialize(WindowHanled);
}

LRESULT CALLBACK WndProc(HWND hWnd,
                        UINT msg,
                        WPARAM wParam,
                        LPARAM lParam)
{
    Input_ProcessMessage(hWnd, msg, wParam, lParam);

    switch(msg)
    {

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        break;

    }

    return DefWindowProc(
        hWnd,
        msg,
        wParam,
        lParam
    );
}

void CleanWindow(HINSTANCE instanceHanled)
{
    UnregisterClass("MDPIV-Class", instanceHanled);
    DestroyWindow(WindowHanled);
}