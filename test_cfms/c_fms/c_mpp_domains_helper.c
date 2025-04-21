/************************************************************************
!*                   GNU Lesser General Public License
!*
!* This file is part of the GFDL Flexible Modeling System (FMS).
!*
!* FMS is free software: you can redistribute it and/or modify it under
!* the terms of the GNU Lesser General Public License as published by
!* the Free Software Foundation, either version 3 of the License, or (at
!* your option) any later version.
!*
!* FMS is distributed in the hope that it will be useful, but WITHOUT
!* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
!* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
!* for more details.
!*
!* You should have received a copy of the GNU Lesser General Public
!* License along with FMS.  If not, see <http://www.gnu.org/licenses/>.
************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <c_fms.h>
#include <c_mpp_domains_helper.h>

int cFMS_define_domains_easy(cDomainStruct cdomain)
{
  return cFMS_define_domains(cdomain.global_indices,
                             cdomain.layout,
                             cdomain.npelist,
                             cdomain.pelist,
                             cdomain.xflags, cdomain.yflags,
                             cdomain.xhalo, cdomain.yhalo,
                             cdomain.xextent, cdomain.yextent,
                             cdomain.maskmap,
                             cdomain.name,
                             cdomain.symmetry,
                             cdomain.memory_size,
                             cdomain.whalo, cdomain.ehalo, cdomain.shalo, cdomain.nhalo,
                             cdomain.is_mosaic,
                             cdomain.tile_count, cdomain.tile_id,
                             cdomain.complete,
                             cdomain.x_cyclic_offset, cdomain.y_cyclic_offset);
}


int cFMS_define_nest_domains_easy(cNestDomainStruct cnestdomain)
{

  return cFMS_define_nest_domains(cnestdomain.num_nest,
                                  cnestdomain.ntiles,
                                  cnestdomain.nest_level,
                                  cnestdomain.tile_fine,
                                  cnestdomain.tile_coarse,
                                  cnestdomain.istart_coarse,
                                  cnestdomain.icount_coarse,
                                  cnestdomain.jstart_coarse,
                                  cnestdomain.jcount_coarse,
                                  cnestdomain.npes_nest_tile,
                                  cnestdomain.x_refine,
                                  cnestdomain.y_refine,
                                  cnestdomain.domain_id,
                                  cnestdomain.extra_halo,
                                  cnestdomain.name);
}


void cFMS_null_cdomain(cDomainStruct *cdomain)
{
  cdomain->global_indices = NULL;
  cdomain->layout = NULL;
  cdomain->npelist = NULL;
  cdomain->pelist = NULL;
  cdomain->xflags = NULL;
  cdomain->yflags = NULL;
  cdomain->xhalo = NULL;
  cdomain->yhalo = NULL;
  cdomain->xextent = NULL;
  cdomain->yextent = NULL;
  cdomain->maskmap = NULL;
  cdomain->name = NULL;
  cdomain->symmetry = NULL;
  cdomain->memory_size = NULL;
  cdomain->whalo = NULL;
  cdomain->ehalo = NULL;
  cdomain->shalo = NULL;
  cdomain->nhalo = NULL;
  cdomain->is_mosaic = NULL;
  cdomain->tile_count = NULL;
  cdomain->tile_id = NULL;
  cdomain->complete = NULL;
  cdomain->x_cyclic_offset = NULL;
  cdomain->y_cyclic_offset = NULL;
}


void cFMS_null_cnest_domain(cNestDomainStruct *cnest_domain)
{
  cnest_domain->num_nest = NULL;
  cnest_domain->ntiles = NULL;
  cnest_domain->nest_level = NULL;
  cnest_domain->tile_fine = NULL;
  cnest_domain->tile_coarse = NULL;
  cnest_domain->istart_coarse = NULL;
  cnest_domain->icount_coarse = NULL;
  cnest_domain->jstart_coarse = NULL;
  cnest_domain->jcount_coarse = NULL;
  cnest_domain->npes_nest_tile = NULL;
  cnest_domain->x_refine = NULL;
  cnest_domain->y_refine = NULL;
  cnest_domain->domain_id = NULL;
  cnest_domain->extra_halo = NULL;
  cnest_domain->name = NULL;
}

int any(int n, int* array, int value)
{
  for(int i=0 ; i<n ; i++){
    if(value == array[i]) return TRUE;
  }
  return FALSE;
}
                            
int errmsg_int(int answer, int test, char *message)
{
  printf("\nEXPECTED %d BUT GOT %d FOR %s\n", answer, test, message);
  printf("HEREHERE %d\n", FATAL);
  cFMS_error(FATAL, "GOODBYE!");
  exit(FAIL);
}
