package me.croute.example.custom.listview.adapter;

import java.util.ArrayList;

import me.croute.example.custom.listview.R;
import me.croute.example.custom.listview.info.TweetInfo;
import android.content.Context;
import android.text.Html;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.ImageView;
import android.widget.TextView;


public class CustomAdapter extends ArrayAdapter<TweetInfo>
{
	private Context mContext;
	private int mResource;
	private ArrayList<TweetInfo> mList;
	private LayoutInflater mInflater;

	/**
	 * @param context
	 * @param layoutResource
	 * @param objects
	 */
	public CustomAdapter(Context context, int layoutResource, ArrayList<TweetInfo> objects)
	{
		super(context, layoutResource, objects);
		this.mContext = context;
		this.mResource = layoutResource;
		this.mList = objects;
		this.mInflater = (LayoutInflater) mContext.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see android.widget.ArrayAdapter#getView(int, android.view.View,
	 * android.view.ViewGroup)
	 */
	@Override
	public View getView(int position, View convertView, ViewGroup parent)
	{
		TweetInfo tweet = mList.get(position);

		if(convertView == null)
		{
			convertView = mInflater.inflate(mResource, null);
		}

		if(tweet != null)
		{
			ImageView ivThumbnail = (ImageView) convertView.findViewById(R.id.list_view_row_profile_thumbnail);
			TextView tvScreenName = (TextView) convertView.findViewById(R.id.list_view_row_user_screen_name);
			TextView tvCreatedAt = (TextView) convertView.findViewById(R.id.list_view_row_create_at);
			TextView tvText = (TextView) convertView.findViewById(R.id.list_view_row_text);

			// 이미지 세팅은 예제를 넘어선 범위이므로 생략한다.

			// 스크린네임 세팅
			tvScreenName.setText(tweet.getFrom_user());

			// 작성 시간 세팅, UTC 기준시간으로  되어있지만, 우리나라 시간으로 바꾸는 것은 예제에 포함되지 않는 내용이므로 생략한다.
			tvCreatedAt.setText(tweet.getCreated_at());
			
			// 텍스트 세팅
			tvText.setText(Html.fromHtml(tweet.getText()));
		}

		return convertView;
	}
}
