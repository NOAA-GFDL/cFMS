#include <stdlib.h>

#include <stdio.h>
#include <string.h>

#include <c_fms.h>
#include <c_mpp_domains_helper.h>

/*
F -1 0 | 1 2 3 4 5 6 7 8 | 9 10
C  0 1 | 2 3 4 5 6 7 8 9 | 10 11
*/

#define NX 8
#define NY 8
#define NPES 2
#define WHALO 2
#define EHALO 2
#define NHALO 2
#define SHALO 2

void define_domain(int *domain_id);
void test_vector_double2d(int *domain_id);

int main()
{
  int domain_id = 0;

  cFMS_init(NULL,NULL,NULL,NULL,NULL);

  define_domain(&domain_id);
  cFMS_set_current_pelist(NULL,NULL);

  test_vector_double2d(&domain_id);

  cFMS_end();
  return EXIT_SUCCESS;

}

void define_domain(int *domain_id)
{
  int global_indices[4] = {0, NX-1, 0, NY-1};
  int npes = NPES;
  int cyclic_global_domain = CYCLIC_GLOBAL_DOMAIN;
  int fold_north_edge = FOLD_NORTH_EDGE;
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
  cdomain.yflags = &fold_north_edge;

  cFMS_define_domains_easy(cdomain);
}

void test_vector_double2d(int *domain_id)
{
    //Adapted from test_mpp_update_domains_real.F90 - Folded xy_halo - CGRID_NE

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
    
    /*
      allocate(global1r8(1-xhalo:nx+xhalo, 1-yhalo:ny+yhalo))
      allocate(global2r8(1-xhalo:nx+xhalo, 1-yhalo:ny+yhalo))
      global1r8(:,:) = 0.0
      global2r8(:,:) = 0.0
    */
    int xdatasize = WHALO+NX+EHALO;
    int ydatasize = SHALO+NY+NHALO;
    double *global_data1 = (double *)calloc(xdatasize*ydatasize,sizeof(double));
    double *global_data2 = (double *)calloc(xdatasize*ydatasize,sizeof(double));

    /*
      allocate(xr8 (isd:ied+shift,jsd:jed), yr8 (isd:ied,jsd:jed+shift) )
      xr8(:,:) = 0.; yr8(:,:) = 0.
    */
    double *x_data = (double *)calloc(xsize_d*ysize_d,sizeof(double));
    int x_shape[2] = {xsize_d, ysize_d};
    double *y_data = (double *)calloc(xsize_d*ysize_d,sizeof(double));
    int y_shape[2] = {xsize_d, ysize_d};

    /*
    do j = 1,ny
      do i = 1,nx
        global1r8(i,j,k) = 1 + i*1e-3 + j*1e-6
        global2r8(i,j,k) = 1 + i*1e-3 + j*1e-6
      end do
    end do
    */
    for(int i = 0; i<NX; i++)
    {
        for(int j = 0; j<NY; j++)
        {
                global_data1[(i+WHALO)*ydatasize + j + SHALO] = 1 + (i+WHALO)*1e-3 + (j+SHALO)*1e-6;
                global_data2[(i+WHALO)*ydatasize + j + SHALO] = 1 + (i+WHALO)*1e-3 + (j+SHALO)*1e-6;
        }
    }

    /*
    global1r8(1-xhalo:0,                   1:ny,:) =  global1r8(nx-xhalo+1:nx,                     1:ny,:)
    global1r8(nx+1:nx+xhalo,               1:ny,:) =  global1r8(1:xhalo,                           1:ny,:)
    global2r8(1-xhalo:0,                   1:ny,:) =  global2r8(nx-xhalo+1:nx,                     1:ny,:)
    global2r8(nx+1:nx+xhalo,               1:ny,:) =  global2r8(1:xhalo,                           1:ny,:)
    global1r8(1-xhalo:nx+xhalo-1, ny+1:ny+yhalo,:) = -global1r8(nx+xhalo-1:1-xhalo:-1, ny:ny-yhalo+1:-1,:)
    global1r8(nx+xhalo,           ny+1:ny+yhalo,:) = -global1r8(nx-xhalo,              ny:ny-yhalo+1:-1,:)
    global2r8(1-xhalo:nx+xhalo,   ny+1:ny+yhalo,:) = -global2r8(nx+xhalo:1-xhalo:-1,   ny-1:ny-yhalo:-1,:)
    */
    for(int i=0; i<WHALO; i++)
    {
        for(int j=SHALO; j<NY+SHALO; j++)
        {
                global_data1[i*ydatasize + j] = global_data1[(NX+i)*ydatasize + j];
                global_data1[(i + NX + WHALO)*ydatasize + j] = global_data1[(WHALO + i)*ydatasize + j];
                global_data2[i*ydatasize + j] = global_data2[(NX + i)*ydatasize + j];
                global_data2[(NX + WHALO + i)*ydatasize + j] = global_data2[(WHALO + i)*ydatasize + j];
        }
    }
    for(int i=0; i<NX+EHALO+1; i++)
    {
        for(int j=0; j<NHALO; j++)
        {
                global_data1[i*ydatasize + j+NY+SHALO] = -global_data1[(NX+EHALO-i)*ydatasize + NY + 1 - j];
                global_data1[(NX+WHALO+1)*ydatasize + j + NY + SHALO] = -global_data1[(NX-1)*ydatasize + NY + 1 - j];
        }
    }
    for(int i=0; i<WHALO+NX+EHALO; i++)
    {
        for(int j=0; j<NHALO; j++)
        {
                global_data2[i*ydatasize + j + NY + SHALO] = -global_data2[(WHALO+NX+1-i)*ydatasize + NY - j];
        }
    }
    
    /*
    xr8(is:ie+shift, js:je) = global1r8(is:ie+shift, js:je)
    yr8(is:ie, js:je+shift) = global2r8(is:ie, js:je+shift)
    */
    for(int i = 0; i<xsize_c; i++)
    {
        for(int j = 0; j<ysize_c; j++)
        {
            x_data[(i+WHALO)*ysize_d + j + SHALO] = global_data1[(i+isc)*ydatasize + j + jsc];
            y_data[(i+WHALO)*ysize_d + j + SHALO] = global_data2[(i+isc)*ydatasize + j + jsc];
        }
    }

    int *flags = NULL;
    int gridtype = CGRID_NE;
    int *complete = NULL;
    char *name = NULL;

    cFMS_v_update_domains_double_2d(x_shape, x_data, y_shape, y_data, domain_id, flags,
                                     &gridtype, complete, &whalo, &ehalo, &shalo, &nhalo,
                                     name, tile_count);

    /*
    global2r8(nx/2+1:nx, ny+shift) = -global2r8(nx/2:1:-1, ny+shift)
    global2r8(1-xhalo:0, ny+shift) = -global2r8(nx-xhalo+1:nx, ny+shift)
    global2r8(nx+1:nx+xhalo, ny+shift) = -global2r8(1:xhalo, ny+shift)
    */
    for(int i = 0; i<EHALO+WHALO; i++)
    {
        global_data2[(NX - WHALO + i)*ydatasize + NY + 1] = -global_data2[(NX - WHALO - 1 - i)*ydatasize + NY + 1];
    }
    for(int i = 0; i<WHALO; i++)
    {
        global_data2[i*ydatasize + NY + 1] = -global_data2[(NX + i)*ydatasize +  NY + 1];
        global_data2[(NX + WHALO + i)*ydatasize + NY + 1] = -global_data2[(WHALO + i )*ydatasize + NY + 1];
    }

    for(int i = 0; i<xsize_d; i++)
    {
        for(int j = 0; j<ysize_d; j++)
        {
            if(cFMS_pe() == 0 && y_data[i*ysize_d + j] != global_data2[i*ydatasize + j]) cFMS_error(FATAL, "y_data domain did not update correctly!");
            if(cFMS_pe() == 0 && x_data[i*ysize_d + j] != global_data1[i*ydatasize + j]) cFMS_error(FATAL, "x_data domain did not update correctly!");
        }
    }


    free(global_data1);
    free(global_data2);
    free(x_data);
    free(y_data);




}