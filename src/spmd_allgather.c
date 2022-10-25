#include "spmd.h"

/* ----- allgather ----- */
SEXP spmd_allgather_integer(SEXP R_send_data, SEXP R_recv_data,
		SEXP R_comm){
#ifdef LONG_VECTOR_SUPPORT
	SEXP R_buff_data;
	int *C_send_data = INTEGER(R_send_data),
	    *C_recv_data = INTEGER(R_recv_data),
	    *C_recv_data_fix = INTEGER(R_recv_data),
	    *C_buff_data,
	    *C_buff_data_fix;
	R_xlen_t C_length_send_data = XLENGTH(R_send_data),
	         C_length_send_data_fix = XLENGTH(R_send_data);
	int C_comm = INTEGER(R_comm)[0], C_comm_size, i;

	if(C_length_send_data > SPMD_SHORT_LEN_MAX){
		#if (MPI_LONG_DEBUG & 1) == 1
			int C_comm_rank;
			MPI_Comm_rank(global_spmd_comm[C_comm], &C_comm_rank);
		#endif

		/* R_send_data is a long vector, so is R_recv_data. */

		/* Since C_recv_data is not contiguious, use extra buffer to
		   store chunk data for MPI calls. */
		MPI_Comm_size(global_spmd_comm[C_comm], &C_comm_size);
		PROTECT(R_buff_data = allocVector(INTSXP,
			(R_xlen_t) C_comm_size * (R_xlen_t) SPMD_SHORT_LEN_MAX));
		C_buff_data = INTEGER(R_buff_data);
		C_buff_data_fix = INTEGER(R_buff_data);

		/* Loop through all. */
		while(C_length_send_data > SPMD_SHORT_LEN_MAX){
			#if (MPI_LONG_DEBUG & 1) == 1
				if(C_comm_rank == 0){
					Rprintf("C_length_send_data: %ld\n",
						C_length_send_data);
				}
			#endif

			/* Send C_send_data out to C_buff_data. */
			spmd_errhandler(MPI_Allgather(C_send_data,
				SPMD_SHORT_LEN_MAX,
				MPI_INT, C_buff_data, SPMD_SHORT_LEN_MAX,
				MPI_INT, global_spmd_comm[C_comm]));
			C_send_data = C_send_data + SPMD_SHORT_LEN_MAX;

			/* Memory copy from C_buff_data to C_recv_data. */
			for(i = 0; i < C_comm_size; i++){
				memcpy(C_recv_data, C_buff_data,
					SPMD_SHORT_LEN_MAX * sizeof(int));
				C_recv_data = C_recv_data +
					C_length_send_data_fix;
				C_buff_data = C_buff_data + SPMD_SHORT_LEN_MAX;
			}
			C_recv_data_fix = C_recv_data_fix + SPMD_SHORT_LEN_MAX;
			C_recv_data = C_recv_data_fix;
			C_buff_data = C_buff_data_fix;
			C_length_send_data = C_length_send_data -
				SPMD_SHORT_LEN_MAX;
		}

		/* Remainder. */
		if(C_length_send_data > 0){
			#if (MPI_LONG_DEBUG & 1) == 1
				if(C_comm_rank == 0){
					Rprintf("C_length_send_data: %ld\n",
						C_length_send_data);
				}
			#endif

			/* Send C_send_data out to C_buff_data. */
			spmd_errhandler(MPI_Allgather(C_send_data,
				(int) C_length_send_data,
				MPI_INT, C_buff_data, (int) C_length_send_data,
				MPI_INT, global_spmd_comm[C_comm]));

			/* Memory copy from C_buff_data to C_recv_data. */
			for(i = 0; i < C_comm_size; i++){
				memcpy(C_recv_data, C_buff_data,
					(int) C_length_send_data * sizeof(int));
				C_recv_data = C_recv_data +
					C_length_send_data_fix;
				C_buff_data = C_buff_data + C_length_send_data;
			}
		}

		UNPROTECT(1);
	} else{
		/* It doesn't matter if R_recv_data is a long vector or not,
		   since pointer address is already long int.*/
		spmd_errhandler(MPI_Allgather(C_send_data,
			(int) C_length_send_data,
			MPI_INT, C_recv_data, (int) C_length_send_data,
			MPI_INT, global_spmd_comm[C_comm]));
	}
#else
	int C_length_send_data = LENGTH(R_send_data);
	spmd_errhandler(MPI_Allgather(INTEGER(R_send_data), C_length_send_data,
		MPI_INT, INTEGER(R_recv_data), C_length_send_data,
		MPI_INT, global_spmd_comm[INTEGER(R_comm)[0]]));
#endif
	return(R_recv_data);
} /* End of spmd_allgather_integer(). */

SEXP spmd_allgather_double(SEXP R_send_data, SEXP R_recv_data, SEXP R_comm){
#ifdef LONG_VECTOR_SUPPORT
	SEXP R_buff_data;
	double *C_send_data = REAL(R_send_data),
	       *C_recv_data = REAL(R_recv_data),
	       *C_recv_data_fix = REAL(R_recv_data),
	       *C_buff_data,
	       *C_buff_data_fix;
	R_xlen_t C_length_send_data = XLENGTH(R_send_data),
	         C_length_send_data_fix = XLENGTH(R_send_data);
	int C_comm = INTEGER(R_comm)[0], C_comm_size, i;

	if(C_length_send_data > SPMD_SHORT_LEN_MAX){
		#if (MPI_LONG_DEBUG & 1) == 1
			int C_comm_rank;
			MPI_Comm_rank(global_spmd_comm[C_comm], &C_comm_rank);
		#endif

		/* R_send_data is a long vector, so is R_recv_data. */

		/* Since C_recv_data is not contiguious, use extra buffer to
		   store chunk data for MPI calls. */
		MPI_Comm_size(global_spmd_comm[C_comm], &C_comm_size);
		PROTECT(R_buff_data = allocVector(REALSXP,
			(R_xlen_t) C_comm_size * (R_xlen_t) SPMD_SHORT_LEN_MAX));
		C_buff_data = REAL(R_buff_data);
		C_buff_data_fix = REAL(R_buff_data);

		/* Loop through all. */
		while(C_length_send_data > SPMD_SHORT_LEN_MAX){
			#if (MPI_LONG_DEBUG & 1) == 1
				if(C_comm_rank == 0){
					Rprintf("C_length_send_data: %ld\n",
						C_length_send_data);
				}
			#endif

			/* Send C_send_data out to C_buff_data. */
			spmd_errhandler(MPI_Allgather(C_send_data,
				SPMD_SHORT_LEN_MAX,
				MPI_DOUBLE, C_buff_data, SPMD_SHORT_LEN_MAX,
				MPI_DOUBLE, global_spmd_comm[C_comm]));
			C_send_data = C_send_data + SPMD_SHORT_LEN_MAX;

			/* Memory copy from C_buff_data to C_recv_data. */
			for(i = 0; i < C_comm_size; i++){
				memcpy(C_recv_data, C_buff_data,
					SPMD_SHORT_LEN_MAX * sizeof(double));
				C_recv_data = C_recv_data +
					C_length_send_data_fix;
				C_buff_data = C_buff_data + SPMD_SHORT_LEN_MAX;
			}
			C_recv_data_fix = C_recv_data_fix + SPMD_SHORT_LEN_MAX;
			C_recv_data = C_recv_data_fix;
			C_buff_data = C_buff_data_fix;
			C_length_send_data = C_length_send_data -
				SPMD_SHORT_LEN_MAX;
		}

		/* Remainder. */
		if(C_length_send_data > 0){
			#if (MPI_LONG_DEBUG & 1) == 1
				if(C_comm_rank == 0){
					Rprintf("C_length_send_data: %ld\n",
						C_length_send_data);
				}
			#endif

			/* Send C_send_data out to C_buff_data. */
			spmd_errhandler(MPI_Allgather(C_send_data,
				(int) C_length_send_data,
				MPI_DOUBLE, C_buff_data,
				(int) C_length_send_data,
				MPI_DOUBLE, global_spmd_comm[C_comm]));

			/* Memory copy from C_buff_data to C_recv_data. */
			for(i = 0; i < C_comm_size; i++){
				memcpy(C_recv_data, C_buff_data,
					(int) C_length_send_data *
					sizeof(double));
				C_recv_data = C_recv_data +
					C_length_send_data_fix;
				C_buff_data = C_buff_data + C_length_send_data;
			}
		}

		UNPROTECT(1);
	} else{
		/* It doesn't matter if R_recv_data is a long vector or not,
		   since pointer address is already long int.*/
		spmd_errhandler(MPI_Allgather(C_send_data,
			(int) C_length_send_data,
			MPI_DOUBLE, C_recv_data, (int) C_length_send_data,
			MPI_DOUBLE, global_spmd_comm[C_comm]));
	}
#else
	int C_length_send_data = LENGTH(R_send_data);
	spmd_errhandler(MPI_Allgather(REAL(R_send_data), C_length_send_data,
		MPI_DOUBLE, REAL(R_recv_data), C_length_send_data,
		MPI_DOUBLE, global_spmd_comm[INTEGER(R_comm)[0]]));
#endif
	return(R_recv_data);
} /* End of spmd_allgather_double(). */

SEXP spmd_allgather_raw(SEXP R_send_data, SEXP R_recv_data, SEXP R_comm){
#ifdef LONG_VECTOR_SUPPORT
	SEXP R_buff_data;
	Rbyte *C_send_data = RAW(R_send_data),
	      *C_recv_data = RAW(R_recv_data),
	      *C_recv_data_fix = RAW(R_recv_data),
	      *C_buff_data,
	      *C_buff_data_fix;
	R_xlen_t C_length_send_data = XLENGTH(R_send_data),
	         C_length_send_data_fix = XLENGTH(R_send_data);
	int C_comm = INTEGER(R_comm)[0], C_comm_size, i;

	if(C_length_send_data > SPMD_SHORT_LEN_MAX){
		#if (MPI_LONG_DEBUG & 1) == 1
			int C_comm_rank;
			MPI_Comm_rank(global_spmd_comm[C_comm], &C_comm_rank);
		#endif

		/* R_send_data is a long vector, so is R_recv_data. */

		/* Since C_recv_data is not contiguious, use extra buffer to
		   store chunk data for MPI calls. */
		MPI_Comm_size(global_spmd_comm[C_comm], &C_comm_size);
		PROTECT(R_buff_data = allocVector(RAWSXP,
			(R_xlen_t) C_comm_size * (R_xlen_t) SPMD_SHORT_LEN_MAX));
		C_buff_data = RAW(R_buff_data);
		C_buff_data_fix = RAW(R_buff_data);

		/* Loop through all. */
		while(C_length_send_data > SPMD_SHORT_LEN_MAX){
			#if (MPI_LONG_DEBUG & 1) == 1
				if(C_comm_rank == 0){
					Rprintf("C_length_send_data: %ld\n",
						C_length_send_data);
				}
			#endif

			/* Send C_send_data out to C_buff_data. */
			spmd_errhandler(MPI_Allgather(C_send_data,
				SPMD_SHORT_LEN_MAX,
				MPI_BYTE, C_buff_data, SPMD_SHORT_LEN_MAX,
				MPI_BYTE, global_spmd_comm[C_comm]));
			C_send_data = C_send_data + SPMD_SHORT_LEN_MAX;

			/* Memory copy from C_buff_data to C_recv_data. */
			for(i = 0; i < C_comm_size; i++){
				memcpy(C_recv_data, C_buff_data,
					SPMD_SHORT_LEN_MAX);
				C_recv_data = C_recv_data +
					C_length_send_data_fix;
				C_buff_data = C_buff_data + SPMD_SHORT_LEN_MAX;
			}
			C_recv_data_fix = C_recv_data_fix + SPMD_SHORT_LEN_MAX;
			C_recv_data = C_recv_data_fix;
			C_buff_data = C_buff_data_fix;
			C_length_send_data = C_length_send_data -
				SPMD_SHORT_LEN_MAX;
		}

		/* Remainder. */
		if(C_length_send_data > 0){
			#if (MPI_LONG_DEBUG & 1) == 1
				if(C_comm_rank == 0){
					Rprintf("C_length_send_data: %ld\n",
						C_length_send_data);
				}
			#endif

			/* Send C_send_data out to C_buff_data. */
			spmd_errhandler(MPI_Allgather(C_send_data,
				(int) C_length_send_data,
				MPI_BYTE, C_buff_data,
				(int) C_length_send_data,
				MPI_BYTE, global_spmd_comm[C_comm]));

			/* Memory copy from C_buff_data to C_recv_data. */
			for(i = 0; i < C_comm_size; i++){
				memcpy(C_recv_data, C_buff_data,
					(int) C_length_send_data);
				C_recv_data = C_recv_data +
					C_length_send_data_fix;
				C_buff_data = C_buff_data + C_length_send_data;
			}
		}

		UNPROTECT(1);
	} else{
		/* It doesn't matter if R_recv_data is a long vector or not,
		   since pointer address is already long int.*/
		spmd_errhandler(MPI_Allgather(C_send_data,
			(int) C_length_send_data,
			MPI_BYTE, C_recv_data, (int) C_length_send_data,
			MPI_BYTE, global_spmd_comm[C_comm]));
	}
#else
	int C_length_send_data = LENGTH(R_send_data);
	spmd_errhandler(MPI_Allgather(RAW(R_send_data), C_length_send_data,
		MPI_BYTE, RAW(R_recv_data), C_length_send_data,
		MPI_BYTE, global_spmd_comm[INTEGER(R_comm)[0]]));
#endif
	return(R_recv_data);
} /* End of spmd_allgather_raw(). */

