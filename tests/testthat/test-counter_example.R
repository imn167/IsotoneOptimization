test_that("multiplication works", {
  g = c(5, 4, 2, 7, 7, 5, 6, 6, 3, 4, 2, 4, 2, 2, 3, 1,4, 4,
       4, 6, 4, 4, 4, 7, 8, 3, 4, 4, 5, 4, 3, 1)
  #partial order
  edges <- c(
    1, 3,  5, 9,  2, 10,  4, 11,  7, 12,  8, 12,  10, 12,
    4, 13,  6, 13,  9, 13,  8, 14,  10, 14,  12, 15,  12, 16,
    18, 22,  20, 22,  22, 25,  22, 26,  18, 27,  20, 27,
    23, 27,  21, 28,  24, 28,  18, 29,  21, 29,  4, 30,
    6, 30,  9, 30,  25, 31,  26, 31,  18, 32,  28, 32
  )
  Emat = matrix(edges, ncol = 2, byrow = TRUE)

  unreg_huber = solve_isotone_DAG(g, Emat = Emat, delta = .9, loss = "huber",
                                  eps = .Machine$double.xmin,
                                  sol_ir = rep(0, 32))
  reg_huber = solve_isotone_DAG(g, Emat = Emat, delta = .9, loss = "huber", eps = 1e-7,
                                  sol_ir = rep(0, 32))

})
