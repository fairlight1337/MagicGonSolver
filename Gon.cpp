// Copyright (C) 2015 Jan Winkler <jan.winkler.84@gmail.com>
// 
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

/** \author Jan Winkler */


#include <iostream>
#include <vector>
#include <iomanip>


std::vector<std::vector<int>> checkGonPermutation(std::vector<int> vecPermutation) {
  int nArms = vecPermutation.size() / 2;
  
  int nBaseSum = vecPermutation[0] + vecPermutation[1] + vecPermutation[2];
  int nFormerEnd = vecPermutation[2];
  
  bool bCheckOK = true;
  for(int nI = 3; nI < vecPermutation.size(); nI += 2) {
    if(nI == vecPermutation.size() - 1) {
      if(vecPermutation[nI] + nFormerEnd + vecPermutation[1] != nBaseSum) {
	bCheckOK = false;
	break;
      }
    } else {
      if(vecPermutation[nI] + nFormerEnd + vecPermutation[nI + 1] != nBaseSum) {
	bCheckOK = false;
	break;
      } else {
	nFormerEnd = vecPermutation[nI + 1];
      }
    }
  }
  
  std::vector<std::vector<int>> vecResult;
  
  if(bCheckOK) {
    nFormerEnd = vecPermutation[2];
    
    vecResult.push_back({vecPermutation[0], vecPermutation[1], vecPermutation[2]});
    for(int nI = 3; nI < vecPermutation.size(); nI += 2) {
      if(nI == vecPermutation.size() - 1) {
	vecResult.push_back({vecPermutation[nI], nFormerEnd, vecPermutation[1]});
      } else {
	vecResult.push_back({vecPermutation[nI], nFormerEnd, vecPermutation[nI + 1]});
    	nFormerEnd = vecPermutation[nI + 1];
      }
    }
  }
  
  int nSmallestPrimary = -1;
  int nDisplacement = 0;
  for(int nI = 0; nI < vecResult.size(); ++nI) {
    std::vector<int> vecBlock = vecResult[nI];
    
    if(nSmallestPrimary == -1 || vecBlock[0] < nSmallestPrimary) {
      nSmallestPrimary = vecBlock[0];
      nDisplacement = nI;
    }
  }
  
  for(int nI = 0; nI < nDisplacement; ++nI) {
    std::vector<int> vecBlock = vecResult.front();
    vecResult.erase(vecResult.begin());
    vecResult.push_back(vecBlock);
  }
  
  return vecResult;
}


std::vector<std::vector<std::vector<int>>> gon(int nArms) {
  int nFreeVariables = 2 * nArms;
  std::vector<int> vecFree;
  
  for(int nI = 0; nI < nFreeVariables; ++nI) {
    vecFree.push_back(nI + 1);
  }
  
  std::vector<int> vecPermutor;
  vecPermutor.resize(nFreeVariables);
  
  std::vector<std::vector<std::vector<int>>> vecSolutions;
  bool bRun = true;
  while(bRun) {
    std::vector<int> vecPermutation;
    std::vector<int> vecFreeTemp = vecFree;
    for(int nI : vecPermutor) {
      vecPermutation.push_back(vecFreeTemp[nI]);
      vecFreeTemp.erase(vecFreeTemp.begin() + nI);
    }
    
    std::vector<std::vector<int>> vecCheckResult = checkGonPermutation(vecPermutation);
    
    if(vecCheckResult.size() > 0) {
      bool bAlreadyPresent = false;
      for(int nSolution = 0; nSolution < vecSolutions.size(); ++nSolution) {
	bool bSolutionSame = true;
      
	for(int nBlock = 0; nBlock < vecSolutions[nSolution].size(); ++nBlock) {
	  for(int nValue = 0; nValue < vecSolutions[nSolution][nBlock].size(); ++nValue) {
	    if(vecCheckResult[nBlock][nValue] != vecSolutions[nSolution][nBlock][nValue]) {
	      bSolutionSame = false;
	      nBlock = vecSolutions[nSolution].size();
	    
	      break;
	    }
	  }
	}
      
	if(bSolutionSame) {
	  bAlreadyPresent = true;
	  break;
	}
      }
    
      if(!bAlreadyPresent) {
	vecSolutions.push_back(vecCheckResult);
      }
    }
    
    vecPermutor[0]++;
    
    for(int nI = 0; nI < vecPermutor.size(); ++nI) {
      if(vecPermutor[nI] >= vecFree.size() - nI) {
	if(nI == vecPermutor.size() - 1) {
	  bRun = false;
	  break;
	} else {
	  vecPermutor[nI] = 0;
	  vecPermutor[nI + 1]++;
	}
      }
    }
  }
  
  return vecSolutions;
}


int main() {
  std::vector<std::vector<std::vector<int>>> vecSolutions = gon(5);
  
  for(std::vector<std::vector<int>> vecSolution : vecSolutions) {
    int nSum;
    
    std::cout << "Solution:";
    bool bFirst = true;
    
    for(std::vector<int> vecBlock : vecSolution) {
      nSum = 0;
      
      if(bFirst) {
	bFirst = false;
      } else {
	std::cout << ",";
      }
      
      for(int nValue : vecBlock) {
	nSum += nValue;
	std::cout << " " << std::setw(3) << nValue;
      }
    }
    
    std::cout << " (sum = " << nSum << ")" << std::endl;
  }
  
  return 0;
}
