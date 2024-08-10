/**
 * @file window.c
 * @date 2024-08-10
 * @brief
 */

#include "window.h"
#include "graphics/theme.h"
#include "graphics/drawing.h"
#include "../core/program.h"
#include "../core/result.h"
#include "../core/log.h"

#include <wchar.h>


static HWND mWindow = { 0 };
static MSG mMessage = { 0 };
static WNDCLASSEXW mClass = { 0 };
static wchar_t mName[] = L"__NeonBlockWindowClass";
static wchar_t mCaption[] = L"Neon v24.10 - Branch Sandbox/Experimental";
static RECT mWindowArea = { 0 };

static long long _NeonProcedure(HWND windowHandle, UINT message, WPARAM wordParam, LPARAM longParam)
{
    PAINTSTRUCT paint;
    HDC displayContext;

    switch (message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_PAINT:
        displayContext = BeginPaint(mWindow, &paint);
        NeonInitDrawingTools();
        (void) displayContext;
        NeonFreeDrawingTools();
        (void) EndPaint(windowHandle, &paint);
        break;
    case WM_SIZE:
        (void) GetClientRect(windowHandle, &mWindowArea);
        break;
    }
    
    return DefWindowProcW(windowHandle, message, wordParam, longParam);
}

static int _NeonMessageLoop(void)
{
    while (GetMessageW(&mMessage, NULL, 0, 0))
    {
        (void) TranslateMessage(&mMessage);
        (void) DispatchMessageW(&mMessage);
    }

    return (int) mMessage.wParam;
}

static NeonResult _NeonCreateWindow(void)
{
    mWindow = CreateWindowExW(
        WS_EX_OVERLAPPEDWINDOW,
        mName,
        mCaption,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
        NULL, NULL,
        NeonGetHandle(),
        NULL
    );

    if (!mWindow)
    {
        return NeonLogAndReturn(NeonError, NeonCreateResult(NeonFail, L"Failed to create window."));
    }

    return NeonLogAndReturn(NeonInformation, NeonCreateResult(NeonSuccess, L"Successfully created window."));
}

static NeonResult _NeonRegisterWindow(void)
{
    mClass.cbSize = sizeof(WNDCLASSEXW);
    mClass.style = CS_HREDRAW | CS_VREDRAW;
    mClass.lpfnWndProc = _NeonProcedure;
    mClass.cbClsExtra = 0;
    mClass.cbWndExtra = 0;
    mClass.hInstance = NeonGetHandle();
    mClass.hCursor = LoadCursorW(NULL, IDC_ARROW);
    mClass.hIcon = LoadIconW(NeonGetHandle(), IDI_APPLICATION);
    mClass.hIconSm = LoadIconW(NeonGetHandle(), IDI_APPLICATION);
    mClass.hbrBackground = CreateSolidBrush(NeonGetBackgroundColour());
    mClass.lpszMenuName = NULL;
    mClass.lpszClassName = mName;

    int success = RegisterClassExW(&mClass);

    if (!success)
    {
        return NeonLogAndReturn(NeonError, NeonCreateResult(NeonFail, L"Failed to register window class."));
    }

    return NeonLogAndReturn(NeonInformation, NeonCreateResult(NeonSuccess, L"Successfully registered window class."));
}

NeonResult NeonInitWindow(void)
{
    NeonResult registerResult = _NeonRegisterWindow();
    if (registerResult.code == NeonFail)
    {
        NeonFreeWindow();
        return NeonLogAndReturn(NeonError, NeonCreateResult(NeonFail, L"Failed to initialise window."));
    }

    NeonResult createResult = _NeonCreateWindow();
        if (createResult.code == NeonFail)
    {
        NeonFreeWindow();
        return NeonLogAndReturn(NeonError, NeonCreateResult(NeonFail, L"Failed to initialise window."));
    }

    (void) ShowWindow(mWindow, NeonGetShowFlag());
    (void) UpdateWindow(mWindow);
    (void) _NeonMessageLoop();

    return NeonLogAndReturn(NeonInformation, NeonCreateResult(NeonSuccess, L"Successfully initialised window."));
}

RECT NeonGetWindowArea(void)
{
    return mWindowArea;
}

NeonResult NeonFreeWindow(void)
{
    (void) UnregisterClassW(mName, NeonGetHandle());

    if (mClass.hIcon)
    {
        (void) DestroyIcon(mClass.hIcon);
    }

    if (mClass.hIconSm)
    {
        (void) DestroyIcon(mClass.hIconSm);
    }

    if (mClass.hCursor)
    {
        (void) DestroyCursor(mClass.hCursor);
    }

    if (mClass.hbrBackground)
    {
        (void) DeleteObject(mClass.hbrBackground);
    }

    if (mWindow)
    {
        (void) DestroyWindow(mWindow);
    }

    return NeonLogAndReturn(NeonInformation, NeonCreateResult(NeonSuccess, L"Cleaning up window resources."));
}