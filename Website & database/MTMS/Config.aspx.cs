using System;
using System.Collections.Generic;
using System.Data;
using System.IO;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

public partial class Config : System.Web.UI.Page
{
    List<string> time = new List<string>();
    DataTable allterminals;
    protected void Page_Load(object sender, EventArgs e)
    {
        allterminals = DB.Instance.GetTerminals();
        if (!IsPostBack)
        {
            time.Add("Hours");
            time.Add("Minutes");
            TimeList.DataSource = time;
            TimeList.DataBind();
        }
    }
    protected void GenerateConfigBtn_Click(object sender, EventArgs e)
    {
        string fileName = "TMSConfig.csv";
        using (var configFile = File.Create(fileName))
        {
            
        }
        File.Delete(fileName);
        for (int i = 0; i < allterminals.Rows.Count; i++)
        {
            
        }
    }

}