
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>


#include "encoder_type.h"

#include "H264encLibApi.h"
#include "capture.h"
#include "render.h"
#include "decode_api.h"

#define ENC_FIFO_LEVEL 5
#define MAX_DISP_ELEMENTS   32

typedef struct cache_data
{
	int size;
	int write_offset;
	int part_num;
	int can_save_data;
	char *data;
	pthread_mutex_t mut_save_bs;
}cache_data;

typedef struct camera_buf_info
{
	VEnc_FrmBuf_Info		buf_info;
	int 					id;
}camera_buf_info;

typedef struct bufMrgQ_t
{
	camera_buf_info			omx_bufhead[ENC_FIFO_LEVEL];
	int						write_id;
	int						read_id;
	int						buf_unused;
}bufMrgQ_t;

int mVideoWidth = 1280;
int mVideoHeight = 720;
int display_time = 100; //ms




bufMrgQ_t	gBufMrgQ;
VENC_DEVICE *g_pCedarV = NULL;
cedarv_decoder_t* hcedarv = NULL;


int g_cur_id = -1;
unsigned long long lastTime = 0 ; 


pthread_t thread_camera_id = 0;
pthread_t thread_enc_id = 0;
pthread_t thread_dec_id = 0;
pthread_t thread_save_bs_id = 0;


pthread_mutex_t mut_cam_buf;
pthread_mutex_t mut_ve;

cache_data *save_bit_stream = NULL;



FILE * pEncFile = NULL;

#ifdef __OS_LINUX

char saveFile[128] = "/mnt/h264.dat";

#else
char saveFile[128] = "/mnt/sdcard/h264.dat";
#endif

#ifdef USE_SUNXI_MEM_ALLOCATOR
extern int sunxi_alloc_open();
#endif


static __s32 WaitFinishCB(__s32 uParam1, void *pMsg)
{
	return cedarv_wait_ve_ready();
}

static __s32 GetFrmBufCB(__s32 uParam1,  void *pFrmBufInfo)
{
	int write_id;
	int read_id;
	int buf_unused;
	int ret = -1;
	V4L2BUF_t Buf;
	VEnc_FrmBuf_Info encBuf;

	memset((void*)&encBuf, 0, sizeof(VEnc_FrmBuf_Info));

	write_id 	= gBufMrgQ.write_id;
	read_id 	= gBufMrgQ.read_id;
	buf_unused	= gBufMrgQ.buf_unused;

	if(buf_unused == ENC_FIFO_LEVEL)
	{
		//printf("GetFrmBufCB: no valid fifo\n");
		return -1;
	}

	pthread_mutex_lock(&mut_cam_buf);
	
	encBuf.addrY = gBufMrgQ.omx_bufhead[read_id].buf_info.addrY;
	encBuf.addrCb = gBufMrgQ.omx_bufhead[read_id].buf_info.addrCb;
	encBuf.pts_valid = 1;
	encBuf.pts = (s64)gBufMrgQ.omx_bufhead[read_id].buf_info.pts;

	encBuf.color_fmt = PIXEL_YUV420;
	encBuf.color_space = BT601;

	g_cur_id = gBufMrgQ.omx_bufhead[read_id].id;

	//printf("g_cur_id, GetFrmBufCB: %d\n", g_cur_id);
	gBufMrgQ.buf_unused++;
	gBufMrgQ.read_id++;
	gBufMrgQ.read_id %= ENC_FIFO_LEVEL;
	pthread_mutex_unlock(&mut_cam_buf);

	memcpy(pFrmBufInfo, (void*)&encBuf, sizeof(VEnc_FrmBuf_Info));
	
	return 0;
}



cache_data *save_bitstream_int(int size)
{
	cache_data *save_bit_stream = (cache_data *)malloc(sizeof(cache_data));
	memset(save_bit_stream, 0, sizeof(cache_data));
	save_bit_stream->size = size;
	save_bit_stream->part_num = 0;
	save_bit_stream->can_save_data = 0;
	save_bit_stream->write_offset = 0;
	save_bit_stream->data = (char *)malloc(save_bit_stream->size);
	
	pthread_mutex_init(&save_bit_stream->mut_save_bs,NULL);
	if(save_bit_stream->data == NULL)
	{
		printf("malloc fail\n", saveFile);
		return NULL;
	}
	return save_bit_stream;
}


int save_bitstream_exit(cache_data *save_bit_stream)
{
	if(save_bit_stream)
	{
		if(save_bit_stream->data)
		{
			free(save_bit_stream->data);
			save_bit_stream->data = NULL;
		}

		pthread_mutex_destroy(&save_bit_stream->mut_save_bs);
		free(save_bit_stream);
		save_bit_stream = NULL;
	
	}
	
	return 0;
}



int update_bitstream_to_cache(cache_data *save_bit_stream, char *output_data, int data_size)
{
	int left_size;
	int offset;
	int last_write_offset;

	pthread_mutex_lock(&save_bit_stream->mut_save_bs);

	if(save_bit_stream->size < data_size)
	{
		pthread_mutex_unlock(&save_bit_stream->mut_save_bs);
		return -1;
	}

	last_write_offset = save_bit_stream->write_offset;
	if((save_bit_stream->write_offset + data_size) >= save_bit_stream->size)
	{
		left_size = save_bit_stream->write_offset + data_size - save_bit_stream->size;
		offset = data_size - left_size;
		
		memcpy(save_bit_stream->data + save_bit_stream->write_offset, output_data, data_size - left_size);

		if(left_size > 0)
		{
			memcpy(save_bit_stream->data, output_data + offset, left_size);
		}
		
		save_bit_stream->write_offset = left_size;
	}
	else
	{
		memcpy(save_bit_stream->data + save_bit_stream->write_offset, output_data, data_size);
		save_bit_stream->write_offset = save_bit_stream->write_offset + data_size;
	}

    if(last_write_offset > save_bit_stream->write_offset)
	{
		save_bit_stream->part_num = 0;
		save_bit_stream->can_save_data = 1;
	}

	else
	{
		if(save_bit_stream->write_offset >= save_bit_stream->size/2 && last_write_offset < save_bit_stream->size/2)
		{
			save_bit_stream->part_num = 1;
			save_bit_stream->can_save_data = 1;
		}
	}

	pthread_mutex_unlock(&save_bit_stream->mut_save_bs);
	
	return 0;
}

int get_bitstream_for_save(cache_data *save_bit_stream, char * tem_data, int *datasize)
{
	int tmp_size;
	
	pthread_mutex_lock(&save_bit_stream->mut_save_bs);
	if(save_bit_stream->can_save_data == 1)
	{
		tmp_size = save_bit_stream->size/2;
		*datasize = tem_data;
		if(save_bit_stream->part_num ==1)
		{
			memcpy(tem_data, save_bit_stream->data, tmp_size);
		}
		else
		{
			memcpy(tem_data, save_bit_stream->data + tmp_size, save_bit_stream->size - tmp_size);
		}

		save_bit_stream->can_save_data = 0;
	}
	else
	{
		*datasize = 0;
	}
	
	pthread_mutex_unlock(&save_bit_stream->mut_save_bs);
	return 0;
}

int save_left_bitstream(cache_data *save_bit_stream, char * tem_data, int *datasize)
{
	int offset;
	pthread_mutex_lock(&save_bit_stream->mut_save_bs);

	if(save_bit_stream->can_save_data == 0)
	{
		if(save_bit_stream->part_num == 1)
		{
			offset = save_bit_stream->size/2;

			if(save_bit_stream->write_offset > offset)
			{
				memcpy(tem_data, save_bit_stream->data + offset, save_bit_stream->write_offset - offset);
			}

			*datasize = save_bit_stream->write_offset - offset;
		}
		else
		{
			if(save_bit_stream->write_offset > 0)
			{
				memcpy(tem_data, save_bit_stream->data, save_bit_stream->write_offset);
			}

			*datasize = save_bit_stream->write_offset;
		}
	}

	else
	{
		printf("save left bitstream error\n");
	}

	
	pthread_mutex_unlock(&save_bit_stream->mut_save_bs);
	return 0;
}







VENC_DEVICE * CedarvEncInit(__u32 width, __u32 height, __u32 avg_bit_rate, __s32 (*GetFrmBufCB)(__s32 uParam1,  void *pFrmBufInfo))
{
	int ret = -1;

	VENC_DEVICE *pCedarV = NULL;
	
#ifdef USE_SUNXI_MEM_ALLOCATOR
	sunxi_alloc_open();
#endif
	pCedarV = H264EncInit(&ret);
	if (ret < 0)
	{
		printf("H264EncInit failed\n");
	}

	__video_encode_format_t enc_fmt;
	enc_fmt.src_width = width;
	enc_fmt.src_height = height;
	enc_fmt.width = width;
	enc_fmt.height = height;
	enc_fmt.frame_rate = 30 * 1000;
	enc_fmt.color_format = PIXEL_YUV420;
	enc_fmt.color_space = BT601;
	enc_fmt.qp_max = 40;
	enc_fmt.qp_min = 20;
	enc_fmt.avg_bit_rate = avg_bit_rate;
	enc_fmt.maxKeyInterval = 8;
	
    //enc_fmt.profileIdc = 77; /* main profile */

	enc_fmt.profileIdc = 66; /* baseline profile */
	enc_fmt.levelIdc = 31;

	pCedarV->IoCtrl(pCedarV, VENC_SET_ENC_INFO_CMD, &enc_fmt);

	ret = pCedarV->open(pCedarV);
	if (ret < 0)
	{
		printf("open H264Enc failed\n");
	}
	
	pCedarV->GetFrmBufCB = GetFrmBufCB;
	pCedarV->WaitFinishCB = WaitFinishCB;

	return pCedarV;
}

void CedarvEncExit(VENC_DEVICE *pCedarV)
{
	if (pCedarV)
	{
		pCedarV->close(pCedarV);
		H264EncExit(pCedarV);
		pCedarV = NULL;
	}
}


void *thread_camera()
{
	int ret = -1;
	V4L2BUF_t Buf;
	unsigned long long curTime;

	while(1)
	{	
		int		write_id;
		int		read_id;
		int		buf_unused;
	    curTime = gettimeofday_curr();
		
		if ((curTime - lastTime) > 1000*1000*display_time) 
		{
			printf("Exit camera thread\n");		
			pthread_exit(NULL);
		}

		buf_unused	= gBufMrgQ.buf_unused;

		//printf("buf_unused: %d\n", buf_unused);
		
		if(buf_unused == 0)
		{
			usleep(10*1000);
			continue;
		}
				
		// get one frame
		ret = GetPreviewFrame(&Buf);

		//printf("GetPreviewFrame: %d\n", ret);
		if (ret != 0)
		{
			usleep(2*1000);
			printf("GetPreviewFrame failed\n");

		}

		pthread_mutex_lock(&mut_cam_buf);

		write_id 	= gBufMrgQ.write_id;
		read_id 	= gBufMrgQ.read_id;
		buf_unused	= gBufMrgQ.buf_unused;
		if(buf_unused != 0)
		{
			
			gBufMrgQ.omx_bufhead[write_id].buf_info.pts = Buf.timeStamp;
			gBufMrgQ.omx_bufhead[write_id].id = Buf.index;

			gBufMrgQ.omx_bufhead[write_id].buf_info.addrY = Buf.addrPhyY;
			gBufMrgQ.omx_bufhead[write_id].buf_info.addrCb = Buf.addrPhyY + mVideoWidth* mVideoHeight;		

			gBufMrgQ.buf_unused--;
			gBufMrgQ.write_id++;
			gBufMrgQ.write_id %= ENC_FIFO_LEVEL;
		}
		else
		{
			
			printf("IN OMX_ErrorUnderflow\n");
		}
		pthread_mutex_unlock(&mut_cam_buf);
		
	}

	return (void *)0;  
}


void *thread_enc()
{
	int ret;
	unsigned long long curTime;
	__vbv_data_ctrl_info_t data_info;
	int motionflag = 0;
	int bFirstFrame = 1; //need do something more in first frame

	while(1)
	{

		curTime = gettimeofday_curr();
		
		if ((curTime - lastTime) > 1000*1000*display_time) 
		{	
			printf("Exit encode thread\n");	
			pthread_exit(NULL);
		}

		/* the value from 0 to 9 can be used to set the level of the sensitivity of motion detection
		it is recommended to use 0 , which represents the hightest level sensitivity*/
//		g_pCedarV->IoCtrl(g_pCedarV, VENC_LIB_CMD_SET_MD_PARA , 0);


		pthread_mutex_lock(&mut_ve);

		/* in this function , the callback function of GetFrmBufCB will be used to get one frame */
#ifdef USE_SUNXI_MEM_ALLOCATOR
		ret = g_pCedarV->encode(g_pCedarV,NULL);
#else
		ret = g_pCedarV->encode(g_pCedarV);
#endif
		
		pthread_mutex_unlock(&mut_ve);

		//printf("encode result: %d\n", ret);


//		if(ret == 0)
//		{
//			/* get the motion detection result ,if the result is 1, it means that motion object have been detected*/
//			g_pCedarV->IoCtrl(g_pCedarV, VENC_LIB_CMD_GET_MD_DETECT, &motionflag);
//			printf("motion detection,result: %d\n", motionflag);
//		}

		
		if (ret != 0)
		{
			/* camera frame buffer is empty */
			usleep(10*1000);
			continue;
		}
	

		/* release the camera frame buffer */
		if(ret == 0)
		{
			pthread_mutex_lock(&mut_cam_buf);

			ReleaseFrame(g_cur_id);	
			
			pthread_mutex_unlock(&mut_cam_buf);

		}


		if(ret == 0)
		{
			memset(&data_info, 0 , sizeof(__vbv_data_ctrl_info_t));
			ret = g_pCedarV->GetBitStreamInfo(g_pCedarV, &data_info);
			
			if(ret == 0)
			{					
				if(1 == bFirstFrame)
				{
					bFirstFrame = 0;

					update_bitstream_to_cache(save_bit_stream, data_info.privateData, data_info.privateDataLen);

					/* encode update data to decode */
					decode_update_data_from_enc(hcedarv, &data_info, 1);	

				}
				else
				{
					/* encode update data to decode */
					decode_update_data_from_enc(hcedarv, &data_info, 0);
				}

							
				/* save bitstream to cache buffer */
				if (data_info.uSize0 != 0)
				{
					update_bitstream_to_cache(save_bit_stream, data_info.pData0, data_info.uSize0);
				}
				
				
				if (data_info.uSize1 != 0)
				{
					update_bitstream_to_cache(save_bit_stream, data_info.pData1, data_info.uSize1);

				}

								
				/* encode release bitstream */
				g_pCedarV->ReleaseBitStreamInfo(g_pCedarV, data_info.idx);
				
			}
		}
	}

}



void *thread_save_bs()
{
	int ret;
	unsigned long long curTime;
	int data_size = 0;
	int *tem_data = malloc(1*1024*1024);
		

	while(1)
	{
		curTime = gettimeofday_curr();
		
		if ((curTime - lastTime) > 1000*1000*display_time) 
		{	
			get_bitstream_for_save(save_bit_stream, tem_data, &data_size);

			if(data_size > 0)
			{
				fwrite(tem_data, data_size, 1, pEncFile);					
			}

			save_left_bitstream(save_bit_stream, tem_data, &data_size);

			if(data_size > 0)
			{
				fwrite(tem_data, data_size, 1, pEncFile);					
			}

			if(tem_data)
			{
				free(tem_data);
				tem_data = NULL;
			}

			printf("Exit encode thread\n");
			pthread_exit(NULL);					
		}

		
		get_bitstream_for_save(save_bit_stream, tem_data, &data_size);

		//printf("data_size: %d\n",data_size);

		if(data_size > 0)
		{
			fwrite(tem_data, data_size, 1, pEncFile);				
		}

		else
		{
			usleep(10 * 1000);
		}
		
	}
}



void *thread_dec()
{
	int ret;
	unsigned long long curTime;
	cedarv_picture_t picture;
	
	while(1)
	{
		curTime = gettimeofday_curr();
		
		if ((curTime - lastTime) > 1000*1000*display_time) 
		{
			printf("Exit decode thread\n");			
			pthread_exit(NULL);
		}


		/* decode one frame */	
		pthread_mutex_lock(&mut_ve);

		ret = decode_one_frame(hcedarv);
		
		pthread_mutex_unlock(&mut_ve);

		//printf("decode, ret: %d\n", ret);
		
		if(ret == 0)
		{
			printf("decode result: %d\n", ret);
		}
		
		
		if (CEDARV_RESULT_NO_FRAME_BUFFER == ret)
		{
			usleep(10*1000);
		}
		else if(CEDARV_RESULT_NO_BITSTREAM == ret)
		{
			usleep(10*1000);	
			continue;		
		}


		/* decoder output one frame for display */
		ret = decode_output_frame(hcedarv, &picture);
				
		if(ret == CEDARV_RESULT_OK)
		{
			/* display the frame */
			render_render(&picture, picture.id);

			/* decoder release the frame */
			decode_release_frame(hcedarv, picture.id);
		}

	}
}




int main()
{
	int ret = -1;
	

	/* init video engine */
	ret = cedarx_hardware_init(0);
	if (ret < 0)
	{
		printf("cedarx_hardware_init failed\n");
	}


	/* init camera */
	ret = InitCapture();
	if(ret != 0)
	{
		printf("InitCapture failed\n");
		goto EXIT;
	}


	/* init camera FIFO buffer */
	memset((void*)&gBufMrgQ, 0, sizeof(bufMrgQ_t));
	gBufMrgQ.buf_unused = ENC_FIFO_LEVEL;
	
	
	/* init  encoder */
	g_pCedarV = CedarvEncInit(mVideoWidth, mVideoHeight, 3*512*1024, GetFrmBufCB);


	/* init decoder */
	hcedarv = decode_init(mVideoWidth, mVideoHeight);
	

	/* init render */
	ret = render_init();

	/* set VE 320M */
	cedarv_set_ve_freq(320);

	pEncFile = fopen(saveFile, "wb+");
	if (pEncFile == NULL)
	{
		printf("open %s failed\n", saveFile);
		goto EXIT;
	}


	save_bit_stream = save_bitstream_int(2*1024*1024);
	if (save_bit_stream == NULL)
	{
		printf("save_bitstream_int failed\n", save_bit_stream);
		goto EXIT;
	}



	/* start camera */
	StartStreaming();

	pthread_mutex_init(&mut_cam_buf,NULL);
	pthread_mutex_init(&mut_ve,NULL);

	lastTime = gettimeofday_curr();

	/* create camera thread*/
	if(pthread_create(&thread_camera_id, NULL, thread_camera, NULL) != 0)
	{
		printf("Create thread_camera fail !\n");
	}

	/* create encode thread*/
	if(pthread_create(&thread_enc_id, NULL, thread_enc, NULL) != 0)
	{
		printf("Create thread_enc fail !\n");
	}

	/* create save bitstream thread*/               
    if(pthread_create(&thread_save_bs_id, NULL, thread_save_bs, NULL) != 0)
	{
		printf("Create thread_save_bs fail !\n");
	}

	/* create decode thread*/               
    if(pthread_create(&thread_dec_id, NULL, thread_dec, NULL) != 0)
	{
		printf("Create thread_dec fail !\n");
	}



EXIT:

	if(thread_camera_id !=0) 
	{                 
        pthread_join(thread_camera_id,NULL);
    }

	if(thread_dec_id !=0) 
	{                 
        pthread_join(thread_dec_id,NULL);
    }

	if(thread_enc_id !=0) 
	{                 
        pthread_join(thread_enc_id,NULL);
    }

	if(thread_save_bs_id !=0) 
	{                 
        pthread_join(thread_save_bs_id,NULL);
    }

	pthread_mutex_destroy(&mut_cam_buf);
	pthread_mutex_destroy(&mut_ve);

	save_bitstream_exit(save_bit_stream);

	DeInitCapture();

	CedarvEncExit(g_pCedarV);

	decode_exit(hcedarv);

	render_exit();
	


	if (pEncFile)
	{
		fclose(pEncFile);
		pEncFile = NULL;
	}

	cedarx_hardware_exit(0);
		
	return 0;
}



