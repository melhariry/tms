using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Data;

public partial class Terminals_Terminals : System.Web.UI.Page
{
    private DataTable allTerminals;
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
        for (int i = 0; i < allTerminals.Rows.Count; i++)
        {
            rawHTML += "<tr> <td class=\"sorting_1\">";
            rawHTML += "<a href='terminal.aspx?id=" + allTerminals.Rows[i][0] + "'>";
            rawHTML += allTerminals.Rows[i][0] + "</a></td>";
            for (int j = 1; j < 8; j++)
                rawHTML += "<td>" + allTerminals.Rows[i][j] + "</td>";
            rawHTML += "</tr>";
        }
        Response.Write(rawHTML);
    }
}