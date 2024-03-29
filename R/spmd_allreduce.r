### S4 functions.

### Default method and without specified x.buffer
spmd.allreduce.default <- function(x, x.buffer = NULL,
    op = .pbd_env$SPMD.CT$op, comm = .pbd_env$SPMD.CT$comm){
  comm <- as.integer(comm)
  
  ## Check if array
  all.array <- spmd.allreduce.integer(
                   as.integer(is.array(x) && length(x) > 0),
                   integer(1), op = "sum",
                   comm = comm) == spmd.comm.size(comm)
  if(all.array){
    spmd.allreduce.array(x, op = op, comm = comm)
  } else{
    spmd.allreduce.object(x, op = op, comm = comm)
  }
} # End of spmd.allreduce.default().


### For basic types with specified x.buffer.
###
spmd.allreduce.integer <- function(x, x.buffer,
    op = .pbd_env$SPMD.CT$op, comm = .pbd_env$SPMD.CT$comm){
  .Call("spmd_allreduce_integer", x, x.buffer,
        which(op[1] == .pbd_env$SPMD.OP), as.integer(comm),
        PACKAGE = "pbdMPI")
} # End of spmd.allreduce.integer().

spmd.allreduce.double <- function(x, x.buffer,
    op = .pbd_env$SPMD.CT$op, comm = .pbd_env$SPMD.CT$comm){
  .Call("spmd_allreduce_double", x, x.buffer,
        which(op[1] == .pbd_env$SPMD.OP), as.integer(comm),
        PACKAGE = "pbdMPI")
} # End of spmd.allreduce.double().

spmd.allreduce.logical <- function(x, x.buffer,
    op = .pbd_env$SPMD.CT$op, comm = .pbd_env$SPMD.CT$comm){
  ret <- .Call("spmd_allreduce_integer", x, x.buffer,
               which(op[1] == .pbd_env$SPMD.OP), as.integer(comm),
               PACKAGE = "pbdMPI")
  as.logical(ret)
} # End of spmd.allreduce.logical().

spmd.allreduce.float <- function(x, x.buffer,
    op = .pbd_env$SPMD.CT$op, comm = .pbd_env$SPMD.CT$comm){
  .Call("spmd_allreduce_float", x, x.buffer,
        which(op[1] == .pbd_env$SPMD.OP), as.integer(comm),
        PACKAGE = "pbdMPI")
} # End of spmd.allreduce.float().

spmd.allreduce.float32 <- function(x, x.buffer,
    op = .pbd_env$SPMD.CT$op, comm = .pbd_env$SPMD.CT$comm){
  ret <- .Call("spmd_allreduce_float", x@Data, x.buffer@Data,
        which(op[1] == .pbd_env$SPMD.OP), as.integer(comm),
        PACKAGE = "pbdMPI")
  float32(ret)
} # End of spmd.allreduce.float32().


### S4 methods.
setGeneric(
  name = "allreduce",
  useAsDefault = spmd.allreduce.default
)

### For allreduce.
setMethod(
  f = "allreduce",
  signature = signature(x = "ANY", x.buffer = "missing"),
  definition = spmd.allreduce.default
)
setMethod(
  f = "allreduce",
  signature = signature(x = "integer", x.buffer = "integer"),
  definition = spmd.allreduce.integer
)
setMethod(
  f = "allreduce",
  signature = signature(x = "numeric", x.buffer = "numeric"),
  definition = spmd.allreduce.double
)
setMethod(
  f = "allreduce",
  signature = signature(x = "logical", x.buffer = "logical"),
  definition = spmd.allreduce.logical
)
setMethod(
  f = "allreduce",
  signature = signature(x = "float32", x.buffer = "float32"),
  definition = spmd.allreduce.float32
)
