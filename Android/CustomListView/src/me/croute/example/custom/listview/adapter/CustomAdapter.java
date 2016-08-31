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

			// �̹��� ������ ������ �Ѿ �����̹Ƿ� �����Ѵ�.

			// ��ũ������ ����
			tvScreenName.setText(tweet.getFrom_user());

			// �ۼ� �ð� ����, UTC ���ؽð�����  �Ǿ�������, �츮���� �ð����� �ٲٴ� ���� ������ ���Ե��� �ʴ� �����̹Ƿ� �����Ѵ�.
			tvCreatedAt.setText(tweet.getCreated_at());
			
			// �ؽ�Ʈ ����
			tvText.setText(Html.fromHtml(tweet.getText()));
		}

		return convertView;
	}
}
