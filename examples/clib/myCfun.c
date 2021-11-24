typedef struct {
    unsigned int Val1 ;
    unsigned int Val2 ;
} myStruct;
    


unsigned int myAdd(unsigned int A, unsigned int B)
{
 return A+B;   
}

void myAdd_ptr(unsigned int n, unsigned int* A, unsigned int* B, unsigned int* C)
{    
    unsigned int i;
    for(i=0;i<n;i++)
        C[i] = A[i]+B[i];    
}

myStruct myFct(unsigned int A, unsigned int B)
{   
    myStruct S;
    
    S.Val1 = A+B;
    S.Val2 = A*B;
    
    return S;
}

