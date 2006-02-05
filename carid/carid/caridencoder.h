
#ifndef __CARID_ENCODER_H__
#define __CARID_ENCODER_H__

#include <carid/caridbuffer.h>
#include <carid/caridparams.h>


typedef struct _CaridEncoder CaridEncoder;
typedef struct _CaridEncoderParams CaridEncoderParams;

struct _CaridEncoderParams {
  int quant_index_dc;
  int quant_index[6];
};

struct _CaridEncoder {
  CaridBuffer *frame_buffer[3];

  CaridBits *bits;

  CaridBuffer *frames[10];

  int need_rap;

  int16_t *tmpbuf;
  int16_t *tmpbuf2;

  int width;
  int height;

  int wavelet_type;

  CaridParams params;
  CaridEncoderParams encoder_params;

  CaridBuffer *subband_buffer;

  CaridSubband subbands[1+6*3];
};

CaridEncoder * carid_encoder_new (void);
void carid_encoder_free (CaridEncoder *encoder);
void carid_encoder_set_size (CaridEncoder *encoder, int width, int height);
void carid_encoder_set_wavelet_type (CaridEncoder *encoder, int wavelet_type);
void carid_encoder_push_buffer (CaridEncoder *encoder, CaridBuffer *buffer);
CaridBuffer * carid_encoder_encode (CaridEncoder *encoder);
void carid_encoder_copy_to_frame_buffer (CaridEncoder *encoder, CaridBuffer *buffer);
void carid_encoder_encode_rap (CaridEncoder *encoder);
void carid_encoder_encode_intra (CaridEncoder *encoder);
void carid_encoder_encode_frame_header (CaridEncoder *encoder);
void carid_encoder_encode_transform_parameters (CaridEncoder *encoder);
void carid_encoder_encode_transform_data (CaridEncoder *encoder, int component);
void carid_encoder_encode_subband (CaridEncoder *encoder, int component, int index);

#endif

