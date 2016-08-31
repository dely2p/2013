package com.skuniv.ce.assignment03;
import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.Menu;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
public class MainActivity extends Activity {
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);
		Button b1 = (Button)findViewById(R.id.button1);
	        b1.setOnClickListener(new OnClickListener(){
	        	public void onClick(View v){
	        	Intent intent1 = new Intent(getApplicationContext(),
	        			MystoryActivity.class);
	        	startActivity(intent1);
	       	}
	    });
	    Button b2 = (Button)findViewById(R.id.button2);
	        b2.setOnClickListener(new OnClickListener(){
	        	public void onClick(View v){
	        	Intent intent2 = new Intent(getApplicationContext(),
	        			MapActivity.class);
	        	startActivity(intent2);
	       	}
	    });
	}
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}
}
