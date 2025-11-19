#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <c_horiz_interp.h>
#include <c_grid_utils.h>
#include <c_constants.h>


int main() {
    int nlon = 100;
    int nlat = 100;
    double* lon, * lat;
    double* mask;

    double lon_start, lat_start, dlon;
    double lon_end, lat_end, dlat;
    double* area_answer;

    int* i_in, * j_in, * i_out, * j_out;
    double* xarea;

    int maxxgrid = cFMS_get_maxxgrid();
    int nxgrid;

    int ij;

    lon = (double*)malloc((nlon + 1) * (nlat + 1) * sizeof(double));
    lat = (double*)malloc((nlat + 1) * (nlat + 1) * sizeof(double));
    mask = (double*)malloc(nlon * nlat * sizeof(double));

    i_in = (int*)malloc(maxxgrid * sizeof(int));
    j_in = (int*)malloc(maxxgrid * sizeof(int));
    i_out = (int*)malloc(maxxgrid * sizeof(int));
    j_out = (int*)malloc(maxxgrid * sizeof(int));
    xarea = (double*)malloc(maxxgrid * sizeof(double));

    area_answer = (double*)malloc(nlon * nlat * sizeof(double));

    lon_start = 0.0;
    lon_end = 5.0 * PI / 4.0;
    lat_start = -PI / 4.0;
    lat_end = PI / 4.0;
    dlon = (lon_end - lon_start) / nlon;
    dlat = (lat_end - lat_start) / nlat;

    ij = 0;
    for (int j = 0; j < nlat + 1; j++) {
        for (int i = 0; i < nlon + 1; i++) {
            double this_lon = lon_start + dlon * (double)i;
            double this_lat = lat_start + dlat * (double)j;
            lon[ij] = this_lon;
            lat[ij++] = this_lat;
        }
    }

    ij = 0;
    for (int j = 0; j < nlat; j++) {
        for (int i = 0; i < nlon; i++) {
            mask[ij++] = 1.0;
        }
    }

    nxgrid = cFMS_create_xgrid_2dx2d_order1(&nlon, &nlat, &nlon, &nlat, lon, lat, lon, lat,
        mask, &maxxgrid, i_in, j_in, i_out, j_out, xarea);

    cFMS_get_grid_area(&nlon, &nlat, lon, lat, area_answer);

    assert(nxgrid == nlon * nlat);

    ij = 0;
    for (int j = 0; j < nlat; j++) {
        for (int i = 0; i < nlon; i++) {
            assert(i_in[ij] == i);
            assert(j_in[ij] == j);
            assert(xarea[ij] == area_answer[ij]);
            ij++;
        }
    }

    return EXIT_SUCCESS;
}
