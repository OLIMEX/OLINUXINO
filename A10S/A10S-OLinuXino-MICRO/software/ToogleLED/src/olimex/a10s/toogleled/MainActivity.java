package olimex.a10s.toogleled;

import olimex.a10s.toogleled.GPIO;
import android.os.Bundle;
import android.app.Activity;
import android.view.Menu;
import android.view.View;
import android.widget.Button;

public class MainActivity extends Activity {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		//Create new object
		final GPIO gpio = new GPIO("PE3", "LED", 1);	
		
		//Set as output
		gpio.setcfg(1);
		
		
		final Button button = (Button) findViewById(R.id.button1);
		button.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				//Read output state and toggle it
				if(gpio.input() == 0)
					gpio.output(1);
				else
					gpio.output(0);				
			}
		});
		
	}
	
	

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

}
