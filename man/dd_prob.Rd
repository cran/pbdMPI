\name{probe}
\alias{probe}
\alias{iprobe}
\title{ Probe Functions }
\description{
  The functions call MPI probe functions.
}
\usage{
probe(rank.source = .pbd_env$SPMD.CT$rank.source,
      tag = .pbd_env$SPMD.CT$tag, comm = .pbd_env$SPMD.CT$comm,
      status = .pbd_env$SPMD.CT$status)
iprobe(rank.source = .pbd_env$SPMD.CT$rank.source,
       tag = .pbd_env$SPMD.CT$tag, comm = .pbd_env$SPMD.CT$comm,
       status = .pbd_env$SPMD.CT$status)
}
\arguments{
  \item{rank.source}{a source rank where an object sent from.}
  \item{tag}{a tag number.}
  \item{comm}{a communicator number.}
  \item{status}{a status number.}
}
\details{
  These functions are for internal functions. Potentially, they set/get
  probe for receiving data.
}
\value{
  An invisible state of MPI call is returned.
}
\references{
  Programming with Big Data in R Website:
  \url{https://pbdr.org/}
}
\author{
  Wei-Chen Chen \email{wccsnow@gmail.com}, George Ostrouchov,
  Drew Schmidt, Pragneshkumar Patel, and Hao Yu.
}
\examples{
\dontrun{
### See source code of spmd.recv.default() for an example.
}
}
\keyword{programming}
