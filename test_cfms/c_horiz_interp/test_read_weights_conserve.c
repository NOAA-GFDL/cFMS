#include <stdio.h>
#include <stdlib.h>
#include <netcdf.h>

#include "c_fms.h"
#include "c_constants.h"
#include "c_grid_utils.h"
#include "c_horiz_interp.h"
#include "c_mpp_domains_helper.h"

#define NLON_SRC 16
#define NLAT_SRC 32
#define NLON_DST 16
#define NLAT_DST 32

#define NC_CHECK(call)                               \
  do                                                 \
  {                                                  \
    int _rc = (call);                                \
    if (_rc != NC_NOERR)                             \
    {                                                \
      fprintf(stderr, "NetCDF error (%s:%d): %s\n",  \
              __FILE__, __LINE__, nc_strerror(_rc)); \
      exit(EXIT_FAILURE);                            \
    }                                                \
  } while (0)

int define_domain();
void write_remap(double *lon, double *lat);

int main()
{

  double *lon_src, *lat_src, *lon_dst, *lat_dst;
  double *data_src, *data_dst;

  // get grid
  lon_src = (double *)malloc((NLON_SRC + 1) * (NLAT_SRC + 1) * sizeof(double));
  lat_src = (double *)malloc((NLON_SRC + 1) * (NLAT_SRC + 1) * sizeof(double));
  lon_dst = (double *)malloc((NLON_DST + 1) * (NLAT_DST + 1) * sizeof(double));
  lat_dst = (double *)malloc((NLON_DST + 1) * (NLAT_DST + 1) * sizeof(double));

  // src grid
  for (int j = 0; j <= NLAT_SRC; j++)
  {
    for (int i = 0; i <= NLON_SRC; i++)
    {
      int ij = j * (NLON_SRC + 1) + i;
      lon_src[ij] = i * DEG_TO_RAD;
      lat_src[ij] = j * DEG_TO_RAD;
    }
  }

  // dst grid
  for (int j = 0; j <= NLAT_DST; j++)
  {
    for (int i = 0; i <= NLON_DST; i++)
    {
      int ij = j * (NLON_DST + 1) + i;
      lon_dst[ij] = i * DEG_TO_RAD;
      lat_dst[ij] = j * DEG_TO_RAD;
    }
  }

  // allocate data
  data_src = (double *)malloc(NLON_SRC * NLAT_SRC * sizeof(double));
  data_dst = (double *)malloc(NLON_DST * NLAT_DST * sizeof(double));

  // initialize src data
  for (int j = 0; j < NLAT_SRC; j++)
  {
    for (int i = 0; i < NLON_SRC; i++)
    {
      int ij = j * NLON_SRC + i;
      data_src[ij] = ij;
    }
  }

  cFMS_init(NULL, NULL, NULL, NULL, NULL);

  // define domain
  int domain_id = define_domain();
  if (cFMS_pe() == CFMS_root_pe())
    write_remap(lon_dst, lat_dst);
}

int define_domain()
{

  cDomainStruct domain;
  cFMS_null_cdomain(&domain);

  int global_indices[4] = {0, NLON_SRC - 1, 0, NLAT_SRC - 1};

  int ndivs = cFMS_npes();
  domain.layout = (int *)malloc(2 * sizeof(int));
  cFMS_define_layout(global_indices, &ndivs, domain.layout);

  domain.global_indices = global_indices;

  return cFMS_define_domains_easy(domain);
}

void write_remap(double *lon, double *lat)
{

  int ncells = NLON_DST * NLAT_DST;
  int nlon = NLON_SRC;
  int nlat = NLAT_SRC;
  int **tile1_cell, **tile2_cell;
  double *xgrid_area;

  tile1_cell = (int **)malloc(ncells * sizeof(int *));
  tile2_cell = (int **)malloc(ncells * sizeof(int *));
  xgrid_area = (double *)malloc(ncells * sizeof(double));

  for (int i = 0; i < ncells; i++)
  {
    tile1_cell[i] = (int *)malloc(2 * sizeof(int));
    tile2_cell[i] = (int *)malloc(2 * sizeof(int));
  }

  // assign
  for (int j = 0; j < nlat; j++)
    for (int i = 0; i < nlat; i++)
    {
      int ij = j * NLON_DST + i;
      tile2_cell[ij][0] = i;
      tile2_cell[ij][1] = j;
    }

  cFMS_get_grid_area(&nlon, &nlat, lon, lat, xgrid_area);

  int ncid, ncells_dimid;
  int tile1_varid, tile2_varid, xgrid_area_varid;

  NC_CHECK(nc_create("remap_weights.nc", NC_CLOBBER, &ncid));
  NC_CHECK(nc_def_dim(ncid, "ncells", ncells, &ncells_dimid));
  int two_dims[2] = {ncells_dimid, 2};
  NC_CHECK(nc_def_var(ncid, "tile1_cell", NC_INT, 2, two_dims, &tile1_varid));
  NC_CHECK(nc_def_var(ncid, "tile2_cell", NC_INT, 2, two_dims, &tile2_varid));
  NC_CHECK(nc_def_var(ncid, "x_grid_area", NC_DOUBLE, 1, &ncells_dimid, &xgrid_area_varid));
  NC_CHECK(nc_enddef(ncid));
  NC_CHECK(nc_put_var_int(ncid, tile1_varid, &tile1_cell[0][0]));
  NC_CHECK(nc_put_var_int(ncid, tile2_varid, &tile2_cell[0][0]));
  NC_CHECK(nc_put_var_double(ncid, xgrid_area_varid, xgrid_area));
  NC_CHECK(nc_close(ncid));
}