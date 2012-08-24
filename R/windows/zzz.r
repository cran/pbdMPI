### Lastest load into a package.

.First.lib <- function(lib, pkg){
} # End of .First.lib().

.Last.lib <- function(libpath){
} # End of .Last.lib().

.onLoad <- function(libname, pkgname){
  library.dynam("pbdMPI", pkgname, libname)

  if(! is.loaded("spmd_initialize", PACKAGE = "pbdMPI")){
    stop("pbdMPI is not loaded.")
  }

  if(! exists(".__DISABLE_MPI_INIT__", envir = .GlobalEnv) ||
      get(".__DISABLE_MPI_INIT__", envir = .GlobalEnv) == FALSE){
    .Call("spmd_initialize", PACKAGE = "pbdMPI")
    assign(".comm.size", pbdMPI:::spmd.comm.size(0L), envir = .GlobalEnv)
    assign(".comm.rank", pbdMPI:::spmd.comm.rank(0L), envir = .GlobalEnv)
  }
} # End of .onLoad().

.onUnload <- function(libpath){
  pbdMPI:::spmd.finalize(mpi.finalize = FALSE)
  library.dynam.unload("pbdMPI", libpath)
} # End of .onUnload().
