#ifndef C_HORIZ_INTERP_H
#define C_HORIZ_INTERP_H

#include <stdbool.h>

extern int cFMS_create_xgrid_2dx2d_order1(int *nlon_in, int *nlat_in, int *nlon_out, int *nlat_out,
                                          double *lon_in, double *lat_in, double *lon_out, double *lat_out,
                                          double *mask_in, int *maxxgrid, int *i_in, int *j_int, int *i_out,
                                          int *j_out, double *xgrid_area);

extern int cFMS_get_maxxgrid();

extern void cFMS_horiz_interp_init(int *ninterp);

extern void cFMS_horiz_interp_end();

extern bool c_horiz_interp_is_initialized();

extern int cFMS_horiz_interp_new_2d_cfloat(int *nlon_in, int *nlat_in, int *nlon_out, int *nlat_out,
                                           float *lon_in_ptr,float *lat_in_ptr, float *lon_out_ptr,
                                           float *lat_out_ptr, float *mask_in_ptr, float *mask_out_ptr,
                                           char *interp_method, int *verbose, float *max_dist, bool *src_modulo, 
                                           bool *is_latlon_in, bool *is_latlon_out, bool *convert_cf_order);

extern int cFMS_horiz_interp_new_2d_cdouble(int *nlon_in, int *nlat_in, int *nlon_out, int *nlat_out,
                                            double *lon_in_ptr,double *lat_in_ptr, double *lon_out_ptr,
                                            double *lat_out_ptr, double *mask_in_ptr, double *mask_out_ptr,
                                            char *interp_method, int *verbose, double *max_dist, bool *src_modulo, 
                                            bool *is_latlon_in, bool *is_latlon_out, bool *convert_cf_order);

extern void cFMS_horiz_interp_base_2d_cfloat(int *interp_id, float *data_in_ptr, float *data_out_ptr, 
                                             float *mask_in_ptr, float *mask_out_ptr, int *verbose,
                                             float *missing_value, int *missing_permit, bool *new_missing_handle,
                                             bool *convert_cf_order);

extern void cFMS_horiz_interp_base_2d_cdouble(int *interp_id, double *data_in_ptr, double *data_out_ptr,
                                              double *mask_in_ptr, double *mask_out_ptr, int *verbose,
                                              double *missing_value, int *missing_permit, bool *new_missing_handle,
                                              bool *convert_cf_order);

// Integer pointer getters
extern void cFMS_get_i_src(int *interp_id, int *i_src);
extern void cFMS_get_j_src(int *interp_id, int *j_src);
extern void cFMS_get_i_dst(int *interp_id, int *i_dst);
extern void cFMS_get_j_dst(int *interp_id, int *j_dst);
extern void cFMS_get_i_lon(int *interp_id, int *i_lon);
extern void cFMS_get_j_lat(int *interp_id, int *j_lat);

// float/double pointer getters
extern void cFMS_get_area_frac_dst_cfloat(int *interp_id, float *area_frac_dst);
extern void cFMS_get_area_frac_dst_cdouble(int *interp_id, double *area_frac_dst);
extern void cFMS_get_wti_cdouble(int *interp_id, double *wti);
extern void cFMS_get_wti_cfloat(int *interp_id, float *wti);
extern void cFMS_get_wtj_cdouble(int *interp_id, double *wtj);
extern void cFMS_get_wtj_cfloat(int *interp_id, float *wtj);
extern void cFMS_get_is_allocated_cdouble(int *interp_id, bool *is_allocated);
extern void cFMS_get_is_allocated_cfloat(int *interp_id, bool *is_allocated);

// Scalar getters
extern void cFMS_get_version(int *interp_id, int *version);
extern void cFMS_get_nxgrid(int *interp_id, int *nxgrid);
extern void cFMS_get_nlon_src(int *interp_id, int *nlon_src);
extern void cFMS_get_nlat_src(int *interp_id, int *nlat_src);
extern void cFMS_get_nlon_dst(int *interp_id, int *nlon_dst);
extern void cFMS_get_nlat_dst(int *interp_id, int *nlat_dst);
extern void cFMS_get_is_allocated(int *interp_id, bool *is_allocated);
extern void cFMS_get_interp_method(int *interp_id, int *interp_method);


#endif
