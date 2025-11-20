#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "c_constants.h"
#include "c_horiz_interp.h"
#include "c_fms.h"

void set_grid(double *lon_in, double *lat_in, double *lon_out, double *lat_out, char order);
void set_data(double *data_in, char order);

int nin = 5, nout = 5;
double din = 0.5, dout = 0.5;

int main()
{

  double *lon_in, *lat_in;
  double *lon_out, *lat_out;
  double *data_in, *data_out;

  lon_in = (double *)malloc((nin+1)*(nin+1)*sizeof(double));
  lat_in = (double *)malloc((nin+1)*(nin+1)*sizeof(double));
  lon_out = (double *)malloc((nout+1)*(nout+1)*sizeof(double));
  lat_out = (double *)malloc((nout+1)*(nout+1)*sizeof(double));

  data_in = (double *)malloc(nin*nin*sizeof(double));
  data_out = (double *)calloc(nout*nout, sizeof(double));

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

  cFMS_init(NULL,NULL,NULL,NULL,NULL);
  int two = 2; cFMS_horiz_interp_init(&two);

  //test module_is_initialized
  cFMS_horiz_interp_init(&two);

  bool module_is_initialized = c_horiz_interp_is_initialized();
  if(!module_is_initialized){
    cFMS_error(FATAL, "module_is_initialized");
    exit(EXIT_FAILURE);
  }
  
  bool convert_cf_order;

  convert_cf_order = true;
  {
    set_grid(lon_in, lat_in, lon_out, lat_out, 'C');
    int interp = cFMS_horiz_interp_new_2d_cdouble(&nin, &nin, &nout, &nout,
                                                  lon_in, lat_in, lon_out, lat_out,
                                                  mask_in, mask_out, interp_method,
                                                  verbose, max_dist, src_modulo,
                                                  is_latlon_in, is_latlon_out, &convert_cf_order);

    set_data(data_in, 'C');
    cFMS_horiz_interp_base_2d_cdouble(&interp, data_in, data_out, mask_in, mask_out, verbose, missing_value,
                                      missing_permit, new_missing_handle, &convert_cf_order);

    int ij = 0;
    for(int i=0; i<nout; i++){
      for(int j=0; j<nout; j++){
        assert(data_out[ij++] == (double)i);
      }
    }
  }

  convert_cf_order = false;
  {
    set_grid(lon_in, lat_in, lon_out, lat_out, 'F');
    int interp = cFMS_horiz_interp_new_2d_cdouble(&nin, &nin, &nout, &nout,
                                                  lon_in, lat_in, lon_out, lat_out,
                                                  mask_in, mask_out, interp_method,
                                                  verbose, max_dist, src_modulo,
                                                  is_latlon_in, is_latlon_out, &convert_cf_order);

    set_data(data_in, 'F');
    cFMS_horiz_interp_base_2d_cdouble(&interp, data_in, data_out, mask_in, mask_out, verbose, missing_value,
                                      missing_permit, new_missing_handle, &convert_cf_order);

    int ij = 0;
    for(int j=0; j<nout; j++){
      for(int i=0; i<nout; i++){
        assert(data_out[ij++] == (double)i);
      }
    }
  }

  cFMS_horiz_interp_end();
  cFMS_end();

  return EXIT_SUCCESS;

}

void set_grid(double *lon_in, double *lat_in, double *lon_out, double *lat_out, char order)
{

  char C = 'C';

  if(order == C){
    for(int i=0; i<nin+1; i++){
      for(int j=0; j<nin+1; j++){
        int ij = i*(nin+1) + j;
        lon_in[ij] = (double)i*din*DEG_TO_RAD;
        lat_in[ij] = (double)j*din*DEG_TO_RAD;
      }
    }
    for(int i=0; i<nout+1; i++){
      for(int j=0; j<nout+1; j++){
        int ij = i*(nout+1) + j;
        lon_out[ij] = (double)i*dout*DEG_TO_RAD;
        lat_out[ij] = (double)j*dout*DEG_TO_RAD;
      }
    }
  }
  else { //fortran ordered
    for(int j=0; j<nin+1; j++){
      for(int i=0; i<nin+1; i++){
        int ij = j*(nin+1) + i;
        lon_in[ij] = (double)i*din*DEG_TO_RAD;
        lat_in[ij] = (double)j*din*DEG_TO_RAD;
      }
    }
    for(int j=0; j<nout+1; j++){
      for(int i=0; i<nout+1; i++){
        int ij = j*(nout+1) + i;
        lon_out[ij] = (double)i*dout*DEG_TO_RAD;
        lat_out[ij] = (double)j*dout*DEG_TO_RAD;
      }
    }
  }//if

}

void set_data(double *data_in, char order)
{

  char C = 'C';

  if(order == C) {
    for(int i=0; i<nin; i++){
      for(int j=0; j<nin; j++){
        int ij = i*nin + j;
        data_in[ij] = (double)i;
      }
    }
  }
  else {
    for(int j=0; j<nin; j++){
      for(int i=0; i<nin; i++){
        int ij = j*nin + i;
        data_in[ij] = (double)i;
      }
    }
  }

}
