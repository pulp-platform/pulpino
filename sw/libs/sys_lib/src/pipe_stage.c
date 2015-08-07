#include "pipe_stage.h"


void set_req_pipe(int conf)
{
  *(volatile int*) (REC_PIPE_REQ_BASE_ADDR) = conf;
}


void set_resp_pipe(int conf)
{
  *(volatile int*) (REC_PIPE_RESP_BASE_ADDR) = conf;
}




int read_req_pipe_reg(  )
{
  volatile int reg_status;

  reg_status = *(volatile int*) (REC_PIPE_REQ_BASE_ADDR);

  return reg_status;
}


int read_resp_pipe_reg(  )
{
  volatile int reg_status;

  reg_status = *(volatile int*) (REC_PIPE_RESP_BASE_ADDR);

  return reg_status;
}


