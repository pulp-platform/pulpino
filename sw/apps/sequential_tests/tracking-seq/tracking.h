//%%%%%%%%%%%%%%%%%%%%%%%%%%	* DEFINES * 	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
#define LX  		64     // horizontal image size
#define LY  		128    // vertical image size
#define	RLIM 		32768

#define B_MAX		16	// max number of blobs
#define INITMAX		16*4	// max numbers of new blobs


//%%%%%%%%%%%%%%%%%%%%%%%%%%	* PARAMETERS * 	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
#define RK_INI		5
#define DELTA		10
#define	DELTA_INIT 	5
#define RMAX	16
#define MIN_W	10

//%%%%%%%%%%%%%%%%%	* VARIABLE DEFINITION * %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
struct blob {	
		unsigned char 			xc;		// centroid x
		unsigned char 			yc;		// centroid y
		unsigned char	 		rx;		// x radius dimension
		unsigned char	 		rkx;		// x bounding dimension
		unsigned char 			ry;		// y radius dimension
		unsigned char	 		rky;		// y bounding dimension
		unsigned short int 		r2;		// square radius dimension
		unsigned short int 		W;		// weight = number of tracker's pixel
		unsigned char 			xmin;		// x min blob
		unsigned char 			xmax;		// x max blob
		unsigned char 			ymin;		// y min blob
		unsigned char	 		ymax;		// y max blob
		unsigned char	 		next;		// next pointer of the list
		unsigned int 			sum_xc;		// progressive sum of the x coordinates
		unsigned int 			sum_yc;		// progressive sum of the y coordinates
		};


//%%%%%%%%%%%%%%%%	* GLOBAL VARIABLES DECLARATION * %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
// Pixel List 
#include "image.h"
__attribute__ ((section(".heapsram"))) unsigned char 			pixel[LX*LY*2];
__attribute__ ((section(".heapsram"))) unsigned short int 		N_pixel;

//BLOB LISTS and utils
__attribute__ ((section(".heapsram"))) struct blob 		BLOB_LIST[B_MAX];		// main blob list
__attribute__ ((section(".heapsram"))) unsigned char	 	NB_BLOB;			// number of valid blob in list BLOB_LIST

__attribute__ ((section(".heapsram"))) struct blob 		listBlob[B_MAX + INITMAX];	// temp blob list for processing || B_MAX relative for prev blob & INITMAX relative for new blob
__attribute__ ((section(".heapsram"))) unsigned char	 	NnewBlob;			// number of new blobs initialized within init_blob process
//__attribute__ ((section(".heapsram"))) unsigned short int 	NBlob;				// 

//utils after filtering
__attribute__ ((section(".heapsram"))) unsigned char 		prevblob_flag; 			// true if blobs from previous list does exist			
__attribute__ ((section(".heapsram"))) unsigned char 		numBlob_prev;			// number of blobs from previous list
__attribute__ ((section(".heapsram"))) unsigned char 		newblob_flag; 			// true if new blobs does exist	
__attribute__ ((section(".heapsram"))) unsigned char 		numBlob_new;			// number of new blobs
__attribute__ ((section(".heapsram"))) unsigned char		index_start, index_end;		// index for first and last valid records in listBlob

//%%%%%%%%%%%%%%%%%%%%   * FUNCTIONS * %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//

unsigned char max (unsigned char a, unsigned char b)
{
	if(a>=b)	return (a);
	else		return (b);
}

unsigned char min (unsigned char a, unsigned char b)
{
	if(a>=b)	return (b);
	else		return (a);
}

void init_data();
void blob_formation();
void blob_init(unsigned char  x_t, unsigned char y_t ) ;
void prevBlob_filter();
void newBlob_filter();
void blob_merge();
void write_blob_list (unsigned char i, unsigned char j);

