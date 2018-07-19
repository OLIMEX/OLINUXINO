package olimex.a20.tools;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

public class Pins {

	private int number;
	public String label;
	private String directory;
	private String export;
	private String direction;
	private String level;

	public Pins(String name, int number) {
		this.label = name;
		this.number = number;
		this.directory = "/sys/class/gpio/gpio" + String.valueOf(number);
		this.export = "/sys/class/gpio/export";
		this.direction = this.directory + "/direction";
		this.level = this.directory + "/value";

	}

	public int exportPin() {
		try {
			Process process = Runtime.getRuntime().exec(
					new String[] {
							"su",
							"-c",
							"echo " + String.valueOf(this.number) + " > "
									+ this.export });
			process.waitFor();
			if (process.exitValue() == 0)
				return 0;
		} catch (IOException e) {
			e.printStackTrace();
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		return -1;
	}

	public void setDirection(String direction) {
		try {
			Process process = Runtime.getRuntime().exec(
					new String[] { "su", "-c",
							"echo " + direction + " > " + this.direction });
			process.waitFor();
		} catch (IOException e) {
			e.printStackTrace();
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
	}

	public void setLevel(int level) {
		try {
			Process process = Runtime.getRuntime().exec(
					new String[] {
							"su",
							"-c",
							"echo " + String.valueOf(level) + " > "
									+ this.level });
			process.waitFor();
		} catch (IOException e) {
			e.printStackTrace();
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
	}

	public int getStatus() {
		try {
			Process process = Runtime.getRuntime().exec(
					new String[] { "su", "-c", "cat " + this.level });

			process.waitFor();
			BufferedReader reader = new BufferedReader(new InputStreamReader(
					process.getInputStream()));
			StringBuilder text = new StringBuilder();
			String line;
			while ((line = reader.readLine()) != null) {
				text.append(line);
			}
			if (text.toString().isEmpty())
				return -1;
			char ch = text.charAt(0);
			if (Character.isDigit(ch))
				return Character.getNumericValue(ch);
			else
				return -1;
		} catch (IOException e) {
			e.printStackTrace();
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		return -1;
	}

	public String getDirection() {
		try {
			Process process = Runtime.getRuntime().exec(
					new String[] {
							"su",
							"-c",
							"cat " + this.direction });

			process.waitFor();
			BufferedReader reader = new BufferedReader(new InputStreamReader(
					process.getInputStream()));
			StringBuilder text = new StringBuilder();
			String line;
			while ((line = reader.readLine()) != null) {
				text.append(line);
			}
			if (text.toString().isEmpty())
				return null;

			return text.toString();

		} catch (IOException e) {
			e.printStackTrace();
		} catch (InterruptedException e) {
			e.printStackTrace();
		}

		return null;
	}
}
