\name{sendrecv-method}
\docType{methods}

\alias{sendrecv-method}

\alias{sendrecv,ANY,ANY-method}
\alias{sendrecv,integer,integer-method}
\alias{sendrecv,numeric,numeric-method}
\alias{sendrecv,raw,raw-method}

\alias{sendrecv}

\title{ Send and Receive an Object to and from Other Ranks }
\description{
  This method lets a rank send an object to the other rank and
  receive an object from another rank in the same communicator.
  The default return is \code{x}.
}
\usage{
sendrecv(x, x.buffer = NULL,
  rank.dest = (comm.rank(.pbd_env$SPMD.CT$comm) + 1) \%\%
              comm.size(.pbd_env$SPMD.CT$comm),
  send.tag = .pbd_env$SPMD.CT$tag,
  rank.source = (comm.rank(.pbd_env$SPMD.CT$comm) - 1) \%\%
                comm.size(.pbd_env$SPMD.CT$comm),
  recv.tag = .pbd_env$SPMD.CT$tag,
  comm = .pbd_env$SPMD.CT$comm, status = .pbd_env$SPMD.CT$status)
}
\arguments{
  \item{x}{an object to be sent from a rank.}
  \item{x.buffer}{a buffer to store \code{x} sent from the other rank.}
  \item{rank.dest}{a rank of destination where \code{x} send to.}
  \item{send.tag}{a send tag number.}
  \item{rank.source}{a source rank where \code{x} sent from.}
  \item{recv.tag}{a receive tag number.}
  \item{comm}{a communicator number.}
  \item{status}{a status number.}
}
\details{
  A corresponding \code{sendrecv()} should be evoked at the corresponding ranks
  \code{rank.dest} and \code{rank.source}.

  \code{rank.dest} and \code{rank.source} can be \code{as.integer(NULL)} to
  create a silent sendrecv operation which is more efficient than setting
  \code{rank.dest} and \code{rank.source} to be equal.
}
\value{
  A \code{x} is returned by default.
}
\section{Methods}{
For calling \code{spmd.sendrecv.*()}:
\describe{
\item{\code{signature(x = "ANY", x.buffer = "ANY")}}{}
\item{\code{signature(x = "integer", x.buffer = "integer")}}{}
\item{\code{signature(x = "numeric", x.buffer = "numeric")}}{}
\item{\code{signature(x = "raw", x.buffer = "raw")}}{}
}}
\references{
  Programming with Big Data in R Website:
  \url{https://pbdr.org/}
}
\author{
  Wei-Chen Chen \email{wccsnow@gmail.com}, George Ostrouchov,
  Drew Schmidt, Pragneshkumar Patel, and Hao Yu.
}
\seealso{
  \code{\link{sendrecv.replace}()}.
}
\examples{
\dontrun{
### Save code in a file "demo.r" and run with 2 processors by
### SHELL> mpiexec -np 2 Rscript demo.r

spmd.code <- "
### Initialize
suppressMessages(library(pbdMPI, quietly = TRUE))
.comm.size <- comm.size()
.comm.rank <- comm.rank()

### Examples.
N <- 5
x <- (1:N) + N * .comm.size
y <- sendrecv(matrix(x, nrow = 1))
comm.print(y, rank.print = 1)

### Finish.
finalize()
"
# execmpi(spmd.code, nranks = 2L)
}
}
\keyword{methods}
\keyword{collective}
