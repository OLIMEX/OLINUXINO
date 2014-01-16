package olimex.a20.tools.fragments;

import java.util.ArrayList;
import java.util.List;

import olimex.a20.tools.Pins;
import olimex.a20.uart.R;
import android.app.ProgressDialog;
import android.os.Bundle;
import android.os.Handler;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.CompoundButton;
import android.widget.CompoundButton.OnCheckedChangeListener;
import android.widget.Switch;
import android.widget.TableLayout;
import android.widget.TextView;

public class GPIOFragment extends Fragment {

	@Override
	public View onCreateView(final LayoutInflater inflater,
			ViewGroup container, Bundle savedInstanceState) {

		final View rootView = inflater.inflate(R.layout.gpio_layout, container,
				false);

		final TableLayout table = (TableLayout) rootView
				.findViewById(R.id.gpio_table_layout);

		final Handler updateHandler = new Handler();
		final List<Pins> pins = new ArrayList<Pins>();
		pins.add(new Pins("GPIO-3:5", 167));
		pins.add(new Pins("GPIO-3:6", 27));
		pins.add(new Pins("GPIO-3:7", 169));
		pins.add(new Pins("GPIO-3:8", 28));
		pins.add(new Pins("GPIO-3:9", 174));
		pins.add(new Pins("GPIO-3:10", 29));
		pins.add(new Pins("GPIO-3:11", 176));
		pins.add(new Pins("GPIO-3:12", 30));
		pins.add(new Pins("GPIO-3:13", 177));
		pins.add(new Pins("GPIO-3:14", 31));
		pins.add(new Pins("GPIO-3:15", 178));
		pins.add(new Pins("GPIO-3:16", 32));
		pins.add(new Pins("GPIO-3:17", 179));
		pins.add(new Pins("GPIO-3:18", 34));
		pins.add(new Pins("GPIO-3:19", 180));
		pins.add(new Pins("GPIO-3:20", 35));
		pins.add(new Pins("GPIO-3:21", 181));
		pins.add(new Pins("GPIO-3:22", 36));
		pins.add(new Pins("GPIO-3:23", 182));
		pins.add(new Pins("GPIO-3:24", 37));
		pins.add(new Pins("GPIO-3:25", 183));
		pins.add(new Pins("GPIO-3:26", 38));
		pins.add(new Pins("GPIO-3:27", 184));
		pins.add(new Pins("GPIO-3:28", 39));
		pins.add(new Pins("GPIO-3:29", 185));
		pins.add(new Pins("GPIO-3:30", 40));
		pins.add(new Pins("GPIO-3:31", 186));
		pins.add(new Pins("GPIO-3:32", 41));
		pins.add(new Pins("GPIO-3:33", 187));
		pins.add(new Pins("GPIO-3:34", 191));
		pins.add(new Pins("GPIO-3:35", 188));
		pins.add(new Pins("GPIO-3:36", 192));
		pins.add(new Pins("GPIO-3:37", 189));
		pins.add(new Pins("GPIO-3:38", 193));
		pins.add(new Pins("GPIO-3:39", 190));
		pins.add(new Pins("GPIO-3:40", 144));

		/**
		 * Add new array of objects
		 */
		final ProgressDialog barProgressDialog = new ProgressDialog(
				rootView.getContext());
		barProgressDialog.setTitle("Initilizing GPIOs");
		barProgressDialog.setMessage("Checking GPIOs...");
		barProgressDialog.setProgressStyle(ProgressDialog.STYLE_HORIZONTAL);
		barProgressDialog.setProgress(0);
		barProgressDialog.setMax(pins.size());
		barProgressDialog.show();
		new Thread(new Runnable() {
			@Override
			public void run() {
				try {
					for (int i = 0; i < pins.size(); i++) {
						final Pins temp_pin = pins.get(i);
						updateHandler.post(new Runnable() {

							@Override
							public void run() {
								barProgressDialog.setMessage("Checking "
										+ temp_pin.label + "...");
								barProgressDialog.incrementProgressBy(1);

							}
						});
						temp_pin.exportPin();
						/* Add new table row */
						final View tr = inflater.inflate(R.layout.table_row,
								null, false);

						/* Define the controls */
						TextView label_tv = (TextView) tr
								.findViewById(R.id.table_row_label);
						final TextView status_tv = (TextView) tr
								.findViewById(R.id.table_row_status);
						Switch dir_sw = (Switch) tr
								.findViewById(R.id.table_row_switch_direction);
						final Switch lev_sw = (Switch) tr
								.findViewById(R.id.table_row_switch_level);

						label_tv.setText(temp_pin.label);

						/* Initial direction */
						String dir = temp_pin.getDirection();
						if (dir == null)
							continue;
						if (dir.equalsIgnoreCase("in")) {
							dir_sw.setChecked(true);
							lev_sw.setEnabled(false);

							/* Initial level */
							int lev = temp_pin.getStatus();
							if (lev < 0)
								continue;
							if (lev == 0)
								status_tv.setText("LOW");
							else
								status_tv.setText("HIGH");

						} else {
							dir_sw.setChecked(false);
							lev_sw.setEnabled(true);

							/* Initial level */
							int lev = temp_pin.getStatus();
							if (lev < 0)
								continue;
							if (lev == 0) {
								status_tv.setText("LOW");
								lev_sw.setChecked(false);
							} else {
								status_tv.setText("HIGH");
								lev_sw.setChecked(true);
							}
						}

						/* Set controls for direction */
						dir_sw.setOnCheckedChangeListener(new OnCheckedChangeListener() {

							@Override
							public void onCheckedChanged(
									CompoundButton buttonView, boolean isChecked) {
								if (!isChecked) {
									/* Output state */
									lev_sw.setEnabled(true);
									temp_pin.setDirection("out");
								} else {
									lev_sw.setEnabled(false);
									temp_pin.setDirection("in");
								}
							}
						});

						/* Set controls for level */
						lev_sw.setOnCheckedChangeListener(new OnCheckedChangeListener() {

							@Override
							public void onCheckedChanged(
									CompoundButton buttonView, boolean isChecked) {
								if (!isChecked) {
									temp_pin.setLevel(0);
								} else {
									temp_pin.setLevel(1);

								}

							}
						});
						new Thread(new Runnable() {

							@Override
							public void run() {

								while (true) {
									if (temp_pin.getStatus() == 0)
										getActivity().runOnUiThread(
												new Runnable() {

													@Override
													public void run() {
														status_tv
																.setText("LOW");

													}
												});
									else
										getActivity().runOnUiThread(
												new Runnable() {

													@Override
													public void run() {
														status_tv
																.setText("HIGH");

													}
												});

								}
							}
						}).start();

						getActivity().runOnUiThread(new Runnable() {

							@Override
							public void run() {
								table.addView(tr);

							}
						});

					}
				} catch (Exception e) {
					e.printStackTrace();
				}

				barProgressDialog.dismiss();
			}
		}).start();

		return rootView;
	}
}
