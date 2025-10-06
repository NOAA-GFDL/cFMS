#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "c_mpp_domains_helper.h"
#include "c_fms.h"

#define NX 47
#define NY 47

int main()
{

  int *global_pelist = NULL;
  int global_indices[4] = {0,NX, 0, NY};
  cDomainStruct domain;
  
  cFMS_init(NULL,NULL,NULL,NULL,NULL);
  cFMS_null_cdomain(&domain);

  //set domain fields to use the easy doamin method
  int ndivs = cFMS_npes();
  domain.layout = (int *)malloc(2*sizeof(int));
  domain.global_indices = global_indices;

  //set layout
  cFMS_define_layout(global_indices, &ndivs, domain.layout);

  //set domain
  int domain_id = cFMS_define_domains_easy(domain);

  //get compute domain
  int isc, iec, jsc, jec, xsize, ysize;
  cFMS_get_compute_domain(&domain_id, &isc, &iec, &jsc, &jec,
                          &xsize, NULL, &ysize, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

  //get pe
  int pe = cFMS_pe();
  bool is_root_pe = pe==0 ? true : false;

  //get pelist
  int pelist[4] = {0, 0, 0, 0};
  cFMS_get_domain_pelist(&ndivs, pelist, &domain_id);
  
  //set data to send
  int ij = 0;
  float *send; send = (float *)malloc(xsize*ysize*sizeof(float));
  for(int i=0; i<xsize; i++){
    for(int j=0; j<ysize; j++){
      send[ij++] = pe*1000 + i*10 + j;
    }
  }

  //set data to receive
  float *gather;
  int gather_shape[2] = {1,1};
  if(pe==0) {
    gather_shape[0] = NX;
    gather_shape[1] = NY;
    gather = (float *)calloc(NX*NY, sizeof(float));
  }
  else gather = (float *)malloc(1*sizeof(float));  

  cFMS_gather_pelist_2d_cfloat(&isc, &iec, &jsc, &jec, &ndivs, pelist,
                               send, gather_shape, gather, &is_root_pe,NULL, NULL, NULL);

  return EXIT_SUCCESS;

}
