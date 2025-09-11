#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "c_constants.h"
#include "c_horiz_interp.h"
#include "c_fms.h"

int Nin = 5;
int Nout = 5;
double din = 0.5; //degrees
double dout = 0.5; //degrees

int main()
{
   
  double *lon_in, *lat_in;
  double *lon_out, *lat_out;
  double *data_in;
  double *data_out;

  int lonlat_in_shape[2] = {Nin+1, Nin+1};
  int lonlat_out_shape[2] = {Nout+1, Nout+1};
  int data_in_shape[2] = {Nin, Nin};
  int data_out_shape[2] = {Nout, Nout};

  lon_in = (double *)malloc((Nin+1)*(Nin+1)*sizeof(double));
  lat_in = (double *)malloc((Nin+1)*(Nin+1)*sizeof(double));
  lon_out = (double *)malloc((Nout+1)*(Nout+1)*sizeof(double));
  lat_out = (double *)malloc((Nout+1)*(Nout+1)*sizeof(double));

  data_in = (double *)malloc(Nin*Nin*sizeof(double));
  data_out = (double *)malloc(Nout*Nout*sizeof(double));

  //input grid
  for(int i=0; i<Nin+1; i++){
    for(int j=0; j<Nin+1; j++){
      int ij = i*(Nin+1) + j;
      lon_in[ij] = (double)i*din*DEG_TO_RAD;
      lat_in[ij] = (double)j*din*DEG_TO_RAD;
    }
  }

  //output grid
  for(int i=0; i<Nout+1; i++){
    for(int j=0; j<Nout+1; j++){
      int ij = i*(Nout+1) + j;
      lon_out[ij] = (double)i*dout*DEG_TO_RAD;
      lat_out[ij] = (double)j*dout*DEG_TO_RAD;
    }
  }
  
  //input data
  for(int j=0; j<Nin; j++){
    for(int i=0; i<Nin; i++){
      int ij = j*Nin + i;
      data_in[ij] = (double)ij;
    }
  }

  //initialize output data
  for(int j=0; j<Nout; j++){
    for(int i=0; i<Nout; i++){
      int ij = j*Nout + i;
      data_out[ij] = 0.0;
    }
  }
  
  char interp_method[MESSAGE_LENGTH] = "conservative";
  int *verbose = NULL;
  double *max_dist = NULL;
  bool *src_modulo = NULL;
  double *mask_in = NULL;
  double *mask_out = NULL;
  bool *is_latlon_in = NULL;
  bool *is_latlon_out = NULL;

  int one = 1;
  
  cFMS_init(NULL,NULL,NULL,NULL,NULL);
  cFMS_horiz_interp_init(&one);
  int interp = cFMS_horiz_interp_new_2d_cdouble(lon_in, lat_in, lonlat_in_shape,
                                                lon_out, lat_out, lonlat_out_shape,
                                                mask_in, mask_out, interp_method,
                                                verbose, max_dist, src_modulo,
                                                is_latlon_in, is_latlon_out);
  
  
  double *missing_value = NULL;
  int *missing_permit = NULL;
  bool *new_missing_handle = NULL;
  cFMS_horiz_interp_base_2d_cdouble(&interp, data_in, data_in_shape, data_out, data_out_shape,
                                    mask_in, mask_out, verbose, missing_value, missing_permit,
                                    new_missing_handle);

  for(int i=0; i<Nout*Nout; i++) assert(data_out[i] == (double)i);

  cFMS_horiz_interp_end();
  
  return EXIT_SUCCESS;

}
