#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "c_constants.h"
#include "c_fms.h"
#include "c_horiz_interp.h"

#define NX 12
#define NY 24

int main()
{

  int nx_src = NX;
  int ny_src = NY;
  int nx_dst = 2*NX;
  int ny_dst = 2*NY;
  double *lon_src, *lat_src, *lon_dst, *lat_dst;
  double *data_src, *data_answer, *data_test;

  int interp_id_answer, interp_id_test;
    
  //set source grid
  lon_src = (double *)malloc((nx_src+1)*(ny_src+1)*sizeof(double));
  lat_src = (double *)malloc((nx_src+1)*(ny_src+1)*sizeof(double));
  int ij = 0;
  for(int j=0; j<ny_src+1; j++){
    for(int i=0; i<nx_src+1; i++){
      lon_src[ij] = 2*i*DEG_TO_RAD;
      lat_src[ij++] = 2*j*DEG_TO_RAD;
    }
  }
  
  //set dst grid
  lon_dst = (double *)malloc((nx_dst+1)*(ny_dst+1)*sizeof(double));
  lat_dst = (double *)malloc((nx_dst+1)*(ny_dst+1)*sizeof(double));
  ij = 0;
  for(int j=0; j<ny_dst+1; j++){
    for(int i=0; i<nx_dst+1; i++){
      lon_dst[ij] = i*DEG_TO_RAD;
      lat_dst[ij++] = j*DEG_TO_RAD;
    }
  }
  
  //source data  
  data_src = (double *)malloc(nx_src*ny_src*sizeof(double));
  for(int ij=0; ij<nx_src*ny_src; ij++) data_src[ij] = ij;

  //dst data
  data_answer = (double *)calloc(nx_dst*ny_dst, sizeof(double));    

  //initialize
  int two = 2;
  cFMS_init(NULL, NULL, NULL, NULL, NULL);
  cFMS_horiz_interp_init(&two);
  
  //arguments for horiz_interp
  char interp_method[MESSAGE_LENGTH] = "conservative";
  int *verbose = NULL;
  double *max_dist = NULL;
  bool *src_modulo = NULL;
  double *mask_in = NULL;
  double *mask_out = NULL;
  bool *is_latlon_in = NULL;
  bool *is_latlon_out = NULL;
  double *missing_value = NULL;
  int *missing_permit = NULL;
  bool *new_missing_handle = NULL;
  bool convert_cf_order = false;

  //get answer interp
  interp_id_answer = cFMS_horiz_interp_new_2d_cdouble(&nx_src, &ny_src, &nx_dst, &ny_dst,
                                                      lon_src, lat_src, lon_dst, lat_dst,
                                                      mask_in, mask_out, interp_method,
                                                      verbose, max_dist, src_modulo,
                                                      is_latlon_in, is_latlon_out, &convert_cf_order);

  //get answer data
  cFMS_horiz_interp_base_2d_cdouble(&interp_id_answer, data_src, data_answer, mask_in, mask_out,
                                    verbose, missing_value, missing_permit, new_missing_handle,
                                    &convert_cf_order);
  
  //construct new interp
  int interp_id = interp_id_answer + 1;
  int nxgrid;
  int *i_src, *j_src, *i_dst, *j_dst;
  double *area_frac_dst;

  //get data to construct new interp
  cFMS_get_nxgrid(&interp_id_answer, &nxgrid);
  i_src = (int *)calloc(nxgrid, sizeof(int));
  j_src = (int *)calloc(nxgrid, sizeof(int));
  i_dst = (int *)calloc(nxgrid, sizeof(int));
  j_dst = (int *)calloc(nxgrid, sizeof(int));
  area_frac_dst = (double *)calloc(nxgrid, sizeof(double));
  
  cFMS_get_i_src(&interp_id_answer, i_src);
  cFMS_get_j_src(&interp_id_answer, j_src);
  cFMS_get_i_dst(&interp_id_answer, i_dst);
  cFMS_get_j_dst(&interp_id_answer, j_dst);
  cFMS_get_area_frac_dst_cdouble(&interp_id_answer, area_frac_dst);

  //construct new interp
  cFMS_construct_interp(&interp_id, &nxgrid, i_src, j_src, i_dst, j_dst, area_frac_dst);  

  //get data_test
  data_test = (double *)calloc(nx_dst*ny_dst, sizeof(double));
  
  cFMS_horiz_interp_base_2d_cdouble(&interp_id_answer, data_src, data_test, mask_in, mask_out,
                                    verbose, missing_value, missing_permit, new_missing_handle,
                                    &convert_cf_order);

  //check answers
  for(int ij=0; ij<nx_dst*ny_dst; ij++){
    if(data_answer[ij] != data_test[ij]){      
      printf("%d %lf %lf\n", ij, data_answer[ij], data_test[ij]);
      cFMS_error(FATAL, "error setting up new interp type");
    }
  }
 
  return EXIT_SUCCESS;

}
