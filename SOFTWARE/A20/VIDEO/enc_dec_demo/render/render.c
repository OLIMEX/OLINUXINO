/*******************************************************************************
--                                                                            --
--                    CedarX Multimedia Framework                             --
--                                                                            --
--          the Multimedia Framework for Linux/Android System                 --
--                                                                            --
--       This software is confidential and proprietary and may be used        --
--        only as expressly authorized by a licensing agreement from          --
--                         Softwinner Products.                               --
--                                                                            --
--                   (C) COPYRIGHT 2011 SOFTWINNER PRODUCTS                   --
--                            ALL RIGHTS RESERVED                             --
--                                                                            --
--                 The entire notice above must be reproduced                 --
--                  on all copies and should not be removed.                  --
--                                                                            --
*******************************************************************************/

#include "render.h"

VIDEO_RENDER_CONTEXT_TYPE *hnd_video_render = NULL;

unsigned long args[4];
int image_width,image_height;


static int config_de_parameter(unsigned int width, unsigned int height, __disp_pixel_fmt_t format) {
	__disp_layer_info_t tmpLayerAttr;
	int ret;

	image_width = width;
	image_height = height;

	args[0] = 0;
	args[1] = hnd_video_render->de_layer_hdl;
	args[2] = (unsigned long)(&tmpLayerAttr);
	ioctl(hnd_video_render->de_fd, DISP_CMD_LAYER_GET_PARA,args);

	//set color space
	if (image_height < 720) {
		tmpLayerAttr.fb.cs_mode = DISP_BT601;
	} else {
		tmpLayerAttr.fb.cs_mode = DISP_BT709;
	}
	//tmpFrmBuf.fmt.type = FB_TYPE_YUV;
	//printf("format== DISP_FORMAT_ARGB8888 %d\n", format == DISP_FORMAT_ARGB8888 ? 1 : 0);
	if(format == DISP_FORMAT_ARGB8888){
		tmpLayerAttr.fb.mode 	= DISP_MOD_NON_MB_PLANAR;
		tmpLayerAttr.fb.format 	= format; //
		tmpLayerAttr.fb.br_swap = 0;
		tmpLayerAttr.fb.cs_mode	= DISP_YCC;
		tmpLayerAttr.fb.seq 	= DISP_SEQ_P3210;
	}
	else
	{
		tmpLayerAttr.fb.mode 	= DISP_MOD_MB_UV_COMBINED;
		tmpLayerAttr.fb.format 	= format; //DISP_FORMAT_YUV420;
		tmpLayerAttr.fb.br_swap = 0;
		tmpLayerAttr.fb.seq 	= DISP_SEQ_UVUV;
	}

	tmpLayerAttr.fb.addr[0] = 0;
	tmpLayerAttr.fb.addr[1] = 0;

	tmpLayerAttr.fb.size.width 	= image_width;
	tmpLayerAttr.fb.size.height = image_height;

	//set video layer attribute
	tmpLayerAttr.mode = DISP_LAYER_WORK_MODE_SCALER;
	//tmpLayerAttr.ck_mode = 0xff;
	//tmpLayerAttr.ck_eanble = 0;
	tmpLayerAttr.alpha_en = 1;
	tmpLayerAttr.alpha_val = 0xff;
#ifdef CONFIG_DFBCEDAR
	tmpLayerAttr.pipe = 1;
#else
	tmpLayerAttr.pipe = 0;
#endif
	//tmpLayerAttr.prio = 0xff;
	//screen window information
	tmpLayerAttr.scn_win.x = 0;
	tmpLayerAttr.scn_win.y = 0;
	tmpLayerAttr.scn_win.width  = hnd_video_render->video_window.width;
	tmpLayerAttr.scn_win.height = hnd_video_render->video_window.height;
	tmpLayerAttr.prio           = 0xff;
	//frame buffer pst and size information
	tmpLayerAttr.src_win.x = 0;//tmpVFrmInf->dst_rect.uStartX;
	tmpLayerAttr.src_win.y = 0;//tmpVFrmInf->dst_rect.uStartY;
	tmpLayerAttr.src_win.width = image_width;//tmpVFrmInf->dst_rect.uWidth;
	tmpLayerAttr.src_win.height = image_height;//tmpVFrmInf->dst_rect.uHeight;
	printf("width %d, height %d\n",tmpLayerAttr.src_win.width ,tmpLayerAttr.src_win.height );
	hnd_video_render->src_frm_rect.x = tmpLayerAttr.src_win.x;
	hnd_video_render->src_frm_rect.y = tmpLayerAttr.src_win.y;
	hnd_video_render->src_frm_rect.width = tmpLayerAttr.src_win.width;
	hnd_video_render->src_frm_rect.height = tmpLayerAttr.src_win.height;

	
	//tmpLayerAttr.fb.b_trd_src		= 0;
	//tmpLayerAttr.b_trd_out			= 0;
	//tmpLayerAttr.fb.trd_mode 		=  (__disp_3d_src_mode_t)3;
	//tmpLayerAttr.out_trd_mode		= DISP_3D_OUT_MODE_FP;
	//tmpLayerAttr.b_from_screen 		= 0;
	//set channel
	//tmpLayerAttr.channel = DISP_LAYER_OUTPUT_CHN_DE_CH1;
	//FIOCTRL(hnd_video_render->de_fd, DISP_CMD_LAYER_SET_PARA, hnd_video_render->de_layer_hdl, &tmpLayerAttr);
	printf("set video layer param\n");
	args[0] = 0;
	args[1] = hnd_video_render->de_layer_hdl;
	args[2] = (unsigned long) (&tmpLayerAttr);
	args[3] = 0;
	ioctl(hnd_video_render->de_fd, DISP_CMD_LAYER_SET_PARA, args);
	
	args[0]							= 0;
	args[1]                 		= hnd_video_render->de_layer_hdl;
	args[2]                 		= 0;
	args[3]                 		= 0;
	ret                     		= ioctl(hnd_video_render->de_fd, DISP_CMD_LAYER_TOP,args);
	if(ret != 0)
	{		//open display layer failed, need send play end command, and exit
		printf("Open video display layer failed!\n");
		return NULL;
	}	
	return 0;
}

int render_init() 
{
	int ret;

	if (hnd_video_render != (VIDEO_RENDER_CONTEXT_TYPE *) 0) {
		printf("Cedar:vply: video play back has been opended already!\n");
		return -1;
	}

	hnd_video_render = (VIDEO_RENDER_CONTEXT_TYPE *) malloc(sizeof(VIDEO_RENDER_CONTEXT_TYPE ));
	if (hnd_video_render == (VIDEO_RENDER_CONTEXT_TYPE *) 0) {
		printf("Cedar:vply: malloc hnd_video_render error!\n");
		return -1;
	}
	memset(hnd_video_render, 0, sizeof(VIDEO_RENDER_CONTEXT_TYPE ));

	hnd_video_render->first_frame_flag = 1;

	hnd_video_render->de_fd = open("/dev/disp", O_RDWR);
	if (hnd_video_render->de_fd < 0) {
		printf("Open display driver failed!\n");
		return -1;
	}

	args[0] = 0;
	args[1] = DISP_LAYER_WORK_MODE_SCALER;
	args[2] = 0;
	args[3] = 0;
	hnd_video_render->de_layer_hdl = ioctl(hnd_video_render->de_fd, DISP_CMD_LAYER_REQUEST,	args);
	if (hnd_video_render->de_layer_hdl == 0) {
		printf("Open display layer failed! de fd:%d \n", hnd_video_render->de_fd);
		return -1;
	}


	//set video window information to default value, full screen
	hnd_video_render->video_window.x = 0;
	hnd_video_render->video_window.y = 0;
	args[0] = 0;
	args[1] = hnd_video_render->de_layer_hdl;
	args[2] = 0;
	args[3] = 0;
	hnd_video_render->video_window.width = ioctl(hnd_video_render->de_fd, DISP_CMD_SCN_GET_WIDTH, args);
	hnd_video_render->video_window.height = ioctl(hnd_video_render->de_fd,DISP_CMD_SCN_GET_HEIGHT, args);

	printf("de---w:%d,h:%d\n", hnd_video_render->video_window.width, hnd_video_render->video_window.height);
	return 0;
}

void render_exit(void) {
	if (hnd_video_render == NULL) {
		printf("video playback has been closed already!\n");
		return;
	}
	int			ret;

	//close displayer driver context
	if (hnd_video_render->de_fd) {
		args[0] = 0;
		args[1] = hnd_video_render->de_layer_hdl;
		args[2] = 0;
		args[3] = 0;
		ioctl(hnd_video_render->de_fd, DISP_CMD_VIDEO_STOP, args);

		args[0] = 0;
		args[1] = hnd_video_render->de_layer_hdl;
		args[2] = 0;
		args[3] = 0;
		ioctl(hnd_video_render->de_fd, DISP_CMD_LAYER_RELEASE, args);

		args[0]	= 0;
		ret = ioctl(hnd_video_render->de_fd, DISP_CMD_GET_OUTPUT_TYPE, args);

		if(ret == DISP_OUTPUT_TYPE_HDMI)
		{
			args[0] 					= 0;
			args[1] 					= 0;
			args[2] 					= 0;
			args[3] 					= 0;
			ioctl(hnd_video_render->de_fd,DISP_CMD_HDMI_OFF,(unsigned long)args);

			args[0] 					= 0;
			args[1] 					= hnd_video_render->hdmi_mode;
			args[2] 					= 0;
			args[3] 					= 0;
			ioctl(hnd_video_render->de_fd, DISP_CMD_HDMI_SET_MODE, args);

			args[0] 					= 0;
			args[1] 					= 0;
			args[2] 					= 0;
			args[3] 					= 0;
			ioctl(hnd_video_render->de_fd,DISP_CMD_HDMI_ON,(unsigned long)args);

		}
		close(hnd_video_render->de_fd);
		hnd_video_render->de_fd = 0;
	}

	if (hnd_video_render) {
		free(hnd_video_render);
		hnd_video_render = NULL;
	}
}

int render_render(void *frame_info, int frame_id)
{
	cedarv_picture_t *display_info = (cedarv_picture_t *) frame_info;
	__disp_video_fb_t tmpFrmBufAddr;
	 __disp_layer_info_t         layer_info;
	int ret;

	memset(&tmpFrmBufAddr, 0, sizeof(__disp_video_fb_t ));
	tmpFrmBufAddr.interlace 		= display_info->is_progressive? 0: 1;
	tmpFrmBufAddr.top_field_first 	= display_info->top_field_first;
	tmpFrmBufAddr.frame_rate 		= display_info->frame_rate;
	//tmpFrmBufAddr.first_frame = 0;//first_frame_flg;
	//first_frame_flg = 0;
	
	tmpFrmBufAddr.addr[0] = display_info->y;
	tmpFrmBufAddr.addr[1] = display_info->u;	

	tmpFrmBufAddr.id = frame_id;
	
	if (hnd_video_render->first_frame_flag == 1) 
	{
		__disp_layer_info_t         layer_info;
		__disp_pixel_fmt_t			pixel_format;
		
		pixel_format = display_info->pixel_format==CEDARV_PIXEL_FORMAT_AW_YUV422 ? DISP_FORMAT_YUV422 : DISP_FORMAT_YUV420;
		
		//printf("config de parameter!\n");
		if(display_info->display_width && display_info->display_height)
			config_de_parameter(display_info->display_width, display_info->display_height, pixel_format);
		else
			config_de_parameter(display_info->width, display_info->height,pixel_format);

		//set_display_mode(display_info, &layer_info);

		args[0] = 0;
	    args[1] = hnd_video_render->de_layer_hdl;
	    args[2] = (unsigned long) (&layer_info);
	    args[3] = 0;
	    ioctl(hnd_video_render->de_fd, DISP_CMD_LAYER_GET_PARA, args);
	
	    layer_info.fb.addr[0] 	= tmpFrmBufAddr.addr[0];
	    layer_info.fb.addr[1] 	= tmpFrmBufAddr.addr[1];
	    
	    args[0] 				= 0;
	    args[1] 				= hnd_video_render->de_layer_hdl;
	    args[2] 				= (unsigned long) (&layer_info);
	    args[3] 				= 0;
	    ret = ioctl(hnd_video_render->de_fd, DISP_CMD_LAYER_SET_PARA, args);
	    	
		args[0] = 0;
		args[1] = hnd_video_render->de_layer_hdl;
		args[2] = 0;
		args[3] = 0;
		ret = ioctl(hnd_video_render->de_fd, DISP_CMD_LAYER_OPEN, args);
		printf("layer open hdl:%d,ret:%d w:%d h:%d\n", hnd_video_render->de_layer_hdl, ret,display_info->width,display_info->height);
		if (ret != 0){
			//open display layer failed, need send play end command, and exit
			printf("Open video display layer failed!\n");
			return -1;
		}
		args[0] = 0;
		args[1] = hnd_video_render->de_layer_hdl;
		args[2] = 0;
		args[3] = 0;
		ret = ioctl(hnd_video_render->de_fd, DISP_CMD_VIDEO_START, args);
		if(ret != 0) {
			printf("Start video layer failed!\n");
			return -1;
		}
		
		hnd_video_render->layer_open_flag = 1;
		hnd_video_render->first_frame_flag = 0;	
	}
	else
	{
		args[0] = 0;
		args[1] = hnd_video_render->de_layer_hdl;
		args[2] = (unsigned long) (&tmpFrmBufAddr);
		args[3] = 0;
		ioctl(hnd_video_render->de_fd, DISP_CMD_VIDEO_SET_FB, args);
	}	

	return 0;
}

int render_get_disp_frame_id(void)
{
	return ioctl(hnd_video_render->de_fd, DISP_CMD_VIDEO_GET_FRAME_ID, args);
}

