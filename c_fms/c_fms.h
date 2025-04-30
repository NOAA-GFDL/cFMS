#ifndef CFMS_H
#define CFMS_H

#include <stdbool.h>

#define NAME_LENGTH 64 
#define MESSAGE_LENGTH 128

extern const int NOTE;
extern const int WARNING;
extern const int FATAL;

extern int GLOBAL_DATA_DOMAIN;
extern int BGRID_NE;
extern int CGRID_NE;
extern int DGRID_NE;
extern int AGRID;
extern int FOLD_SOUTH_EDGE;
extern int FOLD_NORTH_EDGE;
extern int FOLD_WEST_EDGE;
extern int FOLD_EAST_EDGE;  
extern int CYCLIC_GLOBAL_DOMAIN;
extern int NUPDATE;
extern int EUPDATE;
extern int XUPDATE;
extern int YUPDATE;
extern int NORTH;
extern int NORTH_EAST;
extern int EAST;
extern int SOUTH_EAST;
extern int CORNER;
extern int CENTER;
extern int SOUTH;
extern int SOUTH_WEST;
extern int WEST;
extern int NORTH_WEST;
extern int CYCLIC_GLOBAL_DOMAIN;

extern const int THIRTY_DAY_MONTHS;
extern const int GREGORIAN;
extern const int JULIAN;
extern const int NOLEAP;

extern void cFMS_init(int *localcomm, char *alt_input_nml_path, int *ndomain, int *nnest_domain, int *calendar_type);

extern void cFMS_end();

extern void cFMS_set_pelist_npes(int *npes_in);

extern void cFMS_error(int errortype, char *errormsg);

extern int cFMS_get_domain_count();

extern int cFMS_get_nest_domain_count();

extern void cFMS_declare_pelist(int *pelist, char *name, int *commID);

extern void cFMS_get_current_pelist(int *pelist, char *name, int *commID);

extern int cFMS_npes();

extern int cFMS_pe();

extern void cFMS_set_current_pelist(int *pelist, bool *no_sync);

extern int cFMS_define_domains(int* global_indices, int* layout, int *npelist, int *pelist, 
                               int *xflags, int *yflags, int *xhalo, int *yhalo, int* xextent, int *yextent,
                               bool *maskmap, char *name, bool *symmetry, int* memory_size,
                               int *whalo, int *ehalo, int *shalo, int *nhalo, bool *is_mosaic,
                               int *tile_count, int *tile_id, bool *complete, int *x_cyclic_offset,
                               int *y_cyclic_offset);

extern void cFMS_define_io_domain(int* io_layout, int *domain_id);

extern void cFMS_define_layout(int* global_indices, int *ndivs, int* layout);

extern int cFMS_define_nest_domains(int *num_nest, int *ntiles, int* nest_level, int* tile_fine, int* tile_course,
                                    int* istart_coarse, int* icount_coarse, int* jstart_coarse, int* jcount_coarse,
                                    int* npes_nest_tile, int* x_refine, int* y_refine, int* domain_id,
                                    int *extra_halo, char *name);

extern bool cFMS_domain_is_initialized(int *domain_id);

extern void cFMS_get_compute_domain(int *domain_id, int *xbegin, int *xend, int *ybegin, int *yend,
                                    int *xsize, int *xmax_size, int *ysize, int *ymax_size,
                                    bool *x_is_global, bool *y_is_global, int *tile_count, int *position,
                                    int *whalo, int *shalo);
  
extern void cFMS_get_data_domain(int *domain_id, int *xbegin, int *xend, int *ybegin, int *yend,
                                 int *xsize, int *xmax_size, int *ysize, int *ymax_size,
                                 bool *x_is_global, bool *y_is_global, int *tile_count, int *position,
                                 int *whalo, int *shalo);

extern void cFMS_get_domain_name(char *domain_name_c, int *domain_id);

extern void cFMS_get_domain_pelist(int* pelist, int *domain_id);

extern void cFMS_get_layout(int* layout, int *domain_id);

extern void cFMS_set_compute_domain(int *domain_id, int *xbegin, int *xend, int *ybegin, int *yend,
                                    int *xsize, int *ysize, bool *x_is_global, bool *y_is_global, int *tile_count,
                                    int *whalo, int *shalo);

extern void cFMS_set_current_domain(int *domain_id);

extern void cFMS_set_data_domain(int *domain_id, int *xbegin, int *xend, int *ybegin, int *yend,
                                 int *xsize, int *ysize, bool *x_is_global, bool *y_is_global, int *tile_count,
                                 int *whalo, int *shalo);

extern void cFMS_set_global_domain(int *domain_id, int *xbegin, int *xend, int *ybegin, int *yend,
                                   int *xsize, int *ysize, int *tile_count);

extern void cFMS_update_domains_double_2d(int *field_shape, double *field, int *domain_id, int *flags, int *complete,
                                         int *position, int *whalo, int *ehalo, int *shalo, int *nhalo,
                                         char *name, int *tile_count);

extern void cFMS_update_domains_double_3d(int *field_shape, double *field, int *domain_id, int *flags, int *complete,
                                         int *position, int *whalo, int *ehalo, int *shalo, int *nhalo,
                                         char *name, int *tile_count);

extern void cFMS_update_domains_double_4d(int *field_shape, double *field, int *domain_id, int *flags, int *complete,
                                         int *position, int *whalo, int *ehalo, int *shalo, int *nhalo,
                                         char *name, int *tile_count);

extern void cFMS_update_domains_double_5d(int *field_shape, double *field, int *domain_id, int *flags, int *complete,
                                         int *position, int *whalo, int *ehalo, int *shalo, int *nhalo,
                                         char *name, int *tile_count);

extern void cFMS_update_domains_float_2d(int *field_shape, float *field, int *domain_id, int *flags, int *complete,
                                         int *position, int *whalo, int *ehalo, int *shalo, int *nhalo,
                                         char *name, int *tile_count);

extern void cFMS_update_domains_float_3d(int *field_shape, float *field, int *domain_id, int *flags, int *complete,
                                         int *position, int *whalo, int *ehalo, int *shalo, int *nhalo,
                                         char *name, int *tile_count);

extern void cFMS_update_domains_float_4d(int *field_shape, float *field, int *domain_id, int *flags, int *complete,
                                         int *position, int *whalo, int *ehalo, int *shalo, int *nhalo,
                                         char *name, int *tile_count);

extern void cFMS_update_domains_float_5d(int *field_shape, float *field, int *domain_id, int *flags, int *complete,
                                         int *position, int *whalo, int *ehalo, int *shalo, int *nhalo,
                                         char *name, int *tile_count);

extern void cFMS_update_domains_int_2d(int *field_shape, int *field, int *domain_id, int *flags, int *complete,
                                         int *position, int *whalo, int *ehalo, int *shalo, int *nhalo,
                                         char *name, int *tile_count);

extern void cFMS_update_domains_int_3d(int *field_shape, int *field, int *domain_id, int *flags, int *complete,
                                         int *position, int *whalo, int *ehalo, int *shalo, int *nhalo,
                                         char *name, int *tile_count);

extern void cFMS_update_domains_int_4d(int *field_shape, int *field, int *domain_id, int *flags, int *complete,
                                         int *position, int *whalo, int *ehalo, int *shalo, int *nhalo,
                                         char *name, int *tile_count);

extern void cFMS_update_domains_int_5d(int *field_shape, int *field, int *domain_id, int *flags, int *complete,
                                         int *position, int *whalo, int *ehalo, int *shalo, int *nhalo,
                                         char *name, int *tile_count);

extern void cFMS_v_update_domains_double_2d(int *fieldx_shape, double *fieldx, int *fieldy_shape, double *fieldy,
                                              int *domain_id, int *flags, int *gridtype, int *complete,
                                              int *whalo, int *ehalo, int *shalo, int *nhalo,
                                              char *name, int *tile_count);

extern void cFMS_v_update_domains_double_3d(int *fieldx_shape, double *fieldx, int *fieldy_shape, double *fieldy,
                                              int *domain_id, int *flags, int *gridtype, int *complete,
                                              int *whalo, int *ehalo, int *shalo, int *nhalo,
                                              char *name, int *tile_count);

extern void cFMS_v_update_domains_double_4d(int *fieldx_shape, double *fieldx, int *fieldy_shape, double *fieldy,
                                              int *domain_id, int *flags, int *gridtype, int *complete,
                                              int *whalo, int *ehalo, int *shalo, int *nhalo,
                                              char *name, int *tile_count);

extern void cFMS_v_update_domains_double_5d(int *fieldx_shape, double *fieldx, int *fieldy_shape, double *fieldy,
                                              int *domain_id, int *flags, int *gridtype, int *complete,
                                              int *whalo, int *ehalo, int *shalo, int *nhalo,
                                              char *name, int *tile_count);

extern void cFMS_v_update_domains_float_2d(int *fieldx_shape, float *fieldx, int *fieldy_shape, float *fieldy,
                                              int *domain_id, int *flags, int *gridtype, int *complete,
                                              int *whalo, int *ehalo, int *shalo, int *nhalo,
                                              char *name, int *tile_count);

extern void cFMS_v_update_domains_float_3d(int *fieldx_shape, float *fieldx, int *fieldy_shape, float *fieldy,
                                              int *domain_id, int *flags, int *gridtype, int *complete,
                                              int *whalo, int *ehalo, int *shalo, int *nhalo,
                                              char *name, int *tile_count);

extern void cFMS_v_update_domains_float_4d(int *fieldx_shape, float *fieldx, int *fieldy_shape, float *fieldy,
                                              int *domain_id, int *flags, int *gridtype, int *complete,
                                              int *position, int *whalo, int *ehalo, int *shalo, int *nhalo,
                                              char *name, int *tile_count);

extern void cFMS_v_update_domains_float_5d(int *fieldx_shape, float *fieldx, int **fieldy_shape, float *fieldy,
                                              int *domain_id, int *flags, int *gridtype, int *complete,
                                              int *whalo, int *ehalo, int *shalo, int *nhalo,
                                              char *name, int *tile_count);

extern int cFMS_define_cubic_mosaic(char *type, int *ni, int *nj, int *global_indices, int *layout, int *ntiles,
                                              int *halo, bool *use_memsize);


#endif
