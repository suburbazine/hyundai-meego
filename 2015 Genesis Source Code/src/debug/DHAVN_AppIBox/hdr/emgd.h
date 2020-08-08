#ifndef EMGD_H
#define EMGD_H


#include <xcb/xcb.h>
#include <xcb/xcbext.h>
#include <xcb/dri2.h>

#include <emgd_escape.h>

typedef struct xcb_iegd_escape_reply_t {
    uint8_t  response_type; /**<  */
    uint8_t  pad0; /**<  */
    uint16_t sequence; /**<  */
    uint32_t length; /**<  */
    uint32_t out_length; /**<  */
    uint32_t status; /**<  */
    uint32_t pad1; /**<  */
    uint32_t pad2; /**<  */
    uint32_t pad3; /**<  */
    uint32_t pad4; /**<  */
} xcb_iegd_escape_reply_t;

typedef struct xcb_iegd_escape_request_t {
    uint8_t  major_opcode; /**<  */
    uint8_t  minor_opcode; /**<  */
    uint16_t length; /**<  */
    uint32_t escape_function; /**<  */
    uint32_t in_size; /**<  */
    uint32_t do_reply; /**<  */
} xcb_iegd_escape_request_t;


void emgd_init();

xcb_iegd_escape_reply_t *
xcb_iegd_escape_reply (xcb_connection_t          *c  /**< */,
                       unsigned int sequence  /**< */,
                       xcb_generic_error_t      **e  /**< */);

unsigned int
xcb_iegd_escape (xcb_connection_t *c  /**< */,
                 uint32_t          escape_function  /**< */,
                 uint32_t          do_reply  /**< */,
                 uint32_t          in_size  /**< */,
                 uint32_t          in_data_len  /**< */,
                 const uint8_t    *in_data  /**< */);

static int escape_xcb(
        void *connection,
        unsigned long function,
        unsigned long in_size, void *in_data,
        unsigned long out_size, void *out_data);

int do_wait_on_2dcopy_blt(unsigned long en_dis);


#endif // EMGD_H
