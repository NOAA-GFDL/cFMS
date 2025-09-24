#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <c_fms.h>

extern void test_3d_cdouble(int *nx, int *ny, int *nz, double *c_pointer, bool *convert_cf_order);

int main()
{

  int nx = 10, ny = 5, nz = 2;
  double *array_3d;
  int ijk;
  bool convert_cf_order;
  
  array_3d = (double *)malloc(nx*ny*nz*sizeof(double));

  convert_cf_order = true;
  {
    ijk = 0;
    for(int i=0; i<nx; i++){
      for(int j=0; j<ny; j++){
        for(int k=0; k<nz; k++){
          array_3d[ijk++] = i*100. + j*10. + k*1.;
        }
      }
    }
    
    test_3d_cdouble(&nx, &ny, &nz, array_3d, &convert_cf_order);
    
    ijk = 0;
    for(int i=0; i<nx; i++){
      for(int j=0; j<ny; j++){
        for(int k=0; k<nz; k++){
          if(array_3d[ijk++] != -i*100. - j*10. - k*1.) {
            cFMS_error(FATAL, "ERROR CONVERTING ARRAY TO POINTER");
            exit(EXIT_FAILURE);
          }
        }
      }
    }
  }

  convert_cf_order = false;
  {
    ijk = 0;
    for(int k=0; k<nz; k++){
      for(int j=0; j<ny; j++){
        for(int i=0; i<nx; i++){
          array_3d[ijk++] = i*100. + j*10. + k*1.;
        }
      }
    }
    
    test_3d_cdouble(&nx, &ny, &nz, array_3d, &convert_cf_order);

    ijk = 0;
    for(int k=0; k<nz; k++){
      for(int j=0; j<ny; j++){
        for(int i=0; i<nx; i++){
          if(array_3d[ijk++] != -i*100. - j*10. - k*1.) {
            cFMS_error(FATAL, "ERROR CONVERTING ARRAY TO POINTER");
            exit(EXIT_FAILURE);
          }
        }
      }
    }
  }

  return EXIT_SUCCESS;
  
}
