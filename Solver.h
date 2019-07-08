/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Solver.h
 * Author: radim
 *
 * Created on July 7, 2019, 11:56 PM
 */

#ifndef SOLVER_H
#define SOLVER_H

#include <minisat/core/Solver.h>

using Words = std::vector<std::vector<int>>;
using Characters = std::vector<char>;
using Result = std::vector<int>;

class Solver {
public:
  Solver();
  Solver(const Solver& orig);
  virtual ~Solver();
  
  void setProblem(int chars, Words words);
  bool solve();
  Result getSolution(); 
  
private:
  Minisat::Solver solver;
  void createVars();
  Minisat::Var charValueToVar(int charIndex, int digitIndex);
  Minisat::Var tensToVar(int order);
  void addAdditionRules(Words words);
  void oneCharOneDigit();
  void exactlyOne(Minisat::vec<Minisat::Lit> &literals);
  
  int chars;
  int digits;
  int orders;
  
  //Words words;
  //int chars;
  
};

#endif /* SOLVER_H */

