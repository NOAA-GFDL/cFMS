#include <stdio.h>
#include <stdlib.h>
#include <netcdf.h>

#include "c_fms.h"
#include "c_constants.h"
#include "c_grid_utils.h"
#include "c_horiz_interp.h"
#include "c_mpp_domains_helper.h"

#define NLON_SRC 16
#define NLAT_SRC 16
#define NLON_DST 16
#define NLAT_DST 16

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
void generate_grid(int nlon, int nlat, double *lon, double *lat);
void write_remap(double *lon, double *lat, char *weight_filename);

int main()
{

  int nlon_src = NLON_SRC;
  int nlat_src = NLAT_SRC;
  int nlon_dst = NLON_DST;
  int nlat_dst = NLAT_DST;
  double *lon_src, *lat_src, *lon_dst, *lat_dst;
  double *data_src, *data_dst;

  char weight_filename[NAME_LENGTH] = "remap.nc";

  cFMS_init(NULL, NULL, NULL, NULL, NULL);
  cFMS_horiz_interp_init(NULL);

  // define domain
  int domain_id = define_domain();
  int isc, iec, jsc, jec, xsize, ysize;
  cFMS_get_compute_domain(&domain_id, &isc, &iec, &jsc, &jec, &xsize, NULL, &ysize, NULL,
                          NULL, NULL, NULL, NULL, NULL, NULL);

  // allocate grid and data
  lon_src = (double *)malloc((nlon_src + 1) * (nlat_src + 1) * sizeof(double));
  lat_src = (double *)malloc((nlon_src + 1) * (nlat_src + 1) * sizeof(double));
  lon_dst = (double *)malloc((nlon_dst + 1) * (nlat_dst + 1) * sizeof(double));
  lat_dst = (double *)malloc((nlon_dst + 1) * (nlat_dst + 1) * sizeof(double));
  data_src = (double *)malloc(nlon_src * nlat_src * sizeof(double));
  data_dst = (double *)calloc(xsize * ysize, sizeof(double));

  // get grid
  generate_grid(nlon_src, nlat_src, lon_src, lat_src);
  generate_grid(nlon_dst, nlat_dst, lon_dst, lat_dst);

  // initialize src data
  for (int ij = 0; ij < nlon_src * nlat_src; ij++)
    data_src[ij] = (double)ij;

  // write remap file
  if (cFMS_pe() == cFMS_root_pe())
    write_remap(lon_dst, lat_dst, weight_filename);

  cFMS_sync();

  // read weights
  char weight_file_src[NAME_LENGTH] = "fregrid";
  int src_tile = 1;
  int interp_id = cFMS_horiz_interp_read_weights_conserve(weight_filename, weight_file_src,
                                                          &nlon_src, &nlat_src, &xsize, &ysize,
                                                          &isc, &iec, &jsc, &jec, &src_tile);

  // remap
  bool convert_cf_order = false;
  cFMS_horiz_interp_base_2d_cdouble(&interp_id, data_src, data_dst, NULL, NULL, NULL, NULL,
                                    NULL, NULL, &convert_cf_order);

  // check answers
  for (int j = 0; j < ysize; j++)
  {
    for (int i = 0; i < xsize; i++)
    {
      int ij_dst = j * xsize + i;
      int ij_src = (j + jsc) * (nlon_src) + isc + i;
      if (data_dst[ij_dst] != data_src[ij_src])
        cFMS_error(FATAL, "error remapping in read-in weights");
    }
  }

  cFMS_end();
  return EXIT_SUCCESS;
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

void generate_grid(int nlon, int nlat, double *lon, double *lat)
{
  int ij = 0;
  for (int j = 0; j <= nlat; j++)
  {
    for (int i = 0; i <= nlon; i++)
    {
      lon[ij] = (double)i * DEG_TO_RAD;
      lat[ij++] = (double)j * DEG_TO_RAD;
    }
  }
}

void write_remap(double *lon, double *lat, char *weight_filename)
{

  int ncells = NLON_DST * NLAT_DST;
  int *tile1_cell, *tile2_cell, *tile1;
  double *xgrid_area;

  // set xgrid
  tile1 = (int *)malloc(ncells * sizeof(int));
  tile1_cell = (int *)malloc(ncells * 2 * sizeof(int));
  tile2_cell = (int *)malloc(ncells * 2 * sizeof(int));
  xgrid_area = (double *)malloc(ncells * sizeof(double));

  for (int ij = 0; ij < ncells; ij++)
  {
    tile1[ij] = 1;
    tile1_cell[ij * 2] = (ij % NLON_SRC) + 1;
    tile1_cell[ij * 2 + 1] = (ij / NLON_SRC) + 1;
    tile2_cell[ij * 2] = (ij % NLON_DST) + 1;
    tile2_cell[ij * 2 + 1] = (ij / NLON_DST) + 1;
  }

  int nlon = NLON_DST, nlat = NLAT_DST;
  cFMS_get_grid_area(&nlon, &nlat, lon, lat, xgrid_area);

  // write
  int ncid, ncells_dimid, two_dimid;
  int tile1_id, tile1_cell_id, tile2_cell_id, xgrid_area_id;

  NC_CHECK(nc_create(weight_filename, NC_CLOBBER, &ncid));
  NC_CHECK(nc_def_dim(ncid, "two", 2, &two_dimid));
  NC_CHECK(nc_def_dim(ncid, "ncells", ncells, &ncells_dimid));
  int two_dims[2] = {ncells_dimid, two_dimid};
  NC_CHECK(nc_def_var(ncid, "tile1", NC_INT, 1, &ncells_dimid, &tile1_id));
  NC_CHECK(nc_def_var(ncid, "tile1_cell", NC_INT, 2, two_dims, &tile1_cell_id));
  NC_CHECK(nc_def_var(ncid, "tile2_cell", NC_INT, 2, two_dims, &tile2_cell_id));
  NC_CHECK(nc_def_var(ncid, "xgrid_area", NC_DOUBLE, 1, &ncells_dimid, &xgrid_area_id));
  NC_CHECK(nc_enddef(ncid));
  NC_CHECK(nc_put_var_int(ncid, tile1_id, tile1));
  NC_CHECK(nc_put_var_int(ncid, tile1_cell_id, tile1_cell));
  NC_CHECK(nc_put_var_int(ncid, tile2_cell_id, tile2_cell));
  NC_CHECK(nc_put_var_double(ncid, xgrid_area_id, xgrid_area));
  NC_CHECK(nc_close(ncid));
}
