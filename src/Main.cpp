#include "Graphics.h"
#include "Scene.h"
#include "SimpleInput.h"

int run = 1;
extern HWND WindowHanled;

void CenterCursorInWindow(HWND hwnd) {
    RECT windowRect;
    if (GetWindowRect(hwnd, &windowRect)) {
        int centerX = windowRect.left + (windowRect.right - windowRect.left) / 2;
        int centerY = windowRect.top + (windowRect.bottom - windowRect.top) / 2;
        
        SetCursorPos(centerX, centerY);
    }
}

void InitWindow(const char* title, HINSTANCE instanceHanled, int show);
void CleanWindow(HINSTANCE instanceHanled);

int WINAPI WinMain(HINSTANCE hInstance,
                    HINSTANCE hPrevInstance,
                    PSTR pCmdLine,
                    int nShowCmd)
{
    InitWindow("MDPIV",hInstance,nShowCmd);

    CenterCursorInWindow(WindowHanled);
    ShowCursor(FALSE);

    LONG style = GetWindowLong(WindowHanled, GWL_STYLE); 
    style &= ~(WS_MAXIMIZEBOX); //| WS_MINIMIZEBOX);
    SetWindowLong(WindowHanled, GWL_STYLE, style); 

    Graphics graph = Graphics();
    graph.MakeContext();

    Scene sc = Scene();
    sc.InitEnv();

    MSG msg;

    while(run)
    {
        if(PeekMessage(&msg,0,0,0,PM_REMOVE))
        {
            if(msg.message == WM_QUIT)
                run = 0;

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            Input_Update();
            sc.Render();
            graph.Present();
        }
    }
    
    CleanWindow(hInstance);

    return msg.wParam;
}

void ErrorMessage_1(const char* message)
{
    MessageBoxA(NULL,message,"Engine Error!",MB_OK);
}

void ErrorMessage_2(HWND hwnd,const char* message)
{
    MessageBoxA(hwnd,message,"Engine Error!",MB_OK);
}

float TimeDelta()
{
    static float lastTime = (float)timeGetTime(); 
	float currTime  = (float)timeGetTime();
	float timeDelta = (currTime - lastTime)*0.001f;
	return timeDelta;
	lastTime = currTime;
}