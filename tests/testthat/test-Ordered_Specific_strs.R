test_that("Isotone Optimization for some specific isotonic constraints works", {
  load("Specific_strs.RData")
  x_chain <- solve_isotone_chain(y, w)
  x_tree <- solve_isotone_binary_tree(y,w)
  x_2d_grid <- solve_isotone_2d_grid(y,w)
  expect_equal(x_chain$x, x_chain_check)
  expect_equal(x_tree$x, x_tree_check)
  expect_equal(x_2d_grid$x, x_2d_grid_check)
})
