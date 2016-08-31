package com.skuniv.ce.assignment04_2;
import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.TextView;

public class MainActivity extends Activity {
	static final int GET_STRING = 1;
	TextView text;
	@Override
	protected void onCreate(Bundle icicle) {
		super.onCreate(icicle);
		setContentView(R.layout.main);
		
		Button button = (Button) findViewById(R.id.button);
		text = (TextView) findViewById(R.id.text);
		button.setOnClickListener(new OnClickListener(){			
			public void onClick(View arg0){
				Intent in = new Intent(MainActivity.this,SubActivity.class);
				startActivityForResult(in,GET_STRING);
			}
		});
	}
	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		if(requestCode == GET_STRING){
			if(resultCode == RESULT_OK){
				text.setText(data.getStringExtra("INPUT_TEXT"));
			}			
		}
	}
}
