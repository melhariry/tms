using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Data;

public partial class groups : System.Web.UI.Page
{
    private DataTable allGroups;
    protected void Page_Load(object sender, EventArgs e)
    {
        if (!IsPostBack)
            if (Session["id"] == null)
                Response.Redirect("~/Login.aspx");
        allGroups = DB.Instance.GetPosGroups();
    }
    protected void PrintGroups()
    {
        string rawHTML = string.Empty;
        for (int i = 0; i < allGroups.Rows.Count; i++)
        {
            rawHTML += "<tr> <td class=\"sorting_1\">";
            rawHTML += "<a href='group.aspx?id=" + allGroups.Rows[i][0] + "&";
            rawHTML += "name=" + allGroups.Rows[i][1] + "'>";
            rawHTML += allGroups.Rows[i][0] + "</a></td>";
            for (int j = 1; j < 4; j++)
                rawHTML += "<td>" + allGroups.Rows[i][j] + "</td>";
            rawHTML += "</tr>";
        }
        Response.Write(rawHTML);
    }
}