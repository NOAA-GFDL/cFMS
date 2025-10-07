#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "c_mpp_domains_helper.h"
#include "c_fms.h"

#define NX 16
#define NY 8

void (cFMS_gather_2d_cfloat(int*, int*, int*, int*, int*, int*, *double, int*, *double, bool*, int*, int*, bool*);

int main()
{

  int *global_pelist = NULL;
  int global_indices[4] = {0,NX-1, 0, NY-1};
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
      send[ij++] = (CFMS_TEST_KIND_)(isc+i)*100 + j+jsc;
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

  if(sizeof(CFMS_TEST_KIND_)==sizeof(float)){
    cFMS_gather_pelist_2d_cfloat(&isc, &iec, &jsc, &jec, &ndivs, pelist,
                                 send, gather_shape, gather, &is_root_pe, NULL, NULL, NULL);
  }
  else if(sizeof(CFMS_TEST_KIND_)==sizeof(double)){
    cFMS_gather_pelist_2d_cdouble(&isc, &iec, &jsc, &jec, &ndivs, pelist,
                                  send, gather_shape, gather, &is_root_pe, NULL, NULL, NULL);
  }
  /*
  else if(sizeof(CFMS_TEST_KIND_)==sizeof(int)){
    cFMS_gather_pelist_2d_cint(&isc, &iec, &jsc, &jec, &ndivs, pelist,
                                  send, gather_shape, gather, &is_root_pe, NULL, NULL, NULL);
  }
  */
  else {
    cFMS_error(FATAL, "variable type not known");
    return EXIT_FAILURE;
  }
    

  if(is_root_pe){
    int ij=0;
    for(int i=0; i<NX; i++){
      for(int j=0; j<NY; j++){
        assert(gather[ij++] == (CFMS_TEST_KIND_)i*100 + j);
      }
    }
  }
          
  return EXIT_SUCCESS;

}
