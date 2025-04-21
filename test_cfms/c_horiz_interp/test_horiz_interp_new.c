#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <c_fms.h>
#include <c_horiz_interp.h>
#include <c_mpp_domains_helper.h>
#include <math.h>

#define NX 8
#define NY 8
#define NPES 4
#define WHALO 2
#define EHALO 2
#define NHALO 2
#define SHALO 2
#define NI_SRC 360
#define NJ_SRC 180
#define NI_DST 144
#define NJ_DST 72

/*
Adapted from test_horiz_interp :: test_assign
*/

void define_domain(int *domain_id)
{
  int global_indices[4] = {0, NI_SRC-1, 0, NJ_SRC-1};
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
  cdomain.domain_id = domain_id;
  cdomain.whalo = &whalo;
  cdomain.ehalo = &ehalo;
  cdomain.shalo = &shalo;
  cdomain.nhalo = &nhalo;
  cdomain.xflags = &cyclic_global_domain;
  cdomain.yflags = &cyclic_global_domain;

  cFMS_define_domains_easy(cdomain);
}

int main()
{
    // domain info
    int domain_id = 0;
    int isc, iec, jsc, jec;
    int xsize_c = 0;
    int ysize_c = 0;
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
    
    // grid data
    double *lat_in_1D, *lon_in_1D;
    double *lat_in_2D, *lon_in_2D;
    double *lat_out_1D, *lon_out_1D;
    double *lat_out_2D, *lon_out_2D;
    double *lon_src_1d, *lat_src_1d;
    double *lon_dst_1d, *lat_dst_1d;
    double dlon_src, dlat_src, dlon_dst, dlat_dst;

    double lon_src_beg = 0.;
    double lon_src_end = 360.;
    double lat_src_beg = -90.;
    double lat_src_end = 90.;
    double lon_dst_beg = 0.;
    double lon_dst_end = 360.;
    double lat_dst_beg = -90.;
    double lat_dst_end = 90.;
    double D2R = 3.14/180.;
    double SMALL = 1.0e-10;

    char interp_method[MESSAGE_LENGTH] = "conservative";

    dlon_src = (lon_src_end-lon_src_beg)/NI_SRC;
    dlat_src = (lat_src_end-lat_src_beg)/NJ_SRC;
    dlon_dst = (lon_dst_end-lon_dst_beg)/NI_DST;
    dlat_dst = (lat_dst_end-lat_dst_beg)/NJ_DST;
    
    cFMS_init(NULL,NULL,NULL,NULL,NULL);

    define_domain(&domain_id);

    cFMS_get_compute_domain(&domain_id, &isc, &iec, &jsc, &jec, &xsize_c, xmax_size, &ysize_c, ymax_size,
        x_is_global, y_is_global, tile_count, position, &whalo, &shalo);


    int lon_in_1d_size = NI_SRC+1;
    int lat_in_1d_size = NJ_SRC+1;
    int lon_out_1d_size = iec+1-isc;
    int lat_out_1d_size = jec+1-jsc;

    lon_in_1D = (double *)malloc(lon_in_1d_size*sizeof(double));
    for(int i=0; i<lon_in_1d_size; i++) lon_in_1D[i] = (lon_src_beg + (i-1)*dlon_src)*D2R;

    lat_in_1D = (double *)malloc(lat_in_1d_size*sizeof(double));
    for(int j=0; j<lat_in_1d_size; j++) lat_in_1D[j] = (lat_src_beg + (j-1)*dlat_src)*D2R;

    lon_out_1D = (double *)malloc(lon_out_1d_size*sizeof(double));
    for(int i=0; i<lon_out_1d_size; i++) lon_out_1D[i] = (lon_dst_beg + (i-1)*dlon_dst)*D2R;

    lat_out_1D = (double *)malloc(lat_out_1d_size*sizeof(double));
    for(int j=0; j<lat_out_1d_size; j++) lat_out_1D[j] = (lat_dst_beg + (j-1)*dlat_dst)*D2R;


    int in_2d_size = lon_in_1d_size*lat_in_1d_size;
    int out_2d_size = lon_out_1d_size*lat_out_1d_size;

    lon_in_2D = (double *)malloc(in_2d_size*sizeof(double));
    for(int i=0; i<lon_in_1d_size; i++)
    {
        for(int j=0; j<lat_in_1d_size; j++)
        {
            lon_in_2D[lat_in_1d_size*i + j] = lon_in_1D[i];
        }
    }
    int lon_in_shape[2] = {lon_in_1d_size, lat_in_1d_size};

    lat_in_2D = (double *)malloc(in_2d_size*sizeof(double));
    for(int i=0; i<lon_in_1d_size; i++)
    {
        for(int j=0; j<lat_in_1d_size; j++)
        {
            lat_in_2D[lat_in_1d_size*i + j] = lat_in_1D[j];
        }
    }
    int lat_in_shape[2] = {lon_in_1d_size, lat_in_1d_size};

    lon_out_2D = (double *)malloc(out_2d_size*sizeof(double));
    for(int i=0; i<lon_out_1d_size; i++)
    {
        for(int j=0; j<lat_out_1d_size; j++)
        {
            lon_out_2D[lat_out_1d_size*i + j] = lon_out_1D[i];
        }
    }
    int lon_out_shape[2] = {lon_out_1d_size, lat_out_1d_size};

    lat_out_2D = (double *)malloc(out_2d_size*sizeof(double));
    for(int i=0; i<lon_out_1d_size; i++)
    {
        for(int j=0; j<lat_out_1d_size; j++)
        {
            lat_out_2D[lat_out_1d_size*i + j] = lat_out_1D[j];
        }
    }
    int lat_out_shape[2] = {lon_out_1d_size, lat_out_1d_size};

    cFMS_horiz_interp_init(NULL);

    int interp_id = 0;
    int test_interp_id;

    cFMS_set_current_interp(&interp_id);

    test_interp_id = cFMS_horiz_interp_2d_cdouble(lon_in_2D, lon_in_shape, lat_in_2D, lat_in_shape,
                                    lon_out_2D, lon_out_shape, lat_out_2D, lat_out_shape,
                                    interp_method, NULL, NULL, NULL, NULL,
                                    NULL, NULL, NULL);

    int nxgrid;

    cFMS_get_interp_cdouble(NULL, NULL, NULL, NULL, NULL, 
        NULL, NULL, &nxgrid, 
        NULL, NULL, NULL, NULL, 
        NULL, NULL);

    int nlon_src;
    int nlat_src;
    int nlon_dst;
    int nlat_dst;
    int *i_src = (int *)malloc(nxgrid*sizeof(int));
    int *j_src = (int *)malloc(nxgrid*sizeof(int));
    int *i_dst = (int *)malloc(nxgrid*sizeof(int));
    int *j_dst = (int *)malloc(nxgrid*sizeof(int));
    double *area_frac_dst = (double *)malloc(nxgrid*sizeof(double));

    cFMS_get_interp_cdouble(NULL, i_src, j_src, i_dst, j_dst, 
        area_frac_dst, NULL, NULL, 
        &nlon_src, &nlat_src, &nlon_dst, &nlat_dst, 
        NULL, NULL);

    assert(nlon_src == NI_SRC);
    assert(nlat_src == NJ_SRC);
    assert(nlon_dst == iec-isc);
    assert(nlat_dst == jec-jsc);
    assert(interp_id == 0);
    
    cFMS_end();

    free(lon_in_1D);
    free(lat_in_1D);
    free(lon_out_1D);
    free(lat_out_1D);
    free(lon_in_2D);
    free(lat_in_2D);
    free(lon_out_2D);
    free(lat_out_2D);
    free(i_src);
    free(j_src);
    free(i_dst);
    free(j_dst);
    free(area_frac_dst);

    return EXIT_SUCCESS;
}

