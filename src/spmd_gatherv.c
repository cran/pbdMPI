#include "spmd.h"

/* ----- gatherv ----- */
SEXP spmd_gatherv_integer(SEXP R_send_data, SEXP R_recv_data,
		SEXP R_recv_counts, SEXP R_displs, SEXP R_rank_dest,
		SEXP R_comm){
#ifdef LONG_VECTOR_SUPPORT
	if(XLENGTH(R_send_data) > SPMD_SHORT_LEN_MAX ||
	   XLENGTH(R_recv_data) > SPMD_SHORT_LEN_MAX){
		error("long vectors not supported yet: %s:%d\n",
			__FILE__, __LINE__);
	}
#endif
	spmd_errhandler(MPI_Gatherv(INTEGER(R_send_data), LENGTH(R_send_data),
		MPI_INT, INTEGER(R_recv_data), INTEGER(R_recv_counts),
		INTEGER(R_displs), MPI_INT, INTEGER(R_rank_dest)[0],
		global_spmd_comm[INTEGER(R_comm)[0]]));
	return(R_recv_data);
} /* End of spmd_gatherv_integer(). */

SEXP spmd_gatherv_double(SEXP R_send_data, SEXP R_recv_data,
		SEXP R_recv_counts, SEXP R_displs, SEXP R_rank_dest,
		SEXP R_comm){
#ifdef LONG_VECTOR_SUPPORT
	if(XLENGTH(R_send_data) > SPMD_SHORT_LEN_MAX ||
	   XLENGTH(R_recv_data) > SPMD_SHORT_LEN_MAX){
		error("long vectors not supported yet: %s:%d\n",
			__FILE__, __LINE__);
	}
#endif
	spmd_errhandler(MPI_Gatherv(REAL(R_send_data), LENGTH(R_send_data),
		MPI_DOUBLE, REAL(R_recv_data), INTEGER(R_recv_counts),
		INTEGER(R_displs), MPI_DOUBLE, INTEGER(R_rank_dest)[0],
		global_spmd_comm[INTEGER(R_comm)[0]]));
	return(R_recv_data);
} /* End of spmd_gatherv_double(). */

SEXP spmd_gatherv_raw(SEXP R_send_data, SEXP R_recv_data,
		SEXP R_recv_counts, SEXP R_displs, SEXP R_rank_dest,
		SEXP R_comm){
#ifdef LONG_VECTOR_SUPPORT
	if(XLENGTH(R_send_data) > SPMD_SHORT_LEN_MAX ||
	   XLENGTH(R_recv_data) > SPMD_SHORT_LEN_MAX){
		error("long vectors not supported yet: %s:%d\n",
			__FILE__, __LINE__);
	}
#endif
	spmd_errhandler(MPI_Gatherv(RAW(R_send_data), LENGTH(R_send_data),
		MPI_BYTE, RAW(R_recv_data), INTEGER(R_recv_counts),
		INTEGER(R_displs), MPI_BYTE, INTEGER(R_rank_dest)[0],
		global_spmd_comm[INTEGER(R_comm)[0]]));
	return(R_recv_data);
} /* End of spmd_gatherv_raw(). */

