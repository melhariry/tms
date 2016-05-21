using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Data;

public partial class Group : System.Web.UI.Page
{
    DataTable groupTerminals;
    protected void Page_Load(object sender, EventArgs e)
    {
        groupTerminals = DB.Instance.GetGroupTerminals(Convert.ToInt32(Context.Request.Params["id"]));
    }
    protected void PrintGroupTerminals()
    {
        string rawHTML = string.Empty;
        for (int i = 0; i < groupTerminals.Rows.Count; i++)
        {
            rawHTML += "<tr> <td class=\"sorting_1\">";
            rawHTML += "<a href='terminal.aspx?id=" + groupTerminals.Rows[i][0] + "'>";
            rawHTML += groupTerminals.Rows[i][0] + "</a></td>";
            for (int j = 1; j < 7; j++)
                rawHTML += "<td>" + groupTerminals.Rows[i][j] + "</td>";
            rawHTML += "</tr>";
        }
        Response.Write(rawHTML);
    }
}