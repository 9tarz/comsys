#include <systemc.h>
SC_MODULE(NOR2)
{
  sc_in<bool> a;  // input pin a
  sc_in<bool> b;  // input pin b
  sc_out<bool> o; // output pin o
  SC_CTOR(NOR2)    // the ctor
    {
      SC_METHOD(nor_process);
      sensitive << a << b;
    }
  void nor_process() {
    o.write( !(a.read() || b.read()) );
  }
};