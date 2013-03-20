-------------------------------------------------------------------------------
-- system_handelc_0_wrapper.vhd
-------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

library handelc_v1_00_a;
use handelc_v1_00_a.all;

entity system_handelc_0_wrapper is
  port (
    FSL_Clk : in std_logic;
    FSL_Rst : in std_logic;
    FSL_S_Clk : in std_logic;
    FSL_S_Read : out std_logic;
    FSL_S_Data : in std_logic_vector(0 to 31);
    FSL_S_Control : in std_logic;
    FSL_S_Exists : in std_logic;
    FSL_M_Clk : in std_logic;
    FSL_M_Write : out std_logic;
    FSL_M_Data : out std_logic_vector(0 to 31);
    FSL_M_Control : out std_logic;
    FSL_M_Full : in std_logic;
    UartTX : out std_logic;
    UartRX : in std_logic;
    dac_cs : out std_logic;
    spi_sck : out std_logic;
    spi_mosi : out std_logic;
    spi_miso : in std_logic;
    dac_clr : out std_logic
  );
end system_handelc_0_wrapper;

architecture STRUCTURE of system_handelc_0_wrapper is

  component handelc is
    port (
      FSL_Clk : in std_logic;
      FSL_Rst : in std_logic;
      FSL_S_Clk : in std_logic;
      FSL_S_Read : out std_logic;
      FSL_S_Data : in std_logic_vector(0 to 31);
      FSL_S_Control : in std_logic;
      FSL_S_Exists : in std_logic;
      FSL_M_Clk : in std_logic;
      FSL_M_Write : out std_logic;
      FSL_M_Data : out std_logic_vector(0 to 31);
      FSL_M_Control : out std_logic;
      FSL_M_Full : in std_logic;
      UartTX : out std_logic;
      UartRX : in std_logic;
      dac_cs : out std_logic;
      spi_sck : out std_logic;
      spi_mosi : out std_logic;
      spi_miso : in std_logic;
      dac_clr : out std_logic
    );
  end component;

begin

  handelc_0 : handelc
    port map (
      FSL_Clk => FSL_Clk,
      FSL_Rst => FSL_Rst,
      FSL_S_Clk => FSL_S_Clk,
      FSL_S_Read => FSL_S_Read,
      FSL_S_Data => FSL_S_Data,
      FSL_S_Control => FSL_S_Control,
      FSL_S_Exists => FSL_S_Exists,
      FSL_M_Clk => FSL_M_Clk,
      FSL_M_Write => FSL_M_Write,
      FSL_M_Data => FSL_M_Data,
      FSL_M_Control => FSL_M_Control,
      FSL_M_Full => FSL_M_Full,
      UartTX => UartTX,
      UartRX => UartRX,
      dac_cs => dac_cs,
      spi_sck => spi_sck,
      spi_mosi => spi_mosi,
      spi_miso => spi_miso,
      dac_clr => dac_clr
    );

end architecture STRUCTURE;

