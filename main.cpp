/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: radim
 *
 * Created on July 6, 2019, 11:05 AM
 */

#include <cstdlib>
#include <iostream>
#include <algorithm>

#include "Solver.h"

using namespace std;

void readProblem(Words &w, Characters &c ){
  std::string words[3];
  
  for(int line = 0; line < 3; line++){
    //TODO: check input
    std::getline(std::cin, words[line]);
  }
  
  int wordNum = 0;
  
  for( auto const &word : words){
    for( auto ch = word.rbegin(); ch != word.rend(); ++ch){
      auto i = distance(c.begin(), std::find(c.begin(),c.end(),*ch));
      if(i == c.size()){
        c.push_back(*ch);
      }
      w[wordNum].push_back(i);
    }
    wordNum++;
  }
}

void printSolution(Result &res, Characters &c ){
  
  for(int i = 0; i < c.size(); i++){
    //TODO: check vectors (size)
    std::cout << c[i] << " = " << res[i] << std::endl;
  }
}

/*
 * 
 */
int main(int argc, char** argv) {

  Characters chars;
  Words words(3);
  
  readProblem(words, chars);
  
  Solver s;
  s.setProblem(chars.size(), words);
  if(s.solve()){
    Result res = s.getSolution();
    printSolution(res, chars);
  }else{
    std::cout << "Solution did not found!" << std::endl;
  }
  
  return 0;
}

