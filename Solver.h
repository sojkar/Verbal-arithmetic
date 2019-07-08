/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Solver.h
 * Author: Radim Sojka
 *
 * Created on July 7, 2019, 11:56 PM
 */

#ifndef SOLVER_H
#define SOLVER_H

#include <minisat/core/Solver.h>
#include <vector>

using Words = std::vector<std::vector<int>>;
using Characters = std::vector<char>;
using Result = std::vector<int>;

class Solver {
public:  
  void setProblem(int chars, int digits, Words words);
  bool solve();
  Result getSolution(); 
  
private:
  Minisat::Solver solver;
  //create vars for specific number of characters, digits and orders
  void createVars();
  //add clauses defining addition rules
  void addAdditionRules(Words words);
  //add clauses ensuring that each character has only one value
  void oneCharOneDigit();
  void exactlyOne(Minisat::vec<Minisat::Lit> &literals);
  //add clauses ensuring that characters in notZero cannot be zero
  void notZeroRule();
    
  Minisat::Var charValueToVar(int charIndex, int digitIndex);
  Minisat::Var orderToVar(int order);
  
  int chars; //number of different characters in problem
  int digits; //specify numerical system 
  int orders; //max. order of magnitudes of 
  std::vector<int> notZero; //character which cannot be zero
};

#endif /* SOLVER_H */

