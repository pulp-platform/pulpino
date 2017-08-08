#ifndef _CONFIG_CONV_
#define _CONFIG_CONV_

#define DATA_WIDTH 14
#define DATA_TYPE 16
#define IMG_ROW 64
#define IMG_COL 64
#define IMG_DIM IMG_ROW*IMG_COL

#define CONV_DIM (IMG_ROW-4)*(IMG_COL-4)

#define POOL_DIM CONV_DIM/4

#define OUT_DIM POOL_DIM

#define FILT_WIN 5
#define FILT_DIM FILT_WIN*FILT_WIN

#endif