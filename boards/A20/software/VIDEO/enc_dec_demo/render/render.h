#ifndef RENDER_H
#define RENDER_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

#include <libcedarv.h>
#include "drv_display.h"

#define HDMI_OUTPUT 1
#ifdef __cplusplus
extern "C" {
#endif
typedef struct VIDEO_RENDER_CONTEXT_TYPE
{
    int                 de_fd;
    int             	de_layer_hdl;
    int             	vid_wind_ratio;
    __disp_rect_t       video_window;
    __disp_rect_t       src_frm_rect;
    int            		pic_pix_ratio;

    unsigned int        layer_open_flag;
    unsigned int        first_frame_flag;

    unsigned int		hdmi_mode;
} VIDEO_RENDER_CONTEXT_TYPE;

extern int render_init();
extern void render_exit(void);
extern int render_render(void *frame_info, int frame_id);
extern int render_get_disp_frame_id(void);
#ifdef __cplusplus
}
#endif

#endif
