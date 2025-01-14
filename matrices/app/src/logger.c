#include "mop.h"

/**
 * Log the resulting matrix to the file descriptor specified.
 * @return 1 to indicate success and 0 to indicate failure.
 */
int log_coo_result(COO *coo_mtx, FILE *fp) {
    int k = 0;
    bool seen = false;
    if((*coo_mtx).is_int) {
        int **mtx = malloc((*coo_mtx).size * sizeof(int *));
        if(mtx == NULL) {
            perror(NULL);
            return 0;
        }
        for(int i = 0; i < (*coo_mtx).size; i++) {
            mtx[i] = malloc(3 * sizeof(int));
            memcpy(mtx[i], (*coo_mtx).mtxi[i], 3 * sizeof(int));
        }
        for(int i = 0; i < (*coo_mtx).row; i++) {
            for(int j = 0; j < (*coo_mtx).col; j++) {
                for(k = 0; k < (*coo_mtx).size; k++) {
                    if(mtx[k][0] == i && mtx[k][1] == j) {
                        seen = true;
                        break;
                    }
                }
                if(seen) fprintf(fp, "%i ", mtx[k][2]);
                else fprintf(fp, "%i ", 0);
                seen = false;
            }
        }
        // -- DEALLOCATE --
        for(int i = 0; i < (*coo_mtx).size; i++) {
            free(mtx[i]);
        }
        free(mtx);
    }
    else {
        float **mtx = malloc((*coo_mtx).size * sizeof(float *));
        for(int i = 0; i < (*coo_mtx).size; i++) {
            mtx[i] = malloc(3 * sizeof(float));
            memcpy(mtx[i], (*coo_mtx).mtxf[i], 3 * sizeof(float));
        }
        for(int i = 0; i < (*coo_mtx).row; i++) {
            for(int j = 0; j < (*coo_mtx).col; j++) {
                for(k = 0; k < (*coo_mtx).size; k++) {
                    if(mtx[k][0] == (float) i && mtx[k][1] == (float) j) {
                        seen = true;
                        break;
                    }
                }
                if(seen) fprintf(fp, "%3.2f ", mtx[k][2]);
                else fprintf(fp, "%3.2f ", 0.0);
                seen = false;
            }
        }
        // -- DEALLOCATE --
        for(int i = 0; i < (*coo_mtx).size; i++) {
            free(mtx[i]);
        }
        free(mtx);
    }
    fprintf(fp, "\n");
    return 1;
}

/**
 * Log the resulting trace sum to the specified file descriptor.
 * @param coo_mtx type COO*
 */
int log_trace_result(COO *coo_mtx, int *tracei, float *tracef, FILE *fp) {
    if((*coo_mtx).is_int) fprintf(fp, "%i", *tracei);
    else fprintf(fp, "%3.2f", *tracef);
    fprintf(fp, "\n");
    return 1;
}

/**
 * Log the resulting matrix to the specified file descriptor.
 * @param csr_mtx type CSR
 */
int log_csr_result(CSR *csr_mtx, FILE *fp) {
    int nz, row_el;
    nz = 0; // index of current nnz element
    if((*csr_mtx).is_int) {
    	for(int i = 0; i < (*csr_mtx).row; i++) { //rows
            row_el = (*csr_mtx).mtx_offset[i+1]; //convention offset;
            for(int j = 0; j < (*csr_mtx).col; j++) {
                if((*csr_mtx).mtx_offset[i+1] == 0) fprintf(fp, "%i ", 0);
                else if(row_el > 0) { // nnz elements left to be 'consumed'
                    if((*csr_mtx).mtx_col[nz] == j) { //the column of the nz element
                        fprintf(fp, "%i ", (*csr_mtx).mtxi[nz++]);
                        row_el--;
                    }
                    else fprintf(fp, "%i ", 0); //No non-zero element for this column
              	}
                else fprintf(fp, "%i ", 0);
           }
        }
    }
    else {
    	for(int i = 0; i < (*csr_mtx).row; i++) { //rows
            row_el = (*csr_mtx).mtx_offset[i+1]; //convention offset
            for(int j = 0; j < (*csr_mtx).col; j++) {
                if((*csr_mtx).mtx_offset[i+1] == 0) fprintf(fp, "%3.2f ", 0.0);
                else if(row_el > 0) { // nnz elements left to be 'consumed'
                    if((*csr_mtx).mtx_col[nz] == j) { //the column of the nz element
                        fprintf(fp, "%3.2f ", (*csr_mtx).mtxf[nz++]);
                        row_el--;
                    }
                    else fprintf(fp, "%3.2f ", 0.0); //No non-zero element for this column
              	}
                else fprintf(fp, "%3.2f ", 0.0);
           }
        }
    }
    fprintf(fp, "\n");
    return 1;
}

/**
 * Log the resulting matrix to stdout or file if specified.
 * Reads a CSC struct values as a CSR struct.
 * @param csc_mtx type CSC*
 */
int log_csc_ts_result(CSC *csc_mtx, FILE *fp) {
    int n, col, index, row;
    n = (*csc_mtx).row * (*csc_mtx).col;
    int num_nz = (*csc_mtx).size;
    bool *seen = calloc(num_nz, sizeof(bool)); //non-zero elements seen (true); not-seen (false)
    index = 0;
    if((*csc_mtx).is_int) {
    	for(int i = 0; i < n; i++) {
            col = i % (*csc_mtx).col; //the elements column index
            row = i / (*csc_mtx).col; //the elements row index
            if((*csc_mtx).mtx_offset[col+1] == 0) {
                fprintf(fp, "%i ", 0);
                continue;
            }
            else { //offset has value
                index = get_non_zero_csc(&(*csc_mtx), &seen, col+1, row); // O(N)
                if(index < 0) {
                    fprintf(fp, "%i ", 0);
                    continue;
                }
                fprintf(fp, "%i ", (*csc_mtx).mtxi[index]);
           }
        }
    }
    else {
        for(int i = 0; i < n; i++) {
            col = i % (*csc_mtx).col; //the elements column index
            row = i / (*csc_mtx).col; //the elements row index
            if((*csc_mtx).mtx_offset[col + 1] == 0.0) {
                fprintf(fp, "%3.2f ", 0.0);
                continue;
            }
            else { //offset has value
                index = get_non_zero_csc(&(*csc_mtx), &seen, col+1, row); // O(N)
                if(index < 0) {
                    fprintf(fp, "%3.2f ", 0.0);
                    continue;
                }
                fprintf(fp, "%3.2f ", (*csc_mtx).mtxf[index]);
           }
        }
    }
    free(seen);
    fprintf(fp, "\n");
    return 1;
}
