#ifndef C_MPP_DOMAINS_HELPER_H
#define C_MPP_DOMAINS_HELPER_H

#define TRUE 1
#define FALSE 0
#define SUCCESS 0
#define FAIL 1

//DefineDomainStruct is not defined as an interoperable Fortran derived type
//to avoid dynamic allocation of arrays using iso_Fortran_bindings and complicating
//usage for users.
typedef struct {
  int* global_indices;
  int* layout;
  int* npelist;
  int* pelist;
  int* xflags;
  int* yflags;
  int* xhalo;
  int* yhalo;
  int* xextent;
  int* yextent;
  bool* maskmap;
  char* name;
  bool* symmetry;
  int *memory_size; //memory_size[2]
  int* whalo;
  int* ehalo;
  int* shalo;
  int* nhalo;
  bool* is_mosaic;
  int* tile_count;
  int* tile_id;
  bool* complete;
  int* x_cyclic_offset;
  int* y_cyclic_offset;
} cDomainStruct;


typedef struct {
  int *num_nest;
  int *ntiles;
  int *nest_level;
  int *tile_fine;
  int *tile_coarse;
  int *istart_coarse;
  int *icount_coarse;
  int *jstart_coarse;
  int *jcount_coarse;
  int *npes_nest_tile;
  int *x_refine;
  int *y_refine;
  int *domain_id;
  int *extra_halo;
  char *name;
} cNestDomainStruct;


int cFMS_define_domains_easy(cDomainStruct cdomain);
int cFMS_define_nest_domains_easy(cNestDomainStruct cnestdomain);
void cFMS_null_cdomain(cDomainStruct *cdomain);
void cFMS_null_cnest_domain(cNestDomainStruct *cnest_domain);
int any(int n, int* array, int value);
int errmsg_int(int answer, int test, char *message);


#endif
