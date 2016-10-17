//
//  enum.c
//  
//
//  Created by Florent Jacquemard on 17/10/16.
//
// tmp test file
// (TBD)
//
// enumeration of all digit vectors

#include <stdio.h>



// just for the sample -- print the data sets
std::ostream& operator<<(std::ostream& os, const std::vector<int>& vi)
{
    os << "( ";
    std::copy(vi.begin(), vi.end(), std::ostream_iterator<int>(os, " "));
    os << ")\n";
    return os;
}

// m: vector of max value for each digit (i.e. digit result[i] must be in [0..m[i]])
// len: number of digits
void product(std::vector<int> m, size_t len)
{
    std::vector<int> result;
    
    // initial result = null vector
    for (int i = 0; i < len; i++)
        result.push_back(0);
    
    while (true)
    {
        std::cout << result;
        
        for (int i = 0; ; )
        {
            result[i]++;
            if (result[i] == m[i]) // we reach max value for i
            {
                if (i+1 == len) // last digit
                {
                    return;
                }
                else
                {
                    result[i] = 0;
                    i++;
                }
            }
            else
            {
                break;
            }
        }
    }
}


int main(int argc, const char * argv[])
{
    
    std::vector<int> bases = { 3, 3, 3 };
    
    product(bases, bases.size());
    
    return 0;
}
