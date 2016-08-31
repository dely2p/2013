package com.skuniv.ce.assignment04_2;
import java.util.Calendar;
import android.app.Activity;
import android.app.DatePickerDialog;
import android.app.Dialog;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.DatePicker;
import android.widget.EditText;
import android.widget.TextView;

public class SubActivity extends Activity{
	EditText edit;
	private TextView mDateDisplay;
	private Button mPickDate;
	private int mYear;
	private int mMonth;
	private int mDay;

	static final int DATE_DIALOG_ID =0;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		setContentView(R.layout.sub);
		
		mDateDisplay = (TextView) findViewById(R.id.dateDisplay);
		mPickDate = (Button) findViewById(R.id.pickDate);		
		mPickDate.setOnClickListener(new View.OnClickListener() {			
			@Override
			public void onClick(View v) {
				showDialog(DATE_DIALOG_ID);				
			}
		});		
		final Calendar c = Calendar.getInstance();
		mYear = c.get(Calendar.YEAR);
		mMonth = c.get(Calendar.MONTH);
		mDay = c.get(Calendar.DAY_OF_MONTH);		
		
		updateDisplay();		
		Button button_ok = (Button) findViewById(R.id.button_ok);
		button_ok.setOnClickListener(new OnClickListener(){
			public void onClick(View v){
				Intent intent = new Intent();
				intent.putExtra("INPUT_TEXT", mDateDisplay.getText().toString());
				setResult(RESULT_OK, intent);
				finish();				
			}
		});
		
		Button button_cancel = (Button) findViewById(R.id.button_cancel);
		button_cancel.setOnClickListener(new OnClickListener() {
			public void onClick(View v) {
				setResult(RESULT_CANCELED);
				finish();		
			} }); }
	private void updateDisplay(){		
		mDateDisplay.setText(new StringBuilder()
		.append(mMonth+1).append("-")
		.append(mDay).append("-")
		.append(mYear).append("-"));
	}
	private DatePickerDialog.OnDateSetListener mDateSetListener = new
			DatePickerDialog.OnDateSetListener() {				
				@Override
				public void onDateSet(DatePicker view, int year, int monthOfYear,
						int dayOfMonth) {
					mYear = year;
					mMonth = monthOfYear;
					mDay = dayOfMonth;
					updateDisplay();
				} 	};
	protected Dialog onCreateDialog(int id){
		switch(id){
		case DATE_DIALOG_ID:
			return new DatePickerDialog(this,mDateSetListener,mYear,mMonth,mDay);		
		}
		return null;		
	} }
