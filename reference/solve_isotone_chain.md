# Isotonic Regression with chain-ordered isotonic constraints

Isotonic Regression with chain-ordered isotonic constraints

## Usage

``` r
solve_isotone_chain(
  y,
  w = NULL,
  delta = 1,
  loss = "quadratic",
  eps = 1e-07,
  sol_ir = NULL
)
```

## Arguments

- y:

  data vector

- w:

  weights (default is NULL)

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

  (default is NULL) Isotonic regression solution used as the Tikhonov
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

Wang, X., Ying, J., Cardoso, J. V. M., & Palomar, D. P. (2022, June).
Efficient Algorithms for General Isotone Optimization. In Proceedings of
the *AAAI Conference on Artificial Intelligence* (Vol. 36, No. 8, pp.
8575-8583).

## Author

Xiwen Wang, Zé Vinícius, and Daniel P. Palomar

## Examples

``` r
library(IsotoneOptimization)
set.seed(10)
solve_isotone_chain(rnorm(10), rep(1,10))
#> $x
#>  [1] -0.5340012 -0.5340012 -0.5340012 -0.5340012 -0.5028171 -0.5028171
#>  [7] -0.5028171 -0.5028171 -0.5028171 -0.2564784
#> 
#> $blocks
#> $blocks[[1]]
#> [1] 1 2 3 4
#> 
#> $blocks[[2]]
#> [1] 5 6 7 8 9
#> 
#> $blocks[[3]]
#> [1] 10
#> 
#> 
#> $nb_outer_loop
#> [1] 7
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
