#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "c_mpp_domains_helper.h"
#include "c_fms.h"

#define NX 16
#define NY 8

int main()
{

  int *global_pelist = NULL;
  int global_indices[4] = {0,NX-1, 0, NY-1};
  cDomainStruct domain;

  cFMS_init(NULL,NULL,NULL,NULL,NULL);
  cFMS_null_cdomain(&domain);

  //set domain fields to use the easy domain method
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
  int root_pe = cFMS_root_pe();
  bool is_root_pe = pe==root_pe ? true : false;

  //get pelist
  int pelist[4] = {0, 0, 0, 0};
  cFMS_get_domain_pelist(&ndivs, pelist, &domain_id);

  //set data to send
  int ij = 0;
  CFMS_TEST_KIND_ *send; send = (CFMS_TEST_KIND_ *)malloc(xsize*ysize*sizeof(CFMS_TEST_KIND_));
  for(int i=0; i<xsize; i++){
    for(int j=0; j<ysize; j++){
      send[ij++] = (CFMS_TEST_KIND_)((isc+i)*100 + j+jsc);
    }
  }

  //set data to receive
  CFMS_TEST_KIND_ *gather;
  int gather_shape[2] = {1,1};
  if(is_root_pe) {
    gather_shape[0] = NX;
    gather_shape[1] = NY;
    gather = (CFMS_TEST_KIND_ *)calloc(NX*NY, sizeof(CFMS_TEST_KIND_));
  }
  else gather = (CFMS_TEST_KIND_ *)malloc(1*sizeof(CFMS_TEST_KIND_));


  CFMS_GATHER_PELIST_2D_(&isc, &iec, &jsc, &jec, &ndivs, pelist,
                         send, gather_shape, gather, &is_root_pe, NULL, NULL, NULL);

  if(is_root_pe){
    int ij=0;
    for(int i=0; i<NX; i++){
      for(int j=0; j<NY; j++){
        if(gather[ij++] != (CFMS_TEST_KIND_)(i*100 + j)) cFMS_error(FATAL, "error testing cFMS_gather_pelist");
      }
    }
  }

  cFMS_end();
  return EXIT_SUCCESS;

}
