#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <c_fms.h>
#include <c_horiz_interp.h>
#include <c_mpp_domains_helper.h>
#include <math.h>
#include "c_constants.h"

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
void define_domain(int* domain_id);

int test_conservative_new(int domain_id);

int test_bilinear_new(int domain_id);

// run both tests and also check their return statuses
int main() {
    int test_conserve, test_bilinear;

    int domain_id = 0;
    int ninterp = 2;

    define_domain(&domain_id);

    cFMS_horiz_interp_init(&ninterp);

    //printf("starting conservative test...\n");
    test_conserve = test_conservative_new(domain_id);
    //printf("done.\n");

    /*
    bilinear test needs to be fixed
    printf("starting bilinear test...\n");
    test_bilinear = test_bilinear_new(domain_id);
    printf("done.\n");
    */

    cFMS_horiz_interp_end();

    cFMS_end();

    return test_conserve && test_bilinear;

}

void define_domain(int* domain_id) {
    int global_indices[4] = { 0, NI_SRC - 1, 0, NJ_SRC - 1 };
    int npes = NPES;
    int cyclic_global_domain = CYCLIC_GLOBAL_DOMAIN;
    int whalo = WHALO;
    int ehalo = EHALO;
    int nhalo = NHALO;
    int shalo = SHALO;

    cFMS_init(NULL, NULL, NULL, NULL, NULL);

    cDomainStruct cdomain;

    cFMS_null_cdomain(&cdomain);

    cdomain.layout = (int*)calloc(2, sizeof(int));
    cFMS_define_layout(global_indices, &npes, cdomain.layout);

    cdomain.global_indices = global_indices;
    cdomain.whalo = &whalo;
    cdomain.ehalo = &ehalo;
    cdomain.shalo = &shalo;
    cdomain.nhalo = &nhalo;
    cdomain.xflags = &cyclic_global_domain;
    cdomain.yflags = &cyclic_global_domain;

    domain_id[0] = cFMS_define_domains_easy(cdomain);
}

int test_conservative_new(int domain_id) {
    // domain info

    int isc, iec, jsc, jec;
    int xsize_c = 0;
    int ysize_c = 0;
    int* xmax_size = NULL;
    int* ymax_size = NULL;
    int* tile_count = NULL;
    int* position = NULL;
    bool* x_is_global = NULL;
    bool* y_is_global = NULL;
    int whalo = WHALO;
    int shalo = SHALO;

    cFMS_get_compute_domain(&domain_id, &isc, &iec, &jsc, &jec, &xsize_c, xmax_size, &ysize_c,
        ymax_size, x_is_global, y_is_global, tile_count, position, &whalo, &shalo);

    // grid data
    double* lat_1D, * lon_1D;
    double* lon_in_2D, * lat_in_2D;
    double* lon_out_2D, * lat_out_2D;

    double lon_beg = 0.;
    double lon_end = 360.;
    double lat_beg = -90.;
    double lat_end = 90.;
    double dlon = (lon_end - lon_beg) / (double)NI_SRC;
    double dlat = (lat_end - lat_beg) / (double)NJ_SRC;

    char interp_method[MESSAGE_LENGTH] = "conservative";

    int lon_in_1d_size = NI_SRC + 1;
    lon_1D = (double*)malloc(lon_in_1d_size * sizeof(double));
    for (int i = 0; i < lon_in_1d_size; i++) lon_1D[i] = (lon_beg + i * dlon) * DEG_TO_RAD;

    int lat_in_1d_size = NJ_SRC + 1;
    lat_1D = (double*)malloc(lat_in_1d_size * sizeof(double));
    for (int j = 0; j < lat_in_1d_size; j++) lat_1D[j] = (lat_beg + j * dlat) * DEG_TO_RAD;

    int in_2d_size = lon_in_1d_size * lat_in_1d_size;
    lon_in_2D = (double*)malloc(in_2d_size * sizeof(double));
    lat_in_2D = (double*)malloc(in_2d_size * sizeof(double));

    int ij = 0;
    for (int i = 0; i < lon_in_1d_size; i++) {
        for (int j = 0; j < lat_in_1d_size; j++) {
            lon_in_2D[ij++] = lon_1D[i];
        }
    }

    ij = 0;
    for (int i = 0; i < lon_in_1d_size; i++) {
        for (int j = 0; j < lat_in_1d_size; j++) {
            lat_in_2D[ij++] = lat_1D[j];
        }
    }

    int out_2d_size = (xsize_c + 1) * (ysize_c + 1);
    lon_out_2D = (double*)malloc(out_2d_size * sizeof(double));
    lat_out_2D = (double*)malloc(out_2d_size * sizeof(double));

    ij = 0;
    for (int i = isc; i < iec + 1; i++) {
        for (int j = jsc; j < jec + 1; j++) {
            int ij_in = i * lon_in_1d_size + j;
            lat_out_2D[ij] = lat_in_2D[ij_in];
            lon_out_2D[ij++] = lon_in_2D[ij_in];
        }
    }
    
    int interp_id = 0;
    int test_interp_id;
    int nlon_in = NI_SRC;
    int nlat_in = NJ_SRC;
    int nlon_out = xsize_c;
    int nlat_out = ysize_c;
    bool save_weights_as_fregrid = true;

    test_interp_id = cFMS_horiz_interp_new_2d_cdouble(&nlon_in, &nlat_in, &nlon_out, &nlat_out,
        lon_in_2D, lat_in_2D, lon_out_2D, lat_out_2D, NULL, NULL, interp_method, NULL, NULL,
        NULL, NULL, NULL, &save_weights_as_fregrid, NULL);

    if (test_interp_id != interp_id) {
        cFMS_error(FATAL, "unexpected interp_id");
        cFMS_end();
        return EXIT_FAILURE;        
    }

    int nxgrid;
    cFMS_get_nxgrid(&test_interp_id, &nxgrid);

    int nlon_src, nlat_src, nlon_dst, nlat_dst, interp_method_enum, version;
    int* i_src = (int*)malloc(nxgrid * sizeof(int));
    int* j_src = (int*)malloc(nxgrid * sizeof(int));
    int* i_dst = (int*)malloc(nxgrid * sizeof(int));
    int* j_dst = (int*)malloc(nxgrid * sizeof(int));
    double* area_frac_dst = (double*)malloc(nxgrid * sizeof(double));
    double* xgrid_area = (double*)malloc(nxgrid * sizeof(double));

    cFMS_get_i_src(&test_interp_id, i_src);
    cFMS_get_j_src(&test_interp_id, i_src);
    cFMS_get_i_dst(&test_interp_id, i_dst);
    cFMS_get_j_dst(&test_interp_id, i_dst);
    cFMS_get_version(&test_interp_id, &version);
    cFMS_get_nlon_src(&test_interp_id, &nlon_src);
    cFMS_get_nlat_src(&test_interp_id, &nlat_src);
    cFMS_get_nlon_dst(&test_interp_id, &nlon_dst);
    cFMS_get_nlat_dst(&test_interp_id, &nlat_dst);
    cFMS_get_interp_method(&test_interp_id, &interp_method_enum);
    cFMS_get_area_frac_dst_cdouble(&test_interp_id, area_frac_dst);
    cFMS_get_xgrid_area_cdouble(&test_interp_id, xgrid_area);

    if(nlon_src != NI_SRC){
        cFMS_error(FATAL, "unexpected nlon_src");
        cFMS_end();
        exit(EXIT_FAILURE);
    }
    if(nlat_src != NJ_SRC){
        cFMS_error(FATAL, "unexpected nlat_src");
        cFMS_end();
        exit(EXIT_FAILURE);
    }
    if(nlon_dst != xsize_c){
        cFMS_error(FATAL, "unexlected nlon_dst");
        cFMS_end();
        exit(EXIT_FAILURE);
    }
    if(nlat_dst != ysize_c){
        cFMS_error(FATAL, "unexpected nlat_dst");
        cFMS_end();
        exit(EXIT_FAILURE);
    }
    if(interp_method_enum != 1){
        cFMS_error(FATAL, "unexpected interp_method");
        cFMS_end();
        exit(EXIT_FAILURE);
    } //1 is the enum for conservative set in horiz_interp_types.F90
    if(version != 2){
        cFMS_error(FATAL, "unexpected version");
        cFMS_end();
        exit(EXIT_FAILURE);
    }

    free(lon_1D);
    free(lat_1D);
    free(lon_in_2D);
    free(lat_in_2D);
    free(lon_out_2D);
    free(lat_out_2D);
    free(i_src);
    free(j_src);
    free(i_dst);
    free(j_dst);
    free(area_frac_dst);
    free(xgrid_area);

    return EXIT_SUCCESS;
}

/*
int test_bilinear_new(int domain_id) {
    // domain info
    int isc, iec, jsc, jec;
    int xsize_c = 0;
    int ysize_c = 0;
    int whalo = WHALO;
    int ehalo = EHALO;
    int shalo = SHALO;
    int nhalo = NHALO;
    int* xmax_size = NULL;
    int* ymax_size = NULL;
    int* tile_count = NULL;
    int* position = NULL;
    bool* x_is_global = NULL;
    bool* y_is_global = NULL;

    // grid data
    double* lat_in_1D, * lon_in_1D;
    double* lat_in_2D, * lon_in_2D;
    double* lat_out_1D, * lon_out_1D;
    double* lat_out_2D, * lon_out_2D;
    double dlon_src, dlat_src, dlon_out, dlat_out;

    double lon_src_beg = 0.;
    double lon_src_end = 360.;
    double lat_src_beg = -90.;
    double lat_src_end = 90.;
    double lon_out_beg = 0.;
    double lon_out_end = 360.;
    double lat_out_beg = -90.;
    double lat_out_end = 90.;
    double DEG_TO_RAD = 3.14 / 180.;
    double SMALL = 1.0e-10;

    char interp_method[MESSAGE_LENGTH] = "bilinear";

    // set up same source/destination lat/lon sizes
    dlon_src = (lon_src_end - lon_src_beg) / NI_SRC;
    dlat_src = (lat_src_end - lat_src_beg) / NJ_SRC;
    dlon_out = dlon_src;
    dlat_out = dlat_src;

    cFMS_get_compute_domain(&domain_id, &isc, &iec, &jsc, &jec, &xsize_c, xmax_size, &ysize_c, ymax_size,
        x_is_global, y_is_global, tile_count, position, &whalo, &shalo);

    int lon_in_1d_size = NI_SRC + 1;
    int lat_in_1d_size = NJ_SRC + 1;
    int lon_out_1d_size = iec + 1 - isc;
    int lat_out_1d_size = jec + 1 - jsc;

    lon_in_1D = (double*)malloc(lon_in_1d_size * sizeof(double));
    for (int i = 0; i < lon_in_1d_size; i++) lon_in_1D[i] = (lon_src_beg + i * dlon_src) * DEG_TO_RAD;

    lat_in_1D = (double*)malloc(lat_in_1d_size * sizeof(double));
    for (int j = 0; j < lat_in_1d_size; j++) lat_in_1D[j] = (lat_src_beg + j * dlat_src) * DEG_TO_RAD;

    lon_out_1D = (double*)malloc(lon_out_1d_size * sizeof(double));
    for (int i = 0; i < lon_out_1d_size; i++) lon_out_1D[i] = (lon_out_beg + i * dlon_out) * DEG_TO_RAD;

    lat_out_1D = (double*)malloc(lat_out_1d_size * sizeof(double));
    for (int j = 0; j < lat_out_1d_size; j++) lat_out_1D[j] = (lat_out_beg + j * dlat_out) * DEG_TO_RAD;

    int in_2d_size = lon_in_1d_size * lat_in_1d_size;
    int out_2d_size = lon_out_1d_size * lat_out_1d_size;

    lon_in_2D = (double*)malloc(in_2d_size * sizeof(double));
    for (int i = 0; i < lon_in_1d_size; i++) {
        for (int j = 0; j < lat_in_1d_size; j++) {
            lon_in_2D[lat_in_1d_size * i + j] = lon_in_1D[i];
        }
    }
    
    lat_in_2D = (double*)malloc(in_2d_size * sizeof(double));
    for (int i = 0; i < lon_in_1d_size; i++) {
        for (int j = 0; j < lat_in_1d_size; j++) {
            lat_in_2D[lat_in_1d_size * i + j] = lat_in_1D[j];
        }
    }
    
    // for the destination, we take the midpoints between 1d source points
    lon_out_2D = (double*)malloc(out_2d_size * sizeof(double));
    for (int i = 0; i < lon_out_1d_size; i++) {
        double midpoint = (lon_out_1D[i] + lon_out_1D[i + 1]) * 0.5;
        for (int j = 0; j < lat_out_1d_size; j++) {
            lon_out_2D[lat_out_1d_size * i + j] = midpoint;
        }
    }
    
    lat_out_2D = (double*)malloc(out_2d_size * sizeof(double));

    for (int i = 0; i < lat_out_1d_size; i++) {
        double midpoint = (lat_out_1D[i] + lat_out_1D[i + 1]) * 0.5;
        for (int j = 0; j < lon_out_1d_size; j++) {
            lat_out_2D[lat_out_1d_size * j + i] = midpoint;
        }
    }
    
    int interp_id = 1;
    int test_interp_id;

    test_interp_id = cFMS_horiz_interp_new_2d_cdouble(&lon_in_1d_size, &lat_in_1d_size, &lon_out_1d_size,
        &lat_out_1d_size, lon_in_2D, lat_in_2D, lon_out_2D, lat_out_2D,
        NULL, NULL, interp_method, NULL, NULL,
        NULL, NULL, NULL, NULL);

    assert(test_interp_id == interp_id);

    int nlon_src;
    int nlat_src;
    int nlon_out;
    int nlat_out;

    int* i_lon = (int*)malloc(nlon_out * nlat_out * 2 * sizeof(int));
    int* j_lat = (int*)malloc(nlon_out * nlat_out * 2 * sizeof(int));
    cFMS_get_i_lon(&test_interp_id, i_lon);
    cFMS_get_j_lat(&test_interp_id, j_lat);
    double* area_frac_dst;
    double* wti = (double*)malloc(nlon_out * nlat_out * 2 * sizeof(double));
    double* wtj = (double*)malloc(nlon_out * nlat_out * 2 * sizeof(double));
    cFMS_get_wti_cdouble(&test_interp_id, wti);
    cFMS_get_wtj_cdouble(&test_interp_id, wtj);

    bool is_allocated = false;
    cFMS_get_is_allocated_cdouble(&test_interp_id, &is_allocated);
    assert(is_allocated);

    // since output grid is the midpoints, weights will all be 0.5
    const double tolerance = 1.0e-10;
    const double expected = 0.5;
    for (int i = 0; i < nlon_out * nlat_out * 2; i++) {
        assert(abs(wti[i] - expected) < tolerance);
        assert(abs(wtj[i] - expected) < tolerance);
    }

    // nlon/lat_src/dst for bilinear is exactly the size of the indices sent in
    assert(nlon_src == NI_SRC + 1);
    assert(nlat_src == NJ_SRC + 1);
    assert(nlon_out == iec - isc + 1);
    assert(nlat_out == jec - jsc + 1);

    free(lon_in_1D);
    free(lat_in_1D);
    free(lon_out_1D);
    free(lat_out_1D);
    free(lon_in_2D);
    free(lat_in_2D);
    free(lon_out_2D);
    free(lat_out_2D);
    free(i_lon);
    free(j_lat);
    free(wti);
    free(wtj);

    cFMS_horiz_interp_end();

    return EXIT_SUCCESS;
}
*/
