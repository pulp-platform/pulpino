/******************************************************************************
 *                                                                            *
 *              Integrated Systems Laboratory @ ETH Zurich                    *
 *          ERC Multitherman Lab @ DEI - University of Bologna                *
 *                                                                            *
 * Engineer:       Daniele Palossi - daniele.palossi@iis.ee.ethz.ch           *
 *                 Francesco Conti - f.conti@unibo.it                         *
 *                                                                            *
 * Project:        Mia Wallace / PULPv3 chip                                  *
 * File:           testSPI_slave.c                                            *
 * Description:    Full test with                                             *
 *                   - SPI=>L2 + L2=>L1 + compute + L1=>L2 + L2=>SPI          *
 *                   - double buffering                                       *
 *                                                                            *
 ******************************************************************************/

#include "pulp.h"
#include "mchan.h"          // DMA
#include "bar.h"            // synch_barrier
#include "string_lib.h"
#include "uart.h"           // UART printf
#include "cvp.h"            // set_frequency
#include "timer.h"          // timer
#include "events_ids.h"

// `define REG_PADDIR      4'b0000 //BASEADDR+0x00
// `define REG_PADIN       4'b0001 //BASEADDR+0x04
// `define REG_PADOUT      4'b0010 //BASEADDR+0x08
// `define REG_INTEN       4'b0011 //BASEADDR+0x0C
// `define REG_INTTYPE0    4'b0100 //BASEADDR+0x10
// `define REG_INTTYPE1    4'b0101 //BASEADDR+0x14
// `define REG_INTSTATUS   4'b0110 //BASEADDR+0x18
// `define REG_POWEREVENT  4'b0111 //BASEADDR+0x1C

// `define REG_PADCFG0     4'b1000 //BASEADDR+0x20
// `define REG_PADCFG1     4'b1001 //BASEADDR+0x24
// `define REG_PADCFG2     4'b1010 //BASEADDR+0x28
// `define REG_PADCFG3     4'b1011 //BASEADDR+0x2C
// `define REG_PADCFG4     4'b1100 //BASEADDR+0x30
// `define REG_PADCFG5     4'b1101 //BASEADDR+0x34
// `define REG_PADCFG6     4'b1110 //BASEADDR+0x38
// `define REG_PADCFG7     4'b1111 //BASEADDR+0x3C

#define IN_EXT_DATA_ADDR  0x1C004000
#define OUT_EXT_DATA_ADDR 0x1C00C000

#define SOC_CTRL_PADFUN0 (APB_SOC_CTRL_ADDR       )
#define SOC_CTRL_PADFUN1 (APB_SOC_CTRL_ADDR +  0x4)

#define GPIO_PADDIR      (GPIO_BASE_ADDR       )
#define GPIO_PADOUT      (GPIO_BASE_ADDR + 0x08)
#define GPIO_INTEN     (GPIO_BASE_ADDR + 0x0C)
#define GPIO_INTTYPE0     (GPIO_BASE_ADDR + 0x10)
#define GPIO_INTTYPE1     (GPIO_BASE_ADDR + 0x14)
#define GPIO_INTSTATUS     (GPIO_BASE_ADDR + 0x18)

#define N_ITER                4
#define DMA_SIZE            256
#define N_CORES               4

/* CHECKSUMS */
#define CHECKSUM_CORE0 65216   // 0x00000001 254*64+255*192
#define CHECKSUM_CORE1 65280   // 0x00000000 255*256
#define CHECKSUM_CORE2 0       // 0xFFFFFFFF 0*256
#define CHECKSUM_CORE3 64256   // 0x10000000 239*64+255*192

//#define SINGLE_BUFFER

// #define CHECKSUM_TCDMIN
// #define CHECKSUM_L2IN
// #define CHECKSUM_TCDM
// #define CHECKSUM_L2

// #define PRINT_CHECKSUMS_INPUT
// #define PRINT_CHECKSUMS_OUTPUT
// #define PRINT_CHECKSUMS_TCDM

#if 0 //def SINGLE_BUFFER


/* TCDM Input and Output memory buffers */
__attribute__ ((section(".heapsram"))) unsigned char tcdm_mem_in[N_CORES][DMA_SIZE];
__attribute__ ((section(".heapsram"))) unsigned char tcdm_mem_out[N_CORES][DMA_SIZE];


void core_computation(int coreID) {

    int i ;

    for (i=0; i<DMA_SIZE; i++) {
        tcdm_mem_out[coreID][i] = tcdm_mem_in[coreID][i] ^ 0xFF ;
    }

}


int main() {

    int i;
    int coreID = get_core_id();
    checksums[coreID] = 0 ;

    if (coreID == 0)
        uart_set_cfg(0, 26) ;
    BARRIER         0
    if(coreID == 0) {

        /* Pin 16th INPUT  - Host writes
         * Pin 17th OUTPUT - PULP writes
         * NOTE: GPIO pin 5 already occupied, thus pin 16 -> pad 17 and pin 17 -> pad 18 */
        *(volatile unsigned int*)(GPIO_PADDIR)      = 0x20000;     // 17th pin as output all the others as input
        *(volatile unsigned int*)(GPIO_PADOUT)      = 0x00000;     //
        *(volatile unsigned int*)(SOC_CTRL_PADFUN0) = 0x60000;     // sets pin uart_cts as gpio
        *(volatile unsigned int*)(GPIO_INTEN)       = 0x2000;      //

        /* Interrupt Type 01 : raise */
        *(volatile unsigned int*)(GPIO_INTTYPE0)    = 0x0 ;
        *(volatile unsigned int*)(GPIO_INTTYPE1)    = 0x10000 ;

        /* first event sent to the host to start writing in l2 */
        *(volatile unsigned int*)(GPIO_PADOUT)      = 0x20000 ;
        *(volatile unsigned int*)(GPIO_PADOUT)      = 0x0 ;
    }

    unsigned char *data_in  = (unsigned char *) IN_EXT_DATA_ADDR  + coreID*DMA_SIZE ;
    unsigned char *data_out = (unsigned char *) OUT_EXT_DATA_ADDR + coreID*DMA_SIZE ;

    synch_barrier();

    for (i=0; i<N_ITER; i++) {

        /* Wait the event from the Master to start DMA transfers from L2 to TCDM */
        clear_evnt_buff_low(0x200) ;
        set_evnt_mask_low(coreID, 0x200) ;        // event bit 9 0x200 events_ids.h
        wait_event() ;
        /* reset the interrupt signal just reading */
        int dummy_var = *(volatile unsigned int*) GPIO_INTSTATUS ;

        /* ======= DMA INPUT Copy ====== */
        set_tcdm_addr((int)(tcdm_mem_in[coreID])) ;
        set_ext_addr((int)(data_in)) ;
        push_cmd(LD256) ;
        dma_barrier() ;
        /* ============================= */

        core_computation(coreID) ;

        /* ====== DMA OUTPUT Copy ====== */
        set_tcdm_addr((int)(tcdm_mem_out[coreID])) ;
        set_ext_addr((int)(data_out)) ;
        push_cmd(ST256) ;
        dma_barrier() ;
        /* ============================= */

        /* ==== Checksum computation ==== */
        {
            int j ;
            unsigned int checksum = 0 ;
            for (j=0; j<DMA_SIZE; j++)
                checksum += tcdm_mem_out[coreID][j] ;
            checksums[coreID] += checksum ;
        }
        /* ============================= */

        synch_barrier();

        if(coreID == 0) {
            /* Send the event to the Master to start another burst */
            *(volatile unsigned int*)(GPIO_PADOUT)      = 0x1000 ;
            *(volatile unsigned int*)(GPIO_PADOUT)      = 0x0 ;
        }
    }

    switch (coreID) {
        case 0 :
            if (checksums[coreID] != CHECKSUM_CORE0*N_ITER)
                printf("CORE: %d ERROR\t[%d %d]\n", coreID, CHECKSUM_CORE0*N_ITER, checksums[coreID], 0) ;
            else
                printf("CORE: %d OK\t\t[%d %d]\n", coreID, CHECKSUM_CORE0*N_ITER, checksums[coreID], 0) ;
            break ;
        case 1 :
            if (checksums[coreID] != CHECKSUM_CORE1*N_ITER)
                printf("CORE: %d ERROR\t[%d %d]\n", coreID, CHECKSUM_CORE1*N_ITER, checksums[coreID], 0) ;
            else
                printf("CORE: %d OK\t\t[%d %d]\n", coreID, CHECKSUM_CORE1*N_ITER, checksums[coreID], 0) ;
            break ;
        case 2 :
            if (checksums[coreID] != CHECKSUM_CORE2*N_ITER)
                printf("CORE: %d ERROR\t[%d %d]\n", coreID, CHECKSUM_CORE2*N_ITER, checksums[coreID], 0) ;
            else
                printf("CORE: %d OK\t\t[%d %d]\n", coreID, CHECKSUM_CORE2*N_ITER, checksums[coreID], 0) ;
            break ;
        case 3 :
            if (checksums[coreID] != CHECKSUM_CORE3*N_ITER)
                printf("CORE: %d ERROR\t[%d %d]\n", coreID, CHECKSUM_CORE3*N_ITER, checksums[coreID], 0) ;
            else
                printf("CORE: %d OK\t\t[%d %d]\n", coreID, CHECKSUM_CORE3*N_ITER, checksums[coreID], 0) ;
            break ;
        default :
            printf("WRONG CORE ID: %d\n", coreID, 0, 0, 0) ;
            break ;
    }

    uart_wait_tx_done() ;

    synch_barrier();

    if(coreID == 0) {
      eoc(0);
    }

    return 0;
}

#else

__attribute__ ((section(".heapsram"))) unsigned int checksums_tcdmin[N_CORES];
__attribute__ ((section(".heapsram"))) unsigned int checksums_l2in  [N_CORES];
__attribute__ ((section(".heapsram"))) unsigned int checksums_tcdm  [N_CORES];
__attribute__ ((section(".heapsram"))) unsigned int checksums_l2    [N_CORES];

__attribute__ ((section(".heapsram"))) signed char tcdm_mem_in [2][DMA_SIZE*N_CORES];
__attribute__ ((section(".heapsram"))) signed char tcdm_mem_out[2][DMA_SIZE*N_CORES];
__attribute__ ((section(".heapsram"))) signed short weights[26];

#include "hwce.h"
// #define SYNTH_COMPUTE
// #define N_SYNTH_ITER 100
#define CONV_COMPUTE
#define N_CONV_ITER 15
#define USE_HWCE
// #define USE_CONV16_GOLD

#ifndef USE_HWCE
#define FIXED_MUL(x,y) ((x*y) >> 13)

void conv16_gold(int16_t *__restrict__ W, int16_t *__restrict__ x, int16_t *__restrict__ y, int h, int w, int fh, int fw, int nif, int a, int b) {
   int i;
   int oh = h-fh;
   int ow = w-fw;
   for (i=0; i<oh; i++) {
      int j;
      for (j=0; j<ow; j++) {
         int32_t conv = 0;
         int ui;
         for (ui=0; ui<fh; ui++) {
            int uj;
            for (uj=0; uj<fw; uj++) {
               int m;
               int n;
               m = i-ui+fh-1;
               n = j-uj+fw-1;
               conv += FIXED_MUL(W[((((a*nif)+b)*fh)+ui)*fw+uj] , x[(((b*h)+m)*w)+n]);
            }
         }
         y[(a*oh+i)*ow+j] = conv;
      }
   }
}

void conv16_unrolled_ptr_5x5_four_coarse(int16_t *__restrict__ W, int16_t *__restrict__ x, int16_t *__restrict__ y, int h, int w, int fh, int fw, int nif, int a, int b) {
   register int i;
   register int j;
   register int myid = get_core_id();
   int oh = h-fh;
   int ow = w-fw;
   int16_t *W_ptr;
   int16_t *x_ptr;
   int16_t *y_ptr = y + a*oh*ow;
   int16_t *x_base = x + b*h*w + (fh-1)*w + (fw-1);
   int16_t *W_base = &W[((a*nif)+b)*fh*fw];
   
   for (i=0; i<oh; i++) {
      for (j=myid; j<ow; j+=N_CORES) {

         int conv = 0;

         W_ptr = W_base;
         x_ptr = x_base + i*w + j;

         conv += FIXED_MUL(*W_ptr++, *x_ptr--);
         conv += FIXED_MUL(*W_ptr++, *x_ptr--);
         conv += FIXED_MUL(*W_ptr++, *x_ptr--);
         conv += FIXED_MUL(*W_ptr++, *x_ptr--);
         conv += FIXED_MUL(*W_ptr++, *x_ptr--);
         x_ptr -= w-5;

         conv += FIXED_MUL(*W_ptr++, *x_ptr--);
         conv += FIXED_MUL(*W_ptr++, *x_ptr--);
         conv += FIXED_MUL(*W_ptr++, *x_ptr--);
         conv += FIXED_MUL(*W_ptr++, *x_ptr--);
         conv += FIXED_MUL(*W_ptr++, *x_ptr--);
         x_ptr -= w-5;

         conv += FIXED_MUL(*W_ptr++, *x_ptr--);
         conv += FIXED_MUL(*W_ptr++, *x_ptr--);
         conv += FIXED_MUL(*W_ptr++, *x_ptr--);
         conv += FIXED_MUL(*W_ptr++, *x_ptr--);
         conv += FIXED_MUL(*W_ptr++, *x_ptr--);
         x_ptr -= w-5;

         conv += FIXED_MUL(*W_ptr++, *x_ptr--);
         conv += FIXED_MUL(*W_ptr++, *x_ptr--);
         conv += FIXED_MUL(*W_ptr++, *x_ptr--);
         conv += FIXED_MUL(*W_ptr++, *x_ptr--);
         conv += FIXED_MUL(*W_ptr++, *x_ptr--);
         x_ptr -= w-5;

         conv += FIXED_MUL(*W_ptr++, *x_ptr--);
         conv += FIXED_MUL(*W_ptr++, *x_ptr--);
         conv += FIXED_MUL(*W_ptr++, *x_ptr--);
         conv += FIXED_MUL(*W_ptr++, *x_ptr--);
         conv += FIXED_MUL(*W_ptr++, *x_ptr--);
         x_ptr -= w-5;

         *(y_ptr + i*ow+j) = conv;

      }
   }
}

#endif

void core_computation(int coreID, unsigned char *buffer_in, unsigned char *buffer_out) {

    int i, j;

#ifdef SYNTH_COMPUTE
    for (j=0; j<N_SYNTH_ITER; j++) {
        for (i=0; i<DMA_SIZE; i++)
            buffer_out[coreID*DMA_SIZE + i] = buffer_in[coreID*DMA_SIZE + i] * buffer_in[coreID*DMA_SIZE + i] ;
    }

    for (i=0; i<DMA_SIZE; i++) {
        buffer_out[coreID*DMA_SIZE + i] = buffer_in[coreID*DMA_SIZE + i] ^ 0xFF ;
    }
#endif

#ifdef CONV_COMPUTE
    for (j=0; j<N_CONV_ITER; j++) {
#ifdef USE_HWCE
       if(get_core_id() == 0) {
           short *weights_ptr = &weights;
           hwce5x5_conv16_5x5_nosum(&weights_ptr, (short *) buffer_in, (short *) buffer_out, 32, 16, 1, 0);
       }
#else /* not USE_HWCE */
#ifdef USE_CONV16_GOLD
       if(get_core_id() == 0) {
           conv16_gold(weights, (short *) buffer_in, (short *) buffer_out, 32, 16, 5, 5, 1, 0, 0); 
       }
#else /* not USE_CONV16_GOLD */
       conv16_unrolled_ptr_5x5_four_coarse(weights, (short *) buffer_in, (short *) buffer_out, 32, 16, 5, 5, 1, 0, 0); 
#endif /* USE_CONV16_GOLD */ 
#endif /* USE_HWCE */
    }
#endif /* CONV_COMPUTE */
}

/* Pulp send to to the Host the Event */
void send_host_event() {
    *(volatile unsigned int*)(GPIO_PADOUT) = 0x20000 ; // up pin 17
    *(volatile unsigned int*)(GPIO_PADOUT) = 0x00000 ; // down pin 17
}

/* Pulp wait for the Host Event */
void wait_host_event(int coreID) {
    wait_event() ;                                          // wait for the registerd events
    int status = *(volatile unsigned int*) GPIO_INTSTATUS ; // reset the interrupt signal just reading
    clear_evnt_buff_low(EVTMASK_GPIO) ;                            // clear previous event EVENT_GPIO received
    set_evnt_mask_low(coreID, EVTMASK_GPIO) ;                      // set event mask listening for bit 9 = 0x200 = EVENT_GPIO in events_ids.h
}

/* =========================== DOUBLE BUFFERING DMA =========================== */
int main() {
    // default 50 mhz
//    int cluster_frequency = 100, soc_frequency = 100;

    int i ;
    int coreID = get_core_id() ;
    int status;
    checksums_tcdm  [coreID] = 0 ;
    checksums_l2    [coreID] = 0 ;
    checksums_tcdmin[coreID] = 0;
    checksums_l2in  [coreID] = 0;

    if (coreID == 0) {
//        set_cluster_frequency(cluster_frequency);
//        set_soc_frequency(soc_frequency);
        uart_set_cfg(0, 26) ;
        for(i=0; i<25; i++) {
            weights[i] = 0x2000;
        }
        weights[25] = 0;
        for(i=0; i<DMA_SIZE*N_CORES; i++) {
            tcdm_mem_in [0][i] = 0;
            tcdm_mem_out[0][i] = 0;
            tcdm_mem_in [1][i] = 0;
            tcdm_mem_out[1][i] = 0;
        }
    }

    unsigned char *data_in[2] ;
    unsigned char *data_out[2] ;

    data_in[0]  = (unsigned char *) IN_EXT_DATA_ADDR ;
    data_in[1]  = (unsigned char *) IN_EXT_DATA_ADDR + N_CORES*DMA_SIZE ;
    data_out[0] = (unsigned char *) OUT_EXT_DATA_ADDR ;
    data_out[1] = (unsigned char *) OUT_EXT_DATA_ADDR + N_CORES*DMA_SIZE ;

    unsigned char *ptr_in = (unsigned char *) data_in[0] ;
    unsigned char *ptr_out =  (unsigned char *) data_out[0] ;

    unsigned char *buffer_in_dma = (unsigned char *) &tcdm_mem_in[0] ;
    unsigned char *buffer_in_compute ;
    unsigned char *buffer_out_compute = (unsigned char *) &tcdm_mem_out[0] ;
    unsigned char *buffer_out_dma ;

    if(coreID == 0) {
        /* GPIO pins [0:31]:
         * 16 : Host : Pulp can write
         * 17 : Pulp : Host can write */
        *(volatile unsigned int*)(GPIO_PADDIR)      = 0x20000 ; // pin 17 output all the others as input
        *(volatile unsigned int*)(GPIO_PADOUT)      = 0x00000 ; // all pins values to 0
        *(volatile unsigned int*)(SOC_CTRL_PADFUN0) = 0xC0000 ; // sets pins i2c_sda i2c_scl audio_i2s_ws audio_i2s_sck as gpio
        *(volatile unsigned int*)(GPIO_INTEN)       = 0x40000 ; // interrupt enabled on pins 15,16

        /* Interrupt Type 01 : raise */
        *(volatile unsigned int*)(GPIO_INTTYPE0)    = 0x00000 ; // left bit of pin 16 set to 0
        *(volatile unsigned int*)(GPIO_INTTYPE1)    = 0x40000 ; // right bit of pin 16 set to 1
    }

    synch_barrier() ;

    status = *(volatile unsigned int*) GPIO_INTSTATUS ; // reset the interrupt signal just reading
    clear_evnt_buff_low(EVTMASK_GPIO) ;            // clear previous event EVENT_GPIO received
    set_evnt_mask_low(coreID, EVTMASK_GPIO) ;      // set event mask listening for bit 9 = 0x200 = EVENT_GPIO in events_ids.h

    if(coreID == 0) {
        send_host_event() ;                    // INIT event sent to the host to start writing in l2
    }

    for (i=0; i<N_ITER+2; i++) {

        /* ===== Wait Host Event ====== */
        wait_host_event(coreID) ;
        /* ============================ */

#if 0
        set_evnt_mask_low(coreID, EVTMASK_BARRIER) ;        // set event mask listening for bit 0 = 0x001 = BARRIER in events_ids.h
        synch_barrier() ;
        clear_evnt_buff_low(EVTMASK_GPIO) ;
        set_evnt_mask_low(coreID, EVTMASK_GPIO) ;      // set event mask listening for bit 9 = 0x200 = EVENT_GPIO in events_ids.h
#endif

        /* === Input buffers setup === */
        buffer_in_compute = buffer_in_dma ;
        if (buffer_in_dma == (unsigned char *) &tcdm_mem_in[0])
            buffer_in_dma = (unsigned char *) &tcdm_mem_in[1] ;
        else
            buffer_in_dma = (unsigned char *) &tcdm_mem_in[0] ;
        /* ============================ */

        /* === Output buffers setup === */
        buffer_out_dma = buffer_out_compute ;
        if (buffer_out_compute == (unsigned char *) &tcdm_mem_out[0])
            buffer_out_compute = (unsigned char *) &tcdm_mem_out[1] ;
        else
            buffer_out_compute = (unsigned char *) &tcdm_mem_out[0] ;
        /* ============================ */

        /* ====== DMA OUTPUT Copy =====
         *             TCDM --> EXT         */
        if (i>1) {
            set_tcdm_addr((int)(&buffer_out_dma[coreID*DMA_SIZE])) ;
            set_ext_addr((int)(&ptr_out[coreID*DMA_SIZE])) ;
            push_cmd(ST256) ;
            // switch l2 output area
            if (ptr_out == data_out[0])
                ptr_out = data_out[1] ;
            else
                ptr_out = data_out[0] ;

#ifdef CHECKSUM_L2
            // L2 checksum computation
            if(i>2) {
                int j ;
                unsigned int checksum = 0 ;
                for (j=0; j<DMA_SIZE; j++) {
                    checksum += ptr_out[coreID*DMA_SIZE + j] ;
                }
                checksums_l2[coreID] += checksum ;
            }
#endif

        }
        /* ============================ */

        /* ====== DMA INPUT Copy ======
         *             EXT --> TCDM         */
        if (i<N_ITER) {
#ifdef CHECKSUM_L2IN
            int j ;
            unsigned int checksum = 0 ;
            for (j=0; j<DMA_SIZE; j++) {
                checksum += ptr_in[coreID*DMA_SIZE + j] ;
            }
            checksums_l2in[coreID] += checksum ;
#endif

            set_tcdm_addr((int)(&buffer_in_dma[coreID*DMA_SIZE])) ;
            set_ext_addr((int)(&ptr_in[coreID*DMA_SIZE])) ;
            push_cmd(LD256) ;

            // switch L2 input area
            if (ptr_in == data_in[0])
                ptr_in = data_in[1] ;
            else
                ptr_in = data_in[0] ;

        }
        /* ============================ */

        /* === Compute the DMA block == */
        if ((i>0) && (i<(N_ITER+1))) {

#ifdef CHECKSUM_TCDMIN
            int j ;
            unsigned int checksum = 0 ;
            for (j=0; j<DMA_SIZE; j++) {
                checksum += buffer_in_compute[coreID*DMA_SIZE + j] ;
            }
            checksums_tcdmin[coreID] += checksum ;
#endif

            core_computation(coreID, buffer_in_compute, buffer_out_compute) ;

            // barrier
            set_evnt_mask_low(coreID, EVTMASK_BARRIER) ;        // set event mask listening for bit 0 = 0x001 = BARRIER in events_ids.h
            synch_barrier() ;
            status = *(volatile unsigned int*) GPIO_INTSTATUS ; // reset the interrupt signal just reading
            clear_evnt_buff_low(EVTMASK_GPIO) ;

#ifdef CHECKSUM_TCDM
            // TCDM checksum computation
            int j ;
            unsigned int checksum = 0 ;
            for (j=0; j<DMA_SIZE; j++) {
                checksum += buffer_out_compute[coreID*DMA_SIZE + j] ;
            }
            checksums_tcdm[coreID] += checksum ;
#endif
        }
        /* ============================ */

        /* ====== Synchronization ===== */
        dma_barrier() ;
        set_evnt_mask_low(coreID, EVTMASK_BARRIER) ;        // set event mask listening for bit 0 = 0x001 = BARRIER in events_ids.h
        synch_barrier() ;
        status = *(volatile unsigned int*) GPIO_INTSTATUS ; // reset the interrupt signal just reading
        clear_evnt_buff_low(EVTMASK_GPIO) ;
        set_evnt_mask_low(coreID, EVTMASK_GPIO) ;        // set event mask listening for bit 9 = 0x200 = EVENT_GPIO in events_ids.h
        /* ============================ */

        /* ====== Sent Events to the Host ====== */
        if (coreID == 0) {
            send_host_event() ;
        }
        /* ====================================== */

#if 0
        synch_barrier() ;
        clear_evnt_buff_low(EVTMASK_GPIO) ;
        set_evnt_mask_low(coreID, EVTMASK_GPIO) ;        // set event mask listening for bit 9 = 0x200 = EVENT_GPIO in events_ids.h
#endif

    }

//    switch (coreID) {
//        case 0 :
//            if (checksums[coreID] != CHECKSUM_CORE0*N_ITER)
//                printf("CORE: %d ERROR\t[%d %d]\n", coreID, CHECKSUM_CORE0*N_ITER, checksums[coreID], 0) ;
//            else
//                printf("CORE: %d OK\t\t[%d %d]\n", coreID, CHECKSUM_CORE0*N_ITER, checksums[coreID], 0) ;
//            break ;
//        case 1 :
//            if (checksums[coreID] != CHECKSUM_CORE1*N_ITER)
//                printf("CORE: %d ERROR\t[%d %d]\n", coreID, CHECKSUM_CORE1*N_ITER, checksums[coreID], 0) ;
//            else
//                printf("CORE: %d OK\t\t[%d %d]\n", coreID, CHECKSUM_CORE1*N_ITER, checksums[coreID], 0) ;
//            break ;
//        case 2 :
//            if (checksums[coreID] != CHECKSUM_CORE2*N_ITER)
//                printf("CORE: %d ERROR\t[%d %d]\n", coreID, CHECKSUM_CORE2*N_ITER, checksums[coreID], 0) ;
//            else
//                printf("CORE: %d OK\t\t[%d %d]\n", coreID, CHECKSUM_CORE2*N_ITER, checksums[coreID], 0) ;
//            break ;
//        case 3 :
//            if (checksums[coreID] != CHECKSUM_CORE3*N_ITER)
//                printf("CORE: %d ERROR\t[%d %d]\n", coreID, CHECKSUM_CORE3*N_ITER, checksums[coreID], 0) ;
//            else
//                printf("CORE: %d OK\t\t[%d %d]\n", coreID, CHECKSUM_CORE3*N_ITER, checksums[coreID], 0) ;
//            break ;
//        default :
//            printf("WRONG CORE ID: %d\n", coreID, 0, 0, 0) ;
//            break ;
//    }

//    uart_wait_tx_done() ;

    set_evnt_mask_low(coreID, 0x001) ;        // set event mask listening for bit 0 = 0x001 = BARRIER in events_ids.h
    synch_barrier();

#ifdef CHECKSUM_L2
       
    // L2 checksum computation
    if (ptr_out == data_out[0])
        ptr_out = data_out[1] ;
    else
        ptr_out = data_out[0] ;

    int j ;
    unsigned int checksum = 0 ; 
    for (j=0; j<DMA_SIZE; j++) {
        checksum += ptr_out[coreID*DMA_SIZE + j] ;
    }
    checksums_l2[coreID] += checksum ;
#endif

    set_evnt_mask_low(coreID, 0x001) ;        // set event mask listening for bit 0 = 0x001 = BARRIER in events_ids.h
    synch_barrier();

    if(coreID == 0) {

        volatile unsigned int *ptr = (volatile unsigned int*) (0x10001000) ;

#ifdef PRINT_CHECKSUMS_OUTPUT
        ptr[0] = checksums_tcdm[0] ;
        ptr[1] = checksums_l2  [0] ;
        ptr[2] = checksums_tcdm[1] ;
        ptr[3] = checksums_l2  [1] ;
        ptr[4] = checksums_tcdm[2] ;
        ptr[5] = checksums_l2  [2] ;
        ptr[6] = checksums_tcdm[3] ;
        ptr[7] = checksums_l2  [3] ;
#endif
#ifdef PRINT_CHECKSUMS_TCDM
        ptr[0] = checksums_tcdmin[0] ;
        ptr[1] = checksums_tcdm  [0] ;
        ptr[2] = checksums_tcdmin[1] ;
        ptr[3] = checksums_tcdm  [1] ;
        ptr[4] = checksums_tcdmin[2] ;
        ptr[5] = checksums_tcdm  [2] ;
        ptr[6] = checksums_tcdmin[3] ;
        ptr[7] = checksums_tcdm  [3] ;
#endif
#ifdef PRINT_CHECKSUMS_INPUT
        ptr[0] = checksums_tcdmin[0] ;
        ptr[1] = checksums_l2in  [0] ;
        ptr[2] = checksums_tcdmin[1] ;
        ptr[3] = checksums_l2in  [1] ;
        ptr[4] = checksums_tcdmin[2] ;
        ptr[5] = checksums_l2in  [2] ;
        ptr[6] = checksums_tcdmin[3] ;
        ptr[7] = checksums_l2in  [3] ;
#endif

    }

    synch_barrier();
    eoc(0) ;

    return 0;
}

#endif

