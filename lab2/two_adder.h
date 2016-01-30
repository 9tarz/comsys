#include <systemc.h>
#include "adder.h"

SC_MODULE(FA2)
{
	sc_in<bool> A0;
	sc_in<bool> A1;    
	sc_in<bool> B0;
	sc_in<bool> B1;    
	sc_in<bool> CI; 
	sc_out<bool> SUM0;
	sc_out<bool> SUM1;
	sc_out<bool> CO;

	//sc_uint<2> A_t, B_t, SUM_t;
	sc_signal<bool> cout_0;
	//sc_signal<bool> a0, b0, a1, b1, sum0, sum1;

	FA FA0, FA1;

	SC_CTOR(FA2): 
	FA0("FA0"), FA1("FA1")
	{
		//A_t = A; B_t = B;
		//a0 = A_t[0]; a1 = A_t[1]; b0 = B_t[0]; b1 = B_t[1]; sum0 = SUM_t[0]; sum1 = SUM_t[1];
		//a0 = A[0]; a1 = A[1]; b0 = B[0]; b1 = B[1]; sum0 = SUM[0]; sum1 = SUM[1];
		FA0.A(A0); FA0.B(B0); FA0.CI(CI); FA0.CO(cout_0); FA0.SUM(SUM0);
		FA1.A(A1); FA1.B(B1); FA1.CI(cout_0); FA1.CO(CO); FA1.SUM(SUM1);
	} // End of Constructor
};
