package com.skuniv.ce.databaselistactivity;

import android.app.Activity;
import android.content.Intent;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.graphics.Color;
import android.os.Bundle;
import android.view.Menu;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.TextView;

public class MainActivity extends Activity {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);
		
		LinearLayout layout = (LinearLayout) findViewById(R.id.customers);
		
		int i = 0;
		
		try{
			DBManager dbmgr = new DBManager(this);
			SQLiteDatabase sdb = dbmgr.getReadableDatabase();
			Cursor cursor = sdb.rawQuery("Select name,sex,sms from Customers",null);
			
			while(cursor.moveToNext()){
				String name = cursor.getString(0);
				String sex = cursor.getString(1);
				String sms = cursor.getString(2);
				
				TextView tv_list = new TextView(this);
				tv_list.append(name);
				tv_list.setTextSize(20);
				tv_list.setTextColor(Color.rgb(255, 255, 0));
				tv_list.setBackgroundColor(Color.rgb(0, 0, 255));
				layout.addView(tv_list);
		
				TextView tv_list2 = new TextView(this);
				tv_list2.append(sex + "\n");
				tv_list2.append(sms + "\n");
				layout.addView(tv_list2);
				
				i++;
				
				Button btn = (Button)findViewById(R.id.button_join_form);
				btn.setOnClickListener(new OnClickListener() {
					
					@Override
					public void onClick(View v) {
						// TODO Auto-generated method stub
						
						Intent intent = new Intent(MainActivity.this,CustomRegActivity.class);
						startActivity(intent);
						
					}
				});
			}
			}catch(Exception e){
			
		}
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

}
