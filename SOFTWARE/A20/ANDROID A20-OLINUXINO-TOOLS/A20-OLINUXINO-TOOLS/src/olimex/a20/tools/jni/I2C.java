package olimex.a20.tools.jni;

import java.io.IOException;

public class I2C {

	/* Native methods */
	public static native int init(String bus);

	public static native int open(int fd, int address);

	public static native int close(int fd);

	public static native int write(int fd, int[] buf, int len);

	public static native int read(int fd, int[] buf, int len);

	/* Load shared library */
	static {
		System.loadLibrary("I2C");
	}

	public static int fd;

	public static void getPermission() {
		try {
			Process p = Runtime.getRuntime().exec(
					new String[] { "su", "-c", "chmod 777 /dev/i2c-*" });
			p.waitFor();
		} catch (IOException e) {
			e.printStackTrace();
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
	}
}
