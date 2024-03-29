### SHELL> mpiexec -np 4 Rscript chunk.r
### Check reproducibility for RNG with -np 2

### Initialize
suppressMessages(library(pbdMPI, quietly = TRUE))
.pbd_env$SPMD.CT$print.quiet = TRUE  # set global print.quiet (no rank info)
rank = comm.rank()
size = comm.size()

comm.cat("\ncomm.chunk(13):\n", quiet = TRUE)
my_chunk = comm.chunk(13)
comm.print(my_chunk, "\n", all.rank = TRUE)

comm.cat("\ncomm.chunk(13, all.rank = TRUE):\n")
my_chunk = comm.chunk(13, all.rank = TRUE)
comm.print(my_chunk, "\n", all.rank = TRUE)

comm.cat("\ncomm.chunk(13, form = 'vector'):\n")
my_chunk = comm.chunk(13, form = 'vector')
comm.print(my_chunk, "\n", all.rank = TRUE)

comm.cat("\ncomm.chunk(13, form = 'vector', all.rank = TRUE):\n", quiet = TRUE)
my_chunk = comm.chunk(13, form = 'vector', all.rank = TRUE)
comm.print(my_chunk, "\n", all.rank = TRUE)

comm.cat("\ncomm.chunk(13, type = 'equal'):\n")
my_chunk = comm.chunk(13, type = 'equal')
comm.print(my_chunk, "\n", all.rank = TRUE)

comm.cat("\ncomm.chunk(13, type = 'equal', lo.side = 'left'):\n")
my_chunk = comm.chunk(13, type = 'equal', lo.side = 'left')
comm.print(my_chunk, "\n", all.rank = TRUE)

comm.cat("\nRank-based RNG streams:\n")
my_num = comm.chunk(13, form = 'number', rng = TRUE, seed = 1357)
comm.print(my_num, all.rank = TRUE)

rn = matrix(NA, nrow = my_num, ncol = 5)
for(i in 1:my_num)  rn[i, ] = runif(5)
comm.print(data.frame(rank = rank, rn = rn), all.rank = TRUE)

comm.cat("\nVector-based RNG streams:\n")
my_vec = comm.chunk(13, form = 'vector', rng = TRUE, seed = 1357)
comm.print(my_vec, all.rank = TRUE)

rn = matrix(NA, nrow = length(my_vec), ncol = 5)
for(i in 1:length(my_vec)) {
  comm.set.stream(my_vec[i])                             #RNG
  rn[i, ] = runif(5)                                     #RNG
}
df = data.frame(rank = rep(rank, length(my_vec)), v = my_vec, rn = rn)
comm.print(df, all.rank = TRUE)

## Note above: comm.print() cannot be inside the for loop because loop lengths
##             are not the same. comm.print() is a collective operation in
##             which all ranks must participate. But rank 0 has a longer loop
##             that would cause an infinite wait for other ranks to participate!

comm.cat("\nVector-based streams are reproducible with different numbers of",
         "parallel ranks.\n",
         "Repeat with a different number of ranks to check reproducibility.\n",
         quiet = TRUE)

finalize()
