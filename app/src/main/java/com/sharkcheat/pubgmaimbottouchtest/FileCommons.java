package com.sharkcheat.pubgmaimbottouchtest;

import android.content.Context;
import android.util.Log;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStream;

public class FileCommons {

    public static String ReadFileData(File path){
        StringBuilder builder = new StringBuilder();
        if (path.exists()){
            try {
                BufferedReader bufferedReader = new BufferedReader(new FileReader(path));
                String line;
                while ((line = bufferedReader.readLine()) != null) {
                    builder.append(line);
                }
                bufferedReader.close();
            } catch (IOException e) {
                Log.e("RCG", e.getMessage());
                e.printStackTrace();
            }
        } else {
            Log.e("RCG", path.toString() + " not exist");
        }
        return builder.toString();
    }

    public static boolean copyFromAssets(Context ctx, String outPath, String fileName)
    {
        File file = new File(outPath);
        if (!file.exists()) {
            if (!file.mkdirs())
            {
                Log.e("RCG", "Can't create directory.");
                return false;
            }
        }
        try {
            InputStream inputStream = ctx.getAssets().open(fileName);
            File outFile = new File(file, fileName);
            FileOutputStream fileOutputStream = new FileOutputStream(outFile);
            byte[] buffer = new byte[1024];
            int byteRead;
            while (-1 != (byteRead = inputStream.read(buffer)))
            {
                fileOutputStream.write(buffer, 0, byteRead);
            }
            inputStream.close();
            fileOutputStream.flush();
            fileOutputStream.close();
            return true;
        } catch (IOException e) {
            e.printStackTrace();
            return false;
        }
    }

}
