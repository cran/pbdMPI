### SHELL> mpiexec -np 2 Rscript --vanilla [...].r

### Initial
library(pbdMPI, quiet = TRUE)

### Examples
FUN <- function(jid){
  Sys.sleep(1)
  jid * 10
}

ret <- task.pull(1:10, FUN)
comm.print(ret)

if(comm.rank() == 0){
  ret.jobs <- unlist(ret)
  ret.jobs <- ret.jobs[names(ret.jobs) == "ret"]
  print(ret.jobs)
}

### Finish
finalize()
