library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity handelc is
	port 
	(
		FSL_Clk	: in	std_logic;
		FSL_Rst	: in	std_logic;
		FSL_S_Clk	: in	std_logic;
		FSL_S_Read	: out	std_logic;
		FSL_S_Data	: in	std_logic_vector(0 to 31);
		FSL_S_Control	: in	std_logic;
		FSL_S_Exists	: in	std_logic;
		FSL_M_Clk	: in	std_logic;
		FSL_M_Write	: out	std_logic;
		FSL_M_Data	: out	std_logic_vector(0 to 31);
		FSL_M_Control	: out	std_logic;
		FSL_M_Full	: in	std_logic;

		UartTX : out std_logic;
		UartRX : in std_logic;

		dac_cs : out std_logic;
		spi_sck : out std_logic;
		spi_mosi : out std_logic; 
		spi_miso : in std_logic; 
		dac_clr : out std_logic
	);

attribute SIGIS : string; 
attribute SIGIS of FSL_Clk : signal is "Clk"; 
attribute SIGIS of FSL_S_Clk : signal is "Clk"; 
attribute SIGIS of FSL_M_Clk : signal is "Clk"; 

end handelc;

architecture WRAPPER of handelc is

	component userhandelc is port (
		FSL_Clk : in std_logic;
		FSL_Rst : in std_logic;
		FSL_M_Clk : in std_logic;
		FSL_M_Data : out std_logic_vector(31 downto 0);
		FSL_M_Write : out std_logic;
		FSL_M_Control : out std_logic;
		FSL_M_Full : in std_logic;
		FSL_S_Clk : in std_logic;
		FSL_S_Data : in std_logic_vector(31 downto 0);
		FSL_S_Read : out std_logic;
		FSL_S_Control : in std_logic;	
		FSL_S_Exists : in std_logic;
		UartRX : in std_logic;
		UartTX : out std_logic;
		
		spi_mosi : out std_logic;
		spi_miso : in std_logic;
		spi_sck : out std_logic;
		dac_cs : out std_logic;
		dac_clr : out std_logic
	);
	end component;

	attribute BOX_TYPE : STRING;
	attribute BOX_TYPE of userhandelc : component is "user_black_box";

begin

	hc : userhandelc port map (
		FSL_Clk => FSL_Clk,
		FSL_Rst => FSL_Rst,
		FSL_M_Clk => FSL_M_Clk,
		FSL_M_Data => FSL_M_Data, 
		FSL_M_Write => FSL_M_Write,
		FSL_M_Control => FSL_M_Control,
		FSL_M_Full => FSL_M_Full,
		FSL_S_Clk => FSL_S_Clk,
		FSL_S_Data => FSL_S_Data,
		FSL_S_Read => FSL_S_Read,
		FSL_S_Control => FSL_S_Control,
		FSL_S_Exists => FSL_S_Exists,
		UartRX => UartRX,
		UartTX => UartTX,
		spi_mosi => spi_mosi,
		spi_miso => spi_miso,
		spi_sck => spi_sck,
		dac_cs => dac_cs,
		dac_clr => dac_clr
	);
	
end architecture WRAPPER;

