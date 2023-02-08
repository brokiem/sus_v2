#include <windows.h>
#include <gdiplus.h>
#include <iostream>

#pragma comment(lib, "gdiplus.lib")

HBRUSH EnemyBrush = CreateSolidBrush(0x000000FF);
HDC screenDC = GetDC(HWND_DESKTOP);

void DrawFilledRect(int x, int y, int w, int h) {
    RECT rect = { x, y, x + w, y + h };
    FillRect(screenDC, &rect, EnemyBrush);
}

void DrawBorderBox(int x, int y, int w, int h, int thickness) {
    DrawFilledRect(x, y, w, thickness); // Top horiz line
    DrawFilledRect(x, y, thickness, h); // Left vertical line
    DrawFilledRect(x + w - thickness, y, thicknes, h); // Right vertical line
    DrawFilledRect(x, y + h - thickness, w, thicknes); // Bottom horiz line
}

bool isPurple(byte red, byte green, byte blue) {
    return red >= 110 && blue >= 100 && abs(red - blue) <= 13 && abs(red - green) >= 60 && green < 170;
}

int main() {
    SetConsoleTitleA("sus");

    printf("Starting sus...\n");

    int screenCenterX = GetSystemMetrics(SM_CXSCREEN) / 2;
    int screenCenterY = GetSystemMetrics(SM_CYSCREEN) / 2;

    int width = 5;
    int height = 5;

    int x = screenCenterX - width / 2;
    int y = screenCenterY - height / 2;

    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

    HDC memoryDC = CreateCompatibleDC(screenDC);
    HBITMAP memoryBitmap = CreateCompatibleBitmap(screenDC, width, height);

    printf("sus started successfully!\n");

    while (true) {
        Sleep(6);

        if (GetAsyncKeyState(VK_RBUTTON) & 0x8000) {
            HGDIOBJ oldBitmap = SelectObject(memoryDC, memoryBitmap);
            BitBlt(memoryDC, 0, 0, width, height, screenDC, x, y, SRCCOPY);
            SelectObject(memoryDC, oldBitmap);

            Gdiplus::Bitmap bitmap(memoryBitmap, nullptr);
            Gdiplus::Color color;

            bool stop_loop = false;

            for (int i = 0; i < height && !stop_loop; ++i) {
                for (int j = 0; j < width && !stop_loop; ++j) {
                    bitmap.GetPixel(i, j, &color);

                    if (isPurple(color.GetR(), color.GetG(), color.GetB())) {
                        stop_loop = true;

                        mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);

                        DrawBorderBox(screenCenterX + j - 15, screenCenterY + i - 10, 30, 30, 2);
                        Sleep(300);
                    }
                }
            }
        }
    }

    Gdiplus::GdiplusShutdown(gdiplusToken);

    return 0;
}