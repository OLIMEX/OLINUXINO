#include "decode_api.h"
#include <stdio.h>

cedarv_decoder_t* decode_init(unsigned int width, unsigned int height)
{

	cedarv_decoder_t*    hcedarv;
	cedarv_stream_info_t 	stream_info;
	int ret;

	hcedarv = libcedarv_init(&ret);
	if(ret < 0 || hcedarv == NULL)
	{
		printf("libcedarv_init fail, test program quit.\n");
	}

	stream_info.video_width = width;
	stream_info.video_height = height;
	stream_info.format = CEDARV_STREAM_FORMAT_H264; 
	stream_info.container_format = CEDARV_CONTAINER_FORMAT_UNKNOW;

	stream_info.init_data = NULL;
	stream_info.init_data_len= 0;


	//* set video stream information to libcedarv.
	ret = hcedarv->set_vstream_info(hcedarv, &stream_info);
	if(ret < 0)
	{
		printf("set video stream information to libcedarv fail, test program quit.\n");
	}

	//* open libcedarv.
	ret = hcedarv->open(hcedarv);

	if(ret < 0)
	{
		printf("open libcedarv fail ret=%d, test program quit.\n",ret);
		
	}
		
	//* tell libcedarv to start.
	hcedarv->ioctrl(hcedarv, CEDARV_COMMAND_PLAY, 0);


	printf("cedarv open ok\n");
	return hcedarv;

}



void decode_exit(cedarv_decoder_t*  hcedarv)
{
	if(hcedarv)
	{
		hcedarv->ioctrl(hcedarv, CEDARV_COMMAND_STOP, 0);
		hcedarv->close(hcedarv);
		libcedarv_exit(hcedarv);
	}
	
}


int decode_update_data_from_enc(cedarv_decoder_t*  hcedarv, __vbv_data_ctrl_info_t *data_info, int firstframeflag)
{
	int ret;
	cedarv_stream_data_info_t  stream_data_info;
	u8*					 		buf0;
	u32					 		bufsize0;
	u8*					 		buf1;
	u32					 		bufsize1;

	if (1 == firstframeflag)
	{
		if(data_info->privateDataLen != 0)
		{	
			memset(&stream_data_info, 0, sizeof(cedarv_stream_data_info_t));
			ret = hcedarv->request_write(hcedarv, data_info->privateDataLen, &buf0, &bufsize0, &buf1, &bufsize1);
	
			if(ret < 0)
			{
				printf("request bitstream buffer fail.\n");
				return ret;
			}
	
	
			if(data_info->privateDataLen <= bufsize0) {
				mem_cpy(buf0, data_info->privateData, bufsize0);
				cedarx_cache_op(buf0, (buf0 + bufsize0), 1); //flush cache
			}
	
			else
			{
				printf("private data request write error\n");
			}
	
			// set stream data info
			stream_data_info.pts = -1;	//* get video pts
			stream_data_info.lengh = data_info->privateDataLen;
			//stream_data_info.flags = CEDARV_FLAG_PTS_VALID | CEDARV_FLAG_FIRST_PART | CEDARV_FLAG_LAST_PART;

			stream_data_info.flags = CEDARV_FLAG_FIRST_PART | CEDARV_FLAG_LAST_PART;
			
			//* update data to libcedarv.
			hcedarv->update_data(hcedarv, &stream_data_info);
			
		}
	
		
	}
	
	
	if (data_info->uSize0 != 0)
	{

		memset(&stream_data_info, 0, sizeof(cedarv_stream_data_info_t));
		ret = hcedarv->request_write(hcedarv, data_info->uSize0, &buf0, &bufsize0, &buf1, &bufsize1);
	
		if(ret < 0)
		{
			printf("request bitstream buffer fail.\n");
			return ret;
		}
	
	
		if(data_info->uSize0 <= bufsize0) {
			mem_cpy(buf0, data_info->pData0, bufsize0);
			cedarx_cache_op(buf0, (buf0 + bufsize0), 1); //flush cache
		}
		else 
		{	
			mem_cpy(buf0, data_info->pData0, bufsize0);
			cedarx_cache_op(buf0, (buf0 + bufsize0), 1);
			
			mem_cpy(buf1, data_info->pData0 + bufsize0, bufsize1);
			cedarx_cache_op(buf1, (buf1 + bufsize1), 1);
		}
	
	
		// set stream data info
		stream_data_info.pts = -1;
		stream_data_info.lengh = data_info->uSize0;
	
		if(data_info->uSize1 != 0)
		{
			//stream_data_info.flags = CEDARV_FLAG_PTS_VALID | CEDARV_FLAG_FIRST_PART;
			stream_data_info.flags = CEDARV_FLAG_FIRST_PART;
		}
		else
		{
			//stream_data_info.flags = CEDARV_FLAG_PTS_VALID | CEDARV_FLAG_FIRST_PART | CEDARV_FLAG_LAST_PART;
			stream_data_info.flags = CEDARV_FLAG_FIRST_PART | CEDARV_FLAG_LAST_PART;
		}
	
	
		
		//* update data to libcedarv.
		hcedarv->update_data(hcedarv, &stream_data_info);
	
	
		if (data_info->uSize1 != 0)
		{
			memset(&stream_data_info, 0, sizeof(cedarv_stream_data_info_t));
			ret = hcedarv->request_write(hcedarv, data_info->uSize1, &buf0, &bufsize0, &buf1, &bufsize1);
			if(ret < 0)
			{	
				usleep(5*1000);
				printf("request bitstream buffer fail.\n");
			}


			if(data_info->uSize1 <= bufsize0) {
				mem_cpy(buf0, data_info->pData1, bufsize0);
				cedarx_cache_op(buf0, (buf0 + bufsize0), 1);
			}
			else
			{
				mem_cpy(buf0, data_info->pData1, bufsize0);
				
				cedarx_cache_op(buf0, (buf0 + bufsize0), 1);
				mem_cpy(buf1, data_info->pData1 + bufsize0, bufsize1);
				cedarx_cache_op(buf1, (buf1 + bufsize1), 1);
			}
	
			// set stream data info
			stream_data_info.pts = -1;
			stream_data_info.lengh = data_info->uSize1;

			
			//* update data to libcedarv.
	
			//stream_data_info.flags = CEDARV_FLAG_PTS_VALID | CEDARV_FLAG_LAST_PART;
			stream_data_info.flags = CEDARV_FLAG_LAST_PART;
			
			hcedarv->update_data(hcedarv, &stream_data_info);
		}
	}

	return 0;

}


int decode_one_frame(cedarv_decoder_t*  hcedarv)
{
	return hcedarv->decode(hcedarv);
}



int decode_output_frame(cedarv_decoder_t*  hcedarv, cedarv_picture_t *picture)
{

	int ret;
	
	ret = hcedarv->display_request(hcedarv, picture);

	return ret;
}


int decode_release_frame(cedarv_decoder_t*  hcedarv, unsigned int id)
{
	int ret;

	ret = hcedarv->display_release(hcedarv, id);
	return ret;
}



