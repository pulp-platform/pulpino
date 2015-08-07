#ifndef __PIPE_STAGE_H__
#define __PIPE_STAGE_H__

#include "pulp.h"

#define REC_PIPE_REQ_BASE_ADDR       (REC_PIPE_STARGE_BASE_ADDR + 0x0)
#define REC_PIPE_RESP_BASE_ADDR      (REC_PIPE_STARGE_BASE_ADDR + 0x4)

void set_req_pipe( int conf);
void set_resp_pipe(int conf);

int  read_req_pipe_reg();
int  read_resp_pipe_reg();
#endif
