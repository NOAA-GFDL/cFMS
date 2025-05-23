#ifndef C_HORIZ_INTERP_H
#define C_HORIZ_INTERP_H

#include <stdbool.h>

extern int cFMS_create_xgrid_2dx2d_order1(int *nlon_in, int *nlat_in, int *nlon_out, int *nlat_out,
                                          double *lon_in, double *lat_in, double *lon_out, double *lat_out,
                                          double *mask_in, int *maxxgrid, int *i_in, int *j_int, int *i_out,
                                          int *j_out, double *xgrid_area);

extern int cFMS_get_maxxgrid();

extern void cFMS_horiz_interp_init(int *ninterp);

extern void cFMS_horiz_interp_dealloc();

extern void cFMS_set_current_interp(int *interp_id);

extern int cFMS_horiz_interp_2d_cfloat(float *lon_in_ptr, int *lon_in_shape, float *lat_in_ptr,
                                            int *lat_in_shape, float *lon_out_ptr, int *lon_out_shape,
                                            float *lat_out_ptr, int *lat_out_shape, char *interp_method,
                                            int *verbose, float *max_dist, bool *src_modulo, 
                                            float *mask_in_ptr, float *mask_out_ptr, bool *is_latlon_in, 
                                            bool *is_latlon_out);

extern int cFMS_horiz_interp_2d_cdouble(double *lon_in_ptr, int *lon_in_shape, double *lat_in_ptr,
                                            int *lat_in_shape, double *lon_out_ptr, int *lon_out_shape,
                                            double *lat_out_ptr, int *lat_out_shape, char *interp_method,
                                            int *verbose, double *max_dist, bool *src_modulo, 
                                            double *mask_in_ptr, double *mask_out_ptr, bool *is_latlon_in, 
                                            bool *is_latlon_out);

extern void cFMS_get_interp_cfloat(int *interp_id, int *i_src, int *j_src, int *i_dst, int *j_dst, 
                                            float *area_frac_dst, int *version, int *nxgrid, 
                                            int *nlon_src, int *nlat_src, int *nlon_dst, int *nlat_dst, 
                                            bool *is_allocated, int *interp_method);
                                   
extern void cFMS_get_interp_cdouble(int *interp_id, int *i_src, int *j_src, int *i_dst, int *j_dst, 
                                            double *area_frac_dst, int *version, int *nxgrid, 
                                            int *nlon_src, int *nlat_src, int *nlon_dst, int *nlat_dst, 
                                            bool *is_allocated, int *interp_method);

// Integer pointer getters
extern void cFMS_get_i_src(int *interp_id, int *i_src);
extern void cFMS_get_j_src(int *interp_id, int *j_src);
extern void cFMS_get_i_dst(int *interp_id, int *i_dst);
extern void cFMS_get_j_dst(int *interp_id, int *j_dst);

// Area fraction (float/double) pointer getters
extern void cFMS_get_area_frac_dst_cfloat(int *interp_id, float *area_frac_dst);
extern void cFMS_get_area_frac_dst_cdouble(int *interp_id, double *area_frac_dst);
extern void cFMS_get_i_lon_double(int *interp_id, double *i_lon);
extern void cFMS_get_i_lon_float(int *interp_id, float *i_lon);
extern void cFMS_get_j_lat_double(int *interp_id, double *j_lat);
extern void cFMS_get_j_lat_float(int *interp_id, float *j_lat);

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
