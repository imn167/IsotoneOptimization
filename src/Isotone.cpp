#include <Rcpp.h>
# include <iostream>
# include <cmath>
# include <set>
# include <algorithm>
# include <iterator>
# include <vector>
using namespace Rcpp;
using namespace std;

class node {
public:
  node(int i) {
    index = i;
  }
  int index = 0;
  node* next = NULL;
};

class Adjacency_list {
public:

// Add a default constructor
  Adjacency_list() : n(0), node_ad_array(nullptr) {}


  Adjacency_list(int size) {
    n = size;
    node_ad_array = new node * [size];
    for (int i = 0;i < size;i++) {
      node_ad_array[i] = new node(i);
    }
  }
  // ~Adjacency_list() {
  //   for (int i = 0; i < n;i++) {
  //     node* cur_node = node_ad_array[i];
  //     node* temp_node = cur_node;
  //     while (cur_node->next != NULL) {
  //       temp_node = cur_node->next;
  //       delete cur_node;
  //       cur_node = temp_node;
  //     }
  //     delete cur_node;
  //   }
  //   delete[]node_ad_array;
  // }


  ~Adjacency_list() {
    for (int i = 0; i < n; i++) {
        node* cur = node_ad_array[i];
        while (cur != nullptr) {
            node* next_node = cur->next; // Save the next pointer BEFORE deleting
            delete cur;                  // Safe to delete now
            cur = next_node;             // Move to the saved pointer
        }
    }
    delete[] node_ad_array;
}

  Adjacency_list(const Adjacency_list& src_obj) {
    n = src_obj.n;
    node_ad_array = new node * [n];
    for (int i = 0;i < n;i++) {
      node_ad_array[i] = new node(i);
    }
    // int ii = 0;
    for (int ii = 0;ii < n;ii++) {
      node* cur_node_obj = src_obj.node_ad_array[ii];
      node* cur_node = node_ad_array[ii];

      cur_node_obj = cur_node_obj->next;
      while (cur_node_obj != NULL) {
        cur_node->next = new node(cur_node_obj->index);
        cur_node_obj = cur_node_obj->next;
        cur_node = cur_node->next;
      }
    }

  }

  // FIX: Proper reset that cleans old memory and restarts
  void reset(int new_n) {
    // Call the destructor logic to clear old pointer memory
    for (int i = 0; i < n; i++) {
      node* cur = node_ad_array[i];
      while (cur) {
        node* next = cur->next;
        delete cur;
        cur = next;
      }
    }
    delete[] node_ad_array;

    // Re-initialize for the new size
    n = new_n;
    node_ad_array = new node*[n];
    for (int i = 0; i < n; i++) {
      node_ad_array[i] = new node(i);
    }
  }

  bool isEmpty(int i) {
    // check whether node i is connected with any other node
    return (node_ad_array[i]->next == NULL) ? true : false;
  }

  void connect(int i, int j) {
    // Connecting node i and node j
    if (node_ad_array[i]->next == NULL) {
      node_ad_array[i]->next = new node(j);
    }
    else {
      node* cur_node = node_ad_array[i]->next;
      while (cur_node->next != NULL && cur_node->index < j) {
        cur_node = cur_node->next;
      }
      node* temp_node = cur_node->next;
      cur_node->next = new node(j);
      cur_node->next->next = temp_node;
    }

    if (node_ad_array[j]->next == NULL) {
      node_ad_array[j]->next = new node(i);
    }
    else {
      node* cur_node = node_ad_array[j]->next;
      while (cur_node->next != NULL && cur_node->index < i) {
        cur_node = cur_node->next;
      }
      node* temp_node = cur_node->next;
      cur_node->next = new node(i);
      cur_node->next->next = temp_node;
    }
  }
  void disconnect(int i, int j) {
    // Disconnecting node i and node j
    bool not_connected = true;
    node* cur_node = node_ad_array[i]->next;
    node* pre_node = node_ad_array[i];
    while (cur_node->index != j) {
      pre_node = cur_node;
      cur_node = cur_node->next;
      if (cur_node == NULL) {
        not_connected = false;
        break;
      }
    }
    if (not_connected) {
      pre_node->next = cur_node->next;
      delete cur_node;
      cur_node = node_ad_array[j]->next;
      pre_node = node_ad_array[j];
      while (cur_node->index != i) {
        pre_node = cur_node;
        cur_node = cur_node->next;
      }
      pre_node->next = cur_node->next;
      delete cur_node;
    }

  }
  set<int> compute_subgraph(int i) {
    // Find all the subgraph where the node i is in.
    set<int> searched_set;
    set<int> subgraph_set;
    // initialize searched set and subgraph set
    searched_set.insert(i);
    subgraph_set.insert(i);
    node* cur_node = node_ad_array[i]->next;
    while (cur_node != NULL) {
      subgraph_set.insert(cur_node->index);
      cur_node = cur_node->next;
    }

    while (searched_set.size() != subgraph_set.size()) {
      set<int> to_search_set(subgraph_set);
      set_difference(subgraph_set.begin(), subgraph_set.end(), searched_set.begin(), searched_set.end(), inserter(to_search_set, to_search_set.begin()));
      searched_set = subgraph_set;
      for (set<int>::iterator it = to_search_set.begin();it != to_search_set.end();it++) {
        cur_node = node_ad_array[*it]->next;
        while (cur_node != NULL) {
          subgraph_set.insert(cur_node->index);
          cur_node = cur_node->next;
        }
      }
    }
    return subgraph_set;
  }
  void print() {
    for (int i = 0;i < n;i++) {
      cout << i << " -> ";
      node* cur_node = node_ad_array[i]->next;
      if (cur_node == NULL) {
        cout << "()" << endl;
      }
      else {
        cout << "( ";
        while (cur_node != NULL) {
          cout << cur_node->index << " ";
          cur_node = cur_node->next;
        }
        cout << ")" << endl;
      }
    }
  }
  bool is_directly_connected(int i, int j) {
    bool connected = false;
    node* cur_node = node_ad_array[i]->next;
    while (cur_node != NULL) {
      if (cur_node->index == j) {
        connected = true;
        break;
      }
      cur_node = cur_node->next;
    }
    return connected;
  }
  bool is_connected(int i, int j) {
    set<int> searched_set;
    set<int> subgraph_set;
    // initialize searched set and subgraph set
    searched_set.insert(i);
    subgraph_set.insert(i);
    node* cur_node = node_ad_array[i]->next;
    while (cur_node != NULL) {
      subgraph_set.insert(cur_node->index);
      if (j == cur_node->index) {
        return true;
      }
      cur_node = cur_node->next;
    }

    while (searched_set.size() != subgraph_set.size()) {
      set<int> to_search_set(subgraph_set);
      set_difference(subgraph_set.begin(), subgraph_set.end(), searched_set.begin(), searched_set.end(), inserter(to_search_set, to_search_set.begin()));
      searched_set = subgraph_set;
      for (set<int>::iterator it = to_search_set.begin();it != to_search_set.end();it++) {
        cur_node = node_ad_array[*it]->next;
        while (cur_node != NULL) {
          subgraph_set.insert(cur_node->index);
          if (j == cur_node->index) {
            return true;
          }
          cur_node = cur_node->next;
        }
      }
    }
    return false;
  }

  int n; // number of nodes
  node** node_ad_array; // the address of each node
};

class DAG_node {
public:
  DAG_node(int i) {
    index = i;
  }
  int index = 0;
  DAG_node* next = NULL;
  DAG_node* prev = NULL;
};

class DAG {
public:
  ~DAG() {
    for (int i = 0; i < n;i++) {
      DAG_node* cur_node = node_array[i];
      DAG_node* temp_node = cur_node;
      while (cur_node->next != NULL) {
        temp_node = cur_node->next;
        delete cur_node;
        cur_node = temp_node;
      }
      temp_node = cur_node;
      while (cur_node->prev != NULL) {
        temp_node = cur_node->prev;
        delete cur_node;
        cur_node = temp_node;
      }
      delete cur_node;
    }
    delete[]node_array;
  }

  void initialize(int size) {
    n = size;
    node_array = new DAG_node * [size];
    for (int i = 0;i < size;i++) {
      node_array[i] = new DAG_node(i);
    }
  }

  void connect(int i, int j) {
    if (node_array[i]->next == NULL) {
      node_array[i]->next = new DAG_node(j);
    }
    else {
      DAG_node* cur_node = node_array[i]->next;
      while (cur_node->next != NULL && cur_node->index < j) {
        cur_node = cur_node->next;
      }
      DAG_node* temp_node = cur_node->next;
      cur_node->next = new DAG_node(j);
      cur_node->next->next = temp_node;
    }

    if (node_array[j]->prev == NULL) {
      node_array[j]->prev = new DAG_node(i);
    }
    else {
      DAG_node* cur_node = node_array[j]->prev;
      while (cur_node->prev != NULL && cur_node->index < i) {
        cur_node = cur_node->prev;
      }
      DAG_node* temp_node = cur_node->prev;
      cur_node->prev = new DAG_node(i);
      cur_node->prev->prev = temp_node;
    }
  }

  bool is_connected(int i, int j) {
    bool connected = false;
    DAG_node* cur_node = node_array[i]->next;
    while (cur_node != NULL) {
      if (cur_node->index == j) {
        connected = true;
        break;
      }
      cur_node = cur_node->next;
    }
    return connected;
  }

  set<int> get_children_set(int i) {
    set<int> children_set;
    DAG_node* cur_node = node_array[i]->next;
    while (cur_node != NULL) {
      children_set.insert(cur_node->index);
      cur_node = cur_node->next;
    }
    return children_set;
  }

  set<int> get_parents_set(int i) {
    set<int> parents_set;
    DAG_node* cur_node = node_array[i]->prev;
    while (cur_node != NULL) {
      parents_set.insert(cur_node->index);
      cur_node = cur_node->prev;
    }
    return parents_set;
  }

private:
  int n;
  DAG_node** node_array;

};

class ListNode {
public:
  int row_index;
  int col_index;
  ListNode* next;
  ListNode() {
    next = NULL;
  }
};

class List {
private:
  ListNode* first;
  int size;
public:
  List() {
    size = 0;
    first = NULL;
  }
  ~List() {
    while (first != NULL) {
      ListNode* to_delete_node = first;
      first = to_delete_node->next;
      delete to_delete_node;
    }
  }
  void push(int col_index, int row_index) {
    ListNode* newNode = new ListNode();
    newNode->col_index = col_index;
    newNode->row_index = row_index;
    newNode->next = first;
    first = newNode;
    size++;
  }

  void pop(int& col_index, int& row_index) {
    col_index = first->col_index;
    row_index = first->row_index;
    ListNode* to_delete_node = first;
    first = to_delete_node->next;
    delete to_delete_node;
    size--;
  }

  int get_size() {
    return size;
  }
};


class orderedBlock {
public:
  orderedBlock(int size, const vector<double>& y = vector<double>(), const vector<double>&w = vector<double>(), double delta = 1.0, std::string loss = "quadratic",
               double eps = 0.0) // const vector<double>& sol_ir = vector<double>()
  : n(size), xl(y), calibrated_xl(y),weights(w.empty() ? vector<double>() : w), // Only copy if not empty
      weighted(!w.empty()),
      adj_internal(size),
      loss(loss),
      delta(delta),
      eps(eps)
      // sol_ir(sol_ir)// Initialize here!
  {
    // if (eps > 0.0 && sol_ir.empty()) {
    //   throw std::invalid_argument(
    //       "orderedBlock: eps > 0 require an IR solution (sol_ir) non-empty for Tikhonov regularisation.");
    // }
    // n = size;
    // xl.resize(n);
    // calibrated_xl.resize(n);
    // weights.resize(n);
    // adj_internal.resize(n);
    // weighted = !w.empty();

    // if (w != NULL) {
    //   weighted = true;
    // }
    // else {
    //   weighted = false;
    // }

    // if (!y.empty()) {
    //   xl = y;
    //   calibrated_xl = y;

    // }

    // if (weighted) {
    //   weights = w;
    // }

    // if (y != NULL) {
    //   for (int i = 0;i < n;i++) {
    //     xl[i] = y[i];
    //     calibrated_xl[i] = y[i];
    //   }
    //   if (weighted) {
    //     for (int i = 0;i < n;i++) {
    //       weights[i] = w[i];
    //     }
    //   }
    // }

    // str_DAG = DAG();
    str_DAG.initialize(n);
  }
  // no destructor needed ! Everything auto-cleaned
  ~orderedBlock() = default;
  // ~orderedBlock() {
  //   delete[]xl;
  //   delete[]calibrated_xl;
  // }

  void load_chain_isotonic_str() {
    m = n - 1;
    // E = new int* [m];
    E.resize(m, vector<int>(2));

    // for (int i = 0;i < m;i++) {
    //   E[i] = new int[2];
    // }
    // int E_count = 0;

    for (int i = 0;i < m;i++) {
      str_DAG.connect(i, i + 1);
      E[i][0] = i;
      E[i][1] = i + 1;
      // E_count++;
    }
  }

  // int** E_mat, int m_input
  void load_isotonic_str_via_mats(const vector<vector<int>>& E_mat) {

    m = E_mat.size();
    E.resize(m, vector<int>(2));
    // for (int i = 0;i < m;i++) {
    //   E[i] = new int[2];
    // }

    for (int i = 0;i < m;i++) {
      str_DAG.connect(E_mat[i][0], E_mat[i][1]);
      E[i][0] = E_mat[i][0];
      E[i][1] = E_mat[i][1];
      //cout << E_mat[i][0] << " " << E_mat[i][1] << endl;
      //cout << E[i][0] << " " << E[i][1] << endl;
    }
  }

  void load_tree_isotonic_str() {
    m = n - 1;
    E.resize(m, vector<int>(2));
    // for (int i = 0;i < m;i++) {
    //   E[i] = new int[2];
    // }

    int h = floor(log2(n + 1));
    int count = 0;
    for (int i = 0; i < pow(2, h - 1) - 1;i++) {
      for (int ii = 2 * i + 1; ii < 2 * i + 3; ii++) {
        E[count][0] = i;
        E[count][1] = ii;
        str_DAG.connect(i, ii);
        count++;
      }
    }
    int temp_i;
    for (int i = pow(2, h) - 1;i < n;i++) {
      temp_i = int(floor(double(i) / 2 + 0.5)) - 1;
      E[count][0] = temp_i;
      E[count][1] = i;
      str_DAG.connect(temp_i, i);
      count++;
    }

  }

  void load_2D_isotonic_str() {
    int nl = floor(sqrt(n));
    m = nl * nl - 2 * nl + n;
    E.resize(m, vector<int>(2));
    // for (int i = 0;i < m;i++) {
    //   E[i] = new int[2];
    // }
    int E_count = 0;

    for (int i = 0;i < nl - 1;i++) {
      for (int j = 0;j < nl - 1;j++) {
        str_DAG.connect(j + nl * i, (j + 1) + nl * i);
        E[E_count][0] = j + nl * i;
        E[E_count][1] = (j + 1) + nl * i;
        E_count++;
        str_DAG.connect(j + nl * i, j + nl * (i + 1));
        E[E_count][0] = j + nl * i;
        E[E_count][1] = j + nl * (i + 1);
        E_count++;
      }
      str_DAG.connect(nl - 1 + nl * i, nl - 1 + nl * (i + 1));
      E[E_count][0] = nl - 1 + nl * i;
      E[E_count][1] = nl - 1 + nl * (i + 1);
      E_count++;
    }
    for (int j = nl * (nl - 1);j < n - 1;j++) {
      str_DAG.connect(j, j + 1);
      E[E_count][0] = j;
      E[E_count][1] = j + 1;
      E_count++;
    }
  }

  void print_E() {
    for (int i = 0;i < m;i++) {
      cout << E[i][0] << " " << E[i][1] << endl;
    }
  }

  void solve_by_SBM(bool remember_adj_gra = false) {
    outer_loop = 0;
    inner_loop = 0;
    n_bisection_fallback = 0;
    all_blocks_converged = true;
    calibrated_xl = xl;
    // for (int i = 0;i < n;i++) {
    //   calibrated_xl[i] = xl[i];
    // }

    // Adjacency_list* Adjacency_obj = new Adjacency_list(n);
    // Instead of 'new', just reset the existing object
    if (!remember_adj_gra) {

      adj_internal.reset(n);
    }
    vector<double> Ax = compute_Ax();
    // double* Ax = compute_Ax();
    //--------------------------------------- PART WHERE BLOCK VALUE AND BLOCK SET ARE DEFINED -------------------
    int min_pos = whichMin(Ax, m);
    double min_value = Ax[min_pos];
    int i, j;
    double x_bar;
    vector<double> local_lambda;
    set<int> block_set;
    while (min_value < -1e-9) {
      outer_loop++;
      i = E[min_pos][0];
      j = E[min_pos][1];

      if (adj_internal.isEmpty(i) && adj_internal.isEmpty(j)) { // fresh merge and new block created
        adj_internal.connect(i, j);
        //cout << "Connecting " << i << "-" << j << endl;
        if (loss == "quadratic") {

        if (weighted) {
          x_bar = (xl[i] * weights[i] + xl[j] * weights[j]) / (weights[i] + weights[j]);
        }
        else {
          x_bar = 0.5 * (xl[i] + xl[j]);
          }
        }
        else if (loss == "smoothH") {
          set<int> two_set = {i,j};
          x_bar = solve_block_newton(two_set);
          blocks_converged.push_back(get_block_converged() ? 1 : 0);
          kantorovich.push_back(get_kantorovich_certif());
          // cout << "number of newton iteration " << get_iter_newton() << " : samples" << i << "-" << j << endl;
          // cout << "number of fallback into bisection" << get_bisection_fallback() << endl;
        }
        else if (loss == "huber") {
          set<int> two_set = {i,j};
          x_bar = solve_block_huber(two_set);
        }

        calibrated_xl[i] = x_bar;
        calibrated_xl[j] = x_bar;
      }

      else { // Already an existing block for either j or i or both
        adj_internal.connect(i, j);
        // cout << "Connecting " << i << "-" << j << endl;
        block_set = adj_internal.compute_subgraph(i); // get all the index inside this block
        if (loss == "quadratic") {
        x_bar = get_x_bar(block_set);
        }
        else if (loss == "smoothH") {
          x_bar = solve_block_newton(block_set);
          blocks_converged.push_back(get_block_converged() ? 1 : 0);
          kantorovich.push_back(get_kantorovich_certif());
          // cout << "number of newton iteration  " << get_iter_newton() << endl;
          // cout << "number of fallback into bisection" << get_bisection_fallback() << endl;
        }

        else if (loss == "huber") {
          x_bar = solve_block_huber(block_set);
        }


        local_lambda = solve_local_KKT(block_set, x_bar, El, &adj_internal);
        for (set<int>::iterator it = block_set.begin();it != block_set.end();it++) {
          calibrated_xl[*it] = x_bar;
          // cout << "x[" << *it << "] = " << x_bar << endl;
        }
        if (get_min_value(local_lambda, block_set.size() - 1) < -1e-12) { // a negative lambda value appeared after a merge --> need to split block
          // go to inner loop
          set<int> G_bar_index_set;
          for (int i = 0;i < block_set.size() - 1;i++) {
            G_bar_index_set.insert(i);
          }
          current_nbo = block_set.size();
          solve_inner_loop(G_bar_index_set, local_lambda, &adj_internal);
        }
        // for (int i = 0;i < block_set.size() - 1;i++) {
        //   delete[]El[i];
        // }
        // delete[]El;
        // delete[]local_lambda;
      }
      Compute_min_diff(min_value, min_pos, Ax);
    }
    // if (remember_adj_gra) {
    //   //Adjacency_obj.print();
    //   if (warm_start_obj != NULL) {
    //     delete warm_start_obj;
    //   }
    //   warm_start_obj = Adjacency_obj;
    // }
    // else {
    //   delete Adjacency_obj;
    // }
  }

  void solve_inner_loop(set<int> &inequality_indexset, vector<double>&lambda, Adjacency_list* Adjacency_obj) {
    set<int> current_equality_indexset;
    set<int> current_inequality_indexset;
    // initialization
    for (set<int>::iterator it = inequality_indexset.begin(); it != inequality_indexset.end();it++) {
      if (calibrated_xl[El[*it][0]] != calibrated_xl[El[*it][1]]) {
        current_inequality_indexset.insert(*it);
      }
      else {
        current_equality_indexset.insert(*it);
      }
    }
    bool local_kkt_solved = true;
    double min_lambda = 0;
    int min_lambda_pos = 0;
    vector<vector<int>> El_i; //local_str_info
    vector<vector<int>> El_j; //local_str_info
    for (set<int>::iterator it = inequality_indexset.begin(); it != inequality_indexset.end();it++) {
      if (lambda[*it] < min_lambda) {
        local_kkt_solved = false;
        min_lambda = lambda[*it];
        min_lambda_pos = *it;
      }
    }
    while (!local_kkt_solved) {
      inner_loop++;
      int to_delete_i = El[min_lambda_pos][0];
      int to_delete_j = El[min_lambda_pos][1];
      Adjacency_obj->disconnect(to_delete_i, to_delete_j);
      current_inequality_indexset.insert(min_lambda_pos);
      current_equality_indexset.erase(min_lambda_pos);

      // cout << "Disconnecting " << to_delete_i << "-" << to_delete_j << endl;
      lambda[min_lambda_pos] = 0;
      set<int> block_set_i = Adjacency_obj->compute_subgraph(to_delete_i);
      set<int> block_set_j = Adjacency_obj->compute_subgraph(to_delete_j);

      // re-optimize individual blocks, update x and lambda
      double x_bar_i = 0.0;
      double x_bar_j = 0.0;

      if (loss == "quadratic") {

       x_bar_i = get_x_bar(block_set_i);
       x_bar_j = get_x_bar(block_set_j);
      }

      else if (loss == "smoothH") {
        x_bar_i = solve_block_newton(block_set_i);
        blocks_converged.push_back(get_block_converged() ? 1 : 0);
        kantorovich.push_back(get_kantorovich_certif());
        x_bar_j = solve_block_newton(block_set_j);
        blocks_converged.push_back(get_block_converged() ? 1 : 0);
        kantorovich.push_back(get_kantorovich_certif());
        // cout << "number of newton iteration " << get_iter_newton() << endl;
        // cout << "number of fallback into bisection" << get_bisection_fallback() << endl;
      }

      else if (loss == "huber") {
        x_bar_i = solve_block_huber(block_set_i);
        x_bar_j = solve_block_huber(block_set_j);
      }

      for (set<int>::iterator it = block_set_i.begin();it != block_set_i.end();it++) {
        calibrated_xl[*it] = x_bar_i;
      }
      for (set<int>::iterator it = block_set_j.begin();it != block_set_j.end();it++) {
        calibrated_xl[*it] = x_bar_j;
      }
      if (block_set_i.size() != 1) {
        vector<double> local_lambda_i = solve_local_KKT(block_set_i, x_bar_i, El_i, Adjacency_obj); //double* local_lambda_i
        updata_local_lamdba(lambda, El, local_lambda_i, El_i, block_set_i.size() - 1);
      }
      if (block_set_j.size() != 1) {
        vector<double> local_lambda_j = solve_local_KKT(block_set_j, x_bar_j, El_j, Adjacency_obj);
        updata_local_lamdba(lambda, El, local_lambda_j, El_j, block_set_j.size() - 1);
      }

      // repair primal feasibility if necessary
      int min_i = 0;
      int min_j = 0;
      bool is_still_primal_feasible = is_primal_feasible(El, current_nbo - 1, min_i, min_j);

      while (!is_still_primal_feasible) {
        Adjacency_obj->connect(min_i, min_j);
        //cout << "re-connect " << min_i << "-" << min_j << endl;
        set<int> subcurrent_set = Adjacency_obj->compute_subgraph(min_i);
        // int** l_El = new int* [subcurrent_set.size() - 1];
        vector<vector<int>> l_El(subcurrent_set.size() - 1, vector<int>(2, 0));
        double l_x_bar = 0.0;
        if (loss == "quadratic") {
        l_x_bar = get_x_bar(subcurrent_set);
        }
        else if (loss == "smoothH") {
          l_x_bar = solve_block_newton(subcurrent_set);
          blocks_converged.push_back(get_block_converged() ? 1 : 0);
          kantorovich.push_back(get_kantorovich_certif());
        }
        else if (loss == "huber") {
          l_x_bar = solve_block_huber(subcurrent_set);
        }

        vector<double> l_local_lambda = solve_local_KKT(subcurrent_set, l_x_bar, l_El, Adjacency_obj);
        updata_local_lamdba(lambda, El, l_local_lambda, l_El, subcurrent_set.size() - 1);
        for (set<int>::iterator it = subcurrent_set.begin();it != subcurrent_set.end();it++) {
          calibrated_xl[*it] = l_x_bar;
        }
        // for (int i = 0;i < subcurrent_set.size() - 1;i++) {
        //   delete[]l_El[i];
        // }
        // delete[]l_El;
        // delete[]l_local_lambda;
        is_still_primal_feasible = is_primal_feasible(El, current_nbo - 1, min_i, min_j);
      }

      // Whether go to recursion
      min_lambda = 0;
      for (set<int>::iterator it = current_inequality_indexset.begin(); it != current_inequality_indexset.end();it++) {
        if (lambda[*it] < min_lambda) {
          min_lambda = lambda[*it];
          min_lambda_pos = *it;
        }
      }
      if (min_lambda < 0) {
        solve_inner_loop(current_inequality_indexset, lambda, Adjacency_obj);
      }

      local_kkt_solved = true;
      min_lambda = 0;
      for (set<int>::iterator it = inequality_indexset.begin(); it != inequality_indexset.end();it++) {
        if (lambda[*it] < min_lambda) {
          local_kkt_solved = false;
          min_lambda = lambda[*it];
          min_lambda_pos = *it;
        }
      }
    }
  }


  int get_inner_loop_num() {
    return(inner_loop);
  }
  int get_outer_loop_num() {
    return(outer_loop);
  }

  int get_iter_newton() {
    return(iter_newton);
  }

  int get_bisection_fallback() {
    return(n_bisection_fallback);
  }

  bool get_block_converged() {
    return(all_blocks_converged);
  }

  vector<int> get_converged() {
    return(blocks_converged);
  }

  bool get_kantorovich_certif() {
    return(certified);
  }

  vector<bool> get_kantorovich() {
    return(kantorovich);
  }


  void print_xl() {
    for (int j = 0;j < n;j++) {
      cout << calibrated_xl[j] << " ";
    }
    cout << endl;
  }


  void get_x_calibrated(vector<double>& x_to_write) const {
    x_to_write = calibrated_xl;
    // for (int i = 0;i < n;i++) {
    //   x_to_write[i] = calibrated_xl[i];
    // }
  }

  vector<vector<int>> get_partitions() {
      vector<vector<int>> partitions;
      // Keep track of which nodes we've already assigned to a block
      vector<bool> visited(n, false);

      for (int i = 0; i < n; i++) {
          // If node 'i' hasn't been put in a block yet, it starts a new one
          if (!visited[i]) {
              // Get all nodes connected to 'i' in the final adjacency graph
              // These connected nodes form a single block with the same x_bar
              set<int> block_set = adj_internal.compute_subgraph(i);

              vector<int> current_block;
              // Convert the set to a vector to store it in our results one by one to increment by 1
              for (int node : block_set) {
                visited[node] = true;
                current_block.push_back(node +1);
              }

              // added completed block to the list
              partitions.push_back(current_block);
          }
      }
      return partitions;
  }


private:
  std::string loss;
  int n; // number of node
  int m; // number of constraints or edges
  vector<double> xl; // data of every nodes
  vector<double> calibrated_xl; // calibrated xl
  vector<double> weights; // weights
  bool weighted;
  DAG str_DAG;
  Adjacency_list adj_internal; // No pointer! // Adjacency_list* warm_start_obj = nullptr;

  double delta; // hyperparameter transition in huber / logcosh
  double eps; // Tikhonov regularization in Huber proj

  // vector<double> sol_ir;


  int outer_loop = 0;
  int inner_loop = 0;
  int current_nbo = 0;
  // Log hyperbolic cosine flags
  int iter_newton = 0;
  int n_bisection_fallback = 0;

  bool all_blocks_converged = true;
  vector<int> blocks_converged;

  vector<bool> kantorovich;
  bool certified = false;

  vector<vector<int>> E; // constraints input matrix
  vector<vector<int>> El; //local_str_info
  vector<double> compute_Ax() {
    vector<double> Ax(m);
    for (int i = 0;i < m;i++) {
      Ax[i] = -calibrated_xl[E[i][0]] + calibrated_xl[E[i][1]];
    }
    return Ax;
  }

  int whichMin(const vector<double>& x, int size) {
    int min_pos;
    min_pos = 0;
    for (int i = 1;i < size;i++) {
      if (x[i] < x[min_pos]) {
        min_pos = i;
      }
    }
    return min_pos;
  }
  double get_min_value(vector<double>& x, int size) {
    double min_value = x[0];
    for (int i = 1;i < size;i++) {
      if (x[i] < min_value) {
        min_value = x[i];
      }
    }
    return min_value;
  }



  // Weigheted median via partial_sort for Newton init
  double get_weighted_median(const set<int>& inactive_set) {
    vector<pair<double, double>> vw;
    vw.reserve(inactive_set.size());
    double w_total = 0.0;
    for (int idx : inactive_set) {
      double wi = weighted ? weights[idx] : 1.0;
      vw.push_back({xl[idx], wi});
      w_total += wi;
    }
    sort(vw.begin(), vw.end());
    // walk until cumulative weights >= half
    double cum = 0.0;
    for (auto&p : vw) {
      cum+= p.second;
      if (cum>= 0.5 * w_total) return p.first;
    }
    return vw.back().first;
  }


  // BLOCK SOLVER FUNCTION

  double solve_block_newton(const std::set<int>& inactive_set) {
    int n_block = inactive_set.size();
    iter_newton = 0;

    // --- Trivial cases -------------------------------------------------
    if (n_block == 1) {
      return xl[*inactive_set.begin()];
    }

    if (n_block == 2) {
      auto it = inactive_set.begin();
      double y1 = xl[*it]; double w1 = weighted ? weights[*it] : 1.0; ++it;
      double y2 = xl[*it]; double w2 = weighted ? weights[*it] : 1.0;
      if (!weighted || w1 == w2) return 0.5 * (y1 + y2);
    }

    // --- Bracket from Prop 3.12: root is always strictly inside --------
    double lo = std::numeric_limits<double>::infinity();
    double hi = -std::numeric_limits<double>::infinity();
    double W  = 0.0;
    for (int idx : inactive_set) {
      double xi = xl[idx];
      if (xi < lo) lo = xi;
      if (xi > hi) hi = xi;
      W += weighted ? weights[idx] : 1.0;
    }

    double z = get_weighted_median(inactive_set);

    // --- Numerical tolerances -------------------------------------------
    const double eps_match = std::numeric_limits<double>::epsilon(); // 2.2e-16
    const double eps_S2 =  1e-12; // std::sqrt(n_block) * eps_match * std::max(W, 1.0); // underflow guard
    const double eps_resid = std::sqrt(n_block )* std::sqrt(eps_match);  // |F'| threshold
    const double eps_step  = std::sqrt(eps_match) * delta;
    const int max_iter = 60;

    auto eval = [&](double t, double& S1, double& S2) {
      S1 = 0.0; S2 = 0.0;
      for (int idx : inactive_set) {
        double wi  = weighted ? weights[idx] : 1.0;
        double r   = (t - xl[idx]) / delta;
        double th  = std::tanh(r);
        double sch = 1.0 / std::cosh(r);
        S1 += wi * th;
        S2 += wi * sch * sch;
      }
    };

    //  Newton-Kantorovich certificate at the median start --- NOT SURE NECESSARY
    double S1_0, S2_0;
    eval(z, S1_0, S2_0);

    if (S2_0 >= eps_S2) {
      const double K     = (4.0 * W) / (3.0 * std::sqrt(3.0) * delta);
      const double beta0 = 1.0 / (2.0 * S2_0);
      const double eta0  = delta * std::fabs(S1_0) / S2_0;
      const double h0    = K * beta0 * eta0;
      certified = (h0 <= 0.5);
    }

      double S1 = S1_0, S2 = S2_0;
      for (int iter = 0; iter < max_iter; ++iter) {
        iter_newton += 1;
        if (std::fabs(S2) < eps_S2 && std::fabs(S1) < eps_resid) break;
        double step = delta * S1 / S2;
        z -= step;
        if (std::fabs(S1) <= eps_resid && std::fabs(step) <= eps_step) {
          // cout << "Newton S1 " << fabs(S1) << endl;
          return z;
          }
        eval(z, S1, S2);

        // cout << "Newton S1 " << fabs(S1) << " step :" << fabs(step) << " S2 :" << S2  << endl;
      }

      all_blocks_converged = false;


    // --- Step 2b: uncertified -> safeguarded Newton / bisection hybrid -
    if (!all_blocks_converged) {

      n_bisection_fallback += 1;   // diagnostic counter -- see note below

    for (int iter = 0; iter < max_iter; ++iter) {
      double S1, S2;
      eval(z, S1, S2);

      double z_next = z;
      bool step_ok = false;
      if (S2 >= eps_S2) {
        double step = delta * S1 / S2;
        double candidate = z - step;
        if (candidate > lo && candidate < hi) {
          z_next  = candidate;
          step_ok = true;
          if (std::fabs(S1) <= eps_resid && std::fabs(step) <= eps_step) {
            all_blocks_converged = true;
            return z_next;
          }
        }
      }
      if (!step_ok) {
        z_next = 0.5 * (lo + hi);   // Newton rejected or S2 underflowed -> bisect
      }

      // Tighten the bracket using monotonicity of h_kappa (Prop 3.12)
      double S1_next, S2_next;
      eval(z_next, S1_next, S2_next);
      if (S1_next > 0) hi = z_next; else lo = z_next;
      z = z_next;
      if (hi - lo <= eps_step) {
        all_blocks_converged = true;
        return z;
        }
    }
    }

    all_blocks_converged = false;   // hit max_iter without meeting tolerance
    return z;
  }


  double solve_block_huber(const std::set<int>& inactive_set) const {
    struct BreakEvent {
      double pos;          // theta'_i - delta ou theta'_i + delta
      double w;
      double theta_prime;
      bool is_entry;       // true: entree dans M (U->M), false: sortie de M (M->D)
    };

    const int n_block = static_cast<int>(inactive_set.size());

    if (n_block == 0) {
      throw std::runtime_error("solve_block_huber: inactive_set empty");
    }

    std::vector<BreakEvent> events;
    events.reserve(2 * n_block);

    double S = 0.0;
    // double S_ir = 0.0;
    for (int idx : inactive_set) {
      const double w = weighted ? weights[idx] : 1.0;
      S += w;
      // S_ir += w * sol_ir[idx];
      events.push_back({xl[idx] - delta, w, xl[idx], true});
      events.push_back({xl[idx] + delta, w, xl[idx], false});
    }

    std::sort(events.begin(), events.end(),
              [](const BreakEvent& a, const BreakEvent& b) { return a.pos < b.pos; });

    double sigma_U = S , sigma_D = 0.0, sigma_M = 0.0, sigma_M_theta = 0.0;

    auto slope     = [&]() { return 2.0 * (sigma_M + eps * S); }; // fixed value S
    auto intercept = [&]() { return -2.0 * delta * (sigma_U - sigma_D) - 2.0 * sigma_M_theta; }; // fixed value  S_ir -2*eps* S_ir

    double theta_prev = events.front().pos - 1.0;
    double h_prev = intercept() + slope() * theta_prev;   // < 0 par construction

    for (const auto& ev : events) {
      const double s = slope();
      const double candidate = theta_prev - h_prev / s;

      if (candidate <= ev.pos) {
        return candidate;
      }

      h_prev = intercept() + slope() * ev.pos;
      theta_prev = ev.pos;

      if (ev.is_entry) {
        sigma_U -= ev.w;
        sigma_M += ev.w;
        sigma_M_theta += ev.w * ev.theta_prime;
      } else {
        sigma_M -= ev.w;
        sigma_M_theta -= ev.w * ev.theta_prime;
        sigma_D += ev.w;
      }
    }

    const double s = slope();
    return theta_prev - h_prev / s;
  }

  double get_x_bar(const set<int>& inactive_set) {
    double x_bar = 0.0;
    if (weighted) {
      double w_sum = 0.0;
      for (set<int>::iterator it = inactive_set.begin();it != inactive_set.end();it++) {
        x_bar = x_bar + weights[*it] * xl[*it];
        w_sum = w_sum + weights[*it];
      }
      x_bar = x_bar / w_sum;
    }
    else {
      for (set<int>::iterator it = inactive_set.begin();it != inactive_set.end();it++) {
        x_bar = x_bar + xl[*it];
      }
      x_bar = x_bar / inactive_set.size();
    }
    return x_bar;
  }


  void Compute_min_diff(double& min_value, int& min_pos, vector<double>& Ax) {
    min_value = 0.0;
    for (int i = 0;i < m;i++) {
      Ax[i] = -calibrated_xl[E[i][0]] + calibrated_xl[E[i][1]];
      if (Ax[i] < min_value) {
        min_value = Ax[i];
        min_pos = i;
      }
    }
  }

  set<int> get_violator_set(vector<double>& x) {
    set<int> violator_set;
    for (int i = 0;i < m;i++) {
      if (-x[E[i][0]] + x[E[i][1]] < -1e-9) {
        violator_set.insert(i);
      }
    }
    return violator_set;
  }

// delta_fi
  double partial_fi(int i, double x_i) {
    if (weighted) {
      return(weights[i] * (x_i - xl[i]));
    }
    else {
      return(x_i - xl[i]);
    }
  }

  double partial_rhoi(int i, double x_i) {
    double grad = std::tanh((x_i - xl[i]) / delta);
    if (weighted) {
      return weights[i] * grad;
    }
    return grad;
  }

  double partial_huber(int i, double x_i) {
    const double r = xl[i] - x_i;
    const double abs_r = std::fabs(r);

    // clip(r, -delta, delta), avec garde contre la division par zero
    const double clipped = (abs_r > 1e-12)
      ? std::min(1.0, delta / abs_r) * r
    : 0.0;

    const double grad = -2.0 * clipped + 2.0 * eps * x_i;

    return weighted ? weights[i] * grad : grad;
  }

  void updata_local_lamdba(vector<double>& big_local_lambda, vector<vector<int>>& El_big, vector<double>& small_local_lambda, vector<vector<int>>& El_small, int small_n) {
    int countEl;
    for (int ii = 0; ii < small_n;ii++) {
      countEl = 0;
      while (!(El_big[countEl][0] == El_small[ii][0] && El_big[countEl][1] == El_small[ii][1]))
      {
        countEl++;
      }
      big_local_lambda[countEl] = small_local_lambda[ii];
    }
  }

  vector<double> solve_local_KKT(set<int>& inactive_set, double& x_bar, vector<vector<int>>& El, Adjacency_list* Adjacency_obj) {
    int nl = inactive_set.size() - 1;
    vector<vector<double>> Al(nl, vector<double>(nl, 0.0));
    El.assign(nl, vector<int>(2, 0)); //initialization
    vector<double> bl(nl, 0.0);
    std::vector<int> temp_array(inactive_set.begin(), inactive_set.end());
    // for (int i = 0;i < nl;i++) {
    //   El[i] = new int[2];
    //   Al[i] = new double[nl];
    //   for (int j = 0;j < nl;j++) {
    //     Al[i][j] = 0;
    //   }
    //   bl[i] = 0;
    //   El[i][0] = 0;
    //   El[i][1] = 0;
    // }
    int count_El = 0;
    // vector<int> temp_array(nl + 1);
    // for (set<int>::iterator it = inactive_set.begin();it != inactive_set.end();it++) {
    //   temp_array[count_El] = int(*it);
    //   count_El++;
    // }
    vector<int> row_abs_sum(nl, 0);
    vector<int> unique_col_index(nl);
    // for (int i = 0;i < nl;i++) {
    //   row_abs_sum[i] = 0;
    // }
    count_El = 0;
    int ii, jj;
    for (int i = 0;i < inactive_set.size();i++) {
      for (int j = 0; j < inactive_set.size();j++) {
        if (i != j) {
          ii = temp_array[i];
          jj = temp_array[j];
          if (str_DAG.is_connected(ii, jj) && Adjacency_obj->is_directly_connected(ii, jj)) {
            El[count_El][0] = temp_array[i];
            El[count_El][1] = temp_array[j];
            if (i < nl) {
              Al[i][count_El] = 1;
              row_abs_sum[i] = row_abs_sum[i] + 1;
              unique_col_index[i] = count_El;
            }
            if (j < nl) {
              Al[j][count_El] = -1;
              row_abs_sum[j] = row_abs_sum[j] + 1;
              unique_col_index[j] = count_El;
            }
            //cout << El[count_El][0] << " " << El[count_El][1] << endl;
            count_El++;
          }
        }
      }
    }
    // delete[]temp_array;
    int count_eq = 0;
    for (set<int>::iterator it = inactive_set.begin();it != inactive_set.end();it++) {
      if (count_eq < nl) {
        if (loss == "quadratic") {
        bl[count_eq] = -partial_fi(*it, x_bar);
        }
        else if (loss == "smoothH") {
          bl[count_eq] = -partial_rhoi(*it, x_bar);
        }

        else if (loss == "huber") {
          bl[count_eq] = -partial_huber(*it, x_bar);
        }
        count_eq++;
      }
    }
    vector<double> local_lambda = solve_linear_system(Al, bl, nl);

    // for (int i = 0;i < nl;i++) {
    //   delete[]Al[i];
    // }
    // delete[]Al;
    // delete[]bl;
    // delete[]unique_col_index;

    return local_lambda;
  }

  vector<double> solve_linear_system(vector<vector<double>>& Al, vector<double>& bl, int& nl) {
    // double** Aug = new double* [nl];
    vector<vector<double>> Aug(nl, vector<double>(nl+1, 0.0));
    for (int i = 0;i < nl;i++) {
      // Aug[i] = new double[nl + 1]; # No need
      for (int j = 0;j < nl;j++) {
        Aug[i][j] = Al[i][j];
      }
      Aug[i][nl] = bl[i];
    }
    int k;
    double max, temp, l, s;
    for (int i1 = 0; i1 < nl - 1; i1++)
    {
      max = fabs(Aug[i1][i1]);
      k = i1;
      for (int i = i1; i < nl; i++)
      {
        if (max < fabs(Aug[i][i1]))
        {
          max = fabs(Aug[i][i1]);
          k = i;
        }
      }

      for (int j = i1; j < nl + 1; j++)
      {
        temp = Aug[i1][j];
        Aug[i1][j] = Aug[k][j];
        Aug[k][j] = temp;
      }

      for (k = i1 + 1; k < nl; k++)
      {
        l = -Aug[k][i1] / Aug[i1][i1];
        for (int j = i1; j < nl + 1; j++)
          Aug[k][j] = Aug[k][j] + l * Aug[i1][j];
      }
    }

    // double* x_star = new double[nl];
    vector<double> x_star(nl);
    x_star[nl - 1] = Aug[nl - 1][nl] / Aug[nl - 1][nl - 1];

    for (int i = nl - 2; i >= 0; i = i - 1)
    {
      s = 0;
      for (int j = i + 1; j < nl; j++)
        s = s + Aug[i][j] * x_star[j];
      x_star[i] = (Aug[i][nl] - s) / Aug[i][i];
    }

    // for (int i = 0;i < nl;i++) {
    //   delete[]Aug[i];
    // }
    // delete[]Aug;

    return x_star;
  }

  bool is_primal_feasible(set<int> inactive_set) {
    bool is_feasible = true;
    for (set<int>::iterator it = inactive_set.begin();it != inactive_set.end();it++) {
      for (set<int>::iterator it2 = inactive_set.begin();it2 != inactive_set.end();it2++) {
        if (str_DAG.is_connected(*it, *it2) && calibrated_xl[*it] - calibrated_xl[*it2] > 10e-9) {
          is_feasible = false;
        }
      }
    }
    return is_feasible;
  }

  bool is_primal_feasible(vector<vector<int>>& El, int nl, int& min_i, int& min_j) {//int**& El, int nl, int& min_i, int& min_j
    double min_value = 0.0;
    for (int i = 0; i < nl;i++) {
      if (-calibrated_xl[El[i][0]] + calibrated_xl[El[i][1]] < min_value) {
        min_value = -calibrated_xl[El[i][0]] + calibrated_xl[El[i][1]];
        min_i = El[i][0];
        min_j = El[i][1];
      }
    }
    return (min_value >= 0) ? true : false;
  }
};


// [[Rcpp::export]]
Rcpp::List solve_ordered_chain(NumericVector y_input, NumericVector w_input, double delta=1.0, std::string loss = "quadratic", double eps= 0.0) { //Rcpp::Nullable<NumericVector> sol_ir_input = R_NilValue

  int n = y_input.size();
  // Convert Rcpp vectors to standard C++ vectors
  vector<double> y = Rcpp::as<vector<double>>(y_input);
  vector<double> w = Rcpp::as<vector<double>>(w_input);

  orderedBlock BT(n, y, w, delta, loss, eps);

  BT.load_chain_isotonic_str();
  BT.solve_by_SBM();

  // vector<double> x_ordered(n);
  vector<double> x_calibrated(n);
  BT.get_x_calibrated(x_calibrated);
  // for(int i = 0; i < n; i++) {
  //   x_res[i] = x_ordered[i];
  // }
  // 2. Get the partitions
  vector<vector<int>> part_vec = BT.get_partitions();

  // 3. Convert C++ vector<vector> to R List
  Rcpp::List blocks(part_vec.size());
  for(int i = 0; i < part_vec.size(); i++) {
      // Each element of the list is an integer vector of indices
      blocks[i] = wrap(part_vec[i]) ;
  }

  // 4. Return everything in one tidy package
  return Rcpp::List::create(
    _["x"] = Rcpp::wrap(x_calibrated),
    _["blocks"] = blocks,
    _["nb_outer_loop"] = BT.get_outer_loop_num(),
    _["nb_inner_loop"] = BT.get_inner_loop_num(),
    _["converged"] =  BT.get_converged(),
    _["kantorovich"] = BT.get_kantorovich()
  );
  }



// [[Rcpp::export]]
Rcpp::List solve_ordered_binary_tree(NumericVector y_input, NumericVector w_input, double delta = 1.0, std::string loss = "quadratic", double eps = 0.0) {
  int n = y_input.size();
  // Convert Rcpp vectors to standard C++ vectors
  vector<double> y = Rcpp::as<vector<double>>(y_input);
  vector<double> w = Rcpp::as<vector<double>>(w_input);
  // vector<double> sol_ir;
  //
  // if (sol_ir_input.isNotNull()) {
  //   // L'utilisateur fournit sa propre origine T_P(theta')
  //   sol_ir = Rcpp::as<vector<double>>(NumericVector(sol_ir_input));
  //   if (static_cast<int>(sol_ir.size()) != n) {
  //     Rcpp::stop("sol_ir must have the same length as y (%d expected, %d given).",
  //                n, sol_ir.size());
  //   }}

  // double* y = new double[n];
  // double* w = new double[n];
  // for(int i = 0; i < n; i++) {
  //   y[i] = y_input[i];
  //   w[i] = w_input[i];
  // }
  orderedBlock BT(n, y, w, delta, loss, eps);
  BT.load_tree_isotonic_str();
  BT.solve_by_SBM();

  // double* x_ordered = new double[n];
  // NumericVector x_res(n);
  vector<double> x_calibrated(n);
  BT.get_x_calibrated(x_calibrated);
  // for(int i = 0; i < n; i++) {
  //   x_res[i] = x_ordered[i];
  // }
  // 2. Get the partitions
  vector<vector<int>> part_vec = BT.get_partitions();

  // 3. Convert C++ vector<vector> to R List
  Rcpp::List blocks(part_vec.size());
  for(int i = 0; i < part_vec.size(); i++) {
      // Each element of the list is an integer vector of indices
      blocks[i] = wrap(part_vec[i]) ;
  }

  // 4. Return everything in one tidy package
  return Rcpp::List::create(
    _["x"] = Rcpp::wrap(x_calibrated),
    _["blocks"] = blocks,
    _["nb_outer_loop"] = BT.get_outer_loop_num(),
    _["nb_inner_loop"] = BT.get_inner_loop_num(),
    _["converged"] =  BT.get_converged(),
    _["kantorovich"] = BT.get_kantorovich()
  );
  }

// [[Rcpp::export]]
Rcpp::List solve_ordered_2d_grid(NumericVector y_input, NumericVector w_input, double delta, std::string loss, double eps) {
  int n = y_input.size();
  // Convert Rcpp vectors to standard C++ vectors
  vector<double> y = Rcpp::as<vector<double>>(y_input);
  vector<double> w = Rcpp::as<vector<double>>(w_input);
  // vector<double> sol_ir;
  //
  // if (sol_ir_input.isNotNull()) {
  //   // L'utilisateur fournit sa propre origine T_P(theta')
  //   sol_ir = Rcpp::as<vector<double>>(NumericVector(sol_ir_input));
  //   if (static_cast<int>(sol_ir.size()) != n) {
  //     Rcpp::stop("sol_ir must have the same length as y (%d expected, %d given).",
  //                n, sol_ir.size());
  //   }}
  // for(int i = 0; i < n; i++) {
  //   y[i] = y_input[i];
  //   w[i] = w_input[i];
  // }
  orderedBlock BT(n, y, w, delta, loss, eps);
  BT.load_2D_isotonic_str();
  BT.solve_by_SBM();

  // double* x_ordered = new double[n];
  // NumericVector x_res(n);
  vector<double> x_calibrated(n);
  BT.get_x_calibrated(x_calibrated);
  // for(int i = 0; i < n; i++) {
  //   x_res[i] = x_ordered[i];
  // }
  // 2. Get the partitions
  vector<vector<int>> part_vec = BT.get_partitions();

  // 3. Convert C++ vector<vector> to R List
  Rcpp::List blocks(part_vec.size());
  for(int i = 0; i < part_vec.size(); i++) {
      // Each element of the list is an integer vector of indices
      blocks[i] = wrap(part_vec[i]) ;
  }

  // 4. Return everything in one tidy package
  return Rcpp::List::create(
    _["x"] = Rcpp::wrap(x_calibrated),
    _["blocks"] = blocks,
    _["nb_outer_loop"] = BT.get_outer_loop_num(),
    _["nb_inner_loop"] = BT.get_inner_loop_num(),
    _["converged"] =  BT.get_converged(),
    _["kantorovich"] = BT.get_kantorovich()
  );
  }

// [[Rcpp::export]]
Rcpp::List solve_ordered_arbitrary_DAG(NumericVector y_input, NumericVector w_input, NumericMatrix E_mat_input, double delta, std::string loss, double eps) {
  int n = y_input.size();
  // Convert Rcpp vectors to standard C++ vectors
  vector<double> y = Rcpp::as<vector<double>>(y_input);
  vector<double> w = Rcpp::as<vector<double>>(w_input);
  // vector<double> sol_ir;
  //
  // if (sol_ir_input.isNotNull()) {
  //   // L'utilisateur fournit sa propre origine T_P(theta')
  //   sol_ir = Rcpp::as<vector<double>>(NumericVector(sol_ir_input));
  //   if (static_cast<int>(sol_ir.size()) != n) {
  //     Rcpp::stop("sol_ir must have the same length as y (%d expected, %d given).",
  //                n, sol_ir.size());
  //   }}
  // for(int i = 0; i < n; i++) {
  //   y[i] = y_input[i];
  //   w[i] = w_input[i];
  // }
  orderedBlock BT(n, y, w, delta, loss, eps);

  int m = E_mat_input.nrow();
  vector<vector<int>> E_mat(m, vector<int>(2, 0));
  for(int i=0;i<m;i++) {
    // E_mat[i] = new int[2]; no need
    E_mat[i][0] = int(E_mat_input(i,0));
    E_mat[i][1] = int(E_mat_input(i,1));
    //cout << E_mat[i][0] << " " << E_mat[i][1] << endl;
  }
  //cout << m << endl;

  BT.load_isotonic_str_via_mats(E_mat);
  //BT->print_E();
  BT.solve_by_SBM();

  // double* x_ordered = new double[n];
  // NumericVector x_res(n);
  vector<double> temp_res(n);
  BT.get_x_calibrated(temp_res);
  // for(int i = 0; i < n; i++) {
  //   x_res[i] = x_ordered[i];
  // }
  // return(x_res);

  // 2. Get the partitions
  vector<vector<int>> part_vec = BT.get_partitions();

  // 3. Convert C++ vector<vector> to R List
  Rcpp::List blocks(part_vec.size());
  for(int i = 0; i < part_vec.size(); i++) {
      // Each element of the list is an integer vector of indices
      blocks[i] = wrap(part_vec[i]) ;
  }

  // 4. Return everything in one tidy package
  return Rcpp::List::create(
    _["x"] = Rcpp::wrap(temp_res),
    _["blocks"] = blocks,
    _["nb_outer_loop"] = BT.get_outer_loop_num(),
    _["nb_inner_loop"] = BT.get_inner_loop_num(),
    _["converged"] =  BT.get_converged(),
    _["kantorovich"] = BT.get_kantorovich()
  );

  }






