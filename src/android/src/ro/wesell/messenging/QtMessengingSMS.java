package ro.wesell.messenging;

import ro.wesell.messenging.Base64Util;

import android.app.Activity;
import android.app.PendingIntent;

import android.database.Cursor;
import android.database.ContentObserver;
import android.database.sqlite.SQLiteException;

import android.content.Intent;
import android.content.IntentFilter;
import android.content.Context;
import android.content.BroadcastReceiver;
import android.content.ContentValues;

import android.widget.Toast;

import android.net.Uri;

import android.util.Log;

import android.telephony.SmsManager;

import java.util.ArrayList;
import java.text.SimpleDateFormat;

import java.sql.Date;

public class QtMessengingSMS extends BroadcastReceiver{
    static long qtObject = 0;
    private static Activity m_activity = null;

    public native void onMsgReceived(long qtObject, long threadid);
    public native void onMsgSendOut(long qtObject, long msgid, boolean isSuccess);
    public native void onMsgDelivered(long qtObject, long msgid);
    public native void onMsgConversionCreated(long qtObject, long threadid);

    final String SENT_SMS_ACTION = "SENT_SMS_ACTION";
    final String DELIVERED_SMS_ACTION = "DELIVERED_SMS_ACTION";
    final static String RECEIVE_SMS_ACTION = "android.provider.Telephony.SMS_RECEIVED";
    final static String RECEIVE_SMS_ACTION2 = "android.provider.Telephony.SMS_DELIVER";


    //default constructor
    public QtMessengingSMS(){}

    //set Qt Activity as static to allow dirrect usage
    static public void setActivity(Activity activity)
    {
        Log.d("Set Activity","Complete");
        m_activity = activity;
    }

    public void onReceive(Context context, Intent intent)
    {
        if (intent.getAction().equals("android.provider.Telephony.SMS_DELIVER")
            || intent.getAction().equals("android.provider.Telephony.SMS_RECEIVED") )
        {
            if (qtObject == 0)
                return ;

            onMsgReceived(qtObject, 0);
        }
    }
    private BroadcastReceiver receiver = new BroadcastReceiver() {
        public void onReceive(Context context, Intent intent)
        {
            synchronized (m_activity) {
                if (qtObject == 0)
                    return;
                onMsgDelivered(qtObject, 0);
                Toast.makeText(m_activity, "SMS Sent", Toast.LENGTH_LONG).show();
            }
        }
    };
    private BroadcastReceiver sendMessage = new BroadcastReceiver() {
        public void onReceive(Context context, Intent intent)
        {
            synchronized (m_activity) {
                if (qtObject == 0)
                    return;

                boolean sendSuccessed = (getResultCode() == Activity.RESULT_OK ? true : false);
                onMsgSendOut(qtObject, 0, sendSuccessed);
                Toast.makeText(m_activity, "Delivered " + (sendSuccessed ? "YES" : "NO"), Toast.LENGTH_LONG).show();
            }
        }
    };
    public String sendMessage(long threadid, String phoneNumber, String message)
    {
        SmsManager sms = SmsManager.getDefault();

        // set the sms state listener
        Intent sentIntent = new Intent(SENT_SMS_ACTION);
        PendingIntent sentPI = PendingIntent.getBroadcast(m_activity, 0, sentIntent, 0);

        Intent deliverIntent = new Intent(DELIVERED_SMS_ACTION);
        PendingIntent deliverPI = PendingIntent.getBroadcast(m_activity, 0, deliverIntent, 0);

        // split message
        if (message.length() > 70) {
            ArrayList<String> msgs = sms.divideMessage(message);
            for (String msg : msgs) {
                sms.sendTextMessage(phoneNumber, null, msg, sentPI, deliverPI);
            }
        } else {
            sms.sendTextMessage(phoneNumber, null, message, sentPI, deliverPI);
        }

        m_activity.registerReceiver(sendMessage, new IntentFilter(SENT_SMS_ACTION));
        m_activity.registerReceiver(receiver, new IntentFilter(DELIVERED_SMS_ACTION));

        // write current message into sms db
        long timeStamp = System.currentTimeMillis();
        ContentValues values = new ContentValues();
        values.put("date", timeStamp);
        values.put("read", 0);
        values.put("type", 2);
        values.put("address", phoneNumber);
        values.put("body", message);
        values.put("protocol", 0);
        values.put("status", 64);

        m_activity.getContentResolver().insert(Uri.parse("content://sms/sent"), values);

        final String SMS_URI_SENT = "content://sms/sent";
        StringBuilder dataBuilder = new StringBuilder();

        try {
            Uri uri = Uri.parse(SMS_URI_SENT);
            String[] projection = new String[] { "_id", "address", "person", "body",
                                                 "date", "type", "protocol", "read", "status", "thread_id" };
            Cursor cursor = m_activity.getContentResolver().query(uri, projection, "date = " + timeStamp, null, "date desc");

            if (cursor.moveToFirst()) {

                if (threadid >= 0 && threadid == cursor.getLong(9)) {
                    SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
                    Date d = new Date(cursor.getLong(4));
                    String strDate = dateFormat.format(d);

                    dataBuilder.append(cursor.getLong(0));
                    dataBuilder.append(",");
                    dataBuilder.append(cursor.getString(1));
                    dataBuilder.append(",");
                    dataBuilder.append(cursor.getString(2));
                    dataBuilder.append(",");
                    dataBuilder.append(Base64Util.encodeString(cursor.getString(3)));
                    dataBuilder.append(",");
                    dataBuilder.append(strDate);
                    dataBuilder.append(",");
                    dataBuilder.append(cursor.getLong(5));
                    dataBuilder.append(",");
                    dataBuilder.append(cursor.getLong(6));
                    dataBuilder.append(",");
                    dataBuilder.append(cursor.getLong(7));
                    dataBuilder.append(",");
                    dataBuilder.append(cursor.getLong(8));
                    dataBuilder.append("\n");
                }

                if (qtObject != 0 && threadid < 0)
                    onMsgConversionCreated(qtObject, cursor.getLong(9));

                if (!cursor.isClosed()) {
                    cursor.close();
                    cursor = null;
                }
            }

        } catch (SQLiteException ex) {
            Log.d("SQLiteException in readConversionData", ex.getMessage());
        }

        return dataBuilder.toString();
    }
}
