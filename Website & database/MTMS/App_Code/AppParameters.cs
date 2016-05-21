using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

/// <summary>
/// Summary description for AppParameters
/// </summary>
public static class AppParameters
{
    public enum Commands
    {
        //LSB To MSB
        TEST_HEALTH,
        LIST_FILES,
        LIST_APPS,
        DELETE_FILE,
        PUSH_FILE,
        PULL_FILE,
        UPDATE_APP,
        CREATE_POS_RECORD
    }
}