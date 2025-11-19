#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "c_mpp_domains_helper.h"
#include "c_fms.h"

#define NX 16
#define NY 8

void test_2d(int domain_id);
void test_1d();
void test_1d_v();

int main() {

    int* global_pelist = NULL;
    int global_indices[4] = { 0, NX - 1, 0, NY - 1 };
    CFMS_TEST_KIND_ kind;
    cDomainStruct domain;

    cFMS_init(NULL, NULL, NULL, NULL, NULL);
    cFMS_null_cdomain(&domain);

    // set domain fields to use the easy domain method
    int ndivs = cFMS_npes();
    domain.layout = (int*)malloc(2 * sizeof(int));
    domain.global_indices = global_indices;

    // set layout
    cFMS_define_layout(global_indices, &ndivs, domain.layout);

    // set domain
    int domain_id = cFMS_define_domains_easy(domain);


    printf("testing cFMS_gather_1d %d\n", sizeof(kind));
    test_1d();

    printf("testing cFMS_gather_2d_pelist %d\n", sizeof(kind));
    test_2d(domain_id);

    printf("testing cFMS_gather_v_1d %d\n", sizeof(kind));
    test_1d_v();

    cFMS_end();
    return EXIT_SUCCESS;

}

void test_2d(int domain_id) {
    // get pe
    int pe = cFMS_pe();
    int root_pe = cFMS_root_pe();
    bool is_root_pe = pe == root_pe ? true : false;

    // get pelist
    int ndivs = cFMS_npes();
    int pelist[4] = { 0, 0, 0, 0 };
    cFMS_get_domain_pelist(&ndivs, pelist, &domain_id);

    // get compute domain
    int isc, iec, jsc, jec, xsize, ysize;
    cFMS_get_compute_domain(&domain_id, &isc, &iec, &jsc, &jec,
        &xsize, NULL, &ysize, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

    // set data to send
    int ij = 0;
    CFMS_TEST_KIND_* send;
    send = (CFMS_TEST_KIND_*)malloc(xsize * ysize * sizeof(CFMS_TEST_KIND_));
    for (int i = 0; i < xsize; i++) {
        for (int j = 0; j < ysize; j++) {
            send[ij++] = (CFMS_TEST_KIND_)((isc + i) * 100 + j + jsc);
        }
    }

    // set data to receive
    CFMS_TEST_KIND_* gather;
    int gather_shape[2] = { 1, 1 };
    if (is_root_pe) {
        gather_shape[0] = NX;
        gather_shape[1] = NY;
        gather = (CFMS_TEST_KIND_*)calloc(NX * NY, sizeof(CFMS_TEST_KIND_));
    }
    else
        gather = (CFMS_TEST_KIND_*)malloc(1 * sizeof(CFMS_TEST_KIND_));

    CFMS_GATHER_PELIST_2D_(&isc, &iec, &jsc, &jec, &ndivs, pelist,
        send, gather_shape, gather, &is_root_pe, NULL, NULL, NULL);

    if (is_root_pe) {
        int ij = 0;
        for (int i = 0; i < NX; i++) {
            for (int j = 0; j < NY; j++) {
                if (gather[ij++] != (CFMS_TEST_KIND_)(i * 100 + j))
                    cFMS_error(FATAL, "error testing cFMS_gather_pelist");
            }
        }
    }
}

void test_1d() {

    int sbuf_size = 4;
    int rbuf_size = sbuf_size * cFMS_npes();
    int pe = cFMS_pe();
    CFMS_TEST_KIND_* sbuf, * rbuf;

    bool is_root_pe = pe == cFMS_root_pe() ? true : false;

    //set data
    sbuf = (CFMS_TEST_KIND_*)malloc(sbuf_size * sizeof(CFMS_TEST_KIND_));
    for (int i = 0; i < sbuf_size; i++) {
        sbuf[i] = (CFMS_TEST_KIND_)(pe * 10 + i);
    }

    //set receive data;
    if (is_root_pe) {
        rbuf = (CFMS_TEST_KIND_*)calloc(rbuf_size, sizeof(CFMS_TEST_KIND_));
    }

    CFMS_GATHER_1D_(&sbuf_size, &rbuf_size, sbuf, rbuf, NULL, NULL);

    //check answers
    if (is_root_pe) {
        int ij = 0;
        for (int p = 0; p < cFMS_npes(); p++) {
            for (int i = 0; i < sbuf_size; i++) {
                if (rbuf[ij++] != (CFMS_TEST_KIND_)(p * 10 + i))
                    cFMS_error(FATAL, "error testing cFMS_gather_1d_cCFMS_GATHER_TYPE_");
            }
        }
    }

}

void test_1d_v() {

    int npes = cFMS_npes();
    int pe = cFMS_pe();
    int ssize = pe + 1;

    CFMS_TEST_KIND_* sbuf;
    sbuf = (CFMS_TEST_KIND_*)malloc(ssize * sizeof(CFMS_TEST_KIND_));
    for (int i = 0; i < ssize; i++) {
        sbuf[i] = (CFMS_TEST_KIND_)(pe * 10 + i);
    }

    int* rsize, rbuf_size = 0;
    rsize = (int*)malloc(npes * sizeof(int));
    for (int p = 0; p < npes; p++) {
        rsize[p] = p + 1;
        rbuf_size += rsize[p];
    }

    CFMS_TEST_KIND_* rbuf;
    if (pe == cFMS_root_pe())
        rbuf = (CFMS_TEST_KIND_*)malloc(rbuf_size * sizeof(CFMS_TEST_KIND_));

    CFMS_GATHERV_1D_(&npes, &ssize, &rbuf_size, sbuf, &ssize, rbuf, rsize, NULL);

    CFMS_TEST_KIND_* answers;
    for (int i = 0; i < rbuf_size; i++) {
        answers = (CFMS_TEST_KIND_*)malloc(rbuf_size * sizeof(CFMS_TEST_KIND_));
        int ij = 0;
        for (int p = 0; p < npes; p++) {
            for (int j = 0; j < p + 1; j++) {
                answers[ij++] = (CFMS_TEST_KIND_)(p * 10 + j);
            }
        }
    }

    if (pe == cFMS_root_pe()) {
        for (int i = 0; i < rbuf_size; i++) {
            if (rbuf[i] != answers[i])
                cFMS_error(FATAL, "error testing cFMS_gather_v_1d_cCFMS_GATHER_TYPE_");
        }
    }
}
