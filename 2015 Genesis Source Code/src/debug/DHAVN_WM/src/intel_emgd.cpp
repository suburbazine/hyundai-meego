#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xlibint.h>
#include <dlfcn.h>
#include <xcb/xcb.h>
#include <xcb/xcbext.h>
#include <xcb/dri2.h>
#include <syscall.h>

#include "iegd_escape.h"

#include "dhavnlog.h"

#define XCB_IEGD_ESCAPE     2

static xcb_extension_t xcb_iegd_id = { "Intel-EmbeddedGraphicsDriverExtension", 0 };

typedef struct xcb_iegd_escape_reply_t
{
    uint8_t  response_type;
    uint8_t  pad0;
    uint16_t sequence;
    uint32_t length;
    uint32_t out_length;
    uint32_t status;
    uint32_t pad1;
    uint32_t pad2;
    uint32_t pad3;
    uint32_t pad4;
} xcb_iegd_escape_reply_t;

typedef struct xcb_iegd_escape_request_t {
    uint8_t  major_opcode;
    uint8_t  minor_opcode;
    uint16_t length;
    uint32_t escape_function;
    uint32_t in_size;
    uint32_t do_reply;
} xcb_iegd_escape_request_t;


static xcb_connection_t * pXcbConnection;
static int escape_xcb(
    void *connection,
    unsigned long function,
    unsigned long in_size, void *in_data,
    unsigned long out_size, void *out_data);
static unsigned int xcb_iegd_escape (
    xcb_connection_t *c,
    uint32_t          escape_function,
    uint32_t          do_reply,
    uint32_t          in_size,
    uint32_t          in_data_len,
    const uint8_t    *in_data);
xcb_iegd_escape_reply_t *
xcb_iegd_escape_reply( xcb_connection_t *c,
                       unsigned int sequence,
                       xcb_generic_error_t **e );

void iegd_init(){
	int screenNumber = -1;
	pXcbConnection = xcb_connect(":0", &screenNumber);
}

void iegd_show_desktop(){
	if ( escape_xcb(pXcbConnection,
					INTEL_ESCAPE_SHOW_DESKTOP, 0, 0, 0, NULL) != 0){
		lprintf("Failed iegd_show_desktop");
	}
	else{
		lprintf("iegd_show_desktop");
	}
}

void iegd_freeze_framebuffer(int d, bool isFreeze){
	iegd_esc_display_num_t in_data;

	if ( d != PRIMARY_SCREEN && d != SECONDARY_SCREEN){
		lprintf("iegd_freeze_framebuffer display number wrong");
		return;
	}

	in_data.display_num = d;
	if ( escape_xcb(pXcbConnection, 
						(isFreeze? INTEL_ESCAPE_FREEZE_FB : INTEL_ESCAPE_UNFREEZE_FB),
						sizeof(iegd_esc_display_num_t),
						(void *) &in_data,
						0, NULL) != 0){
		lprintf("Failed iegd_freeze_framebuffer");
	}
	else{
		lprintf("iegd_freeze_framebuffer : %d", d);
	}
}

void iegd_clone_start(int target){
	iegd_esc_display_num_t in_data;

	if ( target != PRIMARY_SCREEN && target != SECONDARY_SCREEN){
		lprintf("iegd_clone_function display number wrong");
		return;
	}

	in_data.display_num = target;
	if ( escape_xcb(pXcbConnection, INTEL_ESCAPE_SET_CLONE_MODE,
					sizeof(iegd_esc_display_num_t),
					(void *) &in_data, 0, NULL) != 0){
		lprintf("Failed iegd_clone_function");
	}
	else{
		lprintf("iegd_clone_function : %d", target);
	}
}

void iegd_clone_stop(){
	iegd_esc_display_num_t in_data;
	in_data.display_num = 0;

	if ( escape_xcb( pXcbConnection, INTEL_ESCAPE_SET_DIH_MODE, 
					sizeof(iegd_esc_display_num_t),
					(char *)&in_data, 0, NULL) != 0){
		lprintf("Failed iegd_clone_stop");
	}
	else{
		lprintf("iegd_clone_stop");
	}
}

void iegd_video_control(bool isOverlay){
	iegd_esc_video_plane_ctrl_override_t in_data;
	in_data.override_func = (1 << OR_FUNC_OVERLAY_BIND);

	if ( isOverlay)
		in_data.override_func_in_overlay_bind.primary_ovl_bind = HW_OVERLAY;
	else
		in_data.override_func_in_overlay_bind.primary_ovl_bind = SPRITEC;

	if ( escape_xcb( pXcbConnection, INTEL_ESCAPE_VIDEO_PLANE_CTRL_OVERRIDE,
					sizeof( iegd_esc_video_plane_ctrl_override_t),
					(void *) &in_data, 0, NULL) != 0){
		lprintf("Failed iegd_video_control : %s", (isOverlay?"Overlay":"SpirteC"));
	}
	else{
		lprintf("iegd_video_control : %s", (isOverlay?"Overlay":"SpriteC"));
	}
}

void iegd_blend_fb(int d, int mask){
	iegd_esc_video_plane_ctrl_override_t in_data;
	in_data.override_func = (1 << OR_FUNC_FB_BLEND_OVL);

	in_data.override_func_in_fb_blend_ovl.fb_blend_target_mask = mask;	

	in_data.override_func_in_fb_blend_ovl.dpy_primary_secondary_on = d;

	if ( escape_xcb(pXcbConnection, INTEL_ESCAPE_VIDEO_PLANE_CTRL_OVERRIDE,
					sizeof( iegd_esc_video_plane_ctrl_override_t),
					(void *) &in_data, 0, NULL) != 0){
		lprintf("Failed iegd_blend_fb : %d, %d", d, mask);
	}
	else{
		lprintf("iegd_blend_fb : %d, %d", d, mask);
	}
}

// make overlay or spirtec b
void iegd_identify_overlay(int d, bool isOverlay){
	iegd_esc_video_plane_ctrl_override_t in_data;
	in_data.override_func = (1 << OR_FUNC_COLOR_CORRECT);

	in_data.override_func_in_color_correct.first_ovl_color_param.flag = GAMMA_FLAG | OVL_COLOR_FLAG;
	in_data.override_func_in_color_correct.second_ovl_color_param.flag = GAMMA_FLAG | OVL_COLOR_FLAG;

	if ( isOverlay ){
		in_data.override_func_in_color_correct.color_correct_target_mask = 0x01;
		lprintf("Overlay");
	}
	else{
		in_data.override_func_in_color_correct.color_correct_target_mask = 0x02;
		lprintf("SprinteC");
	}

	in_data.override_func_in_color_correct.first_ovl_color_param.gamma = 200;
	in_data.override_func_in_color_correct.second_ovl_color_param.gamma = 200;
	
	if ( escape_xcb(pXcbConnection, INTEL_ESCAPE_VIDEO_PLANE_CTRL_OVERRIDE,
					sizeof( iegd_esc_video_plane_ctrl_override_t),
					(void *) &in_data, 0, NULL) != 0){
		lprintf("Failed iegd_identify_overlay : %d, %s", d, (isOverlay?"Overlay":"SpirteC"));
	}
	else{
		lprintf("iegd_identify_overlay : %d, %s", d, (isOverlay?"Overlay":"SpriteC"));
	}
}

static int escape_xcb(
    void *connection,
    unsigned long function,
    unsigned long in_size, void *in_data,
    unsigned long out_size, void *out_data)
{
    unsigned int sequence;
    xcb_iegd_escape_reply_t *reply = NULL;
    xcb_generic_error_t *error;
    xcb_connection_t *xcb = (xcb_connection_t *)connection;
    int ret = -1;

    sequence = xcb_iegd_escape( xcb, function, 1, in_size, in_size,
            (unsigned char *)in_data );

    reply = xcb_iegd_escape_reply( xcb, sequence, &error );
    if ( reply != NULL )
    {
        memcpy( out_data, (uint8_t *)( reply + 1 ), out_size );
        ret = reply->status;
        free( reply );
        return ret;
    }
    else
    {
        lprintf( "Failed to get a reply for escape function." );
    }

    return ret;
}


static unsigned int xcb_iegd_escape (
    xcb_connection_t *c,
    uint32_t          escape_function,
    uint32_t          do_reply,
    uint32_t          in_size,
    uint32_t          in_data_len,
    const uint8_t    *in_data)
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
    xcb_ret = xcb_send_request( c, XCB_REQUEST_CHECKED, xcb_parts + 2, &xcb_req );

    return xcb_ret;
}

// ----------------------------------------------------------------------------
// Intel EMGD driver function - taken from Intel reference implementation
// ----------------------------------------------------------------------------
xcb_iegd_escape_reply_t *
xcb_iegd_escape_reply( xcb_connection_t *c,
                       unsigned int sequence,
                       xcb_generic_error_t **e )
{
    xcb_iegd_escape_reply_t *ret;
    ret = (xcb_iegd_escape_reply_t *) xcb_wait_for_reply( c, sequence, e );

    return ret;
}
