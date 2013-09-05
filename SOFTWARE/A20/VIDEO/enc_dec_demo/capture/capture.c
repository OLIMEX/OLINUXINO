#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <getopt.h>
#include <fcntl.h>             
#include <unistd.h>
#include <errno.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include <asm/types.h>       
#include <linux/videodev.h>
#include <linux/videodev2.h>
#include <time.h>
#include <linux/fb.h>

#ifdef USE_SUNXI_MEM_ALLOCATOR
#include "encoder_type.h"
#else
#include "type.h"
#endif

#include "drv_display.h"
#include "capture.h"

#define LOG_NDEBUG 0

#ifndef __OS_LINUX
#include <utils/Log.h>
#endif

#define DEV_NAME	"/dev/video0"		

#define CAMERA_DRIV_UPDATE

typedef struct buffer 
{
	void * start;
	size_t length;
}buffer;

int disphd;
unsigned int hlay;
int sel = 0;//which screen 0/1
__disp_layer_info_t layer_para;
__disp_video_fb_t video_fb;
__u32 arg[4];

static int 			fd 		= NULL;
struct buffer 		*buffers	= NULL;
static unsigned int	n_buffers	= 0;

int mLayer = 0;
int	mDispHandle = 0;
int mScreenWidth = 0;
int mScreenHeight = 0;
int mFirstFrame = 1;
int mFrameId = 0;

#define CLEAR(x) memset (&(x), 0, sizeof (x))

//#define DISP_PREVIEW
#ifdef DISP_PREVIEW

int disp_int(int w, int h)
{
	/*display start*/ 
    unsigned int preview_left, preview_top, preview_h, preview_w;
	int error;

	preview_left = 80;
	preview_top = 0;

//	preview_h = h;
//	preview_w = w;

	printf("w: %d, h: %d, preview_left: %d, preview_top: %d, preview_h: %d, preview_w : %d\n", 
		w, h, preview_left, preview_top, preview_h, preview_w);
	
	if((disphd = open("/dev/disp",O_RDWR)) <= 0)
	{
		printf("open file /dev/disp fail. \n");
		return 0;
	}

    arg[0] = 0;
    ioctl(disphd, DISP_CMD_LCD_ON, (void*)arg);

    // request a video layer

	memset(&arg, 0, sizeof(arg));
    arg[0] = 0;
    arg[1] = DISP_LAYER_WORK_MODE_SCALER;
    hlay = ioctl(disphd, DISP_CMD_LAYER_REQUEST, (void*)arg);

    if(hlay == 0)
    {
        printf("request layer0 fail\n");
        return 0;
    }
	printf("video layer hdl:%d\n", hlay);

	preview_w	= ioctl(disphd, DISP_CMD_SCN_GET_WIDTH, arg);
    preview_h 	= ioctl(disphd, DISP_CMD_SCN_GET_HEIGHT, arg);

	printf("Get screen,preview_w: %d, preview_h: %d", preview_w, preview_h);

	// set layer information
	memset(&arg, 0, sizeof(arg));
	arg[0] = 0;
	arg[1] = hlay;
	arg[2] = (unsigned long)(&layer_para);
	arg[3] = 0;

	memset(&layer_para, 0, sizeof(layer_para));
	error = ioctl(disphd, DISP_CMD_LAYER_GET_PARA, &arg);
	
	if (error != 0)
	{
		return 0;
	}

    layer_para.mode = DISP_LAYER_WORK_MODE_SCALER; 
    layer_para.pipe = 0; 
    layer_para.fb.addr[0]       = 0;//your Y address,modify this 
    layer_para.fb.addr[1]       = 0; //your C address,modify this 
    layer_para.fb.addr[2]       = 0; 
    layer_para.fb.size.width    = w;
    layer_para.fb.size.height   = h;
    layer_para.fb.mode          = DISP_MOD_NON_MB_UV_COMBINED;	//DISP_MOD_NON_MB_PLANAR;		 
    layer_para.fb.format        = DISP_FORMAT_YUV420;			//DISP_FORMAT_YUV422; 		
    layer_para.fb.br_swap       = 0;
    layer_para.fb.seq           = DISP_SEQ_UVUV;
    layer_para.ck_enable        = 0;
    layer_para.alpha_en         = 1; 
    layer_para.alpha_val        = 0xff;
    layer_para.src_win.x        = 0;
    layer_para.src_win.y        = 0;
    layer_para.src_win.width    = w;
    layer_para.src_win.height   = h;
    layer_para.scn_win.x        = 0;
    layer_para.scn_win.y        = 0;
    layer_para.scn_win.width    = preview_w;
    layer_para.scn_win.height   = preview_h;
	arg[0] = sel;
    arg[1] = hlay;
    arg[2] = (__u32)&layer_para;
//    ioctl(disphd,DISP_CMD_LAYER_SET_PARA,(void*)arg);
	error = ioctl(disphd,DISP_CMD_LAYER_SET_PARA, &arg);

	if (error != 0)
	{
		printf("get layer information failed");
		return 0;
	}


	//set VideoLayer Top
    arg[0] = sel;
    arg[1] = hlay;
    error = ioctl(disphd,DISP_CMD_LAYER_TOP,(void*)arg);
	if (error != 0)
	{
		printf("set layer top failed");
		return 0;
	}

    arg[0] = sel;
    arg[1] = hlay;
    ioctl(disphd,DISP_CMD_LAYER_OPEN,(void*)arg);

	printf("MODE: %d, format: %d\n", layer_para.fb.mode, layer_para.fb.format);

#if 0
	int fb_fd;
	unsigned long fb_layer;
	void *addr = NULL;
	fb_fd = open("/dev/fb0", O_RDWR);

	if (ioctl(fb_fd, FBIOGET_LAYER_HDL, &fb_layer) == -1) {
		printf("get fb layer handel\n");	
	}

#if 0
	addr = malloc(800*480*3);
	memset(addr, 0xff, 800*480*3);
	write(fb_fd, addr, 800*480*3);
	//memset(addr, 0x12, 800*480*3);
#else
	addr = malloc(w*h*3);
	memset(addr, 0xff, w*h*3);
	write(fb_fd, addr, w*h*3);
	//memset(addr, 0x12, 800*480*3);
#endif 

	printf("fb_layer hdl: %ld\n", fb_layer);
	close(fb_fd);

	arg[0] = 0;
	arg[1] = fb_layer;
	ioctl(disphd, DISP_CMD_LAYER_BOTTOM, (void *)arg);
#endif

	return 0;
}

void disp_start(void)
{
	arg[0] = sel;
    arg[1] = hlay;
    ioctl(disphd, DISP_CMD_VIDEO_START,  (void*)arg);
}

void disp_stop(void)
{
	arg[0] = sel;
    arg[1] = hlay;
    ioctl(disphd, DISP_CMD_VIDEO_STOP,  (void*)arg);
}

int disp_set_addr(int w,int h,int *addr)
{
#if 0	
	layer_para.fb.addr[0]       = *addr;//your Y address,modify this 
    layer_para.fb.addr[1]       = *addr+w*h; //your C address,modify this 
    layer_para.fb.addr[2]       = *addr+w*h*3/2; 
    
    arg[0] = sel;
    arg[1] = hlay;
    arg[2] = (__u32)&layer_para;
    ioctl(disphd,DISP_CMD_LAYER_SET_PARA,(void*)arg);
#endif
	__disp_video_fb_t  fb_addr;	
	memset(&fb_addr, 0, sizeof(__disp_video_fb_t));

	fb_addr.interlace       = 0;
	fb_addr.top_field_first = 0;
	fb_addr.frame_rate      = 25;
	fb_addr.addr[0] = *addr;
	fb_addr.addr[1] = *addr + w * h;
	fb_addr.addr[2] = *addr + w*h*3/2;
	fb_addr.id = 0;  //TODO

    arg[0] = sel;
    arg[1] = hlay;
    arg[2] = (__u32)&fb_addr;
    ioctl(disphd, DISP_CMD_VIDEO_SET_FB, (void*)arg);

	return 0;
}

void disp_quit()
{
	__u32 arg[4];
	arg[0] = 0;
    ioctl(disphd, DISP_CMD_LCD_OFF, (void*)arg);

    arg[0] = sel;
    arg[1] = hlay;
    ioctl(disphd, DISP_CMD_LAYER_CLOSE,  (void*)arg);

    arg[0] = sel;
    arg[1] = hlay;
    ioctl(disphd, DISP_CMD_LAYER_RELEASE,  (void*)arg);
    close (disphd);
}

#endif // DISP_PREVIEW


int dispInit()
{	
	int error = 0;

	mFirstFrame = 1;
	
	// open disp driver
	mDispHandle = open("/dev/disp", O_RDWR);
	if (mDispHandle <= 0)
	{
		printf("open display driver failed, %s", strerror(errno));
		goto ERROR;
	}
	printf("open display driver ok");

	// request a video layer
	unsigned long args[4];
	memset(&args, 0, sizeof(args));
	args[0] = 0;
    args[1] = DISP_LAYER_WORK_MODE_SCALER;
	mLayer = ioctl(mDispHandle, DISP_CMD_LAYER_REQUEST, (void*)args);
	if (mLayer == 0)
	{
		printf("request layer failed, %s", strerror(errno));
		goto ERROR;
	}
	printf("request layer ok");

    mScreenWidth	= ioctl(mDispHandle, DISP_CMD_SCN_GET_WIDTH, args);
    mScreenHeight 	= ioctl(mDispHandle, DISP_CMD_SCN_GET_HEIGHT, args);
	printf("get screen width: %d, height: %d", mScreenWidth, mScreenHeight);

	// set layer information
	__disp_layer_info_t layer_para;
	memset(&args, 0, sizeof(args));
	args[0] = 0;
	args[1] = mLayer;
	args[2] = (unsigned long)(&layer_para);
	args[3] = 0;

	memset(&layer_para, 0, sizeof(layer_para));
	error = ioctl(mDispHandle, DISP_CMD_LAYER_GET_PARA, &args);
	if (error != 0)
	{
		printf("get layer information failed, %s", strerror(errno));
		goto ERROR;
	}
	printf("get layer information ok");

	layer_para.mode				= DISP_LAYER_WORK_MODE_SCALER; 
    layer_para.pipe				= 0; 
	layer_para.fb.addr[0]       = 0;
    layer_para.fb.addr[1]       = 0;
    layer_para.fb.addr[2]       = 0; 
    layer_para.fb.size.width    = mVideoWidth;
    layer_para.fb.size.height   = mVideoHeight;
    layer_para.fb.mode          = DISP_MOD_NON_MB_UV_COMBINED;///DISP_MOD_INTERLEAVED;//DISP_MOD_NON_MB_PLANAR;//DISP_MOD_NON_MB_PLANAR;//DISP_MOD_NON_MB_UV_COMBINED;
    layer_para.fb.format        = DISP_FORMAT_YUV420;//DISP_FORMAT_YUV420;//DISP_FORMAT_YUV422;//DISP_FORMAT_YUV420;
    layer_para.fb.br_swap       = 0;
    layer_para.fb.seq           = DISP_SEQ_UVUV;//DISP_SEQ_UVUV;//DISP_SEQ_YUYV;//DISP_SEQ_YVYU;//DISP_SEQ_UYVY;//DISP_SEQ_VYUY//DISP_SEQ_UVUV
    layer_para.ck_enable        = 0;
    layer_para.alpha_en         = 1; 
    layer_para.alpha_val        = 0xff;
    layer_para.src_win.x        = 0;
    layer_para.src_win.y        = 0;
    layer_para.src_win.width    = mVideoWidth;
    layer_para.src_win.height   = mVideoHeight;
    layer_para.scn_win.x        = 0;
    layer_para.scn_win.y        = 0;
    layer_para.scn_win.width    = mScreenWidth;
    layer_para.scn_win.height   = mScreenHeight;

	printf("video : %dx%d, scn: %dx%d", mVideoWidth, mVideoHeight, mScreenWidth, mScreenHeight);

	error = ioctl(mDispHandle, DISP_CMD_LAYER_SET_PARA, &args);
	if (error != 0)
	{
		printf("set layer information failed, %s", strerror(errno));
		goto ERROR;
	}

	printf("set video layer top");
	args[0] = 0;
	args[1] = mLayer;
	error = ioctl(mDispHandle, DISP_CMD_LAYER_TOP, (void *)args);
	if (error != 0)
	{
		printf("set layer top failed, %s", strerror(errno));
		goto ERROR;
	}
	
ERROR:
	return error;
}

void dispDeInit()
{	
	unsigned int args[4];

	if (mLayer != 0)
	{
		args[0] = 0;
		args[1] = mLayer;
		args[2] = 0;
		args[3] = 0;
		ioctl(mDispHandle, DISP_CMD_VIDEO_STOP, (void*)args);
		
		printf("close video layer");
		ioctl(mDispHandle, DISP_CMD_LAYER_CLOSE, (void*)args);
			
		printf("release video layer");
		ioctl(mDispHandle, DISP_CMD_LAYER_RELEASE, (void*)args);
	}

	if (mDispHandle != 0)
	{
		close (mDispHandle);
		mDispHandle = 0;
	}
	
	mFirstFrame = 0;
}

int setLayerAddr(struct v4l2_buffer * buf)
{
	unsigned int args[4];
	__disp_video_fb_t  fb_addr; 
	memset(&fb_addr, 0, sizeof(__disp_video_fb_t));

	if (mFirstFrame)
	{
		printf("first frame");
		
		args[0] = 0;
		args[1] = (unsigned long) mLayer;
		args[2] = 0;
		args[3] = 0;
		ioctl(mDispHandle, DISP_CMD_VIDEO_START, args);

		fb_addr.interlace		= 0;
		fb_addr.top_field_first = 0;
		fb_addr.frame_rate		= 25;
		fb_addr.addr[0] = buf->m.offset;
		fb_addr.addr[1] = buf->m.offset + mVideoWidth * mVideoHeight;
		fb_addr.addr[2] = 0;
		fb_addr.id = mFrameId++;

		args[0] = 0;
		args[1] = mLayer;
		args[2] = (__u32)&fb_addr;
		ioctl(mDispHandle, DISP_CMD_VIDEO_SET_FB, (void*)args);
		
GET_FRAME_ID:
		args[0] = 0;
		args[1] = mLayer;
		int tmpId = ioctl(mDispHandle, DISP_CMD_VIDEO_GET_FRAME_ID, (void*)args);
		if (tmpId != fb_addr.id)
		{
			printf("WAIT...., tmpId: %d, fb_addr.id: %d", tmpId, fb_addr.id);
			usleep(1000);
			goto GET_FRAME_ID;
		}

		args[0] = 0;
		args[1] = (unsigned long) mLayer;
		args[2] = 0;
		args[3] = 0;
		ioctl(mDispHandle, DISP_CMD_LAYER_OPEN, args);

		mFirstFrame = 0;
	}
	else
	{		
		fb_addr.interlace		= 0;
		fb_addr.top_field_first = 0;
		fb_addr.frame_rate		= 25;
		fb_addr.addr[0] = buf->m.offset;
		fb_addr.addr[1] = buf->m.offset + mVideoWidth * mVideoHeight;
		fb_addr.addr[2] = 0;
		fb_addr.id = mFrameId++;

		args[0] = 0;
		args[1] = mLayer;
		args[2] = (__u32)&fb_addr;
		ioctl(mDispHandle, DISP_CMD_VIDEO_SET_FB, (void*)args);
	}

	return 0;
}

int InitCapture()
{
	struct v4l2_capability cap; 
	struct v4l2_format fmt;
	unsigned int i;

	fd = open (DEV_NAME, O_RDWR /* required */ | O_NONBLOCK, 0);
	if (fd == 0)
	{
		printf("open %s failed\n", DEV_NAME);
		return -1;
	}

#ifdef CAMERA_DRIV_UPDATE

	struct v4l2_input inp;
	inp.index = 0;
	if (-1 == ioctl (fd, VIDIOC_S_INPUT, &inp))
	{
		printf("VIDIOC_S_INPUT error!\n");
		return -1;
	}
	
#endif	

	ioctl (fd, VIDIOC_QUERYCAP, &cap);

 
	CLEAR (fmt);
	fmt.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	fmt.fmt.pix.width       = mVideoWidth; //3 
	fmt.fmt.pix.height      = mVideoHeight;  
	fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_NV12;	//V4L2_PIX_FMT_YUV422P;	//
	fmt.fmt.pix.field       = V4L2_FIELD_NONE;
	ioctl (fd, VIDIOC_S_FMT, &fmt); 	


	struct v4l2_requestbuffers req;
	CLEAR (req);
	req.count               = 4;
	req.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	req.memory              = V4L2_MEMORY_MMAP;

	ioctl (fd, VIDIOC_REQBUFS, &req); 

	buffers = calloc (req.count, sizeof(struct buffer));

	for (n_buffers = 0; n_buffers < req.count; ++n_buffers) 
	{
	   struct v4l2_buffer buf;   
	   CLEAR (buf);
	   buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	   buf.memory      = V4L2_MEMORY_MMAP;
	   buf.index       = n_buffers;

	   if (-1 == ioctl (fd, VIDIOC_QUERYBUF, &buf)) 
			printf ("VIDIOC_QUERYBUF error\n");

	   buffers[n_buffers].length = buf.length;
	   buffers[n_buffers].start  = mmap (NULL /* start anywhere */,    
								         buf.length,
								         PROT_READ | PROT_WRITE /* required */,
								         MAP_SHARED /* recommended */,
								         fd, buf.m.offset);

	   if (MAP_FAILED == buffers[n_buffers].start)
			printf ("mmap failed\n");
	}

	for (i = 0; i < n_buffers; ++i) 
	{
	   struct v4l2_buffer buf;
	   CLEAR (buf);

	   buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	   buf.memory      = V4L2_MEMORY_MMAP;
	   buf.index       = i;

	   if (-1 == ioctl (fd, VIDIOC_QBUF, &buf))
		printf ("VIDIOC_QBUF failed\n");
	}

	//dispInit();
	
#ifdef DISP_PREVIEW
	disp_int(640, 480);
	disp_start();
#endif // DISP_PREVIEW

	return 0;
}

void DeInitCapture()
{
	int i;
	enum v4l2_buf_type type;

	printf("DeInitCapture");

	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if (-1 == ioctl (fd, VIDIOC_STREAMOFF, &type)) 	
		printf ("VIDIOC_STREAMOFF failed\n");	
	else		
		printf ("VIDIOC_STREAMOFF ok\n");
	
	for (i = 0; i < n_buffers; ++i) {
		if (-1 == munmap (buffers[i].start, buffers[i].length)) {
			printf ("munmap error\n");
		}
	}

	//dispDeInit();
#ifdef DISP_PREVIEW
	disp_stop();
	disp_quit();
#endif

	if (fd != 0)
	{
		close (fd);
		fd = 0;
	}

	printf("V4L2 close****************************\n");
}

int StartStreaming()
{
    int ret = -1; 
	enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE; 

	printf("V4L2Camera::v4l2StartStreaming\n");
  
    ret = ioctl (fd, VIDIOC_STREAMON, &type); 
    if (ret < 0) 
	{ 
        printf("StartStreaming: Unable to start capture: %s\n", strerror(errno)); 
        return ret; 
    } 

	printf("V4L2Camera::v4l2StartStreaming OK\n");
    return 0; 
}

void ReleaseFrame(int buf_id)
{	
	struct v4l2_buffer v4l2_buf;
	int ret;
	static int index = -1;

	memset(&v4l2_buf, 0, sizeof(struct v4l2_buffer));
	v4l2_buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	v4l2_buf.memory = V4L2_MEMORY_MMAP;
	v4l2_buf.index = buf_id;		// buffer index

	if (index == v4l2_buf.index)
	{
		printf("v4l2 should not release the same buffer twice continuous: index : %d\n", index);
		// return ;
	}
	index = v4l2_buf.index;
	
	ret = ioctl(fd, VIDIOC_QBUF, &v4l2_buf);
	if (ret < 0) {
		printf("VIDIOC_QBUF failed, id: %d\n", v4l2_buf.index);
		return ;
	}
	
}

int WaitCamerReady()
{
	fd_set fds;		
	struct timeval tv;
	int r;

	FD_ZERO(&fds);
	FD_SET(fd, &fds);		
	
	/* Timeout */
	tv.tv_sec  = 2;
	tv.tv_usec = 0;
	
	r = select(fd + 1, &fds, NULL, NULL, &tv);
	if (r == -1) 
	{
		printf("select err\n");
		return -1;
	} 
	else if (r == 0) 
	{
		printf("select timeout\n");
		return -1;
	}

	return 0;
}


int GetPreviewFrame(V4L2BUF_t *pBuf)	// DQ buffer for preview or encoder
{
	int ret = -1; 
	struct v4l2_buffer buf;

	ret = WaitCamerReady();
	if (ret != 0)
	{
		printf("wait time out\n");
		return __LINE__;
	}

	memset(&buf, 0, sizeof(struct v4l2_buffer));
    buf.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE; 
    buf.memory = V4L2_MEMORY_MMAP; 
 
    /* DQ */ 
    ret = ioctl(fd, VIDIOC_DQBUF, &buf); 
    if (ret < 0) 
	{ 
        printf("GetPreviewFrame: VIDIOC_DQBUF Failed\n"); 
        return __LINE__; 
    }
	
	pBuf->addrPhyY	= buf.m.offset;
	pBuf->index 	= buf.index;
	pBuf->timeStamp = (int64_t)((int64_t)buf.timestamp.tv_usec + (((int64_t)buf.timestamp.tv_sec) * 1000000));

	//printf("VIDIOC_DQBUF id: %d\n", buf.index);

	//setLayerAddr(&buf);
	
#ifdef DISP_PREVIEW
	disp_set_addr(640, 480, &buf.m.offset);
#endif

	return 0;
}



