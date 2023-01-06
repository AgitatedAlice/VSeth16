#ifndef CASSERT_H
#define CASSERT_H

#define PASS 0
#define FAIL 1

int assertionCount = 0;

int assert(bool booleanExpression){
  ++assertionCount;
  printf("Assertion #%d ",assertionCount);
  if(booleanExpression){
   printf("\033[32mPassed\n\e[0m");
   return PASS;
  }
  else{
   printf("\033[33mFAILED\e[0m");
   exit(assertionCount);
  }
}

#endif
