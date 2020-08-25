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
#ifndef BUIWINDOWS_H
#define BUIWINDOWS_H 1

#define UI_MAX_NAME 64
#define UI_MAX_PARM 8

#define UI_MAX_WND_POOL 64
#define UI_MAX_MSG_POOL 256

typedef enum
{
    UIMSG_NOOP  = 0,

    UIMSG_INIT  = 2,
    UIMSG_CREATE,
    UIMSG_DESTROY,
    UIMSG_SHOW,
    UIMSG_HIDE,
    UIMSG_MOVE,
    UIMSG_RESIZE,

    UIMSG_PAINT = 10,

    UIMSG_KEYDOWN,
    UIMSG_KEYUP,
    UIMSG_TIMER,

    UIMSG_MOUSEDOWN,
    UIMSG_MOUSEUP,
    UIMSG_MOUSEMOVE,

    UIMSG_SETFRG,
    UIMSG_SETBKG,

    UIMSG_SETTEXT,
    UIMSG_GETTEXT,

    UIMSG_FIRSTUSER = 256
}
uimsgs_t;

typedef enum
{
    UISTYLE_VISIBLE         = 0x0001,
    UISTYLE_DISABLED        = 0x0002,
    UISTYLE_HASTEXT         = 0x0004,
    UISTYLE_HASIMAGE        = 0x0008,
    UISTYLE_HASTFRG         = 0x0010,

    UISTYLE_HCENTER         = 0x10000,
    UISTYLE_VCENTER         = 0x20000,
    UISTYLE_UNDERLINE       = 0x40000,
    UISTYLE_BLINK           = 0x80000,
}
uistyle_t;

typedef struct tag_uimsg
{
    int                  msg;
    void                *parm1;
    void                *parm2;
    struct tag_uimsg    *next;
}
uimsg_t;

struct tag_window;
typedef void *(*ui_windowfunction_t)(struct tag_window *wnd, int message, void *parm1, void *parm2);

typedef struct tag_window
{
    int      x;                 //< left
    int      y;                 //< top
    int      w;                 //< width
    int      h;                 //< height

    char      *text;            //< window text (as needed)
    COLORREF   frg;             //< window text forground color
    COLORREF   bkg;             //< window background color
    uistyle_t  style;           //< window style
    ui_windowfunction_t func;   //< window message handler function

    char     name[UI_MAX_NAME]; //< window name
    void    *parm[UI_MAX_PARM]; //< window parameter storage

    uimsg_t    *msgq_head;
    uimsg_t    *msgq_tail;

    struct tag_window *next;    //< next in alloc chain
    struct tag_window *sibling; //< next in window order
    struct tag_window *child;   //< child window chain
    struct tag_window *parent;  //< parent window
}
window_t;

typedef struct tag_window_stack
{
    gdi_driver_t *gc;

    rect_t      update_rect;
    bool        erase_bkg;

    window_t    *top;
    window_t    *root;

    uimsg_t    *msg_free;
    uimsg_t     msg_pool[UI_MAX_MSG_POOL];

    window_t   *wnd_free;
    window_t    wnd_pool[UI_MAX_WND_POOL];
}
uistack_t;

void  ui_invalidate_rect(rect_t *rect, bool erase);
void  ui_validate_rect(rect_t *rect);

void  ui_window_rect(window_t *src, rect_t *rect);

int ui_union_rect(rect_t *r, rect_t *a, rect_t *b);
int ui_intersect_rect(rect_t *r, rect_t *a, rect_t *b);

void *ui_get_parm(window_t *wnd, int parm);
int   ui_set_parm(window_t *wnd, int parm, void *value);

void ui_mode_window(window_t *wnd, int x, int y);
void ui_size_window(window_t *wnd, int x, int y, int w, int h);
void ui_redraw_window(window_t *wnd);

void *ui_default_window_function(window_t *wnd, int message, void *parm1, void *parm2);

void *ui_send_message(window_t *dest, int message, void *parm1, void *parm2);
int   ui_post_message(window_t *dest, int message, void *parm1, void *parm2);

window_t *ui_window_create_param(
                                const char *name,
                                int x,
                                int y,
                                int w,
                                int h,
                                uistyle_t style,
                                ui_windowfunction_t func,
                                void *parm1,
                                void *parm2
                                );
#define ui_window_create(n, x, y, w, h, s, f) ui_window_create_param(n, x, y, w, h, s, f, NULL, NULL)
void ui_window_destroy(window_t *wnd);

int ui_get_message(window_t *wnd, window_t **mwnd, uimsg_t *rmsg);
void *ui_dispatch_message(window_t *wnd, uimsg_t *msg);

void ui_stack_window(window_t *wnd, window_t *parent);

int ui_windows_init(gdi_driver_t *gc);

#endif
