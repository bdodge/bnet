/*
 * Copyright 2020 Brian Dodge
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "bui.h"
#include "butil.h"

static char *s_dialog_file;
static char *s_artwork;

static gdi_driver_t *s_gc;

int ui_slice(int *updated)
{
    window_t *wnd;
    uimsg_t msg;

    if (ui_get_message(NULL, &wnd, &msg))
    {
        ui_dispatch_message(wnd, &msg);

        if (updated)
        {
            *updated = 1;
        }
    }
    else
    {
        if (updated)
        {
            *updated = 1;
        }
    }
    return 0;
}

gdi_driver_t *ui_get_driver()
{
    return s_gc;
}

int ui_init(const char *artwork, bool usecolor, int w, int h)
{
    window_t *wnd1;
    window_t *wnd2;
    int result;

    // create a graphics context for windowing to draw into
    if (usecolor)
    {
        s_gc = gdi_create_bitmap_driver_24bpp(w, h);
    }
    else
    {
        s_gc = gdi_create_bitmap_driver_1bpp(w, h);
    }
    if (! s_gc)
    {
        return -1;
    }
    // create a window system
    //
    result = ui_windows_init(s_gc);
    /*
    gdi_fill_rect(s_gc, 10, 10, 30, 40, RGB(200,100,100));
    gdi_fill_rect(s_gc, 20, 20, 30, 40, RGB(100,200,100));
    gdi_fill_rect(s_gc, 30, 30, 30, 40, RGB(100,100,200));
    */
    wnd1 = ui_window_create("main", 0, 0, w, h, UISTYLE_VISIBLE, NULL);
    ui_send_message(wnd1, UIMSG_SETBKG, (void*)RGB(100, 200, 230), NULL);
    ui_stack_window(wnd1, NULL);

    wnd2 = ui_window_create("child", 10, 10, 20, 50, UISTYLE_VISIBLE, NULL);
    ui_send_message(wnd2, UIMSG_SETBKG, (void*)RGB(50, 100, 200), NULL);
    ui_stack_window(wnd2, wnd1);

    return result;
}

#define rgbBKG RGB(0,0,127)
#define rgbFRG RGB(0,0,255)

#ifdef WIN32

static HWND s_hwndApp;

static void resize_simulation(int x, int y, int w, int h)
{
    if (s_hwndApp)
    {
        MoveWindow(s_hwndApp, x, y, w + 8, h + 32, TRUE);
    }
}

void update_framebuffer(gdi_driver_t *gc)
{
    int   x = 0,  y = 0;    // offset into window
    int   sx = 1, sy = 1;   // scale up factor
    char *pb, *ps;

    sx = sy = 1;

    pb = pdisp->m_fb;

    switch (pdisp->m_d)
    {
    case 1:

        {
        HBRUSH hbrBkg = CreateSolidBrush(rgbBKG);
        HBRUSH hbrFrg = CreateSolidBrush(rgbFRG);
        HBRUSH lbr, frg;
        unsigned char m, v;
        int           s, xx, yy;
        RECT          rc;

        pb = (char*)pdisp->m_fb;
        m  = 0x80;
        s  = (pdisp->m_w * pdisp->m_d + 7) / 8;

        for (yy = 0; yy < pdisp->m_h; yy++)
        {
            rc.top   = y + sy * yy;
            rc.bottom= rc.top + sy;

            rc.left  = x + sx;
            rc.right = rc.left + sx;

            lbr = 0;

            for (xx = 0, m = 0x80, ps = pb, v = *ps++; xx < pdisp->m_w; xx++)
            {

                frg = (v & m) ? hbrBkg : hbrFrg;
                if (frg != lbr)
                {
                    if (rc.right > rc.left)
                    {
                        FillRect(hdc, &rc, lbr);
                    }
                    lbr = frg;
                    rc.left  = x + sx * xx;
                    rc.right = rc.left;
                }
                rc.right += sx;
                m >>= 1;
                if (! m)
                {
                    m = 0x80;
                    v = *ps++;
                }
            }
            if (rc.right > rc.left)
            {
                FillRect(hdc, &rc, lbr);
            }
            pb += s;
        }
        DeleteObject(hbrBkg);
        DeleteObject(hbrFrg);
        }
        break;

    case 24:
    case 32:

#if 1
        {
        BITMAPINFO bmi;

        memset(&bmi, 0, sizeof(BITMAPINFOHEADER));

        bmi.bmiHeader.biSize        = sizeof(BITMAPINFO);
        bmi.bmiHeader.biWidth       = pdisp->m_w;
        bmi.bmiHeader.biHeight      = -pdisp->m_h;
        bmi.bmiHeader.biPlanes      = 1;
        bmi.bmiHeader.biBitCount    = pdisp->m_d;
        bmi.bmiHeader.biCompression = BI_RGB;

        SetDIBitsToDevice(
                            hdc,
                            x,  y,
                            pdisp->m_w, pdisp->m_h,
                            0, 0,
                            0, pdisp->m_h,
                            pdisp->m_fb,
                            &bmi,
                            DIB_RGB_COLORS
                        );
        }
#else
        {
        DWORD r, g, b;

        for (y = 0; y < pdisp->m_h; y++)
        {
            ps = pb;

            for (x = 0; x < pdisp->m_w; x++)
            {
                r = *ps++;
                g = *ps++;
                b = *ps++;

                SetPixel(hdc, x, y, RGB(r,g,b));
            }
            pb += pdisp->m_s;
        }
        }
#endif
        break;

    default:

        break;
    }
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    //RECT rc;
    PAINTSTRUCT ps;

    switch (message)
    {
    case WM_COMMAND:
        break;

    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        //GetClientRect(hWnd, &rc);
        if (s_display)
            FormatFrameBuffer(hdc, s_display, &ps.rcPaint);
        EndPaint(hWnd, &ps);
        break;

    case WM_LBUTTONDOWN:

        {
            int bx = LOWORD(lParam);
            int by = HIWORD(lParam);

            UIsysMessage(UIM_MOUSEDOWN, (void*)((by << 16) | (bx & 0xFFFF)));
        }
        break;

    case WM_LBUTTONUP:

        {
            int bx = LOWORD(lParam);
            int by = HIWORD(lParam);

            UIsysMessage(UIM_MOUSEUP, (void*)((by << 16) | (bx & 0xFFFF)));
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_KEYDOWN:
    case WM_KEYUP:
        switch (wParam)
        {
        default:
            switch (wParam)
            {
            case VK_LEFT:       wParam = kcLEFT;  break;
            case VK_RIGHT:      wParam = kcRIGHT; break;
            case VK_UP:         wParam = kcUP;    break;
            case VK_DOWN:       wParam = kcDOWN;  break;
            case VK_RETURN:     wParam = kcSELECT;break;
            }
            UIsysMessage((message == WM_KEYDOWN) ? UIM_KEYDOWN : UIM_KEYUP, (void*)wParam);
            break;
        }
        break;

    default:
        break;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

static int run_simulation(int update_display)
{
    MSG      msg;

    if (update_display)
    {
        InvalidateRect(s_hwndApp, NULL, FALSE);
    }

    while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
    {
        if (! GetMessage(&msg, NULL, 0, 0))
        {
            return -1;
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}

extern HINSTANCE s_hInstance;

static int start_simulation(const char* cmdline, int x, int y, int w, int h)
{
    WNDCLASS wcex;

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = (WNDPROC)WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = s_hInstance;
    wcex.hIcon          = NULL; //LoadIcon(s_hInstance, (LPCTSTR)IDI_XHTM);
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.lpszMenuName   = NULL;
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszClassName  = "UIsim";
    //wcex.hIconSm      = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

    RegisterClass(&wcex);

    // create application window
    //
    s_hwndApp = CreateWindow("UIsim", "UI Simulation", WS_OVERLAPPEDWINDOW,
                                x, y, w + 8, h + 32,
                                NULL, NULL, s_hInstance, NULL);

    if (!s_hwndApp)
    {
        return FALSE;
    }
    ShowWindow(s_hwndApp, SW_SHOW);

    return 0;
}

#else /* WIN32 */

#include <X11/Xlib.h>
#include <X11/Xutil.h>

Display*    s_display;
Window      s_rootWnd;
int         s_screen;
uint32_t    s_eventmask;
uint32_t    s_keymask;

Atom        wm_delete_window_atom;
Atom        wm_strins_atom;
Atom        wm_length_atom;

Window      s_xWnd;
XImage*     s_ximg;
char*       s_xbuffer;
int         s_xdepth;

static void resize_simulation(int x, int y, int w, int h)
{
    if (s_display && s_xWnd)
    {
        XMoveResizeWindow(s_display, s_xWnd, x, y, w, h);
    }
}

void update_framebuffer(gdi_driver_t *gc)
{
    int x, y;
    char *pb, *ps, *pxb, *pd;
    unsigned char r, g, b;
    GC xGC;

    if (! gc || ! gc->fb)
    {
        return;
    }
    pb  = gc->fb;
    pxb = s_xbuffer;

    for (y = 0; y < gc->h; y++)
    {
        ps = pb;
        pd = pxb;

        if (gc->d == 24)
        {
            for (x = 0; x < gc->w; x++)
            {
                r = *ps++;
                g = *ps++;
                b = *ps++;

                *pd++ = b;
                *pd++ = g;
                *pd++ = r;
                if (s_xdepth == 4)
                {
                    *pd++ = 0;
                }
            }
        }
        else if (gc->d == 1)
        {
            static const uint8_t mfrg_b = 100;
            static const uint8_t mfrg_g = 100;
            static const uint8_t mfrg_r = 50;
            static const uint8_t mbkg_b = 250;
            static const uint8_t mbkg_g = 255;
            static const uint8_t mbkg_r = 255;

            uint8_t mask = 0x80;

            for (x = 0; x < gc->w; x++)
            {
                r = *ps & mask;

                if (r)
                {
                    *pd++ = mfrg_b;
                    *pd++ = mfrg_g;
                    *pd++ = mfrg_r;
                }
                else
                {
                    *pd++ = mbkg_b;
                    *pd++ = mbkg_g;
                    *pd++ = mbkg_r;
                }
                if (s_xdepth == 4)
                {
                    *pd++ = 0;
                }
                mask >>= 1;
                if (mask == 0)
                {
                    ps++;
                    mask = 0x80;
                }
            }
        }
        pb += gc->s;
        pxb += gc->w * s_xdepth;
    }

    xGC = XCreateGC(s_display, s_xWnd, 0, 0);

    XPutImage(
                    s_display,
                    s_xWnd,
                    xGC,
                    s_ximg,
                    0, 0,
                    0, 0,
                    gc->w, gc->h
                );
    XFreeGC(s_display, xGC);
}

int processX11message()
{
    XEvent  xEvent;
    XEvent  xNextEvent;
    KeySym  aKey;
    int     nq;
    int     bx;
    int     by;
    int     sv;

    struct   timeval timeout;
    int      fd;
    fd_set   rfds;

    XFlush(s_display);

    fd = ConnectionNumber(s_display);

    FD_ZERO (&rfds);
    FD_SET  (fd, &rfds);

    timeout.tv_sec  = 0;
    timeout.tv_usec = 5000;

    sv = select(fd+1, &rfds, NULL, NULL, &timeout);
    if (sv <= 0)
    {
        return 0;
    }

    nq = XEventsQueued(s_display, QueuedAfterReading);

    while (nq-- > 0)
    {
        XNextEvent(s_display, &xEvent);

        switch (xEvent.type)
        {
        case Expose:

            //FormatFrameBuffer(s_display);
            break;

        case ClientMessage:

            if (xEvent.xclient.data.l[0] == wm_delete_window_atom)
            {
                return 1;
            }
            break;

        case ConfigureNotify:

            break;

        case DestroyNotify:

            return 1;

        case KeyPress:
        case KeyRelease:

            XLookupString(
                            (XKeyEvent *)&xEvent,
                            NULL,
                            0,
                            &aKey,
                            0
                        );
            switch (aKey)
            {
            case XK_Left:   aKey = 'L'; break;
            case XK_Up:     aKey = 'U'; break;
            case XK_Right:  aKey = 'R'; break;
            case XK_Down:   aKey = 'D'; break;
            case XK_Escape: aKey = 27;  break;
            case XK_Select: aKey = 13;  break;
            }
            aKey &= 0xFF;
//            UIsysMessage((xEvent.type == KeyPress) ? UIM_KEYDOWN : UIM_KEYUP, (void*)aKey);
            break;

        case MappingNotify:

            XRefreshKeyboardMapping((XMappingEvent *)&xEvent);
            break;

        case FocusIn:

            if (
                    xEvent.xfocus.detail == NotifyInferior
                ||  xEvent.xfocus.detail == NotifyAncestor
                ||  xEvent.xfocus.detail == NotifyNonlinear
            )
            {
                ;
            }
            break;

        case FocusOut:

            if (
                    xEvent.xfocus.detail == NotifyInferior ||
                    xEvent.xfocus.detail == NotifyAncestor ||
                    xEvent.xfocus.detail == NotifyNonlinear
            )
            {
                ;
            }
            break;

        case ButtonPress:

            bx = xEvent.xbutton.x;
            by = xEvent.xbutton.y;

            switch (xEvent.xbutton.button)
            {
            case Button1:
 //               UIsysMessage(UIM_MOUSEDOWN, (void*)((by << 16) | (bx & 0xFFFF)));
                break;
            case Button2:
                break;
            case Button3:
                break;
            case Button4:
            case Button5:
                break;
            }
            break;

        case ButtonRelease:

            bx = xEvent.xbutton.x;
            by = xEvent.xbutton.y;

            switch (xEvent.xbutton.button)
            {
            case Button1:
//                UIsysMessage(UIM_MOUSEUP, (void*)((by << 16) | (bx & 0xFFFF)));
                break;
            case Button2:
                break;
            case Button3:
                break;
            }
            break;

        case MotionNotify:

            while (XEventsQueued(s_display, QueuedAfterReading) > 0)
            {
                XPeekEvent(s_display, &xNextEvent);
                if (xNextEvent.type != MotionNotify)
                    break;
                if (xNextEvent.xmotion.window != xEvent.xmotion.window)
                    break;
                XNextEvent(s_display, &xEvent);
            }
       //     bx = xEvent.xbutton.x;
       //     by = xEvent.xbutton.y;
            break;

        case EnterNotify:

            if (xEvent.xcrossing.mode != NotifyNormal)
                break;
            break;

        case LeaveNotify:

            if (xEvent.xcrossing.mode != NotifyNormal)
                break;

            break;

        default:
            break;
        }
    }
    return 0;
}

static int run_simulation(int update_display)
{
    if (update_display)
    {
        update_framebuffer(ui_get_driver());
    }
    if (! processX11message())
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

static int start_simulation(const char* cmdline, int x, int y, int w, int h)
{
    XSetWindowAttributes xAttribs;
    XSizeHints           xHint;
    Window               xWnd;

    char* argv[2] = { "UIsim", NULL };
    int argc = 1;

    s_display = XOpenDisplay(NULL);
    if (! s_display)
    {
        BERROR("Can't open X display");
        return 1;
    }
    s_screen  = DefaultScreen(s_display);
    s_rootWnd = DefaultRootWindow(s_display);

    s_eventmask = ExposureMask        | FocusChangeMask   |
                  ButtonPressMask     | ButtonReleaseMask |
                  PointerMotionMask   | EnterWindowMask   | LeaveWindowMask |
                  KeyPressMask        | KeyReleaseMask    | KeymapStateMask |
                  PropertyChangeMask  |
                  StructureNotifyMask;

    wm_delete_window_atom = XInternAtom (s_display, "WM_DELETE_WINDOW", False);
    wm_strins_atom = XInternAtom (s_display, "STRING", False);
    wm_length_atom = XInternAtom (s_display, "LENGTH", False);

    xAttribs.background_pixmap  = None;
    xAttribs.background_pixel   = WhitePixel(s_display, s_screen);
    xAttribs.border_pixmap      = CopyFromParent;
    xAttribs.border_pixel       = BlackPixel(s_display, s_screen);

    xAttribs.bit_gravity            = ForgetGravity;
    xAttribs.win_gravity            = NorthWestGravity;
    xAttribs.backing_store          = WhenMapped;
    xAttribs.backing_planes         = 0xFFFFFFFFL;
    xAttribs.backing_pixel          = 0L;
    xAttribs.save_under             = 0;
    xAttribs.event_mask             = s_eventmask;
    xAttribs.do_not_propagate_mask  = 0L;
    xAttribs.override_redirect      = 0;
    xAttribs.colormap               = CopyFromParent;
    xAttribs.cursor                 = None;

    xHint.x         = x;
    xHint.y         = x;
    xHint.width     = w + 8;
    xHint.height    = h + 32;
    xHint.flags     = PPosition | PSize;

    xWnd = XCreateWindow(
                        s_display,
                        DefaultRootWindow(s_display),
                        xHint.x,
                        xHint.y,
                        xHint.width,
                        xHint.height,
                        0,
                        XDefaultDepth(s_display, s_screen),
                        InputOutput,
                        XDefaultVisual(s_display, s_screen),
                        0x7FFFL,
                        &xAttribs
                        );

    if (xWnd == 0)
    {
        BERROR("Null Window");
        return 1;
    }
    XStoreName(s_display, xWnd, "BUI simulation");
    XSetStandardProperties(
                                s_display,
                                xWnd,
                                "BUI simulation",
                                "BUI simulation",
                                None,
                                argv,
                                argc,
                                &xHint
                             );

    XSetWMProtocols(s_display, xWnd, &wm_delete_window_atom, 1L);

    s_xWnd = xWnd;

    s_xbuffer = (char*)malloc((w + 3) * h * 4);
    if (! s_xbuffer)
    {
        BERROR("Can't alloc x fb\n");
        return -1;
    }
    memset(s_xbuffer, 0xff, (w + 3) * h * 4);

    s_ximg = XCreateImage(
                        s_display,
                        DefaultVisual(s_display, DefaultScreen(s_display)),
                        DefaultDepth(s_display, DefaultScreen(s_display)),
                        ZPixmap,
                        0,
                        s_xbuffer,
                        w, h,
                        32,
                        0
                    );
    if (s_ximg)
    {
        if (s_ximg->bits_per_pixel == 24)
        {
            s_xdepth = 3;
        }
        else if (s_ximg->bits_per_pixel == 32)
        {
            s_xdepth = 4;
        }
        else
        {
            BERROR("Need 24 bit RGB Display\n");
            return -1;
        }
    }
    else
    {
        BERROR("Can't make X11 image\n");
        return -1;
    }
    XMapRaised(s_display, xWnd);
    return 0;
}
#endif

static void useage(const char *program, int l)
{
    butil_log(l, "Usage: %s -ldh\n", program);
    butil_log(l, "  -l [level]      - set the log level (default is 3)\n");
    butil_log(l, "  -c              - use color display (default is mono)\n");
    butil_log(l, "  -d [dialogfile] - use dialogfile as artwork source\n");
    butil_log(l, "  -h              - help with usage\n");
}

int main(int argc, char **argv)
{
    const char *program, *arg;
    int loglevel = 3;
    int updated;
    bool usecolor;
    int x, y, w, h;
    int result;

#ifdef Windows
    WSADATA wsaData;

    result = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (result != 0) {
        UPNP_ERROR("WSAStartup failed");
        return -1;
    }
#else
    signal(SIGPIPE, SIG_IGN);
#endif
    butil_set_log_level(loglevel);

    program = *argv++;
    argc--;

    loglevel = 3;
    usecolor = false;
    result = 0;

    while (argc > 0 && ! result)
    {
        arg = *argv;
        if (arg[0] == '-')
        {
            switch (arg[1])
            {
            case 'l': case 'L':
                if ((*argv)[2])
                {
                    loglevel = strtoul(*argv + 2, NULL, 0);
                }
                else if (argc > 1)
                {
                    argv++;
                    argc--;
                    loglevel = strtoul(*argv, NULL, 0);
                }
                else
                {
                    useage(program, 0);
                    return -1;
                }
                break;
            case 'c': case 'C':
                usecolor = true;
                break;
            case 'd': case 'D':
                if ((*argv)[2])
                {
                    s_dialog_file = *argv + 2;
                }
                else if (argc > 1)
                {
                    argv++;
                    argc--;
                    s_dialog_file = *argv;
                }
                else
                {
                    useage(program, 0);
                    return -1;
                }
                break;
            default:
                useage(program, 0);
                return -1;
            }
        }
        else if (! s_dialog_file)
        {
            useage(program, 0);
            return -1;
        }
        argv++;
        argc--;
    }

    if (! s_dialog_file)
    {
        s_dialog_file = "./artwork.txt";
    }

    butil_set_log_level(loglevel);

    if (usecolor)
    {
        x = 100;
        y = 100;
        w = 320;
        h = 240;
    }
    else
    {
        x = 100;
        y = 100;
        w = 132;
        h = 64;
    }
    // Open simulation window view
    //
    result = start_simulation("", x, y, w, h);

    result = ui_init("", usecolor, w, h);

    if (result)
    {
        BERROR("can't init UI");
        return result;
    }

    do
    {
        result = ui_slice(&updated);
        result = run_simulation(updated);
    }
    while (! result);

    return result;
}


