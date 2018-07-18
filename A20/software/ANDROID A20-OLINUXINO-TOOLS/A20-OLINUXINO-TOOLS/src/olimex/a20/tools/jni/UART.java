package olimex.a20.tools.jni;

import java.io.IOException;

import android.util.Log;

public class UART {

	/* Native methods */
	public static native int serialOpen(String port, int baud);

	public static native int serialClose(int fd);

	public static native int serialWrite(int fd, String text);

	public static native int serialRead(int fd, char[] buffer);

	/* Load shared library */
	static {
		System.loadLibrary("UART");
	}

	public static int fd;

	public static void getPermission() {
		try {
			Process p = Runtime.getRuntime().exec(
					new String[] { "su", "-c", "chmod 777 /dev/ttyS*" });
			p.waitFor();
			Log.i("UART", "chmod -> success!");
		} catch (IOException e) {
			e.printStackTrace();
			Log.e("UART", "chmod -> fail!");
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
	}

}
