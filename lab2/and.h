#include <systemc.h>
SC_MODULE(AND2)
{
  sc_in<bool> a;  // input pin a
  sc_in<bool> b;  // input pin b
  sc_out<bool> o; // output pin o
  SC_CTOR(AND2)    // the ctor
    {
      SC_METHOD(and_process);
      sensitive << a << b;
    }
  void and_process() {
    o.write( a.read() && b.read() );
  }
};


SC_MODULE(AND3)
{
  sc_in<bool> a;  // input pin a
  sc_in<bool> b;  // input pin b
  sc_in<bool> c;  // input pin b
  sc_out<bool> o; // output pin o
  SC_CTOR(AND3)    // the ctor
    {
      SC_METHOD(and_process);
      sensitive << a << b;
    }
  void and_process() {
    o.write( (a.read() && b.read()) && c.read() );
  }
};