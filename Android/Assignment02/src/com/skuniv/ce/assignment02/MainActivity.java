package com.skuniv.ce.assignment02;
import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.content.DialogInterface;
import android.os.Bundle;
import android.view.Menu;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemSelectedListener;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.EditText;
import android.widget.RadioButton;
import android.widget.RadioGroup;
import android.widget.RadioGroup.OnCheckedChangeListener;
import android.widget.Spinner;

public class MainActivity extends Activity {
	private static final int DIALOG_YES_NO_MESSAGE =1;
	EditText _name;
	RadioGroup _check;
	RadioButton _gender;
	CheckBox _sms;
	Spinner _fav;
	Button _b;
	String getname,getgend,getsms,getfav;

	@Override
	@Deprecated
	protected Dialog onCreateDialog(int id) {
		switch(id){
		case DIALOG_YES_NO_MESSAGE:
			AlertDialog.Builder builder = new AlertDialog.Builder(this);
			builder.setTitle("알림창")
			.setMessage("성명: "+getname+"\n성별: "+getgend
					+"\n수신여부: "+getsms+"\n관심분야: "+getfav)
			.setCancelable(false)
			.setPositiveButton("확인", 
					new DialogInterface.OnClickListener() {
				public void onClick(DialogInterface dialog, int whichButton){
					MainActivity.this.finish();
				}
											
			});
			AlertDialog alert = builder.create();
			return alert;
		}
		return null;
	}

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        
        _name = (EditText)findViewById(R.id.edittext1);
		_check = (RadioGroup) findViewById(R.id.rad_check);
		_gender = (RadioButton)findViewById(_check.getCheckedRadioButtonId());
		_sms = (CheckBox) findViewById(R.id.checkbox1);
		_fav = (Spinner) findViewById(R.id.spinner1);
		_b = (Button) findViewById(R.id.button);
		
		_check.setOnCheckedChangeListener(new OnCheckedChangeListener() {
			
			@Override
			public void onCheckedChanged(RadioGroup group, int checkedId) {
				if(checkedId ==R.id.radiobutton1){
					getgend = "남";					
				}
				else{
					getgend = "여";
				}				
			}
		});
		_sms.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				if(((CheckBox)v).isChecked()){
					_sms.setText("sms");
					getsms = _sms.getText().toString();
				}
				else{
					_sms.setText("no sms");
					getsms = _sms.getText().toString();
				}	
			}
		});
		_fav.setOnItemSelectedListener(new OnItemSelectedListener() {
			public void onItemSelected(AdapterView<?> parent,View view,
					int position,long id){
				getfav = parent.getItemAtPosition(position).toString();
			}
			public  void onNothingSelected(AdapterView<?> parent){
				
			}
		});
		
		_b.setOnClickListener(new OnClickListener(){
			public void onClick(View v){
				getname =_name.getText().toString();
			//	getgend =_gender.getText().toString();
				showDialog(DIALOG_YES_NO_MESSAGE);
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
