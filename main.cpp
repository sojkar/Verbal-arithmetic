/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: Radim Sojka
 *
 * Created on July 6, 2019, 11:05 AM
 */

#include <cstdlib>
#include <iostream>
#include <algorithm>

#include "Solver.h"

using namespace std;

/**
 * Read verbal arithmetic problem from std::cin
 * 
 * The input must be three words on separate lines.
 * 
 * Example of input:
 * 
 * SEND
 * MORE
 * MONEY
 *
 */
void readProblem(Words &w, Characters &c){
  std::string words[3];
  
  for(int line = 0; line < 3; line++){
    std::getline(std::cin, words[line]);
    assert(words[line].size() > 0);
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

void printSolution(const Result &res,const Characters &c,const Words &words ){
  assert(c.size() == res.size());
  
  std::cout << std::endl << "Values of characters:" << std::endl;
  for(int i = 0; i < c.size(); i++){
    std::cout << c[i] << " = " << res[i] << std::endl;
  }
  
  std::cout << std::endl;
  
  int sumSize = words[2].size();
  for(auto word : words){
    for(int s = 0; s < sumSize-word.size(); s++){
      std::cout << " ";
    }
    for(auto i = word.rbegin(); i != word.rend(); ++i){
      std::cout << res[*i];
    }
    std::cout << std::endl;  
  }
}

/*
 * 
 */
int main(int argc, char** argv) {

  Characters chars; //indexing of characters
  Words words(3); 
  int numeralSystem = 10;
  
  readProblem(words, chars);
  
  Solver s;
  s.setProblem(chars.size(), numeralSystem, words);
  if(s.solve()){
    Result res = s.getSolution();
    printSolution(res, chars, words);
  }else{
    std::cout << "Solution did not found!" << std::endl;
  }
  
  return 0;
}

