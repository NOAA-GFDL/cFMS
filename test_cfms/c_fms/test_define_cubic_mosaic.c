#include <stdlib.h>

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include <c_fms.h>
#include <c_mpp_domains_helper.h>

#define NX 48
#define NY 48
#define NPES 30
#define WHALO 3
#define SHALO 3
#define NTILES 6

/*
Test Define Cubic Mosaic:

This unit test is intended to test the functionality of
defining mosaic domains for a cube sphere. It checks
the resulting values for the indicies, sizes (and their
maximums) for each compute and data domain generated.

The current test uses a c48 grid, with a 1x5 layout, over
6 tiles, with 3 halo points. This results in 5 pes per tile, 
for a total of 30 pes. An example of this distribution (with 
compute domain sizes indicated at center):
-----------------------------
|                           |
|                           |
|          48x10            |
|                           |
|                           |
-----------------------------
-----------------------------
|                           |
|                           |
|          48x10            |
|                           |
|                           |
-----------------------------
-----------------------------
|                           |
|                           |
|          48x8             |
|                           |
-----------------------------
-----------------------------
|                           |
|                           |
|          48x10            |
|                           |
|                           |
-----------------------------
-----------------------------
|                           |
|                           |
|          48x10            |
|                           |
|                           |
-----------------------------
*/

int define_cubic_domain();
void test_define_cubic_mosaic(int *domain_id);

int main()
{
    int domain_id = 0;

    cFMS_init(NULL,NULL,NULL,NULL,NULL);

    domain_id = define_cubic_domain();
    cFMS_set_current_pelist(NULL,NULL,NULL);

    test_define_cubic_mosaic(&domain_id);

    cFMS_end();
    return EXIT_SUCCESS;

}

int define_cubic_domain()
{
    int ntiles = NTILES;
    int npes = NPES;
    int npes_per_tile = npes/ntiles;
    bool use_memsize = 0;
    int global_indices[4] = {0, NX-1, 0, NY-1};
    int layout[2] = {1, 5};

    int ni[6] = {NX, NX, NX, NX, NX, NX};
    int nj[6] = {NY, NY, NY, NY, NY, NY};
    int halo = 3;

    return cFMS_define_cubic_mosaic(ni, nj, global_indices, layout, &ntiles, &halo, &use_memsize);
}

void test_define_cubic_mosaic(int *domain_id)
{
    int jsc_check[30] = {3, 13, 23, 31, 41, 3, 13, 23, 31, 41, 3, 13, 23, 31, 41, 
        3, 13, 23, 31, 41, 3, 13, 23, 31, 41, 3, 13, 23, 31, 41};
    int jec_check[30] = {12, 22, 30, 40, 50, 12, 22, 30, 40, 50, 12, 22, 30, 40, 50, 
        12, 22, 30, 40, 50, 12, 22, 30, 40, 50, 12, 22, 30, 40, 50};
    int jsd_check[30] = {0, 10, 20, 28, 38, 0, 10, 20, 28, 38, 0, 10, 20, 28, 38, 
        0, 10, 20, 28, 38, 0, 10, 20, 28, 38, 0, 10, 20, 28, 38};
    int jed_check[30] = {15, 25, 33, 43, 53, 15, 25, 33, 43, 53, 15, 25, 33, 43, 53, 
        15, 25, 33, 43, 53, 15, 25, 33, 43, 53, 15, 25, 33, 43, 53};
    int ysize_c_check[30] = {10, 10, 8, 10, 10, 10, 10, 8, 10, 10, 10, 10, 8, 10, 10, 
        10, 10, 8, 10, 10, 10, 10, 8, 10, 10, 10, 10, 8, 10, 10};
    int ysize_d_check[30] = {16, 16, 14, 16, 16, 16, 16, 14, 16, 16, 16, 16, 14, 16, 16, 
        16, 16, 14, 16, 16, 16, 16, 14, 16, 16, 16, 16, 14, 16, 16};
    int pe = cFMS_pe();
    int isc, iec, jsc, jec;
    int isd, ied, jsd, jed;
    int xsize_c = 0;
    int xsize_d = 0;
    int ysize_c = 0;
    int ysize_d = 0;
    int whalo = WHALO;
    int shalo = SHALO;

    int xmax_size_c  = 0;
    int ymax_size_c  = 0;
    int xmax_size_d  = 0;
    int ymax_size_d  = 0;
    int tile_count = 0;
    int *position   = NULL;
    bool *x_is_global = NULL;
    bool *y_is_global = NULL;

    // get compute domain indices
    cFMS_get_compute_domain(domain_id, &isc, &iec, &jsc, &jec, &xsize_c, &xmax_size_c, &ysize_c, &ymax_size_c,
        x_is_global, y_is_global, &tile_count, position, &whalo, &shalo);

    // get data domain sizes
    cFMS_get_data_domain(domain_id, &isd, &ied, &jsd, &jed, &xsize_d, &xmax_size_d, &ysize_d, &ymax_size_d,
        x_is_global, y_is_global, &tile_count, position, &whalo, &shalo);

    if(isc != 3) cFMS_error(FATAL, "isc has not been properly set");
    if(iec != 50) cFMS_error(FATAL, "iec has not been properly set");
    if(jsc != jsc_check[pe]) cFMS_error(FATAL, "jsc has not been properly set");
    if(jec != jec_check[pe]) cFMS_error(FATAL, "jec has not been properly set");
    if(isd != 0) cFMS_error(FATAL, "isc has not been properly set");
    if(ied != 53) cFMS_error(FATAL, "iec has not been properly set");
    if(jsd != jsd_check[pe]) cFMS_error(FATAL, "jsc has not been properly set");
    if(jed != jed_check[pe]) cFMS_error(FATAL, "jec has not been properly set");
    if(xsize_c != 48) cFMS_error(FATAL, "incorrect xsize for compute domain");
    if(ysize_c != ysize_c_check[pe]) cFMS_error(FATAL, "incorrect ysize for compute domain");
    if(xsize_d != 54) cFMS_error(FATAL, "incorrect xsize for data domain");
    if(ysize_d != ysize_d_check[pe]) cFMS_error(FATAL, "incorrect ysize for data domain");
    if(xmax_size_c != 48) cFMS_error(FATAL, "incorrect xmax_size for compute domain");
    if(ymax_size_c != 10) cFMS_error(FATAL, "incorrect ymax_size for compute domain");
    if(xmax_size_d != 54) cFMS_error(FATAL, "incorrect xmax_size for data domain");
    if(ymax_size_d != 16) cFMS_error(FATAL, "incorrect ymax_size for data domain");
}



