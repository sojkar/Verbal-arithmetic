/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Solver.cpp
 * Author: radim
 * 
 * Created on July 7, 2019, 11:56 PM
 */

#include "Solver.h"

#include <iostream>

Solver::Solver() {
}

Solver::Solver(const Solver& orig) {
}

Solver::~Solver() {
}

void Solver::setProblem(int chars, Words words){
  //TODO : check input
  this->chars = chars;
  digits = 2;
  if(words[1].size() > words[0].size()){
    std::swap(words[0], words[1]);
  }
  orders = words[0].size();
  createVars();
  oneCharOneDigit();
  addAdditionRules(words);
  
  notZero = {words[0].back(),words[1].back()};
  notZeroRule();
}

void Solver::notZeroRule(){
  for(auto c : notZero){
    solver.addClause(Minisat::mkLit(charValueToVar(c,0),true));
  }
}

void Solver::addAdditionRules(Words words){
  
  //TODO - support for different sizes of words
  for(int o = 0; o < orders; o++){
    int c1 = words[0][o];
    int c2 = -1;  
    if(o < words[1].size()){
      c2 = words[1][o];
    } 
    int c3 = words[2][o];
    
    for(int v1 = 0; v1 < digits; v1++){
      for(int v2 = 0; v2 < digits; v2++){
        Minisat::vec<Minisat::Lit> literals00; // dr-1 = 0 & dr = 0
        Minisat::vec<Minisat::Lit> literals01;
        Minisat::vec<Minisat::Lit> literals10;
        Minisat::vec<Minisat::Lit> literals11;

        literals00.push(Minisat::mkLit(charValueToVar(c1,v1),true));
        literals01.push(Minisat::mkLit(charValueToVar(c1,v1),true));
        literals10.push(Minisat::mkLit(charValueToVar(c1,v1),true));
        literals11.push(Minisat::mkLit(charValueToVar(c1,v1),true));
        
        
        if(c2 > -1){
          literals00.push(Minisat::mkLit(charValueToVar(c2,v2),true));
          literals01.push(Minisat::mkLit(charValueToVar(c2,v2),true));
          literals10.push(Minisat::mkLit(charValueToVar(c2,v2),true));
          literals11.push(Minisat::mkLit(charValueToVar(c2,v2),true));
        }
        
        if(o > 0){
          literals00.push(Minisat::mkLit(tensToVar(o-1)));
          literals01.push(Minisat::mkLit(tensToVar(o-1)));
          
          literals10.push(Minisat::mkLit(tensToVar(o-1),true));
          literals11.push(Minisat::mkLit(tensToVar(o-1),true));
        }
        
        if(v1 + v2 < digits){
          literals00.push(Minisat::mkLit(charValueToVar(c3,v1 + v2)));
          literals01.push(Minisat::mkLit(tensToVar(o),true));
        }else{
          literals00.push(Minisat::mkLit(charValueToVar(c3,(v1 + v2) - digits)));
          literals01.push(Minisat::mkLit(tensToVar(o)));
        }
        
        if(v1 + v2 + 1 < digits){
          literals10.push(Minisat::mkLit(charValueToVar(c3,v1 + v2 + 1)));
          literals11.push(Minisat::mkLit(tensToVar(o),true));
        }else{
          literals10.push(Minisat::mkLit(charValueToVar(c3,(v1 + v2 + 1) - digits)));
          literals11.push(Minisat::mkLit(tensToVar(o)));
        }
        solver.addClause(literals00);
        solver.addClause(literals01);
        
        if(o > 0){
            solver.addClause(literals10);
            solver.addClause(literals11);
        }
      
        if(c2 == -1){
          break;
        }
      }      
    }
  }

  if(words[2].size() == orders){
    solver.addClause(Minisat::mkLit(tensToVar(orders-1),true));
  }else{
    solver.addClause(Minisat::mkLit(tensToVar(orders-1),true),Minisat::mkLit(charValueToVar(words[2][orders],1)));
    solver.addClause(Minisat::mkLit(tensToVar(orders-1)),Minisat::mkLit(charValueToVar(words[2][orders],0)));
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


Minisat::Var Solver::charValueToVar(int charIndex, int digitIndex){
  return charIndex * digits + digitIndex;
} 

Minisat::Var Solver::tensToVar(int order){
  return chars * digits + order;
} 

void Solver::createVars(){
  for(int c = 0; c < chars; c++){
    for(int d = 0; d < digits; d++){
      auto var = solver.newVar();
    }
  }
  for(int o = 0; o < orders; o++){
    auto var = solver.newVar();
  }
}

Result Solver::getSolution(){
  Result res(chars,-1);
  
  for(int c = 0; c < chars; c++){
    int found = 0;
    for(int d = 0; d < digits; d++){
      if(solver.modelValue(charValueToVar(c, d)).isTrue()){
        if(found > 0){
          //TODO: warning
        }
        found++;
        res[c] = d;
      }
    }
  }
  return res;  
} 
