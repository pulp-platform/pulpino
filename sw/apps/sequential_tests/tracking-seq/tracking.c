/* Copyright (C) 2017 ETH Zurich, University of Bologna
 * All rights reserved.
 *
 * This code is under development and not yet released to the public.
 * Until it is released, the code is under the copyright of ETH Zurich and
 * the University of Bologna, and may contain confidential and/or unpublished
 * work. Any reuse/redistribution is strictly forbidden without written
 * permission from ETH Zurich.
 *
 * Bug fixes and contributions will eventually be released under the
 * SolderPad open hardware license in the context of the PULP platform
 * (http://www.pulp-platform.org), under the copyright of ETH Zurich and the
 * University of Bologna.
 */

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//

#include <timer.h>
#include "tracking_functions.c"		

#include "result.h"			// for checksum


int main() {
	
	/* 	Variable Definition	*/
	int coreid;
	int i,j;
	int start_frame;
	int index;
	int time;

	
	coreid = get_core_id();
	if (coreid == 0) {
		//	initialization
		NB_BLOB = 0	;	
		start_frame = 0;

		for(i=0;i<NFRAME;i++){	// for each frame
			
			//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
			//%%%%%%%%%%%%%%%	* DATA TRANSFER FROM L2 TO L1 *	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//

			index = 0;
			N_pixel = N_pixelL2[i];
			
reset_timer();
start_timer();
			for(j=start_frame; j<start_frame + N_pixel*2; j++){
				pixel[index++]=pixelL2[j];
			}
stop_timer();
printf("FRAME: %d (%d-%d) Transfer Time: %d\n",i,start_frame,j,get_time());

			start_frame = j;
			//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//


			//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
			//%%%%%%%%%%%%%%%	* PROCESSING *		%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//

			init_data();
reset_timer();
start_timer();
			blob_formation();
stop_timer();
printf("Blob Formation Time: %d\n",get_time(),0,0,0);

reset_timer();
start_timer();
			prevBlob_filter();
stop_timer();
printf("Filtering prev Blob List Time: %d\n",get_time(),0,0,0);

reset_timer();
start_timer();
			newBlob_filter();
stop_timer();
printf("Filtering new Blob List Time: %d\n",get_time(),0,0,0);

reset_timer();
start_timer();
			blob_merge();
stop_timer();
printf("Blob Merging Time: %d\n",get_time(),0,0,0);
			//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
			

			//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
			//%%%%%%%%%%%%%%%	* CHECKSUM	 *		%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//

			printf("FRAME = %d\n",i,0,0,0);
			printf("NB_BLOB = %d\n",NB_BLOB,0,0,0);
			
			for(j=0;j<NB_BLOB;j++){
				printf("Blob %d: centroid = (%d,%d), weight = %d, ",j,BLOB_LIST[j].xc,BLOB_LIST[j].yc,BLOB_LIST[j].W);
				printf("xmax = %d, xmin = %d, ymax = %d, ymin = %d\n",BLOB_LIST[j].xmax, BLOB_LIST[j].xmin, BLOB_LIST[j].ymax, BLOB_LIST[j].ymin);
				if(BLOB_LIST[j].xc == results[(i*B_MAX+j)*6 ]) 		printf("OK xc!\t",0,0,0,0);	else	printf("FAIL xc!\t",0,0,0,0);
				if(BLOB_LIST[j].yc == results[(i*B_MAX+j)*6+1 ]) 	printf("OK yc!\t",0,0,0,0);	else	printf("FAIL yc!\t",0,0,0,0);
				if(BLOB_LIST[j].xmax == results[(i*B_MAX+j)*6+2 ]) 	printf("OK xmax!\t",0,0,0,0);	else	printf("FAIL xmax!\t",0,0,0,0);
				if(BLOB_LIST[j].xmin == results[(i*B_MAX+j)*6+3 ]) 	printf("OK xmin!\t",0,0,0,0);	else	printf("FAIL xmin!\t",0,0,0,0);
				if(BLOB_LIST[j].ymax == results[(i*B_MAX+j)*6+4 ]) 	printf("OK ymax!\t",0,0,0,0);	else	printf("FAIL ymax!\t",0,0,0,0);
				if(BLOB_LIST[j].ymin == results[(i*B_MAX+j)*6+5 ]) 	printf("OK ymin!\n",0,0,0,0);	else	printf("FAIL ymin!\n",0,0,0,0);
			}
			printf("\n\n",0,0,0,0);
			//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
		}

		eoc(0);	
	}
	
	
}
