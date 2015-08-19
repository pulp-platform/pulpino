#include "mchan.h"

#define round32(N) (N + (4-N%4))
#define round64(N) (N + (8-N%8))

/* 
 * mchan_memcpy[_async]
 *   dst  : pointer to SUGGESTED destination base
 *   src  : pointer to source base
 *   size : transfer size in bytes
 * returns the ACTUAL destination base. This is needed
 * to support some misalignment cases, if 8 more bytes
 * than the requested size are allocated.
 */
static void * mchan_memcpy_async(void *dst, void *src, unsigned int size) {

   char st_ldn = (dst >= 0x1c000000) ? 1 : 0;
   int i=0, bytes_int=0, bytes_count=0;
   int mis_src_start = 0, mis_src_end = 0, mis_dst = 0;

   unsigned int dst_int, src_int;
   unsigned int size_int;
   unsigned int dst_save_start0;
   unsigned int dst_save_start1;
   unsigned int dst_save_end0;
   unsigned int dst_save_end1;

   unsigned int tmp_datum_src_start0;
   unsigned int tmp_datum_src_start1;
   unsigned int tmp_datum_src_end0;
   unsigned int tmp_datum_src_end1;

   // check misaligned addresses
   src_int  = (unsigned) src;
   dst_int  = (unsigned) dst;
   size_int = (unsigned) size;

   if(src_int % 8) {
      // set flag
      mis_src_start = src_int % 8;
      // move bases forward and reduce size
      src_int  = src_int - mis_src_start + 8;
      dst_int  = dst_int - mis_src_start + 8;
      size_int = size_int - 8 + mis_src_start;
      // load manually word before src base
      tmp_datum_src_start0 = *(unsigned *)(src_int - 8);
      tmp_datum_src_start1 = *(unsigned *)(src_int - 4);
   }
   if((src_int+size_int) % 8) {
      // set flag
      mis_src_end = (src_int+size_int) % 8; 
      // reduce size
      size_int = size_int - mis_src_end;
      // load manually word after src base
      tmp_datum_src_end0 = *(unsigned *)(src_int+size_int + 0);
      tmp_datum_src_end1 = *(unsigned *)(src_int+size_int + 4);
   }
   if(dst_int % 8) {
      // set flag
      mis_dst = dst_int % 8;
      // FIXME : this requires we allocate 8 more bytes user-level and when performing a 
      // relatively misaligned transfer, move the data pointer
      dst_int = round64(dst_int);
   }
   if(mis_src_start) {
      dst_save_start0 = *((unsigned int *) (dst_int-8));
      dst_save_start1 = *((unsigned int *) (dst_int-4));
   }
   if(mis_src_end) {
      dst_save_end0 = *((unsigned int *) (dst_int+size_int+0));
      dst_save_end1 = *((unsigned int *) (dst_int+size_int+4));
   }

   if(mis_dst && st_ldn) {
      // revert to byte-by-byte copy :(
      for(i=0; i<size; i++) {
         ((char *) dst)[i] = ((char *) src)[i];
      }
      return dst;
   }

   bytes_int = size_int;
   bytes_count = 0;

#ifdef DMADEBUG
   #pragma omp critical
   {
      printf("[MCHAN] orig dst=%08x src=%08x size=%d\n", dst, src, size);
      printf("[MCHAN] copy dst=%08x src=%08x size=%d\n", dst_int, src_int, size_int);
   }
   #pragma omp barrier
#endif
 
   // greedy MCHAN copy
   i=0;
   while (bytes_int>0) {
      int bytes_mchan;
      bytes_mchan = (bytes_int < 16)    ? 8 :
                    (bytes_int < 32)    ? 16 :
                    (bytes_int < 64)    ? 32 :
                    (bytes_int < 128)   ? 64 :
                    (bytes_int < 256)   ? 128 :
                    (bytes_int < 512)   ? 256 :
                    (bytes_int < 1024)  ? 512 :
                    (bytes_int < 2048)  ? 1024 :
                    (bytes_int < 4096)  ? 2048 :
                    (bytes_int < 8192)  ? 4096 :
                    (bytes_int < 16384) ? 8192 :
                    (bytes_int < 32768) ? 16384 :
                                          32768;
      bytes_int -= bytes_mchan;
      i++;
      if(st_ldn) {
         switch (bytes_mchan) {
            case 8:
               {
                   set_tcdm_addr(((unsigned) src_int) + bytes_count);
                   set_ext_addr (((unsigned) dst_int) + bytes_count);
                   push_cmd(ST8);
               }
               break;
            case 16:
               {
                   set_tcdm_addr(((unsigned) src_int) + bytes_count);
                   set_ext_addr (((unsigned) dst_int) + bytes_count);
                   push_cmd(ST16);
               }
               break;
            case 32:
               {
                   set_tcdm_addr(((unsigned) src_int) + bytes_count);
                   set_ext_addr (((unsigned) dst_int) + bytes_count);
                   push_cmd(ST32);
               }
               break;
            case 64:
               {
                   set_tcdm_addr(((unsigned) src_int) + bytes_count);
                   set_ext_addr (((unsigned) dst_int) + bytes_count);
                   push_cmd(ST64);
               }
               break;
            case 128:
               {
                   set_tcdm_addr(((unsigned) src_int) + bytes_count);
                   set_ext_addr (((unsigned) dst_int) + bytes_count);
                   push_cmd(ST128);
               }
               break;
            case 256:
               {
                   set_tcdm_addr(((unsigned) src_int) + bytes_count);
                   set_ext_addr (((unsigned) dst_int) + bytes_count);
                   push_cmd(ST256);
               }
               break;
            case 512:
               {
                   set_tcdm_addr(((unsigned) src_int) + bytes_count);
                   set_ext_addr (((unsigned) dst_int) + bytes_count);
                   push_cmd(ST512);
               }
               break;
            case 1024:
               {
                   set_tcdm_addr(((unsigned) src_int) + bytes_count);
                   set_ext_addr (((unsigned) dst_int) + bytes_count);
                   push_cmd(ST1024);
               }
               break;
            case 2048:
               {
                   set_tcdm_addr(((unsigned) src_int) + bytes_count);
                   set_ext_addr (((unsigned) dst_int) + bytes_count);
                   push_cmd(ST2048);
               }
               break;
            case 4096:
               {
                   set_tcdm_addr(((unsigned) src_int) + bytes_count);
                   set_ext_addr (((unsigned) dst_int) + bytes_count);
                   push_cmd(ST4096);
               }
               break;
            case 8192:
               {
                   set_tcdm_addr(((unsigned) src_int) + bytes_count);
                   set_ext_addr (((unsigned) dst_int) + bytes_count);
                   push_cmd(ST8192);
               }
               break;
            case 16384:
               {
                   set_tcdm_addr(((unsigned) src_int) + bytes_count);
                   set_ext_addr (((unsigned) dst_int) + bytes_count);
                   push_cmd(ST16384);
               }
               break;
            case 32768:
               {
                   set_tcdm_addr(((unsigned) src_int) + bytes_count);
                   set_ext_addr (((unsigned) dst_int) + bytes_count);
                   push_cmd(ST32768);
               }
               break;
            default:
               break;
         }
      }
      else {
         switch (bytes_mchan) {
            case 8:
               {
                   set_tcdm_addr(((unsigned) dst_int) + bytes_count);
                   set_ext_addr (((unsigned) src_int) + bytes_count);
                   push_cmd(LD8);
               }
               break;
            case 16:
               {
                   set_tcdm_addr(((unsigned) dst_int) + bytes_count);
                   set_ext_addr (((unsigned) src_int) + bytes_count);
                   push_cmd(LD16);
               }
               break;
            case 32:
               {
                   set_tcdm_addr(((unsigned) dst_int) + bytes_count);
                   set_ext_addr (((unsigned) src_int) + bytes_count);
                   push_cmd(LD32);
               }
               break;
            case 64:
               {
                   set_tcdm_addr(((unsigned) dst_int) + bytes_count);
                   set_ext_addr (((unsigned) src_int) + bytes_count);
                   push_cmd(LD64);
               }
               break;
            case 128:
               {
                   set_tcdm_addr(((unsigned) dst_int) + bytes_count);
                   set_ext_addr (((unsigned) src_int) + bytes_count);
                   push_cmd(LD128);
               }
               break;
            case 256:
               {
                   set_tcdm_addr(((unsigned) dst_int) + bytes_count);
                   set_ext_addr (((unsigned) src_int) + bytes_count);
                   push_cmd(LD256);
               }
               break;
            case 512:
               {
                   set_tcdm_addr(((unsigned) dst_int) + bytes_count);
                   set_ext_addr (((unsigned) src_int) + bytes_count);
                   push_cmd(LD512);
               }
               break;
            case 1024:
               {
                   set_tcdm_addr(((unsigned) dst_int) + bytes_count);
                   set_ext_addr (((unsigned) src_int) + bytes_count);
                   push_cmd(LD1024);
               }
               break;
            case 2048:
               {
                   set_tcdm_addr(((unsigned) dst_int) + bytes_count);
                   set_ext_addr (((unsigned) src_int) + bytes_count);
                   push_cmd(LD2048);
               }
               break;
            case 4096:
               {
                   set_tcdm_addr(((unsigned) dst_int) + bytes_count);
                   set_ext_addr (((unsigned) src_int) + bytes_count);
                   push_cmd(LD4096);
               }
               break;
            case 8192:
               {
                   set_tcdm_addr(((unsigned) dst_int) + bytes_count);
                   set_ext_addr (((unsigned) src_int) + bytes_count);
                   push_cmd(LD8192);
               }
               break;
            case 16384:
               {
                   set_tcdm_addr(((unsigned) dst_int) + bytes_count);
                   set_ext_addr (((unsigned) src_int) + bytes_count);
                   push_cmd(LD16384);
               }
               break;
            case 32768:
               {
                   set_tcdm_addr(((unsigned) dst_int) + bytes_count);
                   set_ext_addr (((unsigned) src_int) + bytes_count);
                   push_cmd(LD32768);
               }
               break;
            default:
               break;
         }
      }
      bytes_count += bytes_mchan;
   }

   // misaligned cases - this is probably endianness-dependent
   if(mis_src_start != 0) {
      unsigned tmp1, tmp2;
      unsigned ci;
      volatile unsigned char *c = (volatile unsigned char *) &ci;
      // printf("mis_src_start = %d\n", mis_src_start);
      tmp1 = dst_save_start0; 
      tmp2 = tmp_datum_src_start0;
      if(mis_src_start < 4) {
         if(mis_src_start == 1) {
           c[0] = *((unsigned char *) (&tmp1) + 0);
           c[1] = *((unsigned char *) (&tmp2) + 1);
           c[2] = *((unsigned char *) (&tmp2) + 2);
           c[3] = *((unsigned char *) (&tmp2) + 3);
         }
         else if(mis_src_start == 2) {
           c[0] = *((unsigned char *) (&tmp1) + 0);
           c[1] = *((unsigned char *) (&tmp1) + 1);
           c[2] = *((unsigned char *) (&tmp2) + 2);
           c[3] = *((unsigned char *) (&tmp2) + 3);
         }
         else {
           c[0] = *((unsigned char *) (&tmp1) + 0);
           c[1] = *((unsigned char *) (&tmp1) + 1);
           c[2] = *((unsigned char *) (&tmp1) + 2);
           c[3] = *((unsigned char *) (&tmp2) + 3);
         }
         *(unsigned *) (dst_int-8) = *(unsigned *) c;
         *(unsigned *) (dst_int-4) = tmp_datum_src_start1;
      }
      else if(mis_src_start > 4) {
         tmp1 = dst_save_start1; 
         tmp2 = tmp_datum_src_start1;
         if(mis_src_start == 5) {
           c[0] = *((unsigned char *) (&tmp1) + 0);
           c[1] = *((unsigned char *) (&tmp2) + 1);
           c[2] = *((unsigned char *) (&tmp2) + 2);
           c[3] = *((unsigned char *) (&tmp2) + 3);
         }
         else if(mis_src_start == 6) {
           c[0] = *((unsigned char *) (&tmp1) + 0);
           c[1] = *((unsigned char *) (&tmp1) + 1);
           c[2] = *((unsigned char *) (&tmp2) + 2);
           c[3] = *((unsigned char *) (&tmp2) + 3);
         }
         else {
           c[0] = *((unsigned char *) (&tmp1) + 0);
           c[1] = *((unsigned char *) (&tmp1) + 1);
           c[2] = *((unsigned char *) (&tmp1) + 2);
           c[3] = *((unsigned char *) (&tmp2) + 3);
         }
         *(unsigned *) (dst_int-8) = dst_save_start0;
         *(unsigned *) (dst_int-4) = *(unsigned *) c;
      }
      else {
         *(unsigned *) (dst_int-8) = dst_save_start0;
         *(unsigned *) (dst_int-4) = tmp_datum_src_start1;
      }
   }
   if(mis_src_end != 0) {
      unsigned tmp1, tmp2;
      unsigned ci;
      volatile unsigned char *c = (volatile unsigned char *) &ci;
      // printf("mis_src_end = %d\n", mis_src_end);
      if(mis_src_end < 4) {
         tmp1 = dst_save_end0;
         tmp2 = tmp_datum_src_end0;
         if(mis_src_end == 1) {
           c[0] = *((unsigned char *) (&tmp2) + 0);
           c[1] = *((unsigned char *) (&tmp1) + 1);
           c[2] = *((unsigned char *) (&tmp1) + 2);
           c[3] = *((unsigned char *) (&tmp1) + 3);
         }
         else if(mis_src_end == 2) {
           c[0] = *((unsigned char *) (&tmp2) + 0);
           c[1] = *((unsigned char *) (&tmp2) + 1);
           c[2] = *((unsigned char *) (&tmp1) + 2);
           c[3] = *((unsigned char *) (&tmp1) + 3);
         }
         else {
           c[0] = *((unsigned char *) (&tmp2) + 0);
           c[1] = *((unsigned char *) (&tmp2) + 1);
           c[2] = *((unsigned char *) (&tmp2) + 2);
           c[3] = *((unsigned char *) (&tmp1) + 3);
         }
         *(unsigned *) (dst_int+size_int) = *(unsigned *) c;
         *(unsigned *) (dst_int+size_int+4) = dst_save_end1;
      }
      else if(mis_src_end > 4) {
         tmp1 = dst_save_end1;
         tmp2 = tmp_datum_src_end1;
         if(mis_src_end == 5) {
           c[0] = *((unsigned char *) (&tmp2) + 0);
           c[1] = *((unsigned char *) (&tmp1) + 1);
           c[2] = *((unsigned char *) (&tmp1) + 2);
           c[3] = *((unsigned char *) (&tmp1) + 3);
         }
         else if(mis_src_end == 6) {
           c[0] = *((unsigned char *) (&tmp2) + 0);
           c[1] = *((unsigned char *) (&tmp2) + 1);
           c[2] = *((unsigned char *) (&tmp1) + 2);
           c[3] = *((unsigned char *) (&tmp1) + 3);
         }
         else {
           c[0] = *((unsigned char *) (&tmp2) + 0);
           c[1] = *((unsigned char *) (&tmp2) + 1);
           c[2] = *((unsigned char *) (&tmp2) + 2);
           c[3] = *((unsigned char *) (&tmp1) + 3);
         }
         *(unsigned *) (dst_int+size_int) = tmp_datum_src_end0;
         *(unsigned *) (dst_int+size_int+4) = *(unsigned *) c;
      }
      else {
         *(unsigned *) (dst_int+size_int) = tmp_datum_src_end0;
         *(unsigned *) (dst_int+size_int+4) = dst_save_end1;
      }
      bytes_int += mis_src_end;
   }

   if(mis_dst)   
      return (void *) (dst + 8 - mis_dst);
   else
      return (void *) (dst_int);

}

static inline void mchan_memcpy_wait() {
   dma_barrier();
}

static inline void * mchan_memcpy(void *dst, void *src, unsigned int size) {
   void * r;
   r = mchan_memcpy_async(dst, src, size);
   mchan_memcpy_wait();
   return r;
}

