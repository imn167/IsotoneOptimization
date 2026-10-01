test_that("Isotone Optimization for arbitrary DAG isotonic constraints works", {
  load("Ordered_DGA.RData")
  x_DAG <- solve_isotone_DAG(y, w, E_mat)
  x_DAG_huber <- solve_isotone_DAG(y, w, E_mat, delta = 5, loss = "smoothH")
  x_DAG_huber <- solve_isotone_DAG(y, w, E_mat, delta = 12, loss = "huber", eps = 1e-4, sol_ir = x_DAG$x)
  expect_equal(x_DAG$x, x_DAG_check)
})


