#include "spmd.h"

SEXP spmd_barrier(SEXP R_comm){
	return(AsInt(spmd_errhandler(MPI_Barrier(global_spmd_comm[INTEGER(R_comm)[0]]))));
} /* End of spmd_barrier(). */

SEXP spmd_comm_is_null(SEXP R_comm){
	if(global_spmd_comm == NULL){
		return(AsInt(-1));
	}
	return(AsInt(global_spmd_comm[INTEGER(R_comm)[0]] == MPI_COMM_NULL));
} /* End of spmd_comm_is_null(). */

SEXP spmd_comm_size(SEXP R_comm){
	int size;
	MPI_Comm_size(global_spmd_comm[INTEGER(R_comm)[0]], &size);
	return(AsInt(size));
} /* End of spmd_comm_size(). */

SEXP spmd_comm_rank(SEXP R_comm){
	int rank;
	MPI_Comm_rank(global_spmd_comm[INTEGER(R_comm)[0]], &rank);
	return(AsInt(rank));
} /* End of spmd_comm_rank(). */

SEXP spmd_comm_localrank(SEXP R_comm){
#if MPI_VERSION >= 3
	MPI_Comm tmp_comm;
	int localrank;
	
	if (INTEGER(R_comm)[0] == 0){
		tmp_comm = global_spmd_localcomm;
	}
	else {
		MPI_Comm_split_type(MPI_COMM_WORLD, MPI_COMM_TYPE_SHARED, 0, MPI_INFO_NULL, &tmp_comm);
	}
	
	spmd_errhandler(MPI_Comm_rank(tmp_comm, &localrank));
	
	return AsInt(localrank);
#else
	error("only MPI >= 3 supported\n");
#endif
} /* End of spmd_comm_localrank(). */

SEXP spmd_comm_dup(SEXP R_comm, SEXP R_newcomm){
	int commn = INTEGER(R_comm)[0], newcommn = INTEGER(R_newcomm)[0];
	if(commn == 0){
		return(AsInt(
			spmd_errhandler(MPI_Comm_dup(MPI_COMM_WORLD,
				 &global_spmd_comm[newcommn]))));
	} else{
		return(AsInt(
			spmd_errhandler(MPI_Comm_dup(global_spmd_comm[commn],
				&global_spmd_comm[newcommn]))));
	}
} /* End of spmd_comm_dup(). */

SEXP spmd_comm_free(SEXP R_comm){
	return(AsInt(
		spmd_errhandler(MPI_Comm_free(&global_spmd_comm[INTEGER(R_comm)[0]]))));
} /* End of spmd_comm_free(). */

SEXP spmd_comm_set_errhandler(SEXP R_comm){
	return(AsInt(
		MPI_Comm_set_errhandler(global_spmd_comm[INTEGER(R_comm)[0]],
		MPI_ERRORS_RETURN)));
} /* End of spmd_comm_set_errhandler(). */

SEXP spmd_comm_get_parent(SEXP R_comm){
        return(AsInt(
		spmd_errhandler(MPI_Comm_get_parent(
			&global_spmd_comm[INTEGER(R_comm)[0]]))));
} /* End of spmd_comm_get_parent(). */

SEXP spmd_is_manager(void){
	int check;
	MPI_Comm manager;
	MPI_Comm_get_parent(&manager);
	check = (manager == MPI_COMM_NULL);
	if(! check){
		 MPI_Comm_free(&manager);
	}
	return(AsInt(check));
} /* End of spmd_is_manager(). */

SEXP spmd_comm_abort(SEXP R_comm, SEXP R_errorcode){
	return(AsInt(
		spmd_errhandler(MPI_Abort(
			global_spmd_comm[INTEGER(R_comm)[0]],
			INTEGER(R_errorcode)[0]))));
} /* End of spmd_comm_abort(). */

SEXP spmd_comm_split(SEXP R_comm, SEXP R_color, SEXP R_key, SEXP R_newcomm){
	int color = INTEGER(R_color)[0];

	if(color == NA_INTEGER || color < 0){
		color = MPI_UNDEFINED;
	}

	return(AsInt(
		spmd_errhandler(MPI_Comm_split(
			global_spmd_comm[INTEGER(R_comm)[0]],
			color, INTEGER(R_key)[0],
			&global_spmd_comm[INTEGER(R_newcomm)[0]]))));
} /* End of spmd_comm_split(). */

SEXP spmd_comm_disconnect(SEXP R_comm){
	return(AsInt(
		spmd_errhandler(MPI_Comm_disconnect(
			&global_spmd_comm[INTEGER(R_comm)[0]]))));
} /* End of spmd_comm_disconnect(). */

SEXP spmd_comm_connect(SEXP R_port_name, SEXP R_info, SEXP R_rank_root,
		SEXP R_comm, SEXP R_newcomm){
	return(AsInt(
		spmd_errhandler(MPI_Comm_connect(
			CHARPT(R_port_name, 0),
			global_spmd_info[INTEGER(R_info)[0]],
			INTEGER(R_rank_root)[0],
			global_spmd_comm[INTEGER(R_comm)[0]],
			&global_spmd_comm[INTEGER(R_newcomm)[0]]))));
} /* End of spmd_comm_connect(). */

SEXP spmd_comm_accept(SEXP R_port_name, SEXP R_info, SEXP R_rank_root,
		SEXP R_comm, SEXP R_newcomm){
	return(AsInt(
		spmd_errhandler(MPI_Comm_accept(
			CHARPT(R_port_name, 0),
			global_spmd_info[INTEGER(R_info)[0]],
			INTEGER(R_rank_root)[0],
			global_spmd_comm[INTEGER(R_comm)[0]],
			&global_spmd_comm[INTEGER(R_newcomm)[0]]))));
} /* End of spmd_comm_accept(). */

SEXP spmd_port_open(SEXP R_info){
	char port_name[MPI_MAX_PORT_NAME];
	SEXP R_port_name;
	char errmsg[MPI_MAX_ERROR_STRING];
	int merr, msglen;

	merr = MPI_Open_port(
			global_spmd_info[INTEGER(R_info)[0]],
			port_name);
	if(merr){
		MPI_Error_string(merr, errmsg, &msglen);
		REprintf("Error in Open_port: \"%s\"\n", errmsg);
	}

	PROTECT(R_port_name = allocVector(STRSXP, 1));
	SET_STRING_ELT(R_port_name, 0, mkChar(port_name));
	UNPROTECT(1);

	return(R_port_name);
} /* End of spmd_port_open(). */

SEXP spmd_port_close(SEXP R_port_name){
	return(AsInt(
		spmd_errhandler(MPI_Close_port(
			CHARPT(R_port_name, 0)))));
} /* End of spmd_port_close(). */

SEXP spmd_serv_publish(SEXP R_serv_name, SEXP R_info, SEXP R_port_name){
	return(AsInt(
		spmd_errhandler(MPI_Publish_name(
			CHARPT(R_serv_name, 0),
			global_spmd_info[INTEGER(R_info)[0]],
			CHARPT(R_port_name, 0)))));
} /* End of spmd_serv_publish(). */

SEXP spmd_serv_unpublish(SEXP R_serv_name, SEXP R_info, SEXP R_port_name){
	return(AsInt(
		spmd_errhandler(MPI_Unpublish_name(
			CHARPT(R_serv_name, 0),
			global_spmd_info[INTEGER(R_info)[0]],
			CHARPT(R_port_name, 0)))));
} /* End of spmd_serv_unpublish(). */

SEXP spmd_serv_lookup(SEXP R_serv_name, SEXP R_info){
	char port_name[MPI_MAX_PORT_NAME];
	SEXP R_port_name;
	char errmsg[MPI_MAX_ERROR_STRING];
	int merr, msglen;

	merr = MPI_Lookup_name(
			CHARPT(R_serv_name, 0),
			global_spmd_info[INTEGER(R_info)[0]],
			port_name);
	if(merr){
		MPI_Error_string(merr, errmsg, &msglen);
		REprintf("Error in Lookup_name: \"%s\"\n", errmsg);
	}

	PROTECT(R_port_name = allocVector(STRSXP, 1));
	SET_STRING_ELT(R_port_name, 0, mkChar(port_name));
	UNPROTECT(1);

	return(R_port_name);
} /* End of spmd_serv_lookup(). */

SEXP spmd_intercomm_merge(SEXP R_intercomm, SEXP R_high, SEXP R_comm){
	return(AsInt(
		spmd_errhandler(MPI_Intercomm_merge(
			global_spmd_comm[INTEGER(R_intercomm)[0]], INTEGER(R_high)[0],
			&global_spmd_comm[INTEGER(R_comm)[0]]))));
} /* End of spmd_intercomm_merge(). */

SEXP spmd_intercomm_create(SEXP R_local_comm, SEXP R_local_leader,
		SEXP R_peer_comm, SEXP R_remote_leader, SEXP R_tag,
		SEXP R_newintercomm){
	return(AsInt(
		spmd_errhandler(MPI_Intercomm_create(
			global_spmd_comm[INTEGER(R_local_comm)[0]],
			INTEGER(R_local_leader)[0],
			global_spmd_comm[INTEGER(R_peer_comm)[0]],
			INTEGER(R_remote_leader)[0],
			INTEGER(R_tag)[0],
			&global_spmd_comm[INTEGER(R_newintercomm)[0]]))));
} /* End of spmd_intercomm_create(). */

/* Fortran supporting functions. */
SEXP spmd_comm_c2f(SEXP R_comm){
	return(AsInt((int) MPI_Comm_c2f(global_spmd_comm[INTEGER(R_comm)[0]])));
} /* End of spmd_comm_c2f(). */
