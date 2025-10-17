/***********************************************************************
*                   GNU Lesser General Public License
*
* This file is part of the GFDL Flexible Modeling System (FMS).
*
* FMS is free software: you can redistribute it and/or modify it under
* the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation, either version 3 of the License, or (at
* your option) any later version.
*
* FMS is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
* for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with FMS.  If not, see <http:www.gnu.org/licenses/>.
***********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <c_fms.h>
#include <c_mpp_domains_helper.h>

#define NX 96
#define NY 96
#define NX_FINE 48
#define NY_FINE 48
#define LAYOUT_X 2
#define LAYOUT_Y 2
#define X_REFINE 2
#define Y_REFINE 2
#define COARSE_NPES 4
#define FINE_NPES 4

int main() {
  
  int *global_pelist = NULL;

  int ndomain=2;
  int nnest_domain=2;
  int domain_id=-99;
  int nest_domain_id=1;
  cDomainStruct cdomain;
  cNestDomainStruct cnest_domain;  
  
  int coarse_global_indices[4] = {0, NX-1, 0, NY-1};
  int coarse_npes=COARSE_NPES, coarse_pelist[COARSE_NPES];
  int coarse_tile_id=0;
  int coarse_whalo=2;
  int coarse_ehalo=2;
  int coarse_shalo=2;
  int coarse_nhalo=2;
  int coarse_xflags = WEST;
  int coarse_yflags = SOUTH;
  bool is_mosaic = false;
  bool symmetry = false;
  
  int coarse_xextent[4] = {NX/2,NX/2,NX/2,NX/2};
  int coarse_yextent[4] = {NY/2,NY/2,NY/2,NY/2};

  int fine_global_indices[4] = {0, NX_FINE-1, 0, NY_FINE-1};
  int fine_npes=FINE_NPES, fine_pelist[FINE_NPES];
  int fine_tile_id=1;
  int fine_whalo=2;
  int fine_ehalo=2;
  int fine_shalo=2;
  int fine_nhalo=2;
  
  cFMS_init(NULL, NULL, &ndomain, &nnest_domain, NULL);
  cFMS_null_cdomain(&cdomain);
  cFMS_null_cnest_domain(&cnest_domain);

  if(cFMS_get_domain_count()!=0){
    cFMS_error(FATAL, "domain_count not initialized correctly");
    exit(EXIT_FAILURE);
  }

  if(cFMS_get_nest_domain_count()!=0){
    cFMS_error(FATAL, "nest_domain_count not initialized correctly");
    exit(EXIT_FAILURE);
  }
  
  //get global pelist
  {
  int npes = cFMS_npes();
  global_pelist = (int *)malloc(npes*sizeof(int));
  cFMS_get_current_pelist(&npes, global_pelist, NULL, NULL);
  }
  
  //set coarse domain as tile=0
  {
    for(int i=0 ; i<coarse_npes; i++) coarse_pelist[i] = global_pelist[i];
    char name_coarse[NAME_LENGTH] = "test coarse pelist";
    cFMS_declare_pelist(&coarse_npes, coarse_pelist, name_coarse, NULL);

    if(any(coarse_npes, coarse_pelist, cFMS_pe())) {

      cFMS_set_current_pelist(&coarse_npes, coarse_pelist, NULL);
      
      char name[NAME_LENGTH] = "test coarse domain"; 

      cdomain.maskmap = (bool *)malloc(4*sizeof(bool));
      int ij = 0;
      for(int i=0; i<2 ; i++) {
        for (int j=0; j<2; j++) {
          cdomain.maskmap[ij++] = true;
        }
      }      
      
      int xextent[2] = {0,0};
      int yextent[2] = {0,0};
      bool is_mosaic = false;
      
      cdomain.name   = name;
      cdomain.npes = &coarse_npes;
      cdomain.pelist = coarse_pelist;
      cdomain.global_indices = coarse_global_indices;
      cdomain.whalo = &coarse_whalo;
      cdomain.ehalo = &coarse_ehalo;
      cdomain.shalo = &coarse_shalo;
      cdomain.nhalo = &coarse_nhalo;
      cdomain.tile_id = &coarse_tile_id;
      cdomain.xextent = coarse_xextent;
      cdomain.yextent = coarse_yextent;
      cdomain.xflags = &coarse_xflags;
      cdomain.yflags = &coarse_yflags;
      cdomain.xextent = xextent;
      cdomain.yextent = yextent;
      cdomain.is_mosaic = &is_mosaic;
      cdomain.layout = (int *)malloc(2*sizeof(int));
      int ndivs = coarse_npes; cFMS_define_layout(coarse_global_indices, &ndivs, cdomain.layout);      

      domain_id = cFMS_define_domains_easy(cdomain);

      if(domain_id != 0) {
        cFMS_error(FATAL, "domain did not set up correctly");
        exit(EXIT_FAILURE);
      }

      free(cdomain.layout);
      free(cdomain.maskmap);
      cFMS_null_cdomain(&cdomain);
    }
  }

  cFMS_set_current_pelist(NULL, NULL, NULL);

  //set fine domain as tile=1
  {
    char name_fine[NAME_LENGTH] = "test fine pelist";
    for(int i=0; i<fine_npes; i++) fine_pelist[i] = global_pelist[COARSE_NPES+i];
    cFMS_declare_pelist(&fine_npes, fine_pelist, name_fine, NULL);
    
    if(any(FINE_NPES, fine_pelist, cFMS_pe())) {
      
      cFMS_set_current_pelist(&fine_npes, fine_pelist, NULL);
      
      char name[NAME_LENGTH] = "test fine domain" ; cdomain.name = name;
      cdomain.global_indices = fine_global_indices;
      cdomain.npes = &fine_npes;
      cdomain.tile_id = &fine_tile_id;
      cdomain.whalo = &fine_whalo;
      cdomain.ehalo = &fine_ehalo;
      cdomain.shalo = &fine_shalo;
      cdomain.nhalo = &fine_nhalo;
      cdomain.layout = (int *)malloc(2*sizeof(int));
      int ndivs = FINE_NPES; cFMS_define_layout(fine_global_indices, &ndivs, cdomain.layout);

      domain_id = cFMS_define_domains_easy(cdomain);

      if(domain_id != 0) {
        cFMS_error(FATAL, "TILE IN domain did not set up correctly");
        exit(EXIT_FAILURE);
      }      
      
      free(cdomain.layout);
      cFMS_null_cdomain(&cdomain);
    }
  }
  
  cFMS_set_current_pelist(NULL, NULL, NULL);

  if( !cFMS_domain_is_initialized(&domain_id) ) cFMS_error(FATAL, "domain is not initialized");

  //set nest domain
  { 
    char name[80] = "test nest domain"; cnest_domain.name = name;
    int num_nest=1; cnest_domain.num_nest = &num_nest;
    int ntiles=2;   cnest_domain.ntiles=&ntiles;
    int nest_level=1; cnest_domain.nest_level = &nest_level;
    int istart_coarse = 24; cnest_domain.istart_coarse = &istart_coarse;
    int icount_coarse = 24; cnest_domain.icount_coarse = &icount_coarse;
    int jstart_coarse = 24; cnest_domain.jstart_coarse = &jstart_coarse;
    int jcount_coarse = 24; cnest_domain.jcount_coarse  = &jcount_coarse;
    int npes_nest_tile[2] = {COARSE_NPES,FINE_NPES}; cnest_domain.npes_nest_tile = npes_nest_tile;
    int x_refine = X_REFINE; cnest_domain.x_refine = &x_refine;
    int y_refine = Y_REFINE; cnest_domain.y_refine = &y_refine;
    cnest_domain.tile_fine = &fine_tile_id;
    cnest_domain.tile_coarse = &coarse_tile_id;
    cnest_domain.domain_id = &domain_id;
    
    nest_domain_id = cFMS_define_nest_domains_easy(cnest_domain);
    if(nest_domain_id != 0) {
      cFMS_error(FATAL, "nest domain did not set up correctly");
      exit(EXIT_FAILURE);
    }
    cFMS_null_cnest_domain(&cnest_domain);
  }

  cFMS_set_current_pelist(NULL, NULL, NULL);

  if(cFMS_get_domain_count() != 1) {
    cFMS_error(FATAL, "error in setting domain");
    exit(EXIT_FAILURE);
  }

  if(cFMS_get_nest_domain_count() != 1){
    cFMS_error(FATAL, "error in setting nest domain");
    exit(EXIT_FAILURE);
  }

  cFMS_end();
  return EXIT_SUCCESS;
  
}
