package com.example.bluetoothexample;

import android.app.Activity;
import android.app.AlertDialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.view.View.OnTouchListener;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

public class MainActivity extends Activity {

    // Intent request codes
    private static final int REQUEST_CONNECT_DEVICE = 1;
    private static final int REQUEST_ENABLE_BT = 2;

    private static TextView mTitle;
    


    // Name of the connected device
    private String mConnectedDeviceName = null;

    /**
     * Set to true to add debugging code and logging.
     */
    public static final boolean DEBUG = true;
	
    /**
     * Set to true to log each character received from the remote process to the
     * android log, which makes it easier to debug some kinds of problems with
     * emulating escape sequences and control codes.
     */
    public static final boolean LOG_CHARACTERS_FLAG = DEBUG && false;

    /**
     * Set to true to log unknown escape sequences.
     */
    public static final boolean LOG_UNKNOWN_ESCAPE_SEQUENCES = DEBUG && false;

    /**
     * The tag we use when logging, so that our messages can be distinguished
     * from other messages in the log. Public because it's used by several
     * classes.
     */
	public static final String LOG_TAG = "FirmTech";

    // Message types sent from the BluetoothReadService Handler
    public static final int MESSAGE_STATE_CHANGE = 1;
    public static final int MESSAGE_READ = 2;
    public static final int MESSAGE_WRITE = 3;
    public static final int MESSAGE_DEVICE_NAME = 4;
    public static final int MESSAGE_TOAST = 5;	

    // Key names received from the BluetoothChatService Handler
    public static final String DEVICE_NAME = "device_name";
    public static final String TOAST = "toast";	

	private BluetoothAdapter mBluetoothAdapter = null;

    /**
     * Our main view. Displays the emulated terminal screen.
     */
    //private EmulatorView mEmulatorView;


    private static BluetoothSerialService mSerialService = null;
	
	//private static InputMethodManager mInputManager;
	
	private boolean mEnablingBT;
    private boolean mLocalEcho = false;
    private int mFontSize = 9;
    private int mColorId = 2;
    private int mControlKeyId = 0;
	
    private static final String LOCALECHO_KEY = "localecho";
    private static final String FONTSIZE_KEY = "fontsize";
    private static final String COLOR_KEY = "color";
    private static final String CONTROLKEY_KEY = "controlkey";

    public static final int WHITE = 0xffffffff;
    public static final int BLACK = 0xff000000;
    public static final int BLUE = 0xff344ebd;

    private static final int[][] COLOR_SCHEMES = {
        {BLACK, WHITE}, {WHITE, BLACK}, {WHITE, BLUE}};

//    private static final String[] CONTROL_KEY_NAME = {
//        "Ball", "@", "Left-Alt", "Right-Alt"
//    };
    private static String[] CONTROL_KEY_NAME;

    private int mControlKeyCode;

    //private SharedPreferences mPrefs;
	
	private MenuItem mMenuItemConnect;
	
	private ImageView iv1,iv2,iv3,iv4,iv5,iv6,iv7,iv8;
	private ImageView ivSw1,ivSw2,ivSw3,ivSw4,ivSw5,ivSw6,ivSw7,ivSw8;
	private byte ledStatus;
	private byte receiveCnt = 0;
	private byte [] receiveBuf = new byte [8];
	
	
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
    		
        super.onCreate(savedInstanceState);


        // Set up the window layout
        requestWindowFeature(Window.FEATURE_CUSTOM_TITLE);
        setContentView(R.layout.main);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);	// 화면 안 꺼지도록


        getWindow().setFeatureInt(Window.FEATURE_CUSTOM_TITLE, R.layout.custom_title);        


/*
        // Set up the custom title
        mTitle = (TextView) findViewById(R.id.title_left_text);
        mTitle.setText(R.string.app_name);
        mTitle = (TextView) findViewById(R.id.title_right_text);  
        
        iv1 = (ImageView) findViewById(R.id.led1); 
        iv2 = (ImageView) findViewById(R.id.led2); 
        iv3 = (ImageView) findViewById(R.id.led3); 
        iv4 = (ImageView) findViewById(R.id.led4); 
        iv5 = (ImageView) findViewById(R.id.led5); 
        iv6 = (ImageView) findViewById(R.id.led6); 
        iv7 = (ImageView) findViewById(R.id.led7); 
        iv8 = (ImageView) findViewById(R.id.led8); 

        ivSw1 = (ImageView) findViewById(R.id.sw1); 
        ivSw2 = (ImageView) findViewById(R.id.sw2); 
        ivSw3 = (ImageView) findViewById(R.id.sw3); 
        ivSw4 = (ImageView) findViewById(R.id.sw4); 
        ivSw5 = (ImageView) findViewById(R.id.sw5); 
        ivSw6 = (ImageView) findViewById(R.id.sw6); 
        ivSw7 = (ImageView) findViewById(R.id.sw7); 
        ivSw8 = (ImageView) findViewById(R.id.sw8); 
        
        iv1.setOnTouchListener(onBtnTouchListener);
        iv2.setOnTouchListener(onBtnTouchListener);        
        iv3.setOnTouchListener(onBtnTouchListener);        
        iv4.setOnTouchListener(onBtnTouchListener);        
        iv5.setOnTouchListener(onBtnTouchListener);        
        iv6.setOnTouchListener(onBtnTouchListener);        
        iv7.setOnTouchListener(onBtnTouchListener);        
        iv8.setOnTouchListener(onBtnTouchListener);        
        
        */
        
	mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();

	if (mBluetoothAdapter == null) {
            finishDialogNoBluetooth();
            return;
	}

        mSerialService = new BluetoothSerialService(this, mHandlerBT);            
    }	


	@Override
	public void onStart() {
		super.onStart();
		if (DEBUG)
			Log.e(LOG_TAG, "++ ON START ++");
		
		mEnablingBT = false;	
		
	}

	@Override
	public synchronized void onResume() {
		super.onResume();

		if (DEBUG) {
			Log.e(LOG_TAG, "+ ON RESUME +");
		}	
		
		
		if (!mEnablingBT) { // If we are turning on the BT we cannot check if it's enable
		    if ( (mBluetoothAdapter != null)  && (!mBluetoothAdapter.isEnabled()) ) {
			
                AlertDialog.Builder builder = new AlertDialog.Builder(this);
                builder.setMessage(R.string.alert_dialog_turn_on_bt)
                    .setIcon(android.R.drawable.ic_dialog_alert)
                    .setTitle(R.string.alert_dialog_warning_title)
                    .setCancelable( false )
                    .setPositiveButton(R.string.alert_dialog_yes, new DialogInterface.OnClickListener() {
                    	public void onClick(DialogInterface dialog, int id) {
                    		mEnablingBT = true;
                    		Intent enableIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
                    		startActivityForResult(enableIntent, REQUEST_ENABLE_BT);			
                    	}
                    })
                    .setNegativeButton(R.string.alert_dialog_no, new DialogInterface.OnClickListener() {
                    	public void onClick(DialogInterface dialog, int id) {
                    		finishDialogNoBluetooth();            	
                    	}
                    });
                AlertDialog alert = builder.create();
                alert.show();
}		
		
		    if (mSerialService != null) {
		    	// Only if the state is STATE_NONE, do we know that we haven't started already
		    	if (mSerialService.getState() == BluetoothSerialService.STATE_NONE) {
		    		// Start the Bluetooth chat services
		    		mSerialService.start();
		    	}
		    }

		    if (mBluetoothAdapter != null) {
		    	//readPrefs();
		    	//updatePrefs();

		    	//mEmulatorView.onResume();
		    }
		}
	}
    @Override
    public void onStop() {
        super.onStop();
        if(DEBUG)
        	Log.e(LOG_TAG, "-- ON STOP --");
        
            
    }


	@Override
	public void onDestroy() {
		super.onDestroy();
		if (DEBUG)
			Log.e(LOG_TAG, "--- ON DESTROY ---");
		
        if (mSerialService != null)
        	mSerialService.stop();
        
	}
	
	protected void onPause() {
		super.onPause();
	}
	 
    public int getConnectionState() {
		return mSerialService.getState();
	}


    public void send(byte[] out) {
    	mSerialService.write( out );
    }

    public int getTitleHeight() {
    	return mTitle.getHeight();
    }
    
    // The Handler that gets information back from the BluetoothService
    private final Handler mHandlerBT = new Handler() {
    	
        @Override
        public void handleMessage(Message msg) {        	
            switch (msg.what) {
            case MESSAGE_STATE_CHANGE:
                if(DEBUG) Log.i(LOG_TAG, "MESSAGE_STATE_CHANGE: " + msg.arg1);
                switch (msg.arg1) {
                case BluetoothSerialService.STATE_CONNECTED:
                	if (mMenuItemConnect != null) {
                		mMenuItemConnect.setIcon(android.R.drawable.ic_menu_close_clear_cancel);
                		mMenuItemConnect.setTitle(R.string.disconnect);
                	}
                	
                	//mInputManager.showSoftInput(mEmulatorView, InputMethodManager.SHOW_IMPLICIT);
                	
                    mTitle.setText(R.string.title_connected_to);
                    mTitle.append(mConnectedDeviceName);
                    break;
                    
                case BluetoothSerialService.STATE_CONNECTING:
                    mTitle.setText(R.string.title_connecting);
                    break;
                    
                case BluetoothSerialService.STATE_LISTEN:
                case BluetoothSerialService.STATE_NONE:
                	if (mMenuItemConnect != null) {
                		mMenuItemConnect.setIcon(android.R.drawable.ic_menu_search);
                		mMenuItemConnect.setTitle(R.string.connect);
                	}

            		//mInputManager.hideSoftInputFromWindow(mEmulatorView.getWindowToken(), 0);
                	
                    mTitle.setText(R.string.title_not_connected);
                    break;
                }
                break;
            case MESSAGE_WRITE:
            	if (mLocalEcho) {
            		byte[] writeBuf = (byte[]) msg.obj;
            		//mEmulatorView.write(writeBuf, msg.arg1);
            	}
                
                break;
                
            case MESSAGE_READ:

                int Cnt;
                
            	byte[] readBuf = (byte[]) msg.obj;              
                //mEmulatorView.write(readBuf, msg.arg1);
                
                Cnt = (byte) msg.arg1;
                

                	
                if(DEBUG){
                	Log.e(LOG_TAG, "MESSAGE READ : "+Cnt);    
                	Log.e(LOG_TAG, readBuf[0]+","+readBuf[1]+","+readBuf[2]+","+readBuf[3]+","+readBuf[4]+","+readBuf[5]+","+readBuf[6]+","+readBuf[7]);    
                }  
                
                for(int i = 0; i < Cnt; i++){
                	receiveBuf[receiveCnt] = readBuf[i];
                	receiveCnt++;
                }                
                
                if(receiveCnt >= 8){
                	receiveBufDecoding();
                	receiveCnt = 0;
                }
                
                
                
                break;
               
            case MESSAGE_DEVICE_NAME:
                // save the connected device's name
                mConnectedDeviceName = msg.getData().getString(DEVICE_NAME);
                Toast.makeText(getApplicationContext(), "Connected to "
                               + mConnectedDeviceName, Toast.LENGTH_SHORT).show();
                break;
            case MESSAGE_TOAST:
                Toast.makeText(getApplicationContext(), msg.getData().getString(TOAST),
                               Toast.LENGTH_SHORT).show();
                break;
            }
        }
    };    

    
    public void receiveBufDecoding() {
    	if((receiveBuf[0] == 0x0b)&&(receiveBuf[1] == 0x0b)){
    		
    		if((receiveBuf[2] == 0x03)&&(receiveBuf[3] == 0x07)){
    			
                if(DEBUG){
                	Log.e(LOG_TAG, "Decoding : " + receiveBuf[5]);
                	
                }
                /*
                if((receiveBuf[5] & 0x01) == 0x01){
                	ivSw1.setImageResource(R.drawable.gpio_on);
                } else {
                	ivSw1.setImageResource(R.drawable.gpio_off);                	
                }
                if((receiveBuf[5] & 0x02) == 0x02){
                	ivSw2.setImageResource(R.drawable.gpio_on);
                } else {
                	ivSw2.setImageResource(R.drawable.gpio_off);                	
                }
                if((receiveBuf[5] & 0x04) == 0x04){
                	ivSw3.setImageResource(R.drawable.gpio_on);
                } else {
                	ivSw3.setImageResource(R.drawable.gpio_off);                	
                }
                if((receiveBuf[5] & 0x08) == 0x08){
                	ivSw4.setImageResource(R.drawable.gpio_on);
                } else {
                	ivSw4.setImageResource(R.drawable.gpio_off);                	
                }
                if((receiveBuf[5] & 0x10) == 0x10){
                	ivSw5.setImageResource(R.drawable.gpio_on);
                } else {
                	ivSw5.setImageResource(R.drawable.gpio_off);                	
                }
                if((receiveBuf[5] & 0x20) == 0x20){
                	ivSw6.setImageResource(R.drawable.gpio_on);
                } else {
                	ivSw6.setImageResource(R.drawable.gpio_off);                	
                }
                if((receiveBuf[5] & 0x40) == 0x40){
                	ivSw7.setImageResource(R.drawable.gpio_on);
                } else {
                	ivSw7.setImageResource(R.drawable.gpio_off);                	
                }
                if((receiveBuf[5] & 0x80) == 0x80){
                	ivSw8.setImageResource(R.drawable.gpio_on);
                } else {
                	ivSw8.setImageResource(R.drawable.gpio_off);                	
                }
                */
    			
    		}
    		
    	}
    }
    
    
    public void finishDialogNoBluetooth() {
        AlertDialog.Builder builder = new AlertDialog.Builder(this);
        builder.setMessage(R.string.alert_dialog_no_bt)
        .setIcon(android.R.drawable.ic_dialog_info)
        .setTitle(R.string.app_name)
        .setCancelable( false )
        .setPositiveButton(R.string.alert_dialog_ok, new DialogInterface.OnClickListener() {
                   public void onClick(DialogInterface dialog, int id) {
                       finish();            	
                	   }
               });
        AlertDialog alert = builder.create();
        alert.show(); 
    }
    

  
    
    
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        MenuInflater inflater = getMenuInflater();
        inflater.inflate(R.menu.option_menu, menu);
        mMenuItemConnect = menu.getItem(0);
        return true;
    }  
    
    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
        case R.id.connect:
        	
        	if (getConnectionState() == BluetoothSerialService.STATE_NONE) {
        		// Launch the DeviceListActivity to see devices and do scan
        		Intent serverIntent = new Intent(this, DeviceListActivity.class);
        		startActivityForResult(serverIntent, REQUEST_CONNECT_DEVICE);
        	}
        	else
            	if (getConnectionState() == BluetoothSerialService.STATE_CONNECTED) {
            		mSerialService.stop();
		    		mSerialService.start();
            	}
            return true;
        case R.id.menu_Info:
            doDocumentKeys();
            return true;
        }
        return false;
    }
        


    
    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        if(DEBUG) Log.d(LOG_TAG, "onActivityResult " + resultCode);
        switch (requestCode) {
        
        case REQUEST_CONNECT_DEVICE:

            // When DeviceListActivity returns with a device to connect
            if (resultCode == Activity.RESULT_OK) {
                // Get the device MAC address
                String address = data.getExtras()
                                     .getString(DeviceListActivity.EXTRA_DEVICE_ADDRESS);
                // Get the BLuetoothDevice object
                BluetoothDevice device = mBluetoothAdapter.getRemoteDevice(address);
                // Attempt to connect to the device
                mSerialService.connect(device);                
            }
            break;

        case REQUEST_ENABLE_BT:
            // When the request to enable Bluetooth returns
            if (resultCode == Activity.RESULT_OK) {
                Log.d(LOG_TAG, "BT not enabled");
                
                finishDialogNoBluetooth();                
            }
        }
    }

    private void doDocumentKeys() {
        new AlertDialog.Builder(this).
            setTitle("Program Ver : ").
            setMessage("2.0.0\n").
            show();
     }
    
    public void sendLedStatus() {
    	byte[] buffer = new byte[8];
    	
        if (DEBUG){
			Log.e(LOG_TAG, "LED Status : "+ledStatus);        
		}  
    	
    	buffer[0] = (byte)0x0b;
    	buffer[1] = (byte)0x0b;
    	buffer[2] = (byte)0x03;
    	buffer[3] = (byte)0x07;
    	buffer[4] = (byte)0x00;
    	buffer[5] = (byte)ledStatus;
    	buffer[6] = (byte)0x0f;
    	buffer[7] = (byte)0x0f;
 
    	mSerialService.write(buffer);  	 	
    }    
    

    
    /********************
     * Listener
     ********************/
    /*
    private OnTouchListener onBtnTouchListener = new OnTouchListener(){
   
        public boolean onTouch(View v, MotionEvent $e)
        {
     	   if (mSerialService == null) {
     		   return false;
     	   }
     	   
     	   
            switch ($e.getAction())
            {
                case MotionEvent.ACTION_DOWN:
                	switch(v.getId()){
                	case R.id.led1:
                        if((ledStatus & 0x01) == 0x01){
                        	iv1.setImageResource(R.drawable.gpio_b);
                        	ledStatus = (byte) (ledStatus & (~0x01));                        	
                        } else {
                        	iv1.setImageResource(R.drawable.gpio_w); 
                        	ledStatus = (byte) (ledStatus | 0x01);
                        }
                        break;
                	case R.id.led2:
                        if((ledStatus & 0x02) == 0x02){
                        	iv2.setImageResource(R.drawable.gpio_b);
                        	ledStatus = (byte) (ledStatus & (~0x02));                        	
                        } else {
                        	iv2.setImageResource(R.drawable.gpio_w); 
                        	ledStatus = (byte) (ledStatus | 0x02);
                        }
                        break;
                	case R.id.led3:
		                if((ledStatus & 0x04) == 0x04){
		                	iv3.setImageResource(R.drawable.gpio_b);
		                	ledStatus = (byte) (ledStatus & (~0x04));                        	
		                } else {
		                	iv3.setImageResource(R.drawable.gpio_w); 
		                	ledStatus = (byte) (ledStatus | 0x04);
		                }
		                break;
                	case R.id.led4:
		                if((ledStatus & 0x08) == 0x08){
		                	iv4.setImageResource(R.drawable.gpio_b);
		                	ledStatus = (byte) (ledStatus & (~0x08));                        	
		                } else {
		                	iv4.setImageResource(R.drawable.gpio_w); 
		                	ledStatus = (byte) (ledStatus | 0x08);
		                }
		                break;
                	case R.id.led5:
	                    if((ledStatus & 0x10) == 0x10){
	                    	iv5.setImageResource(R.drawable.gpio_b);
	                    	ledStatus = (byte) (ledStatus & (~0x10));                        	
	                    } else {
	                    	iv5.setImageResource(R.drawable.gpio_w); 
	                    	ledStatus = (byte) (ledStatus | 0x10);
	                    }
	                    break;
                	case R.id.led6:
	                    if((ledStatus & 0x20) == 0x20){
	                    	iv6.setImageResource(R.drawable.gpio_b);
	                    	ledStatus = (byte) (ledStatus & (~0x20));                        	
	                    } else {
	                    	iv6.setImageResource(R.drawable.gpio_w); 
	                    	ledStatus = (byte) (ledStatus | 0x20);
	                    }
	                    break;
                	case R.id.led7:
		                if((ledStatus & 0x40) == 0x40){
		                	iv7.setImageResource(R.drawable.gpio_b);
		                	ledStatus = (byte) (ledStatus & (~0x40));                        	
                	    } else {
		                	iv7.setImageResource(R.drawable.gpio_w); 
		                	ledStatus = (byte) (ledStatus | 0x40);
		                }
		                break;
                	case R.id.led8:
		                if((ledStatus & 0x80) == 0x80){
		                	iv8.setImageResource(R.drawable.gpio_b);
		                	ledStatus = (byte) (ledStatus & (~0x80));                        	
		                } else {
		                	iv8.setImageResource(R.drawable.gpio_w); 
		                	ledStatus = (byte) (ledStatus | 0x80);
		                }
		                break;
                	}
              	
                	sendLedStatus();                	
                    break;
                 
                case MotionEvent.ACTION_UP:
                    if (DEBUG){
            			Log.e(LOG_TAG, "ACTION_UP");        
            		}                	

                    break;
                 
                default:
                    break;
            }
            return false;
        }
    };      
    */
    
}
   