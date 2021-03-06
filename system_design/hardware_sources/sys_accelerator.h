#ifndef __ACCEL__
#define __ACCEL__

#include "bus_master_if.h"
#include "magnitude_angle.h"
//#include "null_edge_detect.h"
//#include <mc_scverify.h>
//#pragma hls_design top

class sys_accelerator : public sc_module, public sysbus_axi {
 public:

    //== Ports 

    sc_in<bool>                           CCS_INIT_S1(clk);
    sc_in<bool>                           CCS_INIT_S1(rstn);

    r_master<>                            CCS_INIT_S1(r_master0);
    w_master<>                            CCS_INIT_S1(w_master0);

    Connections::SyncIn                   CCS_INIT_S1(start);
    Connections::SyncOut                  CCS_INIT_S1(done);

      //Accelerator configuration signals

    sc_in<ac_int<18, false>>              CCS_INIT_S1(input_offset);
    sc_in<ac_int<18, false>>              CCS_INIT_S1(output_offset);
    sc_in<ac_int<12, false>>              CCS_INIT_S1(height);
    sc_in<ac_int<12, false>>              CCS_INIT_S1(width);
  
    //== Local signals

    Connections::Combinational<ac_int<64, false>>  mem_in_data;
    Connections::Combinational<ac_int<64, false>>  mem_out_data;

    //== Instances

    bus_master_if                         CCS_INIT_S1(bus_if_inst);
    magnitude_angle                       CCS_INIT_S1(magnitude_angle_inst);

    //== Constructor

    SC_CTOR(sys_accelerator) {
        bus_if_inst.clk                       (clk);
        bus_if_inst.rstn                      (rstn);
        bus_if_inst.r_master0                 (r_master0);
        bus_if_inst.w_master0                 (w_master0);
        bus_if_inst.mem_in_data               (mem_in_data);
        bus_if_inst.mem_out_data              (mem_out_data);
        bus_if_inst.height                    (height);
        bus_if_inst.width                     (width);
        bus_if_inst.input_offset              (input_offset);
        bus_if_inst.output_offset             (output_offset);
        bus_if_inst.start                     (start);
        bus_if_inst.done                      (done);

        magnitude_angle_inst.clk              (clk);
        magnitude_angle_inst.rstn             (rstn);       
        magnitude_angle_inst.mem_in_data      (mem_in_data);
        magnitude_angle_inst.mem_out_data     (mem_out_data);
        magnitude_angle_inst.height           (height);
        magnitude_angle_inst.width            (width);
      
    }
};

#endif


