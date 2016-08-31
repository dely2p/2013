package me.croute.example.custom.listview;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;

import me.croute.example.custom.listview.adapter.CustomAdapter;
import me.croute.example.custom.listview.info.TweetInfo;

import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.client.ClientProtocolException;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.params.HttpConnectionParams;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.ListView;


/**
 * Custom ListView Example
 * http://croute.me/413
 * 
 * @author croute
 * @since 2011.05.04
 */
public class CustomListViewExampleActivity extends Activity implements OnClickListener
{
	private static String mUrl = "http://search.twitter.com/search.json?q=아이+이유&page=1";
	
	private ArrayList<TweetInfo> mTweetList;
	private CustomAdapter mAdapter;
	private ListView mLvTweets;
	private String mResult;
	
	/* (non-Javadoc)
	 * @see android.app.Activity#onCreate(android.os.Bundle)
	 */
	@Override
	public void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		setContentView(R.layout.custom_list_view_example_activity);
		
		Button bGetTweets = (Button)findViewById(R.id.custom_list_view_example_activity_b_get_tweets);
		Button bSetListView = (Button)findViewById(R.id.custom_list_view_example_activity_b_set_listview);
		Button bRefresh = (Button)findViewById(R.id.custom_list_view_example_activity_b_refresh);
		mLvTweets = (ListView)findViewById(R.id.custom_list_view_example_activity_lv_list);
		
		bGetTweets.setOnClickListener(this);
		bSetListView.setOnClickListener(this);
		bRefresh.setOnClickListener(this);
		
		mTweetList = new ArrayList<TweetInfo>();
		mAdapter = new CustomAdapter(this, R.layout.list_view_row, mTweetList);
		mLvTweets.setAdapter(mAdapter);
	}

	/**
	 * 트윗 검색 결과를 얻어옵니다.
	 */
	private void getTweets()
	{
		HttpClient client = new DefaultHttpClient();
		HttpConnectionParams.setConnectionTimeout(client.getParams(), 10000);
		HttpGet httpGet = new HttpGet(mUrl);
		
		try
		{
			HttpResponse response = client.execute(httpGet);
			processEntity(response.getEntity());
		}
		catch(ClientProtocolException e)
		{
			e.printStackTrace();
		}
		catch(IOException e)
		{
			e.printStackTrace();
		}
	}
	
	/**
	 * 트윗검색결과를 처리합니다.
	 * 
	 * @param entity
	 * @throws IllegalStateException
	 * @throws IOException
	 */
	private void processEntity(HttpEntity entity) throws IllegalStateException, IOException
	{
		BufferedReader br = new BufferedReader(new InputStreamReader(entity.getContent(), "UTF-8"));
		String line, result = "";

		while((line = br.readLine()) != null)
		{
			result += line;
		}
		
		mResult = result;		
	}
	
	/**
	 * 데이터를 리스트뷰에 세팅합니다. 
	 */
	private void setListView()
	{
		mTweetList.clear();
		
		try
		{
			JSONArray array = new JSONObject(mResult).getJSONArray("results");
			
			int arrayLength = array.length();
			
			for(int i = 0 ; i < arrayLength ; i++)
			{
				JSONObject object = array.getJSONObject(i);
				TweetInfo tweet = new TweetInfo();
				
				tweet.setFrom_user_id_str(object.getString("from_user_id_str"));
				tweet.setProfile_image_url((object.getString("profile_image_url")));
				tweet.setCreated_at(object.getString("created_at"));
				tweet.setFrom_user(object.getString("from_user"));
				tweet.setId_str(object.getString("id_str"));
				tweet.setMetadata(object.getJSONObject("metadata").getString("result_type"));
				tweet.setTo_user_id(object.getString("to_user_id"));
				tweet.setText(object.getString("text"));
				tweet.setId(object.getInt("id"));
				tweet.setFrom_user_id(object.getInt("from_user_id"));
				tweet.setTo_user(object.isNull("to_user") ? null : object.getString("to_user"));
				tweet.setGeo(object.isNull("geo") ? null : object.getString("geo"));
				tweet.setIso_language_code(object.isNull("iso_language_code") ? null : object.getString("iso_language_code"));
				tweet.setTo_user_id_str(object.getString("to_user_id_str"));
				tweet.setSource(object.getString("source"));
				
				mTweetList.add(tweet);
			}
			
			mAdapter.notifyDataSetChanged();
		}
		catch(JSONException e)
		{
			e.printStackTrace();
		}
	}

	/**
	 * 간단한 다이얼로그 사용을 위한 메소드
	 * 
	 * @param text
	 */
	private void getAlertDialog(String text)
	{
		DialogInterface.OnClickListener okayListener = new DialogInterface.OnClickListener()
		{
			@Override
			public void onClick(DialogInterface dialog, int which)
			{
				dialog.dismiss();
			}
		};
		
		new AlertDialog.Builder(this)
		.setTitle("CustomListView Example")
		.setMessage(text)
		.setPositiveButton("확인", okayListener)
		.show();
	}
	
	/* (non-Javadoc)
	 * @see android.view.View.OnClickListener#onClick(android.view.View)
	 */
	@Override
	public void onClick(View v)
	{
		switch(v.getId())
		{
		
		// get Tweets 버튼 클릭
		case R.id.custom_list_view_example_activity_b_get_tweets:
			getTweets();
			getAlertDialog("트윗 받아오기 완료");
			break;
		
		// set ListView 버튼 클릭
		case R.id.custom_list_view_example_activity_b_set_listview:
			setListView();
			getAlertDialog("리스트뷰 세팅 완료");
			break;
			
		// refresh 버튼 클릭
		case R.id.custom_list_view_example_activity_b_refresh:
			getTweets();
			setListView();
			getAlertDialog("새로고침 완료");
			break;
			
		}
	}
}