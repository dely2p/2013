package com.skuniv.ce.customlistview;

import java.net.URL;
import java.net.URLEncoder;

import org.xmlpull.v1.XmlPullParser;
import org.xmlpull.v1.XmlPullParserFactory;

import android.R;
import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.widget.EditText;


public class MainActivity extends Activity{
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        
        EditText et = (EditText)findViewById(R.id.edittext);
		
        try {
	    	// �뼱��ȣ�����ȸ URL
			String serviceUrl = "http://www.culture.go.kr/openapi/rest/ticketdiscounts";
			// ���ﱳ���������� OPen API ����Ű 
			String serviceKey = "d3i8sul3Uc8Rd6G1gmkr+GfEi7BIXCNr9XaPN4UI5nRF+DXbzSG55nuQC2KGGHXGmBNjFEMuToST0ofSIwS6VQ=="; 
			// ���� Ű�� URL ���ڵ�
			serviceKey = URLEncoder.encode(serviceKey);
			Log.i("msg",serviceKey);
/*			
			// �˻��� ���� �뼱��ȣ 
			String strSrch = "5500";
*/			// �뼱��ȣ�����ȸ ��û URL
			String strURL = serviceUrl+"?ServiceKey="+serviceKey;
	    	// �� ���� ���ҽ� ����
	    	 

	        URL url = new URL(strURL);
			
	        // XML Pull Parser�� ����� ���� XmlPullParserFactory�� �ν��Ͻ� ���� 
	        XmlPullParserFactory factory = XmlPullParserFactory.newInstance();
	        // XML Pull Parser�� �ν��Ͻ� ���� 
	        XmlPullParser parser = factory.newPullParser();
	        // �� ���ҽ��� ���� Input Stream ���� 
	        parser.setInput(url.openStream(), "utf-8");
	        
            // ���� �̺�Ʈ Ÿ��(START_TAG, END_TAG, TEXT, etc)�� ��ȯ��	        
	        int eventType = parser.getEventType();
	        // �±� �̸�(<busRouteNm>, <stStationNm>, <edStationNm>)�� �˻����� �ʱ�ġ 
	        boolean bSet = false;
	        
	        // �̺�Ʈ Ÿ���� ��ť��Ʈ�� �������� �ƴϸ� �ݺ� 
	        while (eventType != XmlPullParser.END_DOCUMENT) {
	        	
	            switch (eventType) {
	            
	                // �̺�Ʈ Ÿ���� ��ť��Ʈ�� ������ ���
	                case XmlPullParser.START_DOCUMENT:
	                    break;
	                    
	                // �̺�Ʈ Ÿ���� START_TAG�� ���(��: <busRouteNm>)
	                case XmlPullParser.START_TAG:
	                	// �±� �̸��� ������
	                    String tag = parser.getName();
	                    // �±� �̸��� <busRouteNm>, <stStationNm>, <edStationNm>�� ���  
	                    if (tag.equals("imgUrl") || tag.equals("title")|| tag.equals("price") || tag.equals("place")) {
	                        bSet = true;
	                    }
	                    break;
	                    
	                // �̺�Ʈ Ÿ���� END_TAG�� ���(��: </busRouteNm>)    
	                case XmlPullParser.END_TAG:
	                    break;
	                    
	                // �̺�Ʈ Ÿ���� �±� ������ �������� ���� ������ ���� (��: <busRouteNm>������</busRouteNm>)       
	                case XmlPullParser.TEXT:
	                	// �±�(<busRouteNm>, <stStationNm>, <edStationNm>) ������ ������ ����
	                    if (bSet) {
	                    	// ������ ���� 
	                        String content = parser.getText();
	                        // ������ ��� 
	                        et.append(content + "\n");
	                        // �˻����� �ʱ�ġ �缳��
	                        bSet = false;
	                    }
	                    break;
	                    
		            // �̺�Ʈ Ÿ���� ��ť��Ʈ�� ���� ���
	                case XmlPullParser.END_DOCUMENT:
	                    break;
	                    
	            } // switch
	            
	            // ���� �̺�Ʈ Ÿ���� �Ҵ� 
	            eventType = parser.next();
	            
	        } // while
	    } catch (Exception e) {
	    	// �޽��� ���
	      //  Toast.makeText(v.getContext(), e.getMessage(), 0).show();
	    } // try 
    
    }

}