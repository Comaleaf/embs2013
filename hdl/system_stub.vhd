-------------------------------------------------------------------------------
-- system_stub.vhd
-------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity system_stub is
  port (
    fpga_0_RS232_DCE_RX_pin : in std_logic;
    fpga_0_RS232_DCE_TX_pin : out std_logic;
    fpga_0_LEDs_8Bit_GPIO_IO_O_pin : out std_logic_vector(0 to 7);
    fpga_0_DIP_Switches_4Bit_GPIO_IO_I_pin : in std_logic_vector(0 to 3);
    fpga_0_Buttons_4Bit_GPIO_IO_I_pin : in std_logic_vector(0 to 4);
    fpga_0_Ethernet_MAC_PHY_tx_clk_pin : in std_logic;
    fpga_0_Ethernet_MAC_PHY_rx_clk_pin : in std_logic;
    fpga_0_Ethernet_MAC_PHY_crs_pin : in std_logic;
    fpga_0_Ethernet_MAC_PHY_dv_pin : in std_logic;
    fpga_0_Ethernet_MAC_PHY_rx_data_pin : in std_logic_vector(3 downto 0);
    fpga_0_Ethernet_MAC_PHY_col_pin : in std_logic;
    fpga_0_Ethernet_MAC_PHY_rx_er_pin : in std_logic;
    fpga_0_Ethernet_MAC_PHY_tx_en_pin : out std_logic;
    fpga_0_Ethernet_MAC_PHY_tx_data_pin : out std_logic_vector(3 downto 0);
    fpga_0_Ethernet_MAC_PHY_MDC_pin : out std_logic;
    fpga_0_Ethernet_MAC_PHY_MDIO_pin : inout std_logic;
    fpga_0_clk_1_sys_clk_pin : in std_logic;
    fpga_0_rst_1_sys_rst_pin : in std_logic;
    handelc_0_UartTX_pin : out std_logic;
    handelc_0_UartRX_pin : in std_logic;
    fpga_0_amp_cs_pin : out std_logic;
    fpga_0_amp_shdn_pin : out std_logic;
    fpga_0_spi_ss_b_pin : out std_logic;
    fpga_0_sf_ce0_pin : out std_logic;
    fpga_0_fpga_init_b_pin : out std_logic;
    fpga_0_ad_conv_pin : out std_logic;
    fpga_0_sf_oe_pin : out std_logic;
    fpga_0_sf_we_pin : out std_logic;
    fpga_0_dac_cs_pin : out std_logic;
    fpga_0_spi_sck_pin : out std_logic;
    fpga_0_spi_mosi_pin : out std_logic;
    fpga_0_spi_miso_pin : in std_logic;
    fpga_0_dac_clr_pin : out std_logic
  );
end system_stub;

architecture STRUCTURE of system_stub is

  component system is
    port (
      fpga_0_RS232_DCE_RX_pin : in std_logic;
      fpga_0_RS232_DCE_TX_pin : out std_logic;
      fpga_0_LEDs_8Bit_GPIO_IO_O_pin : out std_logic_vector(0 to 7);
      fpga_0_DIP_Switches_4Bit_GPIO_IO_I_pin : in std_logic_vector(0 to 3);
      fpga_0_Buttons_4Bit_GPIO_IO_I_pin : in std_logic_vector(0 to 4);
      fpga_0_Ethernet_MAC_PHY_tx_clk_pin : in std_logic;
      fpga_0_Ethernet_MAC_PHY_rx_clk_pin : in std_logic;
      fpga_0_Ethernet_MAC_PHY_crs_pin : in std_logic;
      fpga_0_Ethernet_MAC_PHY_dv_pin : in std_logic;
      fpga_0_Ethernet_MAC_PHY_rx_data_pin : in std_logic_vector(3 downto 0);
      fpga_0_Ethernet_MAC_PHY_col_pin : in std_logic;
      fpga_0_Ethernet_MAC_PHY_rx_er_pin : in std_logic;
      fpga_0_Ethernet_MAC_PHY_tx_en_pin : out std_logic;
      fpga_0_Ethernet_MAC_PHY_tx_data_pin : out std_logic_vector(3 downto 0);
      fpga_0_Ethernet_MAC_PHY_MDC_pin : out std_logic;
      fpga_0_Ethernet_MAC_PHY_MDIO_pin : inout std_logic;
      fpga_0_clk_1_sys_clk_pin : in std_logic;
      fpga_0_rst_1_sys_rst_pin : in std_logic;
      handelc_0_UartTX_pin : out std_logic;
      handelc_0_UartRX_pin : in std_logic;
      fpga_0_amp_cs_pin : out std_logic;
      fpga_0_amp_shdn_pin : out std_logic;
      fpga_0_spi_ss_b_pin : out std_logic;
      fpga_0_sf_ce0_pin : out std_logic;
      fpga_0_fpga_init_b_pin : out std_logic;
      fpga_0_ad_conv_pin : out std_logic;
      fpga_0_sf_oe_pin : out std_logic;
      fpga_0_sf_we_pin : out std_logic;
      fpga_0_dac_cs_pin : out std_logic;
      fpga_0_spi_sck_pin : out std_logic;
      fpga_0_spi_mosi_pin : out std_logic;
      fpga_0_spi_miso_pin : in std_logic;
      fpga_0_dac_clr_pin : out std_logic
    );
  end component;

  attribute BUFFER_TYPE : STRING;
  attribute BOX_TYPE : STRING;
  attribute BUFFER_TYPE of fpga_0_Ethernet_MAC_PHY_tx_clk_pin : signal is "IBUF";
  attribute BUFFER_TYPE of fpga_0_Ethernet_MAC_PHY_rx_clk_pin : signal is "IBUF";
  attribute BOX_TYPE of system : component is "user_black_box";

begin

  system_i : system
    port map (
      fpga_0_RS232_DCE_RX_pin => fpga_0_RS232_DCE_RX_pin,
      fpga_0_RS232_DCE_TX_pin => fpga_0_RS232_DCE_TX_pin,
      fpga_0_LEDs_8Bit_GPIO_IO_O_pin => fpga_0_LEDs_8Bit_GPIO_IO_O_pin,
      fpga_0_DIP_Switches_4Bit_GPIO_IO_I_pin => fpga_0_DIP_Switches_4Bit_GPIO_IO_I_pin,
      fpga_0_Buttons_4Bit_GPIO_IO_I_pin => fpga_0_Buttons_4Bit_GPIO_IO_I_pin,
      fpga_0_Ethernet_MAC_PHY_tx_clk_pin => fpga_0_Ethernet_MAC_PHY_tx_clk_pin,
      fpga_0_Ethernet_MAC_PHY_rx_clk_pin => fpga_0_Ethernet_MAC_PHY_rx_clk_pin,
      fpga_0_Ethernet_MAC_PHY_crs_pin => fpga_0_Ethernet_MAC_PHY_crs_pin,
      fpga_0_Ethernet_MAC_PHY_dv_pin => fpga_0_Ethernet_MAC_PHY_dv_pin,
      fpga_0_Ethernet_MAC_PHY_rx_data_pin => fpga_0_Ethernet_MAC_PHY_rx_data_pin,
      fpga_0_Ethernet_MAC_PHY_col_pin => fpga_0_Ethernet_MAC_PHY_col_pin,
      fpga_0_Ethernet_MAC_PHY_rx_er_pin => fpga_0_Ethernet_MAC_PHY_rx_er_pin,
      fpga_0_Ethernet_MAC_PHY_tx_en_pin => fpga_0_Ethernet_MAC_PHY_tx_en_pin,
      fpga_0_Ethernet_MAC_PHY_tx_data_pin => fpga_0_Ethernet_MAC_PHY_tx_data_pin,
      fpga_0_Ethernet_MAC_PHY_MDC_pin => fpga_0_Ethernet_MAC_PHY_MDC_pin,
      fpga_0_Ethernet_MAC_PHY_MDIO_pin => fpga_0_Ethernet_MAC_PHY_MDIO_pin,
      fpga_0_clk_1_sys_clk_pin => fpga_0_clk_1_sys_clk_pin,
      fpga_0_rst_1_sys_rst_pin => fpga_0_rst_1_sys_rst_pin,
      handelc_0_UartTX_pin => handelc_0_UartTX_pin,
      handelc_0_UartRX_pin => handelc_0_UartRX_pin,
      fpga_0_amp_cs_pin => fpga_0_amp_cs_pin,
      fpga_0_amp_shdn_pin => fpga_0_amp_shdn_pin,
      fpga_0_spi_ss_b_pin => fpga_0_spi_ss_b_pin,
      fpga_0_sf_ce0_pin => fpga_0_sf_ce0_pin,
      fpga_0_fpga_init_b_pin => fpga_0_fpga_init_b_pin,
      fpga_0_ad_conv_pin => fpga_0_ad_conv_pin,
      fpga_0_sf_oe_pin => fpga_0_sf_oe_pin,
      fpga_0_sf_we_pin => fpga_0_sf_we_pin,
      fpga_0_dac_cs_pin => fpga_0_dac_cs_pin,
      fpga_0_spi_sck_pin => fpga_0_spi_sck_pin,
      fpga_0_spi_mosi_pin => fpga_0_spi_mosi_pin,
      fpga_0_spi_miso_pin => fpga_0_spi_miso_pin,
      fpga_0_dac_clr_pin => fpga_0_dac_clr_pin
    );

end architecture STRUCTURE;

