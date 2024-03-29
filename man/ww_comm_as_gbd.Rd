\name{global as.gbd}
\alias{comm.as.gbd}
\title{ Global As GBD Function }
\description{
  This function redistributes a regular matrix existed in rank.soure
  and turns it in a gbd matrix in row blocks.
}
\usage{
comm.as.gbd(X, balance.method = .pbd_env$SPMD.IO$balance.method,
            rank.source = .pbd_env$SPMD.CT$rank.source,
            comm = .pbd_env$SPMD.CT$comm)
}
\arguments{
  \item{X}{a regular \code{matrix} in \code{rank.source} and to be
           redistributed as a gbd.}
  \item{balance.method}{a balance method.}
  \item{rank.source}{a rank of source where elements of \code{x} scatter from.}
  \item{comm}{a communicator number.}
}
\details{
  \code{X} matrix in \code{rank.source} will be redistributed as a gbd
  matrix in row blocks.

  This function will first set \code{NULL} to \code{X} if it is not
  located in \code{rank.source}, then called \code{\link{comm.load.balance}()}
  to redistributed the one located in \code{rank.source} to all other ranks.
}
\value{
  A \code{X.gbd} will be returned.
}
\references{
  Programming with Big Data in R Website:
  \url{https://pbdr.org/}
}
\author{
  Wei-Chen Chen \email{wccsnow@gmail.com}, George Ostrouchov,
  Drew Schmidt, Pragneshkumar Patel, and Hao Yu.
}
\seealso{
  \code{\link{comm.load.balance}()},
  \code{\link{comm.read.table}()} and
  \code{\link{comm.write.table}()}.
}
\examples{
%\dontrun{
### Save code in a file "demo.r" and run with 2 processors by
### SHELL> mpiexec -np 2 Rscript demo.r

spmd.code <- "
### Initialize
suppressMessages(library(pbdMPI, quietly = TRUE))

### Examples.
X <- matrix(1:15, ncol = 3)
X.gbd <- comm.as.gbd(X)
comm.print(X.gbd, all.rank = TRUE)

### Finish.
finalize()
"
pbdMPI::execmpi(spmd.code, nranks = 2L)
%}
}
\keyword{utility}
