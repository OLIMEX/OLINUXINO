package olimex.a20.tools.fragments;

import olimex.a20.tools.jni.I2C;
import olimex.a20.uart.R;
import android.content.Context;
import android.graphics.Color;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemSelectedListener;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.Toast;

public class I2CFragment extends Fragment {

	public I2CFragment() {

	}

	protected void readI2C(View view) {
		TextView status = (TextView) view.findViewById(R.id.i2c_status);
		TextView result = (TextView) view.findViewById(R.id.i2c_result);
		Spinner bus = (Spinner) view.findViewById(R.id.i2c_bus_spinner);
		TextView address_tv = (TextView) view
				.findViewById(R.id.i2c_address_input);
		TextView register_tv = (TextView) view
				.findViewById(R.id.i2c_register_input);
		TextView bytes_tv = (TextView) view.
				findViewById(R.id.i2c_number_bytes);

		status.setText("Initialating i2c-bus...");
		I2C.fd = I2C.init(bus.getSelectedItem().toString());
		if (I2C.fd < 0) {
			status.setText(status.getText() + "Fail!");
			return;
		} else
			status.setText(status.getText() + "Success!");

		/**
		 * Parsing values
		 */
		status.setText("Parsing values...");
		int address;
		int register;
		int bytes;

		address = StringToHex(address_tv.getText().toString());
		register = StringToHex(register_tv.getText().toString());
		bytes = StringToHex(bytes_tv.getText().toString());
		
		/* Check if entered data is valid */
		if (address < 0 || register < 0) {

			Toast.makeText(view.getContext(),
					"Some of the entered parameters are invalid!",
					Toast.LENGTH_LONG).show();
			if (address < 0)
				address_tv.setTextColor(Color.RED);
			if (register < 0)
				register_tv.setTextColor(Color.RED);
			status.setText(status.getText() + "Fail!");
			return;
		}
		status.setText(status.getText() + "Success!");

		int[] out_buffer = new int[1];
		int[] in_buffer = new int[bytes];
		out_buffer[0] = register;
		result.append("WRITE-->> " + " 0x" + Integer.toHexString(address)
				+ " 0x" + Integer.toHexString(register) + "\n");

		/**
		 * Now finally send some data...
		 */
		status.setText("Reading from device...");
		if (I2C.open(I2C.fd, address) < 0) {
			status.setText(status.getText() + "Fail!");
			return;
		}
		if (I2C.write(I2C.fd, out_buffer, 1) < 0) {
			status.setText(status.getText() + "Fail!");
			return;
		}
		if (I2C.read(I2C.fd, in_buffer, bytes) < 0) {
			status.setText(status.getText() + "Fail!");
			return;
		}
		I2C.close(I2C.fd);
		status.setText(status.getText() + "Success!");

		result.append("READ--<< "); 
		for(int i = 0; i < bytes; i++)
			result.append(" 0x" + Integer.toHexString(in_buffer[i]));
		
		result.append("\n");

		for (int i = 0; i < (int) result.getWidth() / result.getTextSize(); i++)
			result.append("- ");
		result.append("\n");
	}

	protected void writeI2C(View view) {
		TextView status = (TextView) view.findViewById(R.id.i2c_status);
		TextView result = (TextView) view.findViewById(R.id.i2c_result);
		Spinner bus = (Spinner) view.findViewById(R.id.i2c_bus_spinner);
		TextView address_tv = (TextView) view
				.findViewById(R.id.i2c_address_input);
		TextView register_tv = (TextView) view
				.findViewById(R.id.i2c_register_input);
		TextView data_tv = (TextView) view.findViewById(R.id.i2c_data_input);

		status.setText("Initialating i2c-bus...");
		I2C.fd = I2C.init(bus.getSelectedItem().toString());
		if (I2C.fd < 0) {
			status.setText(status.getText() + "Fail!");
			return;
		} else
			status.setText(status.getText() + "Success!");

		/**
		 * Parsing values
		 */
		status.setText("Parsing values...");
		int address;
		int register;
		int data;

		address = StringToHex(address_tv.getText().toString());
		register = StringToHex(register_tv.getText().toString());
		data = StringToHex(data_tv.getText().toString());

		/* Check if entered data is valid */
		if (address < 0 || register < 0 || data < 0) {

			Toast.makeText(view.getContext(),
					"Some of the entered parameters are invalid!",
					Toast.LENGTH_LONG).show();
			if (address < 0)
				address_tv.setTextColor(Color.RED);
			if (register < 0)
				register_tv.setTextColor(Color.RED);
			if (data < 0)
				data_tv.setTextColor(Color.RED);
			status.setText(status.getText() + "Fail!");
			return;
		}
		status.setText(status.getText() + "Success!");

		int[] buffer = new int[2];
		buffer[0] = register;
		buffer[1] = data;
		result.append("WRITE-->> " + " 0x" + Integer.toHexString(address)
				+ " 0x" + Integer.toHexString(register) + " 0x"
				+ Integer.toHexString(data) + "\n");

		/**
		 * Now finally send some data...
		 */
		status.setText("Writing to device...");
		if (I2C.open(I2C.fd, address) < 0) {
			status.setText(status.getText() + "Fail!");
			return;
		}
		if (I2C.write(I2C.fd, buffer, 2) < 0) {
			status.setText(status.getText() + "Fail!");
			return;
		}
		I2C.close(I2C.fd);
		status.setText(status.getText() + "Success!");

		for (int i = 0; i < (int) result.getWidth() / result.getTextSize(); i++)
			result.append("- ");
		result.append("\n");

	}

	private int StringToHex(String string) {
		try {
			return Integer.parseInt(string, 16);
		} catch (NumberFormatException e) {
			return -1;
		}

	}

	protected void scanI2C(View view) {
		TextView status = (TextView) view.findViewById(R.id.i2c_status);
		TextView result = (TextView) view.findViewById(R.id.i2c_result);
		Spinner bus = (Spinner) view.findViewById(R.id.i2c_bus_spinner);

		status.setText("");
		status.setText("Initialating i2c-bus...");
		I2C.fd = I2C.init(bus.getSelectedItem().toString());
		if (I2C.fd < 0) {
			status.setText(status.getText() + "Fail!");
			return;
		} else
			status.setText(status.getText() + "Success!");

		/**
		 * Now do some real work
		 */
		result.append(" --> Scanning:\n\n");

		result.append("\t0\t1\t2\t3\t4\t5\t6\t7\t8\t9\tA\tB\tC\tD\tE\tF\n");
		int[] buf = new int[1];
		for (int i = 0; i < 8; i++) {
			result.append(String.valueOf(i));
			for (int j = 0; j < 16; j++) {
				status.setText("Scanning: " + Integer.toHexString(i * 16 + j));
				I2C.open(I2C.fd, i * 16 + j);
				if (I2C.read(I2C.fd, buf, 1) < 0)
					result.append("\t" + "--");
				else
					result.append("\t" + Integer.toHexString(i * 16 + j));
			}
			result.append("\n");
		}
		result.append("\n --> Done\n");

		I2C.close(I2C.fd);

		for (int i = 0; i < (int) result.getWidth() / result.getTextSize(); i++)
			result.append("- ");
		result.append("\n");

	}

	private void populateSpinner(Spinner spinner, Context context,
			int TextArrayResId) {
		/* Populate spinner */
		ArrayAdapter<CharSequence> adapter = ArrayAdapter.createFromResource(
				context, TextArrayResId,
				android.R.layout.simple_spinner_dropdown_item);
		adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
		spinner.setAdapter(adapter);
	}

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) {

		/* Define some controls */
		final View rootView = inflater.inflate(R.layout.i2c_layout, container,
				false);

		/* Populate action spinner */
		final Spinner action_spinner = (Spinner) rootView
				.findViewById(R.id.i2c_action_spinner);
		populateSpinner(action_spinner, rootView.getContext(),
				R.array.i2c_actions);

		/* Populate action spinner */
		final Spinner bus_spinner = (Spinner) rootView
				.findViewById(R.id.i2c_bus_spinner);
		populateSpinner(bus_spinner, rootView.getContext(), R.array.i2c_bus);

		/**
		 * Define some inputText
		 */
		final TextView address_input = (TextView) rootView
				.findViewById(R.id.i2c_address_input);
		final TextView register_input = (TextView) rootView
				.findViewById(R.id.i2c_register_input);
		final TextView data_input = (TextView) rootView
				.findViewById(R.id.i2c_data_input);
		final TextView bytes_input = (TextView) rootView
				.findViewById(R.id.i2c_number_bytes);

		action_spinner.setOnItemSelectedListener(new OnItemSelectedListener() {

			@Override
			public void onItemSelected(AdapterView<?> parent, View view,
					int pos, long id) {
				switch (pos) {
				case 2:
					address_input.setEnabled(true);
					register_input.setEnabled(true);
					data_input.setEnabled(false);
					bytes_input.setEnabled(true);
					break;
				case 1:
					address_input.setEnabled(true);
					register_input.setEnabled(true);
					data_input.setEnabled(true);
					bytes_input.setEnabled(false);
					break;
				case 0:
					address_input.setEnabled(false);
					register_input.setEnabled(false);
					data_input.setEnabled(false);
					bytes_input.setEnabled(false);
					break;
				}

			}

			@Override
			public void onNothingSelected(AdapterView<?> arg0) {

			}

		});

		/**
		 * Initialise button
		 */
		Button execute_button = (Button) rootView
				.findViewById(R.id.i2c_btn_execute);
		/* Now do some action */

		execute_button.setOnClickListener(new View.OnClickListener() {

			@Override
			public void onClick(View v) {

				address_input.setTextColor(Color.BLACK);
				register_input.setTextColor(Color.BLACK);
				data_input.setTextColor(Color.BLACK);

				switch (action_spinner.getSelectedItemPosition()) {
				case 0:
					scanI2C(rootView);
					break;
				case 1:
					writeI2C(rootView);
					break;
				case 2:
					readI2C(rootView);
					break;
				}

			}

		});

		return rootView;
	}

}
