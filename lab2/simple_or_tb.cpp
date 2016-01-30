#include <systemc.h>
#include "or.h"

int sc_main(int argc, char *argv[])
{
	sc_signal<bool> a;  // input pin a
	sc_signal<bool> b;  // input pin b
	sc_signal<bool> o; // output pin o

	OR2 or2("OR");
	or2.a(a);
	or2.b(b);
	or2.o(o);

	// Open VCD file
	sc_trace_file *wf = sc_create_vcd_trace_file("or2");
	sc_trace(wf, a, "a");
	sc_trace(wf, b, "b");
	sc_trace(wf, o, "o");

	a = 0;
	b = 0;
	sc_start(2,SC_NS);
	a = 0;
	b = 1;
	sc_start(2,SC_NS);
	a = 1;
	b = 0;
	sc_start(2,SC_NS);
	a = 1;
	b = 1;
	sc_start(2,SC_NS);
	a = 0;
	b = 0;
	sc_start(2,SC_NS);

	sc_close_vcd_trace_file(wf);
	return 0;
}
