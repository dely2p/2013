package com.skuniv.ce.assignment04;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.os.Bundle;
import android.view.ContextMenu;
import android.view.ContextMenu.ContextMenuInfo;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

public class MainActivity extends Activity {
	static final int DIALOG_CUSTOM_ID =0;
	String FILENAME = "test.txt";
	 EditText edit;
	 EditText et1;
	 EditText et2;
	 TextView tv1;
	@Override
	protected void onCreate(Bundle savedInstanceState) {				
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);
		
		edit = (EditText)findViewById(R.id.editText01);
		tv1 = (TextView)findViewById(R.id.textview);
		Button readButton = (Button) findViewById(R.id.read);	
		
		readButton.setOnClickListener(new View.OnClickListener() {				
			@Override
			public void onClick(View v) {
				Context mContext = MainActivity.this;
				AlertDialog.Builder builder;
				AlertDialog dialog;
				LayoutInflater inflater = (LayoutInflater)mContext.getSystemService(LAYOUT_INFLATER_SERVICE);
				View layout	= inflater.inflate(R.layout.customdialog2,(ViewGroup)findViewById(R.id.customdialog_layout2));
				
				et2 = (EditText)layout.findViewById(R.id.edittext2);
				
				builder = new AlertDialog.Builder(mContext);
				builder.setView(layout);
				dialog = builder.create();
				dialog.setTitle("Data Search 다이어로그");
				dialog.setButton("Search", new DialogInterface.OnClickListener() {
					
					@Override
					public void onClick(DialogInterface dialog, int which) {
						
						FILENAME = et2.getText().toString();
						dialog.dismiss();	
						
						try{
							FileInputStream fis = openFileInput(FILENAME);
							byte[] buffer = new byte[fis.available()];
							fis.read(buffer);
							edit.setText(new String(buffer));
							tv1.setText(new String(FILENAME+" 검색 됨"));
							fis.close();
						} catch (IOException e){
							
						}	
					}					
				});					
				dialog.show();			
							
			}
		});
		Button writeButton = (Button) findViewById(R.id.write);		
		writeButton.setOnClickListener(new View.OnClickListener() {			
			@Override
			public void onClick(View v) {				
				Context mContext = MainActivity.this;
				AlertDialog.Builder builder;
				AlertDialog dialog;
				LayoutInflater inflater = (LayoutInflater)mContext.getSystemService(LAYOUT_INFLATER_SERVICE);
				View layout	= inflater.inflate(R.layout.customdialog,(ViewGroup)findViewById(R.id.customdialog_layout));
				
				et1 = (EditText)layout.findViewById(R.id.edittext1);
								
				builder = new AlertDialog.Builder(mContext);
				builder.setView(layout);
				dialog = builder.create();
				dialog.setTitle("제목 입력 다이어로그");
				dialog.setButton("Save", new DialogInterface.OnClickListener() {					
					@Override
					public void onClick(DialogInterface dialog, int which) {
						Toast.makeText(getApplicationContext(), "저장되었습니다.", Toast.LENGTH_SHORT).show();
						FILENAME = et1.getText().toString();
						tv1.setText(FILENAME);
						dialog.dismiss();
						try{							
							FileOutputStream fos = openFileOutput(FILENAME, Context.MODE_PRIVATE);
							fos.write(edit.getText().toString().getBytes());
							fos.close();
						}catch (IOException e){
						}
					}					
				});						
				dialog.show();						
			}
		});
	}	
	@Override
	public void onCreateContextMenu(ContextMenu menu, View v,
			ContextMenuInfo menuInfo) {
		// TODO Auto-generated method stub
		super.onCreateContextMenu(menu, v, menuInfo);
	}
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}
}
