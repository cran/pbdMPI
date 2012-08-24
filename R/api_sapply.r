### These functions are supposed to run in SPMD, even when pbd.mode = "mw"

spmd.sapply <- function(X, FUN, ..., simplify = TRUE, USE.NAMES = TRUE,
    pbd.mode = c("mw", "spmd"), rank.source = .SPMD.CT$rank.root,
    comm = .SPMD.CT$comm){
  COMM.SIZE <- spmd.comm.size(comm)
  COMM.RANK <- spmd.comm.rank(comm)

  ### Redistribute data for MW.
  if(pbd.mode[1] == "mw"){
    new.X <- NULL 
    if(COMM.RANK == rank.source){
      if(! is.vector(X) || is.object(X)){
        X <- as.list(X)
      }

      alljid <- get.jid(length(X), comm = comm, all = TRUE)
      new.X <- lapply(alljid, list.to.list, X) 
    }

    if(length(new.X) < COMM.SIZE){
      new.X <- c(new.X, rep(list(NULL), COMM.SIZE - length(new.X)))
    }

    X <- spmd.scatter.object(new.X, rank.source = rank.source, comm = comm)
  }

  ### Run as SPMD.
  ret <- sapply(X, FUN, ..., simplify = simplify, USE.NAMES = USE.NAMES)

  ### Gather data for MW.
  if(pbd.mode[1] == "mw"){
    ret <- spmd.gather.object(ret, rank.dest = rank.source, comm = comm)
    if(COMM.RANK != rank.source){
      ret <- NULL
    }
  }

  ret
} # End of spmd.sapply().

pbdSapply <- spmd.sapply

