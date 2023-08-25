// DS PROJECT.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include"Scheduler.h"
int main()
{
    Scheduler s;
    s.advance();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
/*
1   31   20   0   
1   32   15   2   (3,7)  ,  (10,3) 
1   33   10   0
1   34   9    1   (6,2)
1   35   5    0

1   36   5    0
1   37   20   2   (1,5)  ,   (10,5)
1   38   15   2   (3,7)  ,  (10,3) 
1   39   10   0
1   40   9    1   (6,2)

1  41   5    0
1  42   20   2   (1,5)  ,   (10,5)
1  43   15   2   (3,7)  ,  (10,3) 
1  44   10   0
1  45   9    1   (6,2)

1   46   5    0
1   47   20   2   (1,5)  ,   (10,5)
1   48   15   2   (3,7)  ,  (10,3) 
1   49   10   0
1   50   9    1   (6,2)

1   51    5    0
1   52   20   2   (1,5)  ,   (10,5)
1   53   15   2   (3,7)  ,  (10,3) 
1   54   10   0
1   55   9    1   (6,2)

1   56   5    0
1   57   20   2   (1,5)  ,   (10,5)
1   58   15   2   (3,7)  ,  (10,3) 
1   59   10   0
1   60   9    1   (6,2)*/