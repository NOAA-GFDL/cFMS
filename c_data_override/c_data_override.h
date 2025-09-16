#ifndef C_DATA_OVERRIDE_H
#define C_DATA_OVERRIDE_H

#include <stdbool.h>

extern const int CFLOAT_MODE;
extern const int CDOUBLE_MODE;

extern void cFMS_data_override_0d_cfloat(char *gridname, char *fieldname_code, float *data_out, bool *override,
                                         int *data_index);

extern void cFMS_data_override_0d_cdouble(char *gridname, char *fieldname_code, float *data_out, bool *override,
                                          int *data_index);

extern void cFMS_data_override_2d_cfloat(char *gridname, char *fieldname, int *data_shape, float *data,
                                         bool *override, int *is, int *ie, int *js, int *je, bool *convert_cf_order);

extern void cFMS_data_override_2d_cdouble(char *gridname, char *fieldname, int *data_shape, double *data,
                                          bool *override, int *is, int *ie, int *js, int *je, bool *convert_cf_order);

extern void cFMS_data_override_3d_cfloat(char *gridname, char *fieldname, int *data_shape, float *data,
                                         bool *override, int *data_index, int *is, int *ie, int *js,
                                         int *je, bool *convert_cf_order);

extern void cFMS_data_override_3d_cdouble(char *gridname, char *fieldname, int *data_shape, double *data,
                                          bool *override, int *data_index, int *is, int *ie, int *js, int *je,
                                          bool *convet_cf_order);

extern void cFMS_data_override_init(int *atm_domain_id, int *ocn_domain_id, int *ice_domain_id, int *land_domain_id,
                                    int *land_domainUG_id, int *mode);

extern void cFMS_data_override_set_time(int *year, int *month, int *day, int *hour, int *minute, int *second,
                                        int *tick, char *err_msg);

#endif
