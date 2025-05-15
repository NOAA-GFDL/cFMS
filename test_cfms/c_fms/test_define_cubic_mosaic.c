#include <stdlib.h>

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include <c_fms.h>
#include <c_mpp_domains_helper.h>

#define NX 8
#define NY 8
#define NPES 6
#define WHALO 2
#define EHALO 2
#define NHALO 2
#define SHALO 2
#define NTILES 6

int define_cubic_domain();
void test_define_cubic_mosaic(int *domain_id);
void fill_cubic_grid_halo(double *halo_data, int halodatasize, double *data1_all, int all1_xsize, 
                          int all1_ysize, double *data2_all, int all2_xsize, int all2_ysize,
                          int tile, int ioff, int joff, int sign1, int sign2);

int main()
{
    int domain_id = 0;

    cFMS_init(NULL,NULL,NULL,NULL,NULL);

    domain_id = define_cubic_domain(&domain_id);
    cFMS_set_current_pelist(NULL,NULL);

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
    // int global_indices[4] = {1, NX, 1, NY};
    int *layout = (int *)calloc(2, sizeof(int));
    
    cFMS_define_layout(global_indices, &npes_per_tile, layout);

    int ni[6] = {NX, NX, NX, NX, NX, NX};
    int nj[6] = {NY, NY, NY, NY, NY, NY};

    return cFMS_define_cubic_mosaic(ni, nj, global_indices, layout, &ntiles, NULL, &use_memsize);
}

void test_define_cubic_mosaic(int *domain_id)
{
    int te, tn;
    int tile = cFMS_pe();
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

    int shift = 1;

    int *xmax_size  = NULL;
    int *ymax_size  = NULL;
    int *tile_count = NULL;
    int *position   = NULL;
    bool *x_is_global = NULL;
    bool *y_is_global = NULL;

    // get compute domain indices
    cFMS_get_compute_domain(domain_id, &isc, &iec, &jsc, &jec, &xsize_c, xmax_size, &ysize_c, ymax_size,
        x_is_global, y_is_global, tile_count, position, &whalo, &shalo);

    // get data domain sizes
    cFMS_get_data_domain(domain_id, &isd, &ied, &jsd, &jed, &xsize_d, xmax_size, &ysize_d, ymax_size,
        x_is_global, y_is_global, tile_count, position, &whalo, &shalo);

    int all1x_size = NX + 1;
    int all1y_size = NY;
    int all2x_size = all1x_size - 1;
    int all2y_size = all1y_size + 1;
    double *global1_all = (double *)calloc(all1x_size*all1y_size*NTILES, sizeof(double));
    double *global2_all = (double *)calloc(all2x_size*all2y_size*NTILES, sizeof(double));
    int xdatasize1 = WHALO + NX + EHALO + 1;
    int ydatasize1 = SHALO + NY + NHALO;
    int xdatasize2 = xdatasize1 - 1;
    int ydatasize2 = ydatasize1 + 1;
    double *global1 = (double *)calloc(xdatasize1*ydatasize1, sizeof(double));
    double *global2 = (double *)calloc(xdatasize2*ydatasize2, sizeof(double));

    double *x = (double *)calloc((xsize_d+1)*ysize_d, sizeof(double));
    int x_shape[2] = {xsize_d+1, ysize_d};
    double *y = (double *)calloc(xsize_d*(ysize_d+1), sizeof(double));
    int y_shape[2] = {xsize_d, ysize_d+1};

    // do l = 1, ntiles
    //       do k = 1, nz
    //          do j = 1, ny
    //             do i = 1, nx+shift
    //                global1_all(i,j,k,l) = 1.0e3 + l + i*1.0e-3 + j*1.0e-6 + k*1.0e-9
    //             end do
    //          end do
    //          do j = 1, ny+shift
    //             do i = 1, nx
    //                global2_all(i,j,k,l) = 2.0e3 + l + i*1.0e-3 + j*1.0e-6 + k*1.0e-9
    //             end do
    //          end do
    //       end do
    // end do

    for(int l = 0; l < NTILES; l++)
    {
        for(int i = 0; i < all1x_size; i++)
        {
            for(int j = 0; j < all1y_size; j++)
            {
                global1_all[l*all1x_size*all1y_size + i*all1y_size + j] = 1.0e3 + l + (i+WHALO)*1.0e-3 + (j+SHALO)*1.0e-6;
            }
        }
        for(int n = 0; n < all2x_size; n++)
        {
            for(int k = 0; k < all2y_size; k++)
            {
                global2_all[l*all2x_size*all2y_size + n*all2y_size + k] = 2.0e3 + l + (n+WHALO)*1.0e-3 + (k+SHALO)*1.0e-6;

            }
        }
    }

    //-----------------------------------------------------------------------
    //--- make sure consistency on the boundary for cubic grid
    //--- east boundary will take the value of neighbor tile ( west/south),
    //--- north boundary will take the value of neighbor tile ( south/west).
    //-----------------------------------------------------------------------

    for(int l = 0; l < NTILES; l++)
    {
        if(l%2 == 0) //tiles 0, 2, 4
        {
            te = l + 1;
            tn = l + 2;
            if(tn > 5) tn = tn - 6;
            // global1_all(nx+shift,:,:,l)    = global1_all(1,:,:,te)       ! east
            // global2_all(1:nx,ny+shift,:,l) = global1_all(1,ny:1:-1,:,tn) ! north
            for(int i = 0; i < NY; i++)
            {
                global1_all[l*(NX+1)*NY + NX*NY + i] = global1_all[te*(NX+1)*NY + i];
                global2_all[l*NX*(NY+1) + i*(NY+1) + NY] = global1_all[tn*(NX+1)*NY + NY - 1 - i];
            }
        }
        else //tiles 1, 3, 5
        {
            te = l + 2;
            tn = l + 1;
            if(te > 5) te = te - 6;
            if(tn > 5) tn = tn - 6;
            // global1_all(nx+shift,1:ny,:,l) = global2_all(nx:1:-1,1,:,te)  ! east
            // global2_all(1:nx,ny+shift,:,l) = global2_all(1:nx,1,:,tn)     ! north
            for(int i = 0; i < NY; i++)
            {
                global1_all[l*(NX+1)*NY + NX*NY + i] = global2_all[te*NX*(NY+1) + (NX-1-i)*NY + NY - 1];
                global2_all[l*NX*(NY+1) + i*(NY+1) + NY] = global2_all[tn*NX*(NY+1) + i*(NY+1)];
            }
        }
    }

    // global1(1:nx+shift,1:ny  ,:,n) = global1_all(1:nx+shift,1:ny,  :,tile(n))
    // global2(1:nx  ,1:ny+shift,:,n) = global2_all(1:nx  ,1:ny+shift,:,tile(n))
    for(int i = 0; i < NX+1; i++)
    {
        for(int j = 0; j < NY; j++)
        {
            global1[(i+WHALO)*NY + j + SHALO] = global1_all[tile*all1x_size*all1y_size + i*all1y_size + j];
            global2[(j+WHALO)*(NY+1) + i + SHALO] = global2_all[tile*NX*(NY+1) + j*(NY+1) + i];
        }
    }

    // x (isc:iec+shift,jsc:jec  ,:,:) = global1(isc:iec+shift,jsc:jec  ,:,:)
    // y (isc:iec  ,jsc:jec+shift,:,:) = global2(isc:iec  ,jsc:jec+shift,:,:)
    for(int i = 0; i < xsize_c+1; i++)
    {
        for(int j = 0; j < ysize_c; j++)
        {
            x[(i+WHALO)*ysize_d + j + SHALO] = global1[(i+isc)*ydatasize1 + j + jsc];
            y[(j+WHALO)*(ysize_d+1) + i + SHALO] = global2[(j+isc)*ydatasize2 + i + jsc];
        }
    }

    //DELETE ME
    // if(cFMS_pe() == 0) printf("ysize_d = %d, yadatasize1 = %d\n", ysize_d, ydatasize1);
    // if(cFMS_pe() == 0) printf("isd = %d, ied = %d, ied - isd = %d, xsize_d = %d\n", isd, ied, ied - isd, xsize_d);
    // printf("isc = %d, iec = %d, iec - isc = %d, xsize_d = %d\n", isc, iec, iec - isc, xsize_d);
    for(int i = 0; i<xsize_d+1; i++)
    {
        for(int j = 0; j<ysize_d; j++)
        {
            if(cFMS_pe() == 0) printf("x[%d][%d] = %f\n", i, j, x[i*ysize_d + j]);
        }
    }

    // -----------------------------------------------------------------------
    //                    fill up the value at halo points for cubic-grid.
    //    On the contact line, the following relation will be used to
    //    --- fill the value on contact line ( balance send and recv).
    //        1W --> 0E, 0S --> 5N, 2W --> 0N, 3S --> 1E
    //        3W --> 2E, 2S --> 1N, 0W --> 4N, 1S --> 5E
    //        5W --> 4E, 4S --> 3N, 4W --> 2N, 5S --> 3E
    // ---------------------------------------------------------------------------
    fill_cubic_grid_halo(global1, ydatasize1, global1_all, all1x_size, all1y_size,
                         global2_all, all2x_size, all2y_size, tile, 1, 0, 1, -1);
    fill_cubic_grid_halo(global2, ydatasize1, global1_all, all1x_size, all1y_size,
                         global2_all, all2x_size, all2y_size, tile, 0, 1, -1, 1);

    int *flags = NULL;
    int gridtype = CGRID_NE;
    int *complete = NULL;
    char *name = NULL;
    cFMS_v_update_domains_double_2d(x_shape, x, y_shape, y, domain_id, flags,
                                    &gridtype, complete, &whalo, &ehalo, &shalo, &nhalo,
                                    name, tile_count);
    
    //DELETE ME
    // for(int i = 0; i<xsize_d+1; i++)
    // {
    //     for(int j = 0; j<ysize_d; j++)
    //     {
    //         if(cFMS_pe() == 0) printf("x[%d][%d] = %f\n", i, j, x[i*ysize_d + j]);
    //     }
    // }

}

void fill_cubic_grid_halo(double *halo_data, int halodatasize, double *data1_all, int all1_xsize,
                          int all1_ysize, double *data2_all, int all2_xsize, int all2_ysize, 
                          int tile, int ioff, int joff, int sign1, int sign2)
{
    int lw, le, ls, ln;
    if(tile%2 == 0) // tiles 0, 2, 4
    {
        lw = tile - 2;
        le = tile + 1;
        ls = tile - 1;
        ln = tile + 2;
        if(lw < 0) lw = lw + 6;
        if(ls < 0) ls = ls + 6;
        if(ln > 5) ln = ln - 6;
        // do i = 1, whalo
        //   halo_data(1-i, 1:ny+joff, :)     = sign1*data2_all(nx+joff:1:-1, ny-i+1, :, lw) ! west
        // end do
        // halo_data(nx+1+ioff:nx+ehalo+ioff, 1:ny+joff, :) = data1_all(1+ioff:ehalo+ioff, 1:ny+joff, :, le) ! east
        // halo_data(1:nx+ioff, 1-shalo:0, :)     = data1_all(1:nx+ioff, ny-shalo+1:ny, :, ls) ! south
        // do i = 1, nhalo
        //   halo_data(1:nx+ioff, ny+i+joff, :)    = sign2*data2_all(i+joff, ny+ioff:1:-1, :, ln) ! north
        // end do
        for(int i = 0; i < 2; i++)
        {
            for(int j = 0; j < NY+joff; j++)
            {
                halo_data[(WHALO + 1 -i)*halodatasize + SHALO + j] = sign1*data2_all[lw*all2_xsize*all2_ysize + (NX - 1 + joff - j)*all2_ysize + NY - 1 - i];
                halo_data[(WHALO + NX + ioff + i)*halodatasize + SHALO + j] = data1_all[le*all1_xsize*all1_ysize + (i + ioff)*all1_ysize + j];
            }
            for(int k = 0; k < NX+ioff; k++)
            {
                halo_data[(WHALO + k)*halodatasize + i] = data1_all[ls*all1_xsize*all1_ysize + k*all1_ysize + NY - SHALO + i];
                halo_data[(WHALO + k)*halodatasize + NY + SHALO + joff + i] = sign2*data2_all[ln*all2_xsize*all2_ysize + (i + joff)*all2_ysize + NY - 1 + ioff - k];
            }
        }

    }
    else // tiles 1, 3, 5
    {
        lw = tile - 1;
        le = tile + 2;
        ls = tile - 2;
        ln = tile + 1;
        if(le > 5) le = le - 6;
        if(ls < 0) ls = ls + 6;
        if(ln > 5) ln = ln - 6;
        // halo_data(1-whalo:0, 1:ny+joff, :) = data1_all(nx-whalo+1:nx, 1:ny+joff, :, lw) ! west
        // do i = 1, ehalo
        //   halo_data(nx+i+ioff, 1:ny+joff, :)    = sign1*data2_all(nx+joff:1:-1, i+ioff, :, le) ! east
        // end do
        // do i = 1, shalo
        //   halo_data(1:nx+ioff, 1-i, :)     = sign2*data2_all(nx-i+1, ny+ioff:1:-1, :, ls) ! south
        // end do
        // halo_data(1:nx+ioff, ny+1+joff:ny+nhalo+joff, :) = data1_all(1:nx+ioff, 1+joff:nhalo+joff, :, ln) ! north
        for(int i = 0; i < 2; i++)
        {
            for(int j = 0; j < NY+joff; j++)
            {
                halo_data[i*halodatasize + SHALO + j] = data1_all[lw*all1_xsize*all1_ysize + (NX - WHALO + i)*all1_ysize + j];
                halo_data[(NX + WHALO + ioff + i)*halodatasize + SHALO + j] = sign1*data2_all[le*all2_xsize*all2_ysize + (NX - 1 + joff - j)*all2_ysize + i + ioff];
            }
            for(int k = 0; k < NX+ioff; k++)
            {
                halo_data[(WHALO + k)*halodatasize + SHALO + 1 - i] = sign2*data2_all[ls*all2_xsize*all2_ysize + (NX - 1 - i)*all2_ysize + NY - 1 + ioff - k];
                halo_data[(WHALO + k)*halodatasize + SHALO + NY + joff + i] = data1_all[ln*all1_xsize*all1_ysize + k*all1_ysize + i + joff];
            }
        }
    }
}



