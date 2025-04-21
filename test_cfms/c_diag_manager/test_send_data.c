#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <c_mpp_domains_helper.h>
#include <c_fms.h>
#include <c_diag_manager.h>

#define NX 8
#define NY 8
#define NZ 2

//TODO:  add reading in the outputted file for correctness
//Currently, answers have been checked separately/manually for correctness
int main() 
{
  
  int domain_id = -99;
  int id_x, id_y, id_z;
  
  int id_var3;
  int var3_shape[3] = {NX, NY, NZ};
  float *var3;

  int id_var2;
  int var2_shape[2] = {NX, NY};
  float *var2;

  int calendar_type = NOLEAP;
    
  var3 = (float *)malloc(NX*NY*NZ*sizeof(float));
  int ijk = 0;  
  for(int i=0; i<NX; i++) {
    for(int j=0; j<NY; j++) {
      for(int k=0; k<NZ; k++) {
        var3[ijk++] = i*100. + j*10. + k*1.;
      }
    }
  }

  var2 = (float *)malloc(NX*NY*sizeof(float));
  int ij = 0;  
  for(int i=0; i<NX; i++) {
    for(int j=0; j<NY; j++) {
      var2[ij++] = i*10. + j*1.;
    }
  }    
  
  cFMS_init(NULL, NULL, NULL, NULL, &calendar_type);  

  // define domain
  {
    cDomainStruct cdomain;
    int global_indices[4] = {0, NX-1, 0, NY-1};
    int layout[2] = {1,1};
    int io_layout[2] = {1,1};
    cFMS_null_cdomain(&cdomain);  
    cdomain.global_indices = global_indices;
    cdomain.layout = layout;
    domain_id = cFMS_define_domains_easy(cdomain);
    cFMS_define_io_domain(io_layout,&domain_id);
  }
  
  // diag manager init
  {
    int  diag_model_subset = DIAG_ALL;
    int *time_init = NULL;
    char err_msg[NAME_LENGTH] = "None";
    cFMS_diag_init(&diag_model_subset, time_init, err_msg);
  }
  
  cFMS_set_current_domain(&domain_id);
  
  //diag axis init x
  {    
    char name[NAME_LENGTH] = "x";
    int naxis_data = NX;
    double x[NX];
    char units[NAME_LENGTH] = "point_E";
    char cart_name[NAME_LENGTH] = "x";
    char long_name[NAME_LENGTH] = "point_E";
    char set_name[NAME_LENGTH] = "atm";
    int *direction = NULL;
    int *edges = NULL;
    char *aux = NULL;
    char *req = NULL;
    int *tile_count = NULL;
    int *domain_position = NULL;

    for(int i=0; i<NX; i++) x[i] = i;
    
    id_x = cFMS_diag_axis_init_cdouble(name, &naxis_data, x, units, cart_name, &domain_id, long_name, direction, 
                                       set_name, edges, aux, req, tile_count, domain_position, NULL);
  }

  //diag axis init y
  {    
    char name[NAME_LENGTH] = "y";
    int naxis_data = NY;
    double y[NY];
    char units[NAME_LENGTH] = "point_N";
    char cart_name[NAME_LENGTH] = "y";
    char long_name[NAME_LENGTH] = "point_N";
    char set_name[NAME_LENGTH] = "atm";
    int *direction = NULL;
    int *edges = NULL;
    char *aux = NULL;
    char *req = NULL;
    int *tile_count = NULL;
    int *domain_position = NULL;

    for(int j=0; j<NY; j++) y[j] = j;
    
    id_y = cFMS_diag_axis_init_cdouble(name, &naxis_data, y, units, cart_name, &domain_id, long_name, direction, 
                                       set_name, edges, aux, req, tile_count, domain_position, NULL);
  }

  //diag axis init z
  {    
    char name[NAME_LENGTH] = "z";
    int naxis_data = NZ;
    double z[NZ];
    char units[NAME_LENGTH] = "pointer_Z";
    char cart_name[NAME_LENGTH] = "z";
    char long_name[NAME_LENGTH] = "point_Z";
    char set_name[NAME_LENGTH] = "atm";
    int *direction = NULL;
    int *edges = NULL;
    char *aux = NULL;
    char *req = NULL;
    int *tile_count = NULL;
    int *domain_position = NULL;
    bool not_xy = true;

    for(int k=0; k<NZ; k++) z[k] = k;
    
    id_z = cFMS_diag_axis_init_cdouble(name, &naxis_data, z, units, cart_name, NULL, long_name, direction, 
                                       set_name, edges, aux, req, tile_count, domain_position, &not_xy);
  }
  
  // register_diag_field var3
  {
    char module_name[NAME_LENGTH] = "atm_mod";
    char field_name[NAME_LENGTH] = "var_3d";
    int axes[5] = {id_x, id_y, id_z, 0, 0};
    char long_name[NAME_LENGTH] = "Var in a lon/lat domain";
    char units[NAME_LENGTH] = "muntin";
    float missing_value = -99.99;
    float range[2] = {-1000., 1000.};
    bool *mask_variant = NULL;
    char *standard_name = NULL;
    bool *verbose = NULL;
    bool *do_not_log = NULL;
    char *interp_method = NULL;
    int *tile_count = NULL;
    int *area = NULL;
    int *volume = NULL;
    char *realm = NULL;
    bool *multiple_send_data = NULL;

    char err_msg[MESSAGE_LENGTH]="None";
    
    int year = 2;
    int month = 1;
    int day = 1;
    int hour = 1;
    int minute = 1;
    int second = 1;
    int *tick = NULL;
    
    cFMS_diag_set_field_init_time(&year, &month, &day, &hour, &minute, &second, tick, err_msg);
    id_var3 = cFMS_register_diag_field_array_cfloat(module_name, field_name, axes, long_name, units, &missing_value, range,
                                                    mask_variant, standard_name, verbose, do_not_log, err_msg, interp_method,
                                                    tile_count, area, volume, realm, multiple_send_data);
    int ddays = 0;
    int dseconds = 60*60;
    int dticks = 0;
    cFMS_diag_set_field_timestep(&id_var3, &dseconds, &ddays, &dticks,  NULL);
  }

  // register_diag_field var2
  {
    char module_name[NAME_LENGTH] = "atm_mod";
    char field_name[NAME_LENGTH] = "var_2d";
    int axes[5] = {id_x, id_y, 0, 0, 0};
    char long_name[NAME_LENGTH] = "Var in a lon/lat domain";
    char units[NAME_LENGTH] = "muntin";
    float missing_value = -99.99;
    float range[2] = {-1000., 1000.};
    bool *mask_variant = NULL;
    char *standard_name = NULL;
    bool *verbose = NULL;
    bool *do_not_log = NULL;
    char *interp_method = NULL;
    int *tile_count = NULL;
    int *area = NULL;
    int *volume = NULL;
    char *realm = NULL;
    bool *multiple_send_data = NULL;

    char err_msg[MESSAGE_LENGTH]="None";
    
    int year = 2;
    int month = 1;
    int day = 1;
    int hour = 1;
    int minute = 1;
    int second = 1;
    int *tick = NULL;
    
    cFMS_diag_set_field_init_time(&year, &month, &day, &hour, &minute, &second, tick, err_msg);
    id_var2 = cFMS_register_diag_field_array_cfloat(module_name, field_name, axes, long_name, units, &missing_value, range,
                                                    mask_variant, standard_name, verbose, do_not_log, err_msg, interp_method,
                                                    tile_count, area, volume, realm, multiple_send_data);
    int ddays = 0;
    int dseconds = 60*60;
    int dticks = 0;
    cFMS_diag_set_field_timestep(&id_var2, &dseconds, &ddays, &dticks,  NULL);
  }

  
  // cFMS_diag_set_time_end
  {
    int year = 2;
    int month = 1;
    int day = 2;
    int hour = 1;
    int minute = 1;
    int second = 1;
    int *tick = NULL;
    cFMS_diag_set_time_end(&year, &month, &day, &hour, &minute, &second, tick, NULL);    
  }

  // send_data
  for(int itime=0; itime<24; itime++) {    
    int ijk = 0;
    for(int i=0; i<NX; i++){
      for(int j=0; j<NY; j++){
        for(int k=0; k<NZ; k++){
          var3[ijk] = -1.0 * var3[ijk];
          ijk++;
        }
      }
    }
    cFMS_diag_advance_field_time(&id_var3);
    cFMS_diag_send_data_3d_cfloat(&id_var3, var3_shape, var3, NULL);
    cFMS_diag_send_complete(&id_var3, NULL);

    int ij = 0;
    for(int i=0; i<NX; i++){
      for(int j=0; j<NY; j++){
        var2[ij] = -1.0 * var2[ij];
        ij++;
      }
    }
    cFMS_diag_advance_field_time(&id_var2);
    cFMS_diag_send_data_2d_cfloat(&id_var2, var2_shape, var2, NULL);
    cFMS_diag_send_complete(&id_var2, NULL);    
  }

  cFMS_diag_end();
  
  cFMS_end();
  return EXIT_SUCCESS;
  
}
  
