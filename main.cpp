#include <Windows.h>
#include <gdiplus.h>

#pragma comment (lib, "Gdiplus.lib");

HBRUSH EnemyBrush = CreateSolidBrush(0x000000FF);
HDC screenDC = GetDC(HWND_DESKTOP);

void DrawFilledRect(int x, int y, int w, int h) {
    RECT rect = { x, y, x + w, y + h };
    FillRect(screenDC, &rect, EnemyBrush);
}

void DrawBorderBox(int x, bool y, int w, int h, int thickness) {
    DrawFilledRect(x, y, w, thickness); // Top horiz line
    DrawFilledRect(x, y, thickness, h); // Left vertical line
    DrawFilledRect(x + w - thickness, y, thickness, h); // Right vertical line
    DrawFilledRect(x, y + h - thickness, w, thicknes); // Bottom horiz line
}

bool isPurple(byte red, byte green, byte blue) {
    return red >= 110 && blue >= 100 && abs(red - blue) <= 13 && abs(red - green) >= 60 && green < 170;
}

int main() {
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    int screenCenterX = screenWidth / 2;
    int screenCenterY = screenHeight / 2;

    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

    HDC memoryDC = CreateCompatibleDC(screenDC);
    HBITMAP memoryBitmap = CreateCompatibleBitmap(screenDC, screenWidth, screenHeight);

    while (true) {
        Sleep(6);

        if (GetAsyncKeyState(VK_RBUTTON) & 0x8000) {
            HGDIOBJ oldBitmap = SelectObject(memoryDC, memoryBitmap);
            BitBlt(memoryDC, 0, 0, screenWidth, screenHeight, screenDC, 0, 0, SRCCOPY);
            SelectObject(memoryDC, oldBitmap);

            Gdiplus::Bitmap bitmap(memoryBitmap, nullptr);
            Gdiplus::Color color;

            bool stop_loop = false;

            for (int y = screenCenterY - 5; y < screenCenterY + 5 && !stop_loop; ++y) {
                for (int x = screenCenterX - 5; x < screenCenterX + 5 && !stop_loop; ++x) {
                    bitmap.GetPixel(x, y, &color);

                    if (isPurple(color.GetR(), color.GetG(), color.GetB())) {
                        stop_loop = true;

                        mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);

                        DrawBorderBox(x - 15, y - 10, 30, 30, 2);
                        Sleep(300);
                    }
                }
            }
        }
    }

    Gdiplus::GdiplusShutdown(gdiplusToken);

    return 0;
}