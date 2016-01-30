#include <systemc.h>
#include "two_adder.h"

int sc_main(int argc, char *argv[])
{
	sc_signal<bool> A0; 
	sc_signal<bool> A1; 
	sc_signal<bool> B0;
	sc_signal<bool> B1;  
	sc_signal<bool> CI; 
	sc_signal<bool> SUM0;
	sc_signal<bool> SUM1; 
	sc_signal<bool> CO;

	FA2 fa2("FA2");
	fa2.A0(A0);
	fa2.A1(A1);
	fa2.B0(B0);
	fa2.B1(B1);
	fa2.CI(CI);
	fa2.SUM0(SUM0);
	fa2.SUM1(SUM1);
	fa2.CO(CO);

	sc_initialize();   // initialize the simulation engine
	// create the file to store simulation results
	sc_trace_file *tf = sc_create_vcd_trace_file("trace2");
	// 4: specify the signals we’d like to record in the trace file
	sc_trace(tf, A0, "A0"); sc_trace(tf, B0, "B0");
	sc_trace(tf, A1, "A1"); sc_trace(tf, B1, "B1");
	sc_trace(tf, CI, "CI");
	sc_trace(tf, SUM0, "SUM0"); sc_trace(tf, SUM1, "SUM1"); sc_trace(tf, CO, "CO");
	// 5: put values on the input signals
	A0=0; A1=0; B0=0; B1=0; CI=0;
	for (int i=0;i<16;i++) {
		A0  = ((i & 0x8) != 0);    // value of A is the bit0 of i
		A1  = ((i & 0x4) != 0);    // value of B is the bit1 of i
		B0	= ((i & 0x2) != 0);
		B1 	= ((i & 0x1) != 0);
		sc_start(10, SC_NS);
		cout << A1 << A0 << " o " << B1 << B0 << " Cout : " << CO << " SUM0 : "
		<< SUM0 << " SUM1 : " << SUM1 << endl;
	}
	A0 = 0;
	A1 = 0;
	B0 = 0;
	B1 = 0;
	sc_start(10, SC_NS);

	sc_close_vcd_trace_file(tf);    // close file and we’re done
	return 0;
}
