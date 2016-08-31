package com.jk.simpleCalc;

import android.app.Activity;
import android.os.Bundle;
import android.text.*;
import android.view.*;
import android.widget.*;

public class AndCalc extends Activity {

	EditText et;		// Show Values
	TextView op;		// Show Operator
	TextView num1;		// Show m1
	Button btn01, btn02, btn03, btn04, btn05,btn06, btn07, btn08, btn09, btn00;
	Button btnDiv,btnMul, btnPlus, btnMinus, btnClear, btnDel, btnResult, btnPoint;
	public float m1=0, m2=0;
	public float result;
	public int intValue;

	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);

		et = (EditText) findViewById(R.id.EditText01);
		op = (TextView) findViewById(R.id.OP);
		num1 = (TextView) findViewById(R.id.M1);

		findViewById(R.id.Btn01).setOnClickListener(mClickListener);
		findViewById(R.id.Btn02).setOnClickListener(mClickListener);
		findViewById(R.id.Btn03).setOnClickListener(mClickListener);
		findViewById(R.id.Btn04).setOnClickListener(mClickListener);
		findViewById(R.id.Btn05).setOnClickListener(mClickListener);
		findViewById(R.id.Btn06).setOnClickListener(mClickListener);
		findViewById(R.id.Btn07).setOnClickListener(mClickListener);
		findViewById(R.id.Btn08).setOnClickListener(mClickListener);
		findViewById(R.id.Btn09).setOnClickListener(mClickListener);
		findViewById(R.id.Btn00).setOnClickListener(mClickListener);
		findViewById(R.id.BtnClear).setOnClickListener(mClickListener);
		findViewById(R.id.BtnDel).setOnClickListener(mClickListener);
		findViewById(R.id.BtnResult).setOnClickListener(mClickListener);
		findViewById(R.id.BtnDiv).setOnClickListener(mClickListener);
		findViewById(R.id.BtnMul).setOnClickListener(mClickListener);
		findViewById(R.id.BtnPlus).setOnClickListener(mClickListener);
		findViewById(R.id.BtnMinus).setOnClickListener(mClickListener);
		findViewById(R.id.BtnPoint).setOnClickListener(mClickListener);
	}

	Button.OnClickListener mClickListener = new Button.OnClickListener() {
		public void onClick(View v) {
			switch(v.getId()) {
			// Set Number Buttons
			case R.id.Btn01:
				if(op.getText().toString().equals("") && !(num1.getText().toString().equals(""))) {
					et.setText("");
					num1.setText("");
				}
				et.append("1");
				break;
			case R.id.Btn02:
				if(op.getText().toString().equals("") && !(num1.getText().toString().equals(""))) {
					et.setText("");
					num1.setText("");
				}
				et.append("2");
				break;
			case R.id.Btn03:
				if(op.getText().toString().equals("") && !(num1.getText().toString().equals(""))) {
					et.setText("");
					num1.setText("");
				}
				et.append("3");
				break;
			case R.id.Btn04:
				if(op.getText().toString().equals("") && !(num1.getText().toString().equals(""))) {
					et.setText("");
					num1.setText("");
				}
				et.append("4");
				break;
			case R.id.Btn05:
				if(op.getText().toString().equals("") && !(num1.getText().toString().equals(""))) {
					et.setText("");
					num1.setText("");
				}
				et.append("5");
				break;
			case R.id.Btn06:
				if(op.getText().toString().equals("") && !(num1.getText().toString().equals(""))) {
					et.setText("");
					num1.setText("");
				}
				et.append("6");
				break;
			case R.id.Btn07:
				if(op.getText().toString().equals("") && !(num1.getText().toString().equals(""))) {
					et.setText("");
					num1.setText("");
				}
				et.append("7");
				break;
			case R.id.Btn08:
				if(op.getText().toString().equals("") && !(num1.getText().toString().equals(""))) {
					et.setText("");
					num1.setText("");
				}
				et.append("8");
				break;
			case R.id.Btn09:
				if(op.getText().toString().equals("") && !(num1.getText().toString().equals(""))) {
					et.setText("");
					num1.setText("");
				}
				et.append("9");
				break;
			case R.id.Btn00:
				if(op.getText().toString().equals("") && !(num1.getText().toString().equals(""))) {
					break;
					//et.setText("");
					//num1.setText("");
				}
				if (et.getText().toString().equals("")) break;
				else {
					et.append("0");
				}
				break;  			
			case R.id.BtnPoint:		// floating point
				if(op.getText().toString().equals("") && !(num1.getText().toString().equals(""))) {
					et.setText("");
					num1.setText("");
				}
				if (et.getText().toString().equals(".")) break;
				et.append(".");
				break;

			case R.id.BtnClear:		// All Clear Button
				et.setText("");
				op.setText("");
				num1.setText("");
				m1=0; m2=0;
				break;
			case R.id.BtnDel:			// Delete 1 space
				Editable edit = et.getText();
				int st = edit.length();
				if (st > 1) {
					edit.delete(st-1, st);
					et.setText(edit);
				} else 	if (st <= 1) {
					et.setText("");
				} break;
				// +, -, /, * Buttons
			case R.id.BtnPlus:
				if (!(num1.getText().toString().equals("")) && !(op.getText().toString().equals("")) ) {
					m2=0;
					if (!(et.getText().toString().equals(""))) m2 = Float.parseFloat(et.getText().toString());
					if (op.getText().toString().equals("+")) {
						result = m1 + m2;
					} else if (op.getText().toString().equals("-")) {
						result = m1 - m2;
					} else if (op.getText().toString().equals("÷")) {
						result = m1 / m2;
					} else if (op.getText().toString().equals("×")) {
						result = m1 * m2;
					}
					intValue = (int) result;
					if (result == intValue) et.setText(String.valueOf(intValue));	// remove floating point 
					else et.setText(String.valueOf(result));
					m2=0;
				}
				op.setText("+");
				m1 = Float.parseFloat(et.getText().toString());
				num1.setText(et.getText().toString());
				et.setText("");
				break;
			case R.id.BtnMinus:
				if (!(num1.getText().toString().equals("")) && !(op.getText().toString().equals("")) ) {
					m2=0;
					if (!(et.getText().toString().equals(""))) m2 = Float.parseFloat(et.getText().toString());
					if (op.getText().toString().equals("+")) {
						result = m1 + m2;
					} else if (op.getText().toString().equals("-")) {
						result = m1 - m2;
					} else if (op.getText().toString().equals("÷")) {
						result = m1 / m2;
					} else if (op.getText().toString().equals("×")) {
						result = m1 * m2;
					}
					intValue = (int) result;
					if (result == intValue) et.setText(String.valueOf(intValue));	// remove floating point 
					else et.setText(String.valueOf(result));
					m2=0;
				}
				op.setText("-");
				m1 = Float.parseFloat(et.getText().toString());
				num1.setText(et.getText().toString());
				et.setText("");
				break;
			case R.id.BtnDiv:
				if (!(num1.getText().toString().equals("")) && !(op.getText().toString().equals("")) ) {
					m2=0;
					if (!(et.getText().toString().equals(""))) m2 = Float.parseFloat(et.getText().toString());
					if (op.getText().toString().equals("+")) {
						result = m1 + m2;
					} else if (op.getText().toString().equals("-")) {
						result = m1 - m2;
					} else if (op.getText().toString().equals("÷")) {
						result = m1 / m2;
					} else if (op.getText().toString().equals("×")) {
						result = m1 * m2;
					}
					intValue = (int) result;
					if (result == intValue) et.setText(String.valueOf(intValue));	// remove floating point 
					else et.setText(String.valueOf(result));
					m2=0;
				}
				op.setText("÷");
				m1 = Float.parseFloat(et.getText().toString());
				num1.setText(et.getText().toString());
				et.setText("");
				break;
			case R.id.BtnMul:
				if (!(num1.getText().toString().equals("")) && !(op.getText().toString().equals("")) ) {
					m2=0;
					if (!(et.getText().toString().equals(""))) m2 = Float.parseFloat(et.getText().toString());
					if (op.getText().toString().equals("+")) {
						result = m1 + m2;
					} else if (op.getText().toString().equals("-")) {
						result = m1 - m2;
					} else if (op.getText().toString().equals("÷")) {
						result = m1 / m2;
					} else if (op.getText().toString().equals("×")) {
						result = m1 * m2;
					}
					intValue = (int) result;
					if (result == intValue) et.setText(String.valueOf(intValue));	// remove floating point 
					else et.setText(String.valueOf(result));
					m2=0;
				}
				op.setText("×");
				m1 = Float.parseFloat(et.getText().toString());
				num1.setText(et.getText().toString());
				et.setText("");
				break;
				// Result Button
			case R.id.BtnResult:
				if (op.getText().toString().equals("")) {	
					et.setText("");
					m1=0; 
					m2=0;
					num1.setText("");
					break;
				} else {
					m2=0;
					if (!(et.getText().toString().equals(""))) m2 = Float.parseFloat(et.getText().toString());
					if (op.getText().toString().equals("+")) {
						result = m1 + m2;
					} else if (op.getText().toString().equals("-")) {
						result = m1 - m2;
					} else if (op.getText().toString().equals("÷")) {
						result = m1 / m2;
					} else if (op.getText().toString().equals("×")) {
						result = m1 * m2;
					}
					intValue = (int) result;
					if (result == intValue) et.setText(String.valueOf(intValue));	// remove floating point 
					else et.setText(String.valueOf(result));
					m1=0; m2=0;
					op.setText("");
					num1.setText(et.getText().toString());
					break;
				}
			} // close switch
		} // close onClick
	}; // close mClickListener
}