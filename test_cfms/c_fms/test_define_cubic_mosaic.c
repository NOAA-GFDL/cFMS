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
void fill_cubic_grid_halo(int *halo_data, int *data1_all, int *data2_all, 
                          int tile, int ioff, int joff, int sign1, int sign2);

int main()
{
    int domain_id = 0;

    cFMS_init(NULL,NULL,NULL,NULL,NULL);

    domain_id = define_cubic_domain(&domain_id);
    cFMS_set_current_pelist(NULL,NULL);

    // test_define_cubic_mosaic(&domain_id);

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
    int *layout = (int *)calloc(2, sizeof(int));
    
    cFMS_define_layout(global_indices, &npes_per_tile, layout);

    int ni[6] = {NX, NX, NX, NX, NX, NX};
    int nj[6] = {NY, NY, NY, NY, NY, NY};

    return cFMS_define_cubic_mosaic(ni, nj, global_indices, layout, &ntiles, NULL, &use_memsize);
}

void test_define_cubic_mosaic(int *domain_id)
{
    int te, tn;
    int tile = cFMS_pe() + 1;
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

    double *global1_all = (double *)calloc((NX+1)*NY*NTILES, sizeof(double));
    double *global2_all = (double *)calloc(NX*(NY+1)*NTILES, sizeof(double));
    int xdatasize1 = WHALO + NX + EHALO + 1;
    int ydatasize1 = SHALO + NY + NHALO;
    int xdatasize2 = xdatasize1 - 1;
    int ydatasize2 = ydatasize1 + 1;
    double *global1 = (double *)calloc(xdatasize1*ydatasize1*NTILES, sizeof(double));
    double *global2 = (double *)calloc(xdatasize2*ydatasize2*NTILES, sizeof(double));

    double *x = (double *)calloc((xsize_d+1)*ysize_d, sizeof(double));
    double *y = (double *)calloc(xsize_d*(ysize_d+1), sizeof(double));

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
        for(int i = 0; i < NX+1; i++)
        {
            for(int j = 0; j < NY; j++)
            {
                global1_all[l*(NX+1)*NY + i*NY + j] = 1.0e3 + l + i*1.0e-3 + j*1.0e-6;
            }
        }
        for(int n = 0; n < NX; n++)
        {
            for(int k = 0; k < NY+1; k++)
            {
                global2_all[l*NX*(NY+1) + n*(NY+1) + k] = 2.0e3 + l + n*1.0e-3 + k*1.0e-6;

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
        if(l%2 == 0) //tiles 2, 4, 6
        {
            te = l + 2;
            tn = l + 1;
            if(te > 6) te = te - 6;
            if(tn > 6) tn = tn - 6;
            // global1_all(nx+shift,1:ny,:,l) = global2_all(nx:1:-1,1,:,te)  ! east
            // global2_all(1:nx,ny+shift,:,l) = global2_all(1:nx,1,:,tn)     ! north
            for(int i = 0; i < NY; i++)
            {
                global1_all[l*(NX+1)*NY + NX*NY + i] = global2_all[te*NX*(NY+1) + (NX-1-i)*NY + NY - 1];
                global2_all[l*NX*(NY+1) + i*(NY+1) + NY] = global2_all[tn*NX*(NY+1) + i*(NY+1)];
            }
        }
        else //tiles 1, 3, 5
        {
            te = l + 1;
            tn = l + 2;
            if(tn > 6) tn = tn - 6;
            // global1_all(nx+shift,:,:,l)    = global1_all(1,:,:,te)       ! east
            // global2_all(1:nx,ny+shift,:,l) = global1_all(1,ny:1:-1,:,tn) ! north
            for(int i = 0; i < NY; i++)
            {
                global1_all[l*(NX+1)*NY + NX*NY + i] = global1_all[te*(NX+1)*NY + i];
                global2_all[l*NX*(NY+1) + i*(NY+1) + NY] = global1_all[tn*(NX+1)*NY + NY - 1 - i];
            }
        }
    }
    // global1(1:nx+shift,1:ny  ,:,n) = global1_all(1:nx+shift,1:ny,  :,tile(n))
    // global2(1:nx  ,1:ny+shift,:,n) = global2_all(1:nx  ,1:ny+shift,:,tile(n))
    for(int i = 0; i < NX+1; i++)
    {
        for(int j = 0; j < NY; j++)
        {
            global1[(NX+1)*NY + (i+WHALO)*NY + j + SHALO] = global1_all[tile*(NX+1)*NY + i*NY + j];
            global2[NX*(NY+1) + (j+WHALO)*(NY+1) + i + SHALO] = global2_all[tile*NX*(NY+1) + j*(NY+1) + i];
        }
    }

    // x (isc:iec+shift,jsc:jec  ,:,:) = global1(isc:iec+shift,jsc:jec  ,:,:)
    // y (isc:iec  ,jsc:jec+shift,:,:) = global2(isc:iec  ,jsc:jec+shift,:,:)
    for(int l = 0; l < NTILES; l++)
    {
        for(int i = 0; i < xsize_c+1; i++)
        {
            for(int j = 0; j < ysize_c; j++)
            {
                x[l*(xsize_d+1)*ysize_d + (i+WHALO)*ysize_d + j + SHALO] = global1[l*xdatasize1*ydatasize2 + (i+isc)*ydatasize1 + j + jsc];
                y[l*xsize_d*(ysize_d+1) + (j+WHALO)*(ysize_d+1) + i + SHALO] = global2[l*xdatasize2*ydatasize2 + (j+isc)*ydatasize2 + i + jsc];
            }
        }
    }

}

void fill_cubic_grid_halo(int *halo_data, int *data1_all, int *data2_all, 
                          int tile, int ioff, int joff, int sign1, int sign2)
{
    int lw, le, ls, ln;
    if(tile%2 == 0)
    {
        lw = tile - 1;
        le = tile + 2;
        ls = tile - 2;
        ln = tile + 1;
        if(le > 6) le = le - 6;
        if(ls < 1) ls = ls + 6;
        if(ln > 6) ln = ln - 6;
    }
}



