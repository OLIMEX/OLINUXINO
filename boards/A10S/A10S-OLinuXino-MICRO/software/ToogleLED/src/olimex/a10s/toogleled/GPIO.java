package olimex.a10s.toogleled;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;



public class GPIO{
	
	public String port;
	public String ext;
	public int pin;	
	
	
	public boolean output(int value) {
		String command = String.format("echo %d > /sys/class/gpio_sw/%s/data", value, port);		
		try {
			Runtime.getRuntime().exec(new String[] {"su", "-c", command});
			return true;
		} catch (IOException e) {
			return false;
		}
	}
	public String readinput(){
		Process p;
		String command = String.format("cat /sys/class/gpio_sw/%s/data", port);
		try {
			p = Runtime.getRuntime().exec(new String[] {"su", "-c", command});
			BufferedReader reader = new BufferedReader(new InputStreamReader(p.getInputStream()));
			StringBuilder text = new StringBuilder();
			String line;
			while((line = reader.readLine()) != null){
				text.append(line);
				text.append("\n");
			}
			return text.toString();
		} catch (IOException e) {
			return "";
		}		
	}
	public boolean setcfg(int cfg){
		String command = String.format("echo %d > /sys/class/gpio_sw/%s/mul_sel", cfg, port);		
		try {
			Runtime.getRuntime().exec(new String[] {"su", "-c", command});
			return true;
		} catch (IOException e) {
			return false;
		}
	}
	public String readcfg(){
		Process p;
		String command = String.format("cat /sys/class/gpio_sw/%s/mul_sel", port);
		try {
			p = Runtime.getRuntime().exec(new String[] {"su", "-c", command});
			BufferedReader reader = new BufferedReader(new InputStreamReader(p.getInputStream()));
			StringBuilder text = new StringBuilder();
			String line;
			while((line = reader.readLine()) != null){
				text.append(line);
				text.append("\n");
			}
			return text.toString();
		} catch (IOException e) {
			return "";
		}
	}
	public int input(){		
		char ch;
		String cfg;
		
		cfg = readinput();
		
		if(cfg.isEmpty())
			return -1;
		else{
			ch = cfg.charAt(0);		
			if(Character.isDigit(ch))
				return Character.getNumericValue(ch);			
			else
				return -1;
		}	
	}
	
	public int getcfg(){
		char ch;
		String cfg;
		
		cfg = readcfg();
		if(cfg.isEmpty())
			return -1;
		else{
			ch = cfg.charAt(0);
			if(Character.isDigit(ch))
				return Character.getNumericValue(ch);			
			else
				return -1;
		}
	}
	
	
	//Constructor
	public GPIO(String port, String ext, int pin){
		this.port = port;
		this.ext = ext;
		this.pin = pin;
	}
		
}
