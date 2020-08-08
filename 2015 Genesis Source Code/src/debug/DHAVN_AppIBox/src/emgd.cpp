#include "emgd.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xlibint.h>

static int screen_number = -1;
static Display *x11_display = NULL;
static xcb_connection_t *c = NULL;

xcb_extension_t xcb_iegd_id = { "Intel-EmbeddedGraphicsDriverExtension" };

#define XCB_IEGD_ESCAPE 2


void emgd_init ()
{
    x11_display = XOpenDisplay(":0.0");
    c = xcb_connect(DisplayString(x11_display), &screen_number);
}

xcb_iegd_escape_reply_t *
xcb_iegd_escape_reply (xcb_connection_t          *c  /**< */,
                       unsigned int sequence  /**< */,
                       xcb_generic_error_t      **e  /**< */)
{
    xcb_iegd_escape_reply_t *ret;
    ret = (xcb_iegd_escape_reply_t *) xcb_wait_for_reply(c, sequence, e);

    return ret;
}

unsigned int
xcb_iegd_escape (xcb_connection_t *c  /**< */,
                 uint32_t          escape_function  /**< */,
                 uint32_t          do_reply  /**< */,
                 uint32_t          in_size  /**< */,
                 uint32_t          in_data_len  /**< */,
                 const uint8_t    *in_data  /**< */)
{
    static const xcb_protocol_request_t xcb_req = {
        /* count */ 4,
        /* ext */ &xcb_iegd_id,
        /* opcode */ XCB_IEGD_ESCAPE,
        /* isvoid */ 0
    };

    struct iovec xcb_parts[6];
    unsigned int xcb_ret;
    xcb_iegd_escape_request_t xcb_out;

    xcb_out.escape_function = escape_function;
    xcb_out.do_reply = do_reply;
    xcb_out.in_size = in_size;


    xcb_parts[2].iov_base = (char *) &xcb_out;
    xcb_parts[2].iov_len = sizeof(xcb_out);
    xcb_parts[3].iov_base = 0;
    xcb_parts[3].iov_len = -xcb_parts[2].iov_len & 3;
    xcb_parts[4].iov_base = (char *) in_data;
    xcb_parts[4].iov_len = in_data_len * sizeof(uint8_t);
    xcb_parts[5].iov_base = 0;
    xcb_parts[5].iov_len = -xcb_parts[4].iov_len & 3;
    xcb_ret = xcb_send_request(c, XCB_REQUEST_CHECKED, xcb_parts + 2, &xcb_req);
    return xcb_ret;
}



static int escape_xcb(
        void *connection,
        unsigned long function,
        unsigned long in_size, void *in_data,
        unsigned long out_size, void *out_data)
{
    unsigned int sequence;
    xcb_iegd_escape_reply_t *reply;
    xcb_generic_error_t *error;
    xcb_connection_t *xcb = (xcb_connection_t *)connection;
    int ret = -1;

    sequence = xcb_iegd_escape(xcb, function, 1, in_size, in_size,
            (unsigned char *)in_data);

    reply = xcb_iegd_escape_reply(xcb, sequence, &error);
    if (reply != NULL) {
        memcpy(out_data, (uint8_t *)(reply + 1), out_size);
        ret = reply->status;
        free(reply);
        return ret;
    }

    return ret; /* Error, failed to get a valid reply */
}

int do_wait_on_2dcopy_blt(unsigned long en_dis)
{

    int ret = 0;
    iegd_esc_set_wait_on_2dcopy_blt_t in_data;

    in_data.wait_on_2dcopy_blt_enable = (unsigned int)en_dis;
        ret = escape_xcb(c, INTEL_ESCAPE_SET_WAIT_ON_2DCOPY_BLT,
                        sizeof(iegd_esc_set_wait_on_2dcopy_blt_t),
                    (char *)&in_data, 0, NULL);
    return ret;
}
