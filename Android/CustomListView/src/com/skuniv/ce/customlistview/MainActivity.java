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
	    	// 노선번호목록조회 URL
			String serviceUrl = "http://www.culture.go.kr/openapi/rest/ticketdiscounts";
			// 서울교통정보센터 OPen API 인증키 
			String serviceKey = "d3i8sul3Uc8Rd6G1gmkr+GfEi7BIXCNr9XaPN4UI5nRF+DXbzSG55nuQC2KGGHXGmBNjFEMuToST0ofSIwS6VQ=="; 
			// 서비스 키의 URL 인코딩
			serviceKey = URLEncoder.encode(serviceKey);
			Log.i("msg",serviceKey);
/*			
			// 검색할 버스 노선번호 
			String strSrch = "5500";
*/			// 노선번호목록조회 요청 URL
			String strURL = serviceUrl+"?ServiceKey="+serviceKey;
	    	// 웹 상의 리소스 지정
	    	 

	        URL url = new URL(strURL);
			
	        // XML Pull Parser를 만들기 위한 XmlPullParserFactory의 인스턴스 생성 
	        XmlPullParserFactory factory = XmlPullParserFactory.newInstance();
	        // XML Pull Parser의 인스턴스 생성 
	        XmlPullParser parser = factory.newPullParser();
	        // 웹 리소스에 대한 Input Stream 설정 
	        parser.setInput(url.openStream(), "utf-8");
	        
            // 현재 이벤트 타입(START_TAG, END_TAG, TEXT, etc)을 반환함	        
	        int eventType = parser.getEventType();
	        // 태그 이름(<busRouteNm>, <stStationNm>, <edStationNm>)의 검색여부 초기치 
	        boolean bSet = false;
	        
	        // 이벤트 타입이 도큐먼트의 마지막이 아니면 반복 
	        while (eventType != XmlPullParser.END_DOCUMENT) {
	        	
	            switch (eventType) {
	            
	                // 이벤트 타입이 도큐먼트의 시작인 경우
	                case XmlPullParser.START_DOCUMENT:
	                    break;
	                    
	                // 이벤트 타입이 START_TAG인 경우(예: <busRouteNm>)
	                case XmlPullParser.START_TAG:
	                	// 태그 이름을 추출함
	                    String tag = parser.getName();
	                    // 태그 이름이 <busRouteNm>, <stStationNm>, <edStationNm>인 경우  
	                    if (tag.equals("imgUrl") || tag.equals("title")|| tag.equals("price") || tag.equals("place")) {
	                        bSet = true;
	                    }
	                    break;
	                    
	                // 이벤트 타입이 END_TAG인 경우(예: </busRouteNm>)    
	                case XmlPullParser.END_TAG:
	                    break;
	                    
	                // 이벤트 타입이 태그 사이의 데이터인 경우는 데이터 추출 (예: <busRouteNm>데이터</busRouteNm>)       
	                case XmlPullParser.TEXT:
	                	// 태그(<busRouteNm>, <stStationNm>, <edStationNm>) 사이의 데이터 추출
	                    if (bSet) {
	                    	// 데이터 추출 
	                        String content = parser.getText();
	                        // 데이터 출력 
	                        et.append(content + "\n");
	                        // 검색여부 초기치 재설정
	                        bSet = false;
	                    }
	                    break;
	                    
		            // 이벤트 타입이 도큐먼트의 끝인 경우
	                case XmlPullParser.END_DOCUMENT:
	                    break;
	                    
	            } // switch
	            
	            // 다음 이벤트 타입을 할당 
	            eventType = parser.next();
	            
	        } // while
	    } catch (Exception e) {
	    	// 메시지 출력
	      //  Toast.makeText(v.getContext(), e.getMessage(), 0).show();
	    } // try 
    
    }

}