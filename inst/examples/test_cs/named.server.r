### See ``http://www.mpi-forum.org/docs/mpi-20-html/node106.htm'' for details.

# Initial MPI.
library(pbdMPI, quietly = TRUE)
init()
if(comm.size() != 1){
  stop("Server too big.")
}
client.comm <- .SPMD.CT$newcomm

# Connection start.
port.name <- port.open()
serv.publish(port.name)
comm.accept(port.name, newcomm = client.comm)

# Communicate 1.
buffer <- as.integer(rep(0, 5))
ret <- spmd.recv.integer(buffer, rank.source = NULL, tag = NULL,
                         comm = client.comm)
print(get.sourcetag())
print(ret)

# Communicate 2.
ret <- spmd.allgather.integer(buffer, rep(buffer, 2), comm = client.comm)
print(ret)

# Connection stop.
comm.disconnect(comm = client.comm)
serv.unpublish(port.name)
port.close(port.name)

# Finish.
finalize()
