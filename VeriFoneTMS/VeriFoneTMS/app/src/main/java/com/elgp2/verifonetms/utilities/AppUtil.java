package com.elgp2.verifonetms.utilities;

import java.util.regex.Pattern;

/**
 * Created by POS_GP_TEAM_2016 on 5/1/2016.
 */
public class AppUtil {

    private AppUtil(){
    }

    public static String getCompanyName(String packageName){

        //remove the company domain (com. , net.,etc)
        packageName = packageName.substring(4);
        if(packageName.contains("."))
            return packageName.split("\\.")[0];
        else
            return  packageName;

    }
}
