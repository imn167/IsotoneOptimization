#' @title Isotonic Regression with isotonic constraints represented by arbitrary directed acyclic graph
#'
#' @description Isotonic Regression with isotonic constraints represented by arbitrary directed acyclic graph
#'
#' @author Xiwen Wang, Zé Vinícius, and Daniel P. Palomar
#'
#' @references
#' Wang, X., Ying, J., Cardoso, J. V. D. M., & Palomar, D. P. (2022, June).
#' Efficient Algorithms for General Isotone Optimization.
#' In Proceedings of the \emph{AAAI Conference on Artificial Intelligence}
#' (Vol. 36, No. 8, pp. 8575-8583).
#' @param y regression vector
#' @param w weights (default is NULL)
#' @param Emat An m*2 Matrix to define the edges. Each row, given as (i,j), represents an edge from i to j
#' @param loss type of loss wanted (either "quadratic" which is the isotonic regression or "smoothH" a smooth
#' version of Huber loss)
#' @param delta hyperparameter if method == "smoothH", "huber" or "mbl"
#' @param eps Numeric value greater than zero (default: 1e-7).
#'   Tikhonov regularization parameter that ensures a unique solution
#'   to the optimization problem.
#' @param sol_ir Isotonic regression solution used as the Tikhonov
#'   regularization center, allowing the optimization problem to be
#'   formulated as a projection.
#' @return A list containing the following elements:
#' \item{\code{x}}{Optimal solution satisfying the constraints.}
#' \item{\code{blocks}}{Optimal partition of the solution satisfying the constraints.}
#' \item{\code{nb_outer_loop}}{Number of outer iterations required to solve the optimization problem (OP).}
#' \item{\code{nb_inner_loop}}{Number of inner iterations required to solve the optimization problem (OP).}
#' \item{\code{converged}}{For \code{loss = "smoothH"}, a logical vector indicating whether the optimization
#' converged for each aggregated block. Its length equals the number of aggregated blocks. Otherwise, \code{NULL}.}
#' \item{\code{kantorovich}}{For \code{loss = "smoothH"}, a logical vector indicating whether the block
#' solution satisfies the convergence criterion based on the Kantorovich method.
#' Its length equals the number of aggregated blocks. Otherwise, \code{NULL}.}
#' @examples
#' library(IsotoneOptimization)
#' Emat <- t(matrix(c(1,2,  1,3,  1,4,  2,5,  2,6,  3,7,  4,8,  5,8,  6,9,  6,10),2, 10))
#' solve_isotone_DAG(rnorm(10), rep(1,10), Emat)
#'
#' @import Rcpp
#' @useDynLib IsotoneOptimization
#' @importFrom Rcpp sourceCpp
#' @export
solve_isotone_DAG <- function(y, w = rep(1, length(y)), Emat, delta = 1, loss = "quadratic", eps = 1e-7, sol_ir = NULL) {
  # error control
  if (ncol(Emat) != 2) {
    stop("The matrix Emat should be m * 2.")
  }

  if (max(Emat) > length(y)) {
    stop("The matrix Emat contains invalid edges.")
  }

  if(min(Emat) < 1) {
    stop("The index of first node should be 1 instead of 0.")
  }

  if(!is_dag(graph_from_edgelist(Emat))) {
    stop("The matrix Emat should represent an acyclic graph.")
  }

  if (!is.null(w)) {
    if(min(w) < 0) {
      stop("The weights w should not be negative.")
    }
    if(length(y) != length(w)) {
      stop("Length of ", dQuote("y"), " should be equal to length of ", dQuote("w"), ".")
    }
  }

  if (!(loss %in% c("quadratic", "smoothH", "huber", "mbl"))) stop("accepted loss are quadratic, huber, mbl or smoothH")
  return(solve_ordered_arbitrary_DAG(y, w, Emat - 1, delta, loss, eps, sol_ir))
}


