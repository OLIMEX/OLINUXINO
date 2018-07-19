#ifndef DECODE_API_H
#define DECODE_API_H
#include "libcedarv.h"	//* for decoding video
#include "H264encLibApi.h"


#ifdef __cplusplus
extern "C" {
#endif

#if 0
#define MAX_DISP_ELEMENTS   32

typedef struct disp_element_t
{
	unsigned int curr_disp_frame_id;
	unsigned int dec_frame_id;
}disp_element_t;

typedef struct disp_queue_t
{
	disp_element_t disp_elements[MAX_DISP_ELEMENTS];
	int wr_idx;
	int rd_idx;
}disp_queue_t;

#endif


extern cedarv_decoder_t* decode_init(unsigned int width, unsigned int height);
extern void decode_exit(cedarv_decoder_t*  hcedarv);
extern int decode_update_data_from_enc(cedarv_decoder_t*  hcedarv, __vbv_data_ctrl_info_t *data_info, int firstframeflag);
extern int decode_one_frame(cedarv_decoder_t*  hcedarv);
extern int decode_release_frame(cedarv_decoder_t*  hcedarv, unsigned int id);




#ifdef __cplusplus
}
#endif
#endif

