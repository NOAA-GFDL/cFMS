#include <stdio.h>
#include <stdlib.h>
#include <c_fms.h>
#include <c_mpp_domains_helper.h>

int main()
{

  // global domain
  //      *      *     *     *
  //      *      *     *     *
  // * * (2,5) (3,5) (4,5) (5,5) * * 
  // * * (2,4) (3,4) (4,4) (5,4) * *
  // * * (2,3) (3,3) (4,3) (5,3) * *
  // * * (2,2) (3,2) (4,2) (5,2) * *
  //      *      *     *     *
  //      *      *     *     *

  cDomainStruct domain;  
  int domain_id = -99;
  int ndiv = 4;
  int global_indices[] = {0,3,0,3};
  int whalo = 2;
  int ehalo = 2;
  int shalo = 2;
  int nhalo = 2;
  char name[NAME_LENGTH] = "test domain";
  
  cFMS_init(NULL,NULL, NULL, NULL, NULL);
  cFMS_null_cdomain(&domain);
  
  //set domain 
  { 
    domain.global_indices = global_indices;
    domain.whalo = &whalo;
    domain.ehalo = &ehalo;
    domain.shalo = &shalo;
    domain.nhalo = &nhalo;
    domain.name = name;
    
    domain.layout = (int *)malloc(2*sizeof(int));
    cFMS_define_layout(global_indices, &ndiv, domain.layout);
    
    domain_id = cFMS_define_domains_easy(domain);
    if( !cFMS_domain_is_initialized(&domain_id) ) cFMS_error(FATAL, "error in setting domain");
  }

  cFMS_set_current_pelist(NULL, NULL);

  //flipping the domain
  //pe 0:  isc=2, iec=3, jsc=2, jec=3 --> pe 3
  //pe 1:  isc=4, iec=5, jsc=2, jec=3 --> pe 2
  //pe 2:  isc=2, iec=3, jsc=4, jec=5 --> pe 1
  //pe 3:  isc=4, iec=5, jsc=4, jec=5 --> pe 0  
  int isc[4] = {4,2,4,2};
  int iec[4] = {5,3,5,3};
  int jsc[4] = {4,4,2,2};
  int jec[4] = {5,5,3,3};

  //pe 0:  isd=0, ied=5, jsd=0, jed=5 --> pe 3
  //pe 1:  isd=2, ied=7, jsd=0, jed=5 --> pe 2
  //pe 2:  isd=0, ied=5, jsd=2, jed=7 --> pe 1
  //pe 3:  isd=2, ied=7, jsd=2, jed=7 --> pe 0  
  int isd[4] = {2,0,2,0};
  int ied[4] = {7,5,7,5};
  int jsd[4] = {2,2,0,0};
  int jed[4] = {7,7,5,5};

  int xsize, ysize;
  int pe = cFMS_pe();
  int tile_count = 0; //default
  bool x_is_global = false; //default
  bool y_is_global = false; //default

  // set compute and data domains
  { 
    xsize=2; ysize=2;
    cFMS_set_compute_domain(&domain_id, isc+pe, iec+pe, jsc+pe, jec+pe, &xsize, &ysize,
                            &x_is_global, &y_is_global, &tile_count, &whalo, &shalo);    

    xsize=6; ysize=6;
    cFMS_set_data_domain(&domain_id, isd+pe, ied+pe, jsd+pe, jed+pe, &xsize, &ysize,
                         &x_is_global, &y_is_global, &tile_count, &whalo, &shalo);
  }

  //get domain
  { 
    int is_check = 0;
    int ie_check = 0;
    int js_check = 0;
    int je_check = 0;
    int xsize_check = 0;
    int xmax_size_check = 0;
    int ysize_check = 0;
    int ymax_size_check = 0;
    bool x_is_global_check;
    bool y_is_global_check;
    int *position=NULL;
    
    cFMS_get_compute_domain(&domain_id, &is_check, &ie_check, &js_check, &je_check,
                            &xsize_check, &xmax_size_check, &ysize_check, &ymax_size_check,
                            &x_is_global_check, &y_is_global_check, &tile_count, position,
                            &whalo, &shalo);

    if(is_check != isc[pe]) cFMS_error(FATAL, "isc has not been properly set");
    if(ie_check != iec[pe]) cFMS_error(FATAL, "iec has not been proplerly set");
    if(js_check != jsc[pe]) cFMS_error(FATAL, "jsc has not been properly set");
    if(je_check != jec[pe]) cFMS_error(FATAL, "jec has not been properly set");
    if(xsize_check != 2) cFMS_error(FATAL, "incorrect xsize for compute domain");
    if(ysize_check != 2) cFMS_error(FATAL, "incorrect ysize for compute domain");
    if(xmax_size_check != 2) cFMS_error(FATAL, "incorrect xmax_size for compute domain");
    if(ymax_size_check != 2) cFMS_error(FATAL, "incorrect ymax_size for compute domain");
    if(x_is_global_check != false) cFMS_error(FATAL, "incorrect x_is_global for compute domain");
    if(y_is_global_check != false) cFMS_error(FATAL, "incorrect y_is_global for compute domain");    
    
    cFMS_get_data_domain(&domain_id, &is_check, &ie_check, &js_check, &je_check,
                         &xsize_check, &xmax_size_check, &ysize_check, &ymax_size_check,
                         &x_is_global_check, &y_is_global_check, &tile_count, position,
                         &whalo, &shalo);

    if(is_check != isd[pe]) errmsg_int(isd[pe], is_check, "get data domain");
    if(ie_check != ied[pe]) cFMS_error(FATAL, "ied has not been proplerly set");
    if(js_check != jsd[pe]) cFMS_error(FATAL, "jsd has not been properly set");
    if(je_check != jed[pe]) cFMS_error(FATAL, "jed has not been properly set");    
    if(xsize_check != 6) cFMS_error(FATAL, "incorrect xsize for data domain");
    if(ysize_check != 6) cFMS_error(FATAL, "incorrect ysize for data domain");
    if(xmax_size_check != 6) cFMS_error(FATAL, "incorrect xmax_size for data domain");
    if(ymax_size_check != 6) cFMS_error(FATAL, "incorrect ymax_size for data domain");
  }

  cFMS_end();
  return EXIT_SUCCESS;
                            
}
  
