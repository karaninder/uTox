#include "main.h"

#include "../debug.h"

void video_frame(uint16_t id, uint8_t *img_data, uint16_t width, uint16_t height, bool resize) {
    HWND *hwin;
    if (id >= UINT16_MAX) {
        hwin = &preview_hwnd;
    } else {
        hwin = &video_hwnd[id];
    }

    if (!hwin || !*hwin) {
        LOG_ERR("Windows Video", "frame for null window [%u]", id);
        return;
    }

    if (resize) {
        RECT r = {.left = 0, .top = 0, .right = width, .bottom = height };
        AdjustWindowRect(&r, WS_OVERLAPPEDWINDOW, 0);

        int w, h;
        w = r.right - r.left;
        h = r.bottom - r.top;
        if (w > GetSystemMetrics(SM_CXSCREEN)) {
            w = GetSystemMetrics(SM_CXSCREEN);
        }

        if (h > GetSystemMetrics(SM_CYSCREEN)) {
            h = GetSystemMetrics(SM_CYSCREEN);
        }

        SetWindowPos(*hwin, 0, 0, 0, w, h, SWP_NOZORDER | SWP_NOMOVE);
    }

    BITMAPINFO bmi = {.bmiHeader = {
                          .biSize        = sizeof(BITMAPINFOHEADER),
                          .biWidth       = width,
                          .biHeight      = -height,
                          .biPlanes      = 1,
                          .biBitCount    = 32,
                          .biCompression = BI_RGB,
                      } };


    RECT r = { 0, 0, 0, 0 };
    GetClientRect(*hwin, &r);

    HDC dc = GetDC(*hwin);

    if (width == r.right && height == r.bottom) {
        SetDIBitsToDevice(dc, 0, 0, width, height, 0, 0, 0, height, img_data, &bmi, DIB_RGB_COLORS);
    } else {
        StretchDIBits(dc, 0, 0, r.right, r.bottom, 0, 0, width, height, img_data, &bmi, DIB_RGB_COLORS, SRCCOPY);
    }
}
