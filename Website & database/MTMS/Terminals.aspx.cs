using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Data;

public partial class Terminals_Terminals : System.Web.UI.Page
{
    private DataTable allTerminals, lastHealthTest;
    protected void Page_Load(object sender, EventArgs e)
    {
        if (!IsPostBack)
            if (Session["id"] == null)
                Response.Redirect("~/Login.aspx");
        allTerminals = DB.Instance.GetTerminals();
    }

    protected void PrintTerminals()
    {
        string rawHTML = string.Empty;
        double totalDays;
        for (int i = 0; i < allTerminals.Rows.Count; i++)
        {
            lastHealthTest = DB.Instance.GetPosLastHealthTest(Convert.ToInt32(allTerminals.Rows[i][0]));
            rawHTML += "<tr> <td class=\"sorting_1\">";
            rawHTML += "<a href='terminal.aspx?id=" + allTerminals.Rows[i][0] + "'>";
            rawHTML += allTerminals.Rows[i][0] + "</a></td>";
            for (int j = 1; j < allTerminals.Columns.Count; j++)
            {
                if (j == 4)
                {
                    totalDays = DateTime.Now.Subtract((DateTime)allTerminals.Rows[i][j]).TotalDays;
                    if (totalDays > 7)
                        rawHTML += "<td style=\"background-color:#F51616; color:white;\">" + allTerminals.Rows[i][j] + "</td>";
                    else
                        rawHTML += "<td style=\"background-color:#10EB14; color:white;\">" + allTerminals.Rows[i][j] + "</td>";
                }
                else
                    rawHTML += "<td>" + allTerminals.Rows[i][j] + "</td>";
            }
            if (GetHealthStatus())
                rawHTML += "<td style=\"background-color:#10EB14; color:white;\">Running</td>";
            else
                rawHTML += "<td style=\"background-color:#F51616; color:white;\">Error</td>";

            rawHTML += "</tr>";
        }
        Response.Write(rawHTML);
    }
    private bool GetHealthStatus()
    {
        for (int i = 1; i < lastHealthTest.Columns.Count; i++)
            if (Convert.ToInt32(lastHealthTest.Rows[0][i]) != 0)
                return false;
        return true;
    }
}

