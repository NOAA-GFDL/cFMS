#include <stdio.h>
#include <stdlib.h>
#include <c_fms.h>
#include <c_data_override.h>
#include <c_mpp_domains_helper.h>

#define NX 360
#define NY 180

#define TEST_NTIMES 11

int main()
{

  int ndomain = 1;
  int nnest_domain = 0;
  int domain_id = 0;
  int calendar_type = NOLEAP;
  float answers[TEST_NTIMES] = {1., 2., 3., 3.5, 4., 5., 6., 7., 8., 9., 10.};
  
  cFMS_init(NULL, NULL, &ndomain, &nnest_domain, &calendar_type);

  // define domain
  {
    int global_indices[4] = {0, NX-1, 0, NY-1};  
    int ehalo = 2;
    int whalo = 2;
    int shalo = 2;
    int nhalo = 2;
    
    cDomainStruct domain;
    cFMS_null_cdomain(&domain);

    int ndivs = cFMS_npes();
    domain.layout = (int *)malloc(2*sizeof(int));    
    cFMS_define_layout(global_indices, &ndivs, domain.layout);
    
    domain.global_indices = global_indices;
    domain.ehalo = &ehalo;
    domain.whalo = &whalo;
    domain.shalo = &shalo;
    domain.nhalo = &nhalo;
    
    domain_id = cFMS_define_domains_easy(domain);
  }

  //data override init
  {
    int *atm_domain_id = NULL;
    int *ice_domain_id = NULL;
    int *land_domain_id = NULL;
    int *land_domainUG_id = NULL;
    cFMS_data_override_init(atm_domain_id, &domain_id, ice_domain_id, land_domain_id, land_domainUG_id, NULL);
  }

  //data override scalar
  {
    char gridname[NAME_LENGTH] = "OCN";
    char fieldname_code[NAME_LENGTH] = "co2";
    float data = -100.;
    bool override = false;
    int *data_index = NULL;
    int start_day = 1;
    
    for(int i=0; i<TEST_NTIMES; i++) {
      int year = 1;
      int month = 1;
      int day = start_day + i + 1; //start day 1,1,1,0,0,0
      int hour = 0;
      int minute = 0;
      int second = 0;
      
      cFMS_data_override_set_time(&year, &month, &day, &hour, &minute, &second, NULL, NULL);      
      cFMS_data_override_0d_cfloat(gridname, fieldname_code, &data, &override, NULL);
      if( data != answers[i] ) {
        printf("Expected %f but got %f\n", answers[i], data);        
        exit(EXIT_FAILURE);
      }
    }
  }
  
  return EXIT_SUCCESS;
}
