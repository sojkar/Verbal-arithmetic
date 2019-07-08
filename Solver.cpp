/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Solver.cpp
 * Author: Radim Sojka
 * 
 * Created on July 7, 2019, 11:56 PM
 */

#include "Solver.h"

#include <iostream>

Minisat::Var Solver::charValueToVar(int charIndex, int digitIndex){
  return charIndex * digits + digitIndex;
} 

Minisat::Var Solver::orderToVar(int order){
  return chars * digits + order;
} 

void Solver::setProblem(int chars, int digits, Words words){
  assert(chars > 0);
  assert(digits > 0);
  assert(words.size() == 3);
  
  this->chars = chars;
  this->digits = digits;
  
  //ensure that longer word is the first - important for creating addition rules 
  if(words[1].size() > words[0].size()){
    std::swap(words[0], words[1]);
  }
  orders = words[0].size();
  createVars();
  oneCharOneDigit();
  addAdditionRules(words);
  
  //enforcing non-trivial solution - some variable cannot be zero
  notZero = {words[0].back(),words[1].back()};
  notZeroRule();
}

void Solver::createVars(){
  for(int c = 0; c < chars; c++){
    for(int d = 0; d < digits; d++){
      auto var = solver.newVar();
    }
  }
  //create var which says if sum in order o is greater than 9 (in decimal system)
  for(int o = 0; o < orders; o++){
    auto var = solver.newVar();
  }
}

void Solver::notZeroRule(){
  for(auto c : notZero){
    solver.addClause(Minisat::mkLit(charValueToVar(c,0),true));
  }
}

void Solver::addAdditionRules(Words words){
  
  //for each order
  for(int o = 0; o < orders; o++){
    int c1 = words[0][o];
    int c2 = -1;  
    if(o < words[1].size()){
      c2 = words[1][o];
    } 
    int c3 = words[2][o];
    
    for(int v1 = 0; v1 < digits; v1++){
      for(int v2 = 0; v2 < digits; v2++){
        //Literals for 4 cases
        Minisat::vec<Minisat::Lit> literals00; //one is NOT added from previous order, one is NOT going to following order
        Minisat::vec<Minisat::Lit> literals01; //one is added from previous order, one is NOT going to following order
        Minisat::vec<Minisat::Lit> literals10; //one is NOT added from previous order, one is going to following order
        Minisat::vec<Minisat::Lit> literals11; //one is added from previous order and one is going to following order

        literals00.push(Minisat::mkLit(charValueToVar(c1,v1),true));
        literals01.push(Minisat::mkLit(charValueToVar(c1,v1),true));
        literals10.push(Minisat::mkLit(charValueToVar(c1,v1),true));
        literals11.push(Minisat::mkLit(charValueToVar(c1,v1),true));
        
        //if second has less than 'o' characters, literals will not be added
        if(c2 > -1){ 
          literals00.push(Minisat::mkLit(charValueToVar(c2,v2),true));
          literals01.push(Minisat::mkLit(charValueToVar(c2,v2),true));
          literals10.push(Minisat::mkLit(charValueToVar(c2,v2),true));
          literals11.push(Minisat::mkLit(charValueToVar(c2,v2),true));
        }
        
        //if we are not in first order, one from previous order has to be taken in account
        if(o > 0){
          literals00.push(Minisat::mkLit(orderToVar(o-1)));
          literals01.push(Minisat::mkLit(orderToVar(o-1)));
          
          literals10.push(Minisat::mkLit(orderToVar(o-1),true));
          literals11.push(Minisat::mkLit(orderToVar(o-1),true));
        }
        
        if(v1 + v2 < digits){
          literals00.push(Minisat::mkLit(charValueToVar(c3,v1 + v2)));
          literals01.push(Minisat::mkLit(orderToVar(o),true));
        }else{
          literals00.push(Minisat::mkLit(charValueToVar(c3,(v1 + v2) - digits)));
          literals01.push(Minisat::mkLit(orderToVar(o)));
        }
        
        if(v1 + v2 + 1 < digits){
          literals10.push(Minisat::mkLit(charValueToVar(c3,v1 + v2 + 1)));
          literals11.push(Minisat::mkLit(orderToVar(o),true));
        }else{
          literals10.push(Minisat::mkLit(charValueToVar(c3,(v1 + v2 + 1) - digits)));
          literals11.push(Minisat::mkLit(orderToVar(o)));
        }
        solver.addClause(literals00);
        solver.addClause(literals01);
        
        if(o > 0){
            solver.addClause(literals10);
            solver.addClause(literals11);
        }
      
        //if second has less than 'o' characters, clauses will be added only for v2=0
        if(c2 == -1){
          break;
        }
      }      
    }
  }

  //clauses ensure correct value of higher order of sum
  if(words[2].size() == orders){
    solver.addClause(Minisat::mkLit(orderToVar(orders-1),true));
  }else{
    solver.addClause(Minisat::mkLit(orderToVar(orders-1),true),Minisat::mkLit(charValueToVar(words[2][orders],1)));
    solver.addClause(Minisat::mkLit(orderToVar(orders-1)),Minisat::mkLit(charValueToVar(words[2][orders],0)));
  }
}

void Solver::oneCharOneDigit(){
  for(int c = 0; c < chars; c++){
    Minisat::vec<Minisat::Lit> literals;
    for(int d = 0; d < digits; d++){
      literals.push(Minisat::mkLit(charValueToVar(c,d)));  
    }
    exactlyOne(literals);
  } 
}

void Solver::exactlyOne(Minisat::vec<Minisat::Lit> &literals) {
  solver.addClause(literals);
  for (size_t i = 0; i < literals.size(); i++){
    for (size_t j = i + 1; j < literals.size(); j++){
      solver.addClause(~literals[i], ~literals[j]);
    }
  }
}

bool Solver::solve(){
  solver.solve();
} 

Result Solver::getSolution(){
  Result res(chars,-1);
  
  for(int c = 0; c < chars; c++){
    for(int d = 0; d < digits; d++){
      if(solver.modelValue(charValueToVar(c, d)).isTrue()){
        res[c] = d;
      }
    }
  }
  return res;  
} 
