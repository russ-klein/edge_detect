

// defines needed for the rocket core

`define m_bits         4   
`define id_bits        4
`define wid_bits       4
`define rid_bits       4
`define addr_bits     32
`define len_bits       8
`define data_bits     64
`define strb_bits      (`data_bits/8)
`define size_bits      3
`define burst_bits     2
`define lock_bits      1
`define cache_bits     4
`define prot_bits      3
`define resp_bits      2
`define last_bits      1
`define ruser_bits     7
`define wuser_bits     9
`define qos_bits       4

// defines needed by matchlib axi

`define d_m_bits       0   
`define d_id_bits      4
`define d_wid_bits     4
`define d_rid_bits     4
`define d_reg_bits     0
`define d_addr_bits   32
`define d_len_bits     8
`define d_data_bits   64
`define d_strb_bits    (`d_data_bits/8)
`define d_size_bits    3
`define d_burst_bits   2
`define d_lock_bits    0
`define d_cache_bits   0
`define d_prot_bits    0
`define d_resp_bits    2
`define d_last_bits    1
`define d_ruser_bits   0
`define d_buser_bits   0
`define d_wuser_bits   0
`define d_aruser_bits  0
`define d_auser_bits   0
`define d_awuser_bits  0
`define d_qos_bits     0


`define aw_msg_size    (`d_wid_bits   +   \
                        `d_addr_bits  +   \
                        `d_burst_bits +   \
                        `d_len_bits   +   \
                        `d_size_bits  +   \
                        `d_cache_bits +   \
                        `d_qos_bits   +   \
                        `d_lock_bits  +   \
                        `d_prot_bits  +   \
                        `d_reg_bits   +   \
                        `d_awuser_bits)

