package com.andro;

import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.InputStreamReader;

import org.json.JSONArray;
import org.json.JSONObject;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;

public class JSONActivity extends Activity implements OnClickListener {
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        
        // 버튼 클릭 대기: 시작 
        Button btn = (Button)findViewById(R.id.button_call);
        btn.setOnClickListener(this);
        // 버튼 클릭 대기: 끝 
    }

	// @Override
	public void onClick(View v) {
		
		// 고객정보 출력 영역 인식
		EditText et_webpage_src = (EditText)findViewById(R.id.webpage_src);
		
		try {     
            // 데이터를 다운로드하여 버퍼에 저장  			
			BufferedInputStream buf  = new BufferedInputStream(this.getResources().openRawResource(R.raw.customers_json));    
			// 버퍼의 데이터를 'euc-kr' 형태로 변환 
        	BufferedReader bufreader = new BufferedReader(new InputStreamReader(buf, "utf-8"));
        	
        	String line  = null;
			String page  = "";
        	
        	// 버퍼의 웹문서 소스를 줄(line) 단위로 읽어 page에 저장함 
        	while ((line = bufreader.readLine()) != null) {
        		page += line;
        	}
        	
	        // 읽어들인 JSON 포맷의 데이어틀 JSON 객체로 변환
    		JSONObject json = new JSONObject(page);
    		// customers에 해당하는 배열을 할당  
    		JSONArray  jArr = json.getJSONArray("customers");
    		
    		// 배열의 크기만큼 반복하면서, name과 address의 값을 추출함 
    		for (int i=0; i<jArr.length(); i++) {
    			
    			// i번째 배열 할당  
        		json = jArr.getJSONObject(i);
        		
        		// name과 address의 값을 추출함
        		String name    = json.getString("name");
        		String address = json.getString("address");
        		
        		// name과 address의 값을 출력함
    			et_webpage_src.append(name + "\n");
    			et_webpage_src.append(address + "\n");
    		}
    		
        } catch (Exception e){
        	// 에러 메시지 출력 
        	et_webpage_src.append(e.getMessage());
		} finally {  
		} 
	} 
}