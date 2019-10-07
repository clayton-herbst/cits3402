#include "sp.h"

/**
 * Dealloc config struct metadata.
 */
void dealloc_config(SP_CONFIG *config) {
    //MPI_File_close(&(*config).file_in);
    fclose((*config).fp_in);
    free((*config).filename_in);
    free(config);
}

void dealloc_paths(PATHS *paths) {
    free(paths);
}
