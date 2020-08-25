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

// window stack
uistack_t s_stack;

void ui_window_rect(window_t *wnd, rect_t *rect)
{
    if (wnd && rect)
    {
        rect->top = wnd->y;
        rect->left = wnd->x;
        rect->bottom = wnd->y + wnd->h;
        rect->right = wnd->x + wnd->w;
    }
}

int ui_union_rect(rect_t *r, rect_t *a, rect_t *b)
{
    if (a->top <= b->top)
    {
        r->top = a->top;
    }
    else
    {
        r->top = b->top;
    }
    if (a->left <= b->left)
    {
        r->left = a->left;
    }
    else
    {
        r->left = b->left;
    }
    if (a->bottom >= b->bottom)
    {
        r->bottom = a->bottom;
    }
    else
    {
        r->bottom = b->bottom;
    }
    if (a->right >= b->right)
    {
        r->right = a->right;
    }
    else
    {
        r->right = b->right;
    }
    return r->top != r->bottom || r->left != r->right;
}

int ui_intersect_rect(rect_t *r, rect_t *a, rect_t *b)
{
    if (a->top >= b->top)
    {
        r->top = a->top;
    }
    else
    {
        r->top = b->top;
    }
    if (a->left >= b->left)
    {
        r->left = a->left;
    }
    else
    {
        r->left = b->left;
    }
    if (a->bottom <= b->bottom)
    {
        r->bottom = a->bottom;
    }
    else
    {
        r->bottom = b->bottom;
    }
    if (a->right <= b->right)
    {
        r->right = a->right;
    }
    else
    {
        r->right = b->right;
    }
    if (r->bottom < r->top)
    {
        r->bottom = r->top;
    }
    if (r->right < r->left)
    {
        r->right = r->left;
    }
    return r->top < r->bottom && r->left < r->right;
}

void ui_invalidate_rect(rect_t *rect, bool erase)
{
    if (rect)
    {
        ui_union_rect(&s_stack.update_rect, &s_stack.update_rect, rect);
        s_stack.erase_bkg = erase;
    }
    else
    {
        ui_window_rect(s_stack.root, &s_stack.update_rect);
        s_stack.erase_bkg = true;
    }
}

void ui_validate_rect(rect_t *rect)
{
}

void ui_redraw_window(window_t *wnd)
{
    rect_t window_rect;

    if (wnd)
    {
        ui_window_rect(wnd, &window_rect);
        ui_invalidate_rect(&window_rect, true);
    }
}

void *ui_get_parm(window_t *wnd, int parm)
{
}

int ui_set_parm(window_t *wnd, int parm, void *value)
{
}

void ui_move_window(window_t *wnd, int x, int y)
{
}

void ui_size_window(window_t *wnd, int x, int y, int w, int h)
{
}

void *ui_default_window_function(window_t *wnd, int message, void *parm1, void *parm2)
{
    if (! wnd)
    {
        return NULL;
    }

    switch (message)
    {
    case UIMSG_SETFRG:
        wnd->frg = (COLORREF)parm1;
        ui_redraw_window(wnd);
        break;

    case UIMSG_SETBKG:
        wnd->bkg = (COLORREF)parm1;
        ui_redraw_window(wnd);
        break;

    case UIMSG_PAINT:
        if (s_stack.erase_bkg)
        {
            gdi_fill_rect(s_stack.gc, wnd->x, wnd->y, wnd->w, wnd->h, wnd->bkg);
        }
        if (wnd->text)
        {
            gdi_text_out(s_stack.gc, wnd->text, wnd->x, wnd->y, wnd->frg);
        }
        else
        {
            gdi_text_out(s_stack.gc, wnd->name, wnd->x, wnd->y, wnd->frg);
        }
        break;
    }
}

void *ui_send_message(window_t *wnd, int message, void *parm1, void *parm2)
{
    if (! wnd || ! wnd->func)
    {
        return NULL;
    }
    return wnd->func(wnd, message, parm1, parm2);
}

int ui_post_message(window_t *wnd, int message, void *parm1, void *parm2)
{
    uimsg_t *msg;

    if (! wnd || ! wnd->func)
    {
        return -1;
    }
    if (! s_stack.msg_free)
    {
        return -1;
    }
    msg = s_stack.msg_free;
    s_stack.msg_free = msg->next;

    msg->next = NULL;

    if (! wnd->msgq_head)
    {
        wnd->msgq_head = msg;
    }
    else
    {
        wnd->msgq_tail->next = msg;
    }
    wnd->msgq_tail = msg;

    return 0;
}

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
                                )
{
    window_t *wnd;

    wnd = s_stack.wnd_free;
    if (! wnd)
    {
        return NULL;
    }
    s_stack.wnd_free = wnd->next;

    wnd->msgq_head = NULL;
    wnd->msgq_tail = NULL;

    wnd->x = x;
    wnd->y = y;
    wnd->w = w;
    wnd->h = h;

    wnd->style = style;
    wnd->func = func;

    if (! wnd->func)
    {
        wnd->func = ui_default_window_function;
    }

    if (s_stack.gc->d > 1)
    {
        wnd->frg = RGB(0,0,0);
        wnd->bkg = RGB(255,255,255);
    }
    else
    {
        wnd->frg = RGB(255,255,255);
        wnd->bkg = RGB(0,0,0);
    }

    snprintf(wnd->name, sizeof(wnd->name), "%s", name);

    ui_send_message(wnd, UIMSG_CREATE, parm1, parm2);
    ui_post_message(wnd, UIMSG_INIT, parm1, parm2);

    return wnd;
}

void ui_window_destroy(window_t *wnd)
{
    if (wnd)
    {
        wnd->name[0] = '\0';
        wnd->next = s_stack.wnd_free;
        s_stack.wnd_free = wnd;
    }
}

void ui_repaint_stack(void)
{
    window_t *wnd;
    window_t *last_painted_wnd;

    rect_t update_rect;
    rect_t display_rect;
    rect_t window_rect;

    wnd = s_stack.root;
    if (! wnd)
    {
        return;
    }
    // get rect of root window/display
    //
    ui_window_rect(wnd, &display_rect);

    // and a quick copy of the update region
    //
    update_rect = s_stack.update_rect;

    // this is a double-loop, since painting can
    // spawn updates. loop through whole stack
    // from top to bottom until a definitive last
    // window is painted
    //
    last_painted_wnd = NULL;

    do
    {
        for (wnd = s_stack.top; wnd && wnd->next != last_painted_wnd; wnd = wnd->next)
        {
            ui_window_rect(wnd, &window_rect);

            // if this window covers the update rect and is
            // not transparent, no need to go further
            //
            if (
                    window_rect.left <= update_rect.left
                &&  window_rect.top <= update_rect.top
                &&  window_rect.right >= update_rect.right
                &&  window_rect.bottom >= update_rect.bottom
            )
            {
                if (
                       ((wnd->bkg & 0xFF000000) == 0x00)
                    || (! s_stack.erase_bkg)
                )
                {
                    break;
                }
            }
        }
        if (wnd)
        {
            if (wnd->style & UISTYLE_VISIBLE)
            {
                if (ui_intersect_rect(&window_rect, &update_rect, &window_rect))
                {
                    ui_send_message(wnd, UIMSG_PAINT, (void*)&window_rect, NULL);
                }
            }
            last_painted_wnd = wnd;
        }
    }
    while (wnd && (wnd != s_stack.top));

    s_stack.update_rect.top = 0;
    s_stack.update_rect.left = 0;
    s_stack.update_rect.bottom = 0;
    s_stack.update_rect.right = 0;

    s_stack.erase_bkg = false;
}

void ui_stack_window(window_t *wnd, window_t *parent)
{
    if (! wnd)
    {
        return;
    }

    if (! parent)
    {
        parent = s_stack.root;
    }

    if (parent)
    {
        window_t *px;

        if (parent->child)
        {
            for (px = parent->child; px->sibling;)
            {
                px = px->sibling;
            }
            px->sibling = wnd;
        }
        else
        {
            parent->child = wnd;
        }
        wnd->sibling = NULL;
        wnd->parent  = parent;
    }
    else
    {
        wnd->sibling   = NULL;
        wnd->parent    = NULL;
        wnd->child     = NULL;
        s_stack.root   = wnd;
    }
    wnd->next = s_stack.top;

    s_stack.top = wnd;

    if (wnd->style & UISTYLE_VISIBLE)
    {
        rect_t window_rect;

        ui_window_rect(wnd, &window_rect);
        ui_invalidate_rect(&window_rect, true);
    }
}

int ui_get_message(window_t *wnd, window_t **mwnd, uimsg_t *rmsg)
{
    uimsg_t *msg;

    if (mwnd)
    {
        *mwnd = NULL;
    }
    if (! wnd)
    {
        for (wnd = s_stack.top; wnd; wnd = wnd->next)
        {
            if (wnd->msgq_head)
            {
                break;
            }
        }
    }
    if (! wnd)
    {
        return 0;
    }
    if (mwnd)
    {
        *mwnd = wnd;
    }
    if (! rmsg)
    {
        return (wnd->msgq_head != NULL) ? 1 : 0;
    }
    if (! wnd->msgq_head)
    {
        return 0;
    }
    msg = wnd->msgq_head;
    wnd->msgq_head = msg->next;
    if (! wnd->msgq_head)
    {
        wnd->msgq_tail = NULL;
    }
    *rmsg = *msg;

    msg->next = s_stack.msg_free;
    s_stack.msg_free = msg;

    return 1;
}

void *ui_dispatch_message(window_t *wnd, uimsg_t *msg)
{
    void *mres;

    if (! wnd || ! msg)
    {
        return NULL;
    }

    mres = wnd->func(wnd, msg->msg, msg->parm1, msg->parm2);

    if (s_stack.update_rect.bottom != s_stack.update_rect.top)
    {
        ui_repaint_stack();
    }
}

int ui_windows_init(gdi_driver_t *gc)
{
    uistack_t *ps = &s_stack;
    window_t *wnd;
    int i;

    for (i = 0; i < sizeof(ps->wnd_pool) / sizeof(window_t); i++)
    {
        ps->wnd_pool[i].name[0] = '\0';
        ps->wnd_pool[i].sibling = NULL;
        ps->wnd_pool[i].child = NULL;
        ps->wnd_pool[i].parent = NULL;
        ps->wnd_pool[i].next = &ps->wnd_pool[i + 1];
    }
    ps->wnd_pool[i - 1].next = NULL;
    ps->wnd_free = &ps->wnd_pool[0];

    for (i = 0; i < sizeof(ps->msg_pool) / sizeof(uimsg_t); i++)
    {
        ps->msg_pool[i].msg = UIMSG_NOOP;
        ps->msg_pool[i].next = &ps->msg_pool[i + 1];
    }
    ps->msg_pool[i - 1].next = NULL;
    ps->msg_free = &ps->msg_pool[0];

    s_stack.gc = gc;

    // create a root entry so window stack is never empty
    //
    wnd = ui_window_create_param("root", 0, 0, gc->w, gc->h, 0, NULL, NULL, NULL);
    if (! wnd)
    {
        return -1;
    }

    ui_stack_window(wnd, NULL);

    return 0;
}

