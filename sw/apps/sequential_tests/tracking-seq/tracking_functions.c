// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

#include "tracking.h"	// data structures definitions


void init_data()
{

	unsigned char	 i;


	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
	// 		listBlob structture initailization
	//some structure datas must be resetted respect to the previous acquisition

	NnewBlob		 = 0 ;

	if (NB_BLOB > 0 ){

		for (i=0; i< NB_BLOB; i++) {

			listBlob[i].xc		=	0;
			listBlob[i].yc		=	0;
			listBlob[i].W		=	0;
			listBlob[i].xmin	=	64;
			listBlob[i].xmax	=	0;
			listBlob[i].ymin	=	128;
			listBlob[i].ymax	=	0;
			listBlob[i].sum_xc 	=  	0;
			listBlob[i].sum_yc 	=  	0;
		}
	}

	// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% //
}





void blob_formation(){
//	the function fills up the list of blobs features (listBlob) by scanning pixel list and by referring to the last BLOB_LIST (from the last frame)
//	* listBlob[ 1 : B_MAX]			-->	pixel are aggregated based on informations of previous blob list (BLOB_LIST)
//	* listBlob[ B_MAX : B_MAX + INITMAX]	-->	new blobs
//	if a pixel cannot be placed in one of the previous formed blob, new blob will be formed (with init_blob function)

	//Variables
	unsigned short int 	i,j;
	unsigned char 		x_t, y_t; 					// pixel coordinates
	unsigned char	 	tag; 						// identify the blob for pixel's assignment

	unsigned char	 	rx_t, ry_t, xmax_t, xmin_t, ymax_t, ymin_t;
	unsigned short int	r2min_t, r2_t, W_t;


	unsigned short int 	NleftPixel;					// number of pixel assigned with init procedure

	int time;

	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
	//		* ASSIGNMENT PROCEDURE *
	// pixel list is read and the features list is filled

	if (NB_BLOB > 0 ){	// blobs does exist on previous frame therefore BLOB_LIST contains relevant values

		NleftPixel 	= 0;

		for(i = 0; i < N_pixel*2;) {   //check for each pixel
		// pixel (x_t,y_t) may be assigned to one of the blob (identified by the tag).
		// every existent blob has a bounding box (rkx,rky)
		// if rx=|x_t - xj| < rkx and ry=|y_t - yj| < rky, the radius r2 = rx^2 + ry^2 is calculated.
		// the pixel is assigned to the closer blob (min r2)

			r2min_t = RLIM ;		// to be minimized
			tag 	= NB_BLOB ;		// if tag = NB_BLOB, pixel is assigned to none

			y_t      = (pixel[i++] & 0x7F);
			x_t      = (pixel[i++] & 0x3F);

			for (j=0; j < (unsigned short int)NB_BLOB; j++) { 	// for each existent blob

				// ry = |y_t - yj|
				if(y_t > BLOB_LIST[j].yc) 	ry_t = (y_t - BLOB_LIST[j].yc);
				else 				ry_t = ( BLOB_LIST[j].yc - y_t );

				if(ry_t <= BLOB_LIST[j].rky){

					// rx = |x_t - xj|
					if(x_t > BLOB_LIST[j].xc) 	rx_t = (x_t - BLOB_LIST[j].xc);
					else 				rx_t = (BLOB_LIST[j].xc - x_t);

					if(rx_t <= BLOB_LIST[j].rkx) {

						r2_t = (unsigned short int)((rx_t * rx_t) + (ry_t * ry_t));

						if(r2_t <= r2min_t) {
							r2min_t = r2_t ;
							tag = j ;
						} //if r2min

					}//if rx
				}//if ry
			}//for each prev blob

			if(tag < NB_BLOB ) {  	// pixel (x_t,y_t) is assigned to one of the previous blob
				//  features update in listBlob list
				W_t = listBlob[tag].W;
				listBlob[tag].sum_xc 	= listBlob[tag].sum_xc + (unsigned int) x_t;	// progressive sum of x coord for next centroid calculation
				listBlob[tag].sum_yc 	= listBlob[tag].sum_yc + (unsigned int)y_t;	// progressive sum of y coord for next centroid calculation
				if(x_t < listBlob[tag].xmin) 	listBlob[tag].xmin = x_t;
				if(x_t > listBlob[tag].xmax) 	listBlob[tag].xmax = x_t;
				if(y_t < listBlob[tag].ymin) 	listBlob[tag].ymin = y_t;
				if(y_t > listBlob[tag].ymax) 	listBlob[tag].ymax = y_t;
				listBlob[tag].W = W_t + 1;

			} else { 	// pixel (x_t,y_t) was NOT assigned to one of the previous blob

				NleftPixel = NleftPixel + 1;
				blob_init(x_t, y_t);		// assign the pixel to one of new blobs
			}

		}//for each pixel in the list

	} else { // NO blobs does exist on previous frame

		NleftPixel = 	N_pixel;	// all pixel will be assigned with init procedure

		for(i = 0; i < N_pixel*2;) {    // check for each pixel

			y_t      = (pixel[i++] & 0x7F);
			x_t      = (pixel[i++] & 0x3F);
			blob_init(x_t, y_t);			// assign the pixel to one of new blobs

		}//for each pixel

	} //endif NB_BLOB>0
}



void blob_init (unsigned char  x_t, unsigned char y_t )
// the function receives coordinates of pixel not assigned in previous existent blobs (which may not exist)
// and fills the listBlob section dedicated to new blobs. The dimension of new blob list is returned in NnewBlob
{
	//Variables
	unsigned char	 	i,j;
	unsigned char	 	tag;
	unsigned char		rx_t, ry_t, xmax_t, xmin_t, ymax_t, ymin_t;
	unsigned short int 	r2min_t, r2_t, W_t, Wnext_t;

	unsigned char		tempc;
	unsigned char		compar;		// flag for updating existent blob
	int time;

	unsigned char	 	base;		// base index for section of listBlob dedicated to new blob



	// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% //
	//		* INIT ASSIGNMENT PROCEDURE *

	base = B_MAX;

	if(NnewBlob > 0 ) {		// if new blobs have already been formed
	// check if pixel (x_t,y_t) can be assigned to one of the new blob
	// the procedure is the same as before.
	// the number of valid blob in the list is returned in NnewBlob

		r2min_t = RLIM;
		tag 	= base + NnewBlob ;	//new tracker flag

		for (i=base; i < base + NnewBlob; i++) {

			if (y_t > listBlob[i].yc)	ry_t = (y_t - listBlob[i].yc);
			else 				ry_t = (listBlob[i].yc - y_t);

			if(ry_t <= listBlob[i].rky){

				if(x_t > listBlob[i].xc) 	rx_t = (x_t - listBlob[i].xc);
				else 				rx_t = (listBlob[i].xc - x_t);

				if(rx_t <= listBlob[i].rkx) {

					r2_t = (unsigned short int)((rx_t * rx_t) + (ry_t * ry_t));
					if(r2_t <= r2min_t) {
						r2min_t = r2_t ;
						tag = i ;
					} //if r2min

				}//if rx
			}//if ry
		}//for each present blob

		if( tag < base + NnewBlob ) {	// pixel (x_t,y_t) is assigned to one of the new blob
		// features updating in listBlob structure

			W_t 			= listBlob[tag].W ;
			Wnext_t 		= W_t + 1;
			listBlob[tag].sum_xc 	= listBlob[tag].sum_xc + (unsigned int)x_t;
			listBlob[tag].sum_yc 	= listBlob[tag].sum_yc + (unsigned int)y_t;
			listBlob[tag].xc	= listBlob[tag].sum_xc / Wnext_t;
			listBlob[tag].yc 	= listBlob[tag].sum_yc / Wnext_t;


			//x blob coordinates update only if min/max change
			compar = 0;
			if (x_t < listBlob[tag].xmin) 		{ listBlob[tag].xmin = x_t; compar = 1;}
			else if(x_t > listBlob[tag].xmax) 	{ listBlob[tag].xmax = x_t; compar = 1;}
			if (compar == 1) {
				tempc 			= listBlob[tag].xmax - listBlob[tag].xmin;
				tempc 			= (tempc >> 1); //rmax - rmin / 2
				listBlob[tag].rx 	= tempc;
				tempc 			= tempc + DELTA_INIT; //rx + DELTA_INIT
				listBlob[tag].rkx 	= min(tempc, RMAX);
			}

			//y blob coordinates update only if min/max change
			compar = 0;
			if (y_t < listBlob[tag].ymin) 		{ listBlob[tag].ymin = y_t; compar = 1;}
			else if(y_t > listBlob[tag].ymax) 	{ listBlob[tag].ymax = y_t; compar = 1;}
			if (compar == 1) {
				tempc 			= listBlob[tag].ymax - listBlob[tag].ymin;
				tempc 			= (tempc >> 1); //rmax - rmin / 2
				listBlob[tag].ry 	= tempc;
				tempc 			= tempc + DELTA_INIT; //rx + DELTA_INIT
				listBlob[tag].rky 	= min(tempc, RMAX);
			}

			//weight update
			listBlob[tag].W = Wnext_t;


		} else { 	// pixel (x_t,y_t) NOT assigned to one of the new blob

			// new blob formation -> NnewBlob increase
			tag = NnewBlob+ base;

			NnewBlob 		= NnewBlob + 1 ;
			listBlob[tag].sum_xc 	= x_t;
			listBlob[tag].sum_yc 	= y_t;
			listBlob[tag].xc 	= x_t;
			listBlob[tag].yc 	= y_t;
			listBlob[tag].rx 	= 0 ;
			listBlob[tag].rkx 	= RK_INI;
			listBlob[tag].ry 	= 0;
			listBlob[tag].rky 	= RK_INI;
			listBlob[tag].r2 	= 1;
			listBlob[tag].W 	= 1;
			listBlob[tag].xmin 	= x_t;
			listBlob[tag].xmax 	= x_t;
			listBlob[tag].ymin 	= y_t;
			listBlob[tag].ymax 	= y_t;

		}

	} else {	// no one new blob does exist

		// new blob formation -> NnewBlob increase
		tag = NnewBlob+ base;

		NnewBlob 		= NnewBlob + 1 ;
		listBlob[tag].sum_xc 	= x_t;
		listBlob[tag].sum_yc 	= y_t;
		listBlob[tag].xc 	= x_t;
		listBlob[tag].yc 	= y_t;
		listBlob[tag].rx 	= 0 ;
		listBlob[tag].rkx 	= RK_INI;
		listBlob[tag].ry 	= 0;
		listBlob[tag].rky 	= RK_INI;
		listBlob[tag].r2 	= 1;
		listBlob[tag].W 	= 1;
		listBlob[tag].xmin 	= x_t;
		listBlob[tag].xmax 	= x_t;
		listBlob[tag].ymin 	= y_t;
		listBlob[tag].ymax 	= y_t;

	}//if NB_BLOB>0



}



void prevBlob_filter()
//	filtering of the blob list formed from prevoius blob information
//	all blob with less than MIN_W pixels are cutted off from the list
//	some last operations are performed in this step (centroid and bounding box calculation)
{
	// Variables
	unsigned char		i ;
	unsigned short int	W_t ;
	unsigned char	 	tempx, tempy;

	unsigned char 		flag;
	int time;


	numBlob_prev	= 0 ;	// number of previous blob after filtering
	prevblob_flag 	= 0 ; 	// prev blob are not present

	if (NB_BLOB > 0) {	// previous blob were present
		for(i = 0; i < NB_BLOB; i++){	//radius and centroid calculus

			W_t = listBlob[i].W;

			if(W_t > MIN_W) {	// a blob is valid if it does group more than MIN_W pixels

				listBlob[i].xc   	= (unsigned char) (listBlob[i].sum_xc / W_t);
				listBlob[i].yc   	= (unsigned char) (listBlob[i].sum_yc / W_t);
				tempx 			= ( listBlob[i].xmax - listBlob[i].xmin ) >> 1 ;
				listBlob[i].rx   	= tempx ;
				listBlob[i].rkx  	= min (RMAX, tempx+DELTA);
				tempy 			= ( listBlob[i].ymax - listBlob[i].ymin ) >> 1 ;
				listBlob[i].ry   	= tempy ;
				listBlob[i].rky  	= min (RMAX, tempy+DELTA);
				listBlob[i].r2   	= (unsigned short int) ((tempx * tempx ) + (tempy * tempy));

				numBlob_prev		= numBlob_prev + 1;

				// update listBlob by managing the pointers
				if(prevblob_flag == 0 )	{
					index_start	 		= i;
					index_end 			= i;
					prevblob_flag 			= 1;
				} else {
					listBlob[index_end].next 	= i;
					index_end 			= i;
				}

				listBlob[index_end].next 	= i+1;

			}

		}//end scan prev tracker


	}//end if
}



void newBlob_filter()
//	filtering of newblob list. all new blob with less than MIN_W are cutted off
{

	// Variables
	unsigned char		i ;
	unsigned short int	W_t ;
	unsigned char 		flag;
	int 			time;
	unsigned char	 	base;

	numBlob_new 		= 0; 	// number of new blobs after filtering
	newblob_flag		= 0; 	// new blob are not present in the list

 	if(NnewBlob > 0) {

		base = B_MAX;

		for(i = base; i< ( base + NnewBlob ); i++) {

			W_t = listBlob[i].W;

			if(W_t > MIN_W) {	// a blob is valid if it does group more than MIN_W pixels

				numBlob_new 	= numBlob_new + 1 ;

				// update listBlob by managing the pointers
				if(newblob_flag == 0 )	{
					if( prevblob_flag == 1 )	listBlob[index_end].next 	= i ;
					else				index_start			= i ;
					index_end			= i;
					newblob_flag			= 1;
				} else {
					listBlob[index_end].next 	= i;
					index_end		 	= i;
				}

				listBlob[index_end].next 	= i+1;

			}

		}//end scan init tracker


	}
}



void blob_merge()
// the function merges the previous formed blob list (listBlob) and fills up
// the final BLOB_LIST structure ( with dimension NB_BLOB ).
// the listBlob must be scanned from index_start to index_end by following .next pointers
{

	unsigned char	 	i, j, j_prev ;
	unsigned char	 	coll_flag ;		// true if a merge process happens
	unsigned char		rx_t, ry_t, xmax_t, xmin_t, ymax_t, ymin_t ;
	unsigned char	 	deltaRx, deltaRy ;
	unsigned short int 	r2min_t, r2_t, W_t ;

	unsigned char		NtotBlob;			// tot blobs on listBlob structure


	// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% //
	//			*	MERGE PROCESS	*
	// If two blobs i and j are close, the blob j will be incorporate in blob i.
	// dx = |x_i - x_j| and dy = |y_i - y_j|
	// if { (dx < rkx_i) OR (dx < rkx_j) } AND { (dy < rky_i) OR (dy < rky_j) } --> MERGE
	//

	NB_BLOB 	= 0;	// init number of final blobs
	NtotBlob 	= numBlob_prev + numBlob_new ;

	if (NtotBlob > 1){

		i 	= index_start ;
		j_prev 	= index_start ;
		j 	= listBlob[i].next ;

		while(j <= index_end) { //until j blob is valid and correspondent i blob is valid as well

			coll_flag = 0;	// flag to indicate merging of j into i blob

			while(j <= index_end) {//scan for all the j blob

				if(listBlob[i].yc > listBlob[j].yc) 	deltaRy = (listBlob[i].yc - listBlob[j].yc);
				else 					deltaRy = (listBlob[j].yc - listBlob[i].yc);

				if ((deltaRy <= listBlob[i].rky) || (deltaRy <= listBlob[j].rky))   { // check y coordinates for merging

					if(listBlob[i].xc > listBlob[j].xc) 	deltaRx = (listBlob[i].xc - listBlob[j].xc);
					else 					deltaRx = (listBlob[j].xc - listBlob[i].xc);

					if((deltaRx <= listBlob[i].rkx) || (deltaRx <= listBlob[j].rkx)) {	// check x coordinates for merging

						// merging process of j blob into i blob
						listBlob[i].sum_xc 	= listBlob[i].sum_xc + listBlob[j].sum_xc;
						listBlob[i].xc 		= (unsigned char) (listBlob[i].sum_xc / (listBlob[i].W + listBlob[j].W));
						listBlob[i].sum_yc 	= listBlob[i].sum_yc + listBlob[j].sum_yc;
						listBlob[i].yc 		= (unsigned char) (listBlob[i].sum_yc / (listBlob[i].W + listBlob[j].W));
						xmax_t 			= max(listBlob[i].xmax, listBlob[j].xmax);
						xmin_t 			= min(listBlob[i].xmin, listBlob[j].xmin);
						ymax_t 			= max(listBlob[i].ymax, listBlob[j].ymax);
						ymin_t 			= min(listBlob[i].ymin, listBlob[j].ymin);
						listBlob[i].xmax 	= xmax_t;
						listBlob[i].xmin 	= xmin_t;
						listBlob[i].ymax 	= ymax_t;
						listBlob[i].ymin 	= ymin_t;
						rx_t 			= (xmax_t - xmin_t) >> 1;
						ry_t 			= (ymax_t - ymin_t) >> 1;
						listBlob[i].rx   	= rx_t ;
						listBlob[i].rkx  	= min (RMAX, rx_t + DELTA);
						listBlob[i].ry   	= ry_t ;
						listBlob[i].rky  	= min (RMAX, ry_t + DELTA);
						listBlob[i].r2   	= (unsigned short int)((rx_t * rx_t ) + (ry_t * ry_t));
						listBlob[i].W    	= listBlob[i].W + listBlob[j].W;

						listBlob[j_prev].next 	= listBlob[j].next ;	// update list -j is removed from the list

						j 			= index_end + 1 ; 	// next j blob is not valid
						coll_flag 		= 1;
					} else	{ j_prev = j;	j = listBlob[j].next;	}// no merge -> next j blob


				} else { // no merge -> next j blob
					j_prev 		= j;
					j 		= listBlob[j].next;
				}//endif

			}// scan of j blobs ended

			//update next i blob and final list
			if(!coll_flag) {
			// if i blob hasn't merged any j blob it will be transferred into official blob list
			// else check if new bigger blob can incorporate any other j blob

				write_blob_list(NB_BLOB,i);
				NB_BLOB				= NB_BLOB + 1 ;
				i				= listBlob[i].next ;
			}
			//update next j blob
			j_prev 		= i;
			j		= listBlob[i].next;

		} //end while i

		if (i <= index_end) {	//write last blob
		write_blob_list(NB_BLOB,i);
		NB_BLOB				= NB_BLOB + 1 ;
		}

	} else { //less than two trackers

		if (NtotBlob == 1) { 	write_blob_list( NB_BLOB , index_start ); 	NB_BLOB = 1;	}
	}

}


void write_blob_list (unsigned char i, unsigned char j)
// copy of valid blob in the final list
{
	BLOB_LIST[i].xc		= listBlob[j].xc ;
	BLOB_LIST[i].yc		= listBlob[j].yc ;
	BLOB_LIST[i].xmax	= listBlob[j].xmax ;
	BLOB_LIST[i].xmin	= listBlob[j].xmin ;
	BLOB_LIST[i].ymax	= listBlob[j].ymax ;
	BLOB_LIST[i].ymin	= listBlob[j].ymin ;
	BLOB_LIST[i].rx		= listBlob[j].rx ;
	BLOB_LIST[i].rkx	= listBlob[j].rkx ;
	BLOB_LIST[i].ry		= listBlob[j].ry ;
	BLOB_LIST[i].rky	= listBlob[j].rky ;
	BLOB_LIST[i].r2		= listBlob[j].r2 ;
	BLOB_LIST[i].W		= listBlob[j].W ;
}
