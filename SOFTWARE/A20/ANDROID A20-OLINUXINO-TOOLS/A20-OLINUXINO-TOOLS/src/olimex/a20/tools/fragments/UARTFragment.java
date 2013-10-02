package olimex.a20.tools.fragments;

import olimex.a20.tools.jni.UART;
import olimex.a20.uart.R;
import android.graphics.Color;
import android.os.Bundle;
import android.os.Handler;
import android.support.v4.app.Fragment;
import android.text.SpannableString;
import android.text.method.ScrollingMovementMethod;
import android.text.style.ForegroundColorSpan;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.TextView;

public class UARTFragment extends Fragment {

	public UARTFragment() {
	}

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) {

		/* Define some controls */
		View rootView = inflater
				.inflate(R.layout.uart_layout, container, false);
		Button send_btn = (Button) rootView.findViewById(R.id.send_btn);
		final TextView console = (TextView) rootView.findViewById(R.id.console);
		final TextView command = (TextView) rootView.findViewById(R.id.command);

		/**
		 * Make new runnable class to append console
		 */
		class MyRunnable implements Runnable {

			private final static int MAX_LINE = 25;
			public StringBuffer line = new StringBuffer();

			@Override
			public void run() {
				SpannableString read = new SpannableString("READ:> "
						+ this.line.toString() + "\n");
				read.setSpan(new ForegroundColorSpan(Color.GREEN), 0, 6, 0);
				console.append(read);
				line = null;
				read = null;
				/**
				 * Erase the oldest lines
				 */
				int lineNumber = console.getLineCount() - MAX_LINE;
				if (lineNumber > 0) {
					int eolIndex = -1;
					CharSequence charSequence = console.getText();
					for (int i = 0; i < lineNumber; i++) {
						do {
							eolIndex++;
						} while (eolIndex < charSequence.length()
								&& charSequence.charAt(eolIndex) != '\n');
					}
					if (eolIndex < charSequence.length()) {
						console.getEditableText().delete(0, eolIndex + 1);
					} else {
						console.setText("");
					}
				}
			}
		}

		/**
		 * Start new tread for gaining access
		 */

		console.setMovementMethod(new ScrollingMovementMethod());

		/**
		 * Make new endless thread that scan for received characters over UART
		 */
		final Handler handler = new Handler();
		new Thread(new Runnable() {

			@Override
			public void run() {
				StringBuffer line = new StringBuffer();
				char[] buffer = new char[256];

				while (true) {
					int len = UART.serialRead(UART.fd, buffer); // Receive
																// single char
																// over UART
					if (len != 0) {
						for (int i = 0; i < len; i++) {

							if (buffer[i] == '\r') {

								MyRunnable obj = new MyRunnable();
								obj.line.append(line);
								handler.post(obj);

								line.delete(0, line.length()); 
							} else
								line.append(buffer[i]);
						}
					}
				}

			}
		}).start();

		/**
		 * On button click send some text to the UEXT
		 */
		send_btn.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				CharSequence cmd = command.getText();
				SpannableString line = new SpannableString("SEND:> " + cmd
						+ "\n");
				line.setSpan(new ForegroundColorSpan(Color.RED), 0, 6, 0);
				console.append(line);
				command.setText("");
				UART.serialWrite(UART.fd, cmd.toString() + "\r\n");
				line = null;
			}
		});

		return rootView;
	}
}
