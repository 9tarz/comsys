#include <systemc.h>
SC_MODULE(INV)
{
  sc_in<bool> a;  // input pin a
  sc_out<bool> o; // output pin o
  SC_CTOR(INV)    // the ctor
    {
      SC_METHOD(inv_process);
      sensitive << a;
    }
  void inv_process() {
    o.write( !a.read());
  }
};