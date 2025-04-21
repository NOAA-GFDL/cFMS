#include <stdlib.h>
#include <stdio.h>
#include <c_fms.h>
#include <c_mpp_domains_helper.h>

#define NX 8
#define NY 8
#define NPES 4
#define WHALO 2
#define EHALO 2
#define NHALO 2
#define SHALO 2


//doubly periodic 
//-------|-------|-------|
// d3 d4 | d3 d4 | d3 d4 |
// d1 d2 | d1 d2 | d1 d2 |
//-------|-------|-------|
// d3 d4 | d3 d4 | d3 d4 |
// d1 d2 | d1 d2 | d1 d2 |
//-------|-------|-------|
// d3 d4 | d3 d4 | d3 d4 |
// d1 d2 | d1 d2 | d1 d2 |
//-------|-------|-------|

int define_domain();
void test_float2d(int *domain_id);

int main()
{
  int domain_id = -99;

  cFMS_init(NULL,NULL,NULL,NULL,NULL);

  domain_id = define_domain();
  cFMS_set_current_pelist(NULL,NULL);

  test_float2d(&domain_id);

  cFMS_end();
  return EXIT_SUCCESS;

}

int define_domain()
{
  int global_indices[4] = {0, NX-1, 0, NY-1};
  int npes = NPES;
  int cyclic_global_domain = CYCLIC_GLOBAL_DOMAIN;
  int whalo=WHALO;
  int ehalo=EHALO;
  int nhalo=NHALO;
  int shalo=SHALO;
  
  cDomainStruct cdomain;
  
  cFMS_null_cdomain(&cdomain);

  cdomain.layout = (int *)calloc(2, sizeof(int));
  cFMS_define_layout(global_indices, &npes, cdomain.layout);

  cdomain.global_indices = global_indices;
  cdomain.whalo = &whalo;
  cdomain.ehalo = &ehalo;
  cdomain.shalo = &shalo;
  cdomain.nhalo = &nhalo;
  cdomain.xflags = &cyclic_global_domain;
  cdomain.yflags = &cyclic_global_domain;

  return cFMS_define_domains_easy(cdomain);
}

void test_float2d(int *domain_id)
{
  // global data
  // * *  *  *   *   *   *   *   *   *  * *
  // * *  *  *   *   *   *   *   *   *  * *
  // * * 29  39  49  59  69  79  89  99 * *
  // * * 28  38  48  58  68  78  88  98 * *
  // * * 27  37  47  57  67  77  87  97 * *
  // * * 26  36  46  56  66  76  86  96 * *
  // * * 25  35  45  55  65  75  85  95 * *
  // * * 24  34  44  54  64  74  84  94 * *
  // * * 23  33  43  53  63  73  83  93 * *
  // * * 22  32  42  52  62  72  82  92 * *
  // * *  *  *   *   *   *   *   *   *  * *
  // * *  *  *   *   *   *   *   *   *  * *
  
  // after update
  // pe 2 for d3                        pe 3 for d4 
  // (83)(93)(23)(33)(43)(53)(63)(73)   (43)(53)(63)(73)(83)(93)(23)(33)
  // (82)(92)(22)(32)(42)(52)(62)(72)   (42)(52)(62)(72)(82)(92)(22)(23)
  // (89)(99) 29  39  49  59 (69)(79)   (49)(59) 69  79  89  99 (29)(39)
  // (88)(98) 28  38  48  58 (68)(78)   (48)(58) 68  78  88  98 (28)(38)
  // (87)(97) 27  37  47  57 (67)(77)   (47)(57) 67  77  87  97 (27)(37)
  // (86)(96) 26  36  46  56 (66)(67)   (46)(56) 66  76  86  96 (26)(36)
  // (85)(95)(25)(35)(45)(55)(65)(75)   (45)(55)(65)(75)(85)(95)(25)(35)
  // (84)(94)(24)(34)(44)(54)(64)(74)   (44)(54)(64)(74)(84)(94)(24)(34)
  
  // pe 0 for d1                         pe 1 for d2
  // (87)(97)(27)(37)(47)(57)(67)(77)   (47)(57)(67)(77)(87)(97)(27)(37)
  // (86)(96)(26)(36)(46)(56)(66)(76)   (46)(56)(66)(76)(86)(96)(26)(36)
  // (85)(95) 25  35  45  55 (65)(75)   (45)(55) 65  75  85  95 (25)(35)
  // (84)(94) 24  34  44  54 (64)(74)   (44)(54) 64  74  84  94 (24)(34)
  // (83)(93) 23  33  43  53 (63)(73)   (43)(53) 63  73  83  93 (23)(33)
  // (82)(92) 22  32  42  52 (62)(72)   (42)(52) 62  72  82  92 (22)(32)
  // (89)(99)(29)(39)(49)(59)(69)(79)   (49)(59)(69)(79)(89)(99)(29)(39)
  // (88)(98)(28)(38)(48)(58)(68)(78)   (48)(58)(68)(78)(88)(98)(28)(38)
  
  double answers_t[4][8][8] = {
                              { {88, 98, 28, 38, 48, 58, 68, 78},
                                {89, 99, 29, 39, 49, 59, 69, 79},
                                {82, 92, 22, 32, 42, 52, 62, 72},
                                {83, 93, 23, 33, 43, 53, 63, 73},
                                {84, 94, 24, 34, 44, 54, 64, 74},
                                {85, 95, 25, 35, 45, 55, 65, 75},
                                {86, 96, 26, 36, 46, 56, 66, 76},
                                {87, 97, 27, 37, 47, 57, 67, 77}}, 
                              
                              { {48, 58, 68, 78, 88, 98, 28, 38},
                                {49, 59, 69, 79, 89, 99, 29, 39},
                                {42, 52, 62, 72, 82, 92, 22, 32},
                                {42, 53, 63, 73, 83, 93, 23, 33},
                                {44, 54, 64, 74, 84, 94, 24, 34},
                                {45, 55, 65, 75, 85, 95, 25, 35},
                                {46, 56, 66, 76, 86, 96, 26, 36},
                                {47, 57, 67, 77, 87, 97, 27, 37}},
                              
                              { {84, 94, 24, 34, 44, 54, 64, 74},
                                {85, 95, 25, 35, 45, 55, 65, 75},
                                {86, 96, 26, 36, 46, 56, 66, 76},
                                {87, 97, 27, 37, 47, 57, 67, 77},
                                {88, 98, 28, 38, 48, 58, 68, 68},
                                {89, 99, 29, 39, 49, 59, 69, 79},
                                {82, 92, 22, 32, 42, 52, 62, 72},
                                {83, 93, 23, 33, 43, 53, 63, 73}},
                              
                              { {44, 54, 64, 74, 84, 94, 24, 34},
                                {45, 55, 65, 75, 85, 95, 25, 35},
                                {46, 56, 66, 76, 86, 96, 26, 36},
                                {47, 57, 67, 77, 87, 97, 27, 37},
                                {48, 58, 68, 78, 88, 98, 28, 38},
                                {49, 59, 69, 79, 89, 99, 29, 39},
                                {42, 52, 62, 72, 82, 92, 22, 23},
                                {43, 53, 63, 73, 83, 93, 23, 33}} };

  
  double *global, *idata;

  int xdatasize=(WHALO+NX+EHALO);
  int ydatasize=(SHALO+NY+NHALO);

  int isc, iec, jsc, jec;
  int isd, ied, jsd, jed;
  int xsize_c = 0;
  int xsize_d = 0;
  int ysize_c = 0;
  int ysize_d = 0;
  int whalo = WHALO;
  int ehalo = EHALO;
  int shalo = SHALO;
  int nhalo = NHALO;

  int *xmax_size  = NULL;
  int *ymax_size  = NULL;
  int *tile_count = NULL;
  int *position   = NULL;
  bool *x_is_global = NULL;
  bool *y_is_global = NULL;  

  // get compute domain indices
  cFMS_get_compute_domain(domain_id, &isc, &iec, &jsc, &jec, &xsize_c, xmax_size, &ysize_c, ymax_size,
                          x_is_global, y_is_global, tile_count, position, NULL, NULL);

  // get data domain sizes
  cFMS_get_data_domain(domain_id, &isd, &ied, &jsd, &jed, &xsize_d, xmax_size, &ysize_d, ymax_size,
                       x_is_global, y_is_global, tile_count, position, &whalo, &shalo);

  //allocate global array
  int xsize = NX+WHALO+EHALO;
  int ysize = NY+SHALO+NHALO;
  global = (double *)calloc(xsize*ysize, sizeof(double));
  for(int ix=WHALO ; ix<NX+WHALO; ix++) for(int iy=SHALO ; iy<NY+SHALO; iy++) global[ysize*ix+iy] = (double)iy*10 + ix;
                                                            
  // allocate array for the ith data domain
  idata = (double *)calloc(xsize_d*ysize_d,sizeof(double));
  for(int ix=WHALO; ix<xsize_c+WHALO; ix++) for(int iy=SHALO; iy<ysize_c+SHALO; iy++) {
      idata[ix*ysize_d+iy] = global[(ix+jsc)*ysize+iy+isc];
  }

  int field_shape[2] = {xsize_d, ysize_d};
  int *flags = NULL;
  int *complete = NULL;
  char *name = NULL;
  
  cFMS_update_domains_double_2d(field_shape, idata, domain_id, flags, complete, position,
                               &whalo, &ehalo, &shalo, &nhalo, name, tile_count);

  int ipe = cFMS_pe();  
  for(int ix=WHALO ; ix<xsize_d-WHALO; ix++) {
    for(int iy=SHALO ; iy<ysize_d-SHALO; iy++) {
      if( idata[ysize_d*ix+iy] != answers_t[ipe][ix][iy] ) cFMS_error(FATAL, "data domain did not update correctly!");
    }
  }
  
}
