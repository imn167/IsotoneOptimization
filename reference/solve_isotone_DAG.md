# Isotonic Regression with isotonic constraints represented by arbitrary directed acyclic graph

Isotonic Regression with isotonic constraints represented by arbitrary
directed acyclic graph

## Usage

``` r
solve_isotone_DAG(
  y,
  w = rep(1, length(y)),
  Emat,
  delta = 1,
  loss = "quadratic",
  eps = 1e-07,
  sol_ir = NULL
)
```

## Arguments

- y:

  regression vector

- w:

  weights (default is NULL)

- Emat:

  An m\*2 Matrix to define the edges. Each row, given as (i,j),
  represents an edge from i to j

- delta:

  hyperparameter if method == "smoothH", "huber" or "mbl"

- loss:

  type of loss wanted (either "quadratic" which is the isotonic
  regression or "smoothH" a smooth version of Huber loss)

- eps:

  Numeric value greater than zero (default: 1e-7). Tikhonov
  regularization parameter that ensures a unique solution to the
  optimization problem.

- sol_ir:

  (Default is NULL) Isotonic regression solution used as the Tikhonov
  regularization center, allowing the optimization problem to be
  formulated as a projection.

## Value

A list containing the following elements:

- `x`:

  Optimal solution satisfying the constraints.

- `blocks`:

  Optimal partition of the solution satisfying the constraints.

- `nb_outer_loop`:

  Number of outer iterations required to solve the optimization problem
  (OP).

- `nb_inner_loop`:

  Number of inner iterations required to solve the optimization problem
  (OP).

- `converged`:

  For `loss = "smoothH"`, a logical vector indicating whether the
  optimization converged for each aggregated block. Its length equals
  the number of aggregated blocks. Otherwise, `NULL`.

- `kantorovich`:

  For `loss = "smoothH"`, a logical vector indicating whether the block
  solution satisfies the convergence criterion based on the Kantorovich
  method. Its length equals the number of aggregated blocks. Otherwise,
  `NULL`.

## References

Wang, X., Ying, J., Cardoso, J. V. D. M., & Palomar, D. P. (2022, June).
Efficient Algorithms for General Isotone Optimization. In Proceedings of
the *AAAI Conference on Artificial Intelligence* (Vol. 36, No. 8, pp.
8575-8583).

## Author

Xiwen Wang, Zé Vinícius, and Daniel P. Palomar

## Examples

``` r
library(IsotoneOptimization)
Emat <- t(matrix(c(1,2,  1,3,  1,4,  2,5,  2,6,  3,7,  4,8,  5,8,  6,9,  6,10),2, 10))
solve_isotone_DAG(rnorm(10), rep(1,10), Emat)
#> $x
#>  [1] -0.03046597  0.42256439 -0.03046597  0.51122815  0.51122815  0.42256439
#>  [7] -0.03046597  0.51122815  0.92552126  0.48297852
#> 
#> $blocks
#> $blocks[[1]]
#> [1] 1 3 7
#> 
#> $blocks[[2]]
#> [1] 2 6
#> 
#> $blocks[[3]]
#> [1] 4 5 8
#> 
#> $blocks[[4]]
#> [1] 9
#> 
#> $blocks[[5]]
#> [1] 10
#> 
#> 
#> $nb_outer_loop
#> [1] 5
#> 
#> $nb_inner_loop
#> [1] 0
#> 
#> $converged
#> integer(0)
#> 
#> $kantorovich
#> logical(0)
#> 
```
