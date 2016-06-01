using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Data;
using System.Web.UI.HtmlControls;

public partial class Terminals_Terminals : System.Web.UI.Page
{
    protected DataTable allTerminals, lastHealthTest;
    string rawHTMLError = string.Empty, rawHTMLSuccess = string.Empty;
    double totalDays;
    protected void Page_Load(object sender, EventArgs e)
    {
        allTerminals = DB.Instance.GetTerminals();
        if (!IsPostBack)
        {
            if (Session["id"] == null)
                Response.Redirect("~/Login.aspx");
            TerminalsRep.DataSource = allTerminals;
            TerminalsRep.DataBind();
        }
    }

    protected void PrintTerminals()
    {
        string rawHTML = string.Empty;
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
        if (lastHealthTest.Rows.Count != 0)
            for (int i = 1; i < lastHealthTest.Columns.Count; i++)
                if (Convert.ToInt32(lastHealthTest.Rows[0][i]) != 0)
                    return false;
        return true;
    }

    protected void PrintStatus()
    {
        if (!rawHTMLError.Equals(string.Empty))
            Response.Write(Methods.ReturnHTMLStatus(rawHTMLError, "error", false));
        if (!rawHTMLSuccess.Equals(string.Empty))
            Response.Write(Methods.ReturnHTMLStatus(rawHTMLSuccess, "success", false));
        rawHTMLError = string.Empty;
        rawHTMLSuccess = string.Empty;
    }
    protected void TerminalsRep_ItemDataBound(object sender, RepeaterItemEventArgs e)
    {
        HtmlTableCell lastSeenCell = (e.Item.Controls[3] as HtmlTableCell);
        HtmlTableCell healthStateCell = (e.Item.Controls[5] as HtmlTableCell);
        lastHealthTest = DB.Instance.GetPosLastHealthTest(Convert.ToInt32(allTerminals.Rows[e.Item.ItemIndex][0]));
        DateTime lastSeen;
        if (DateTime.TryParse(lastSeenCell.InnerText, out lastSeen))
            totalDays = DateTime.Now.Subtract(lastSeen).TotalDays;
        lastSeenCell.Attributes.CssStyle.Add("color", "white");
        healthStateCell.Attributes.CssStyle.Add("color", "white");
        if (totalDays > 7)
            lastSeenCell.Attributes.CssStyle.Add("background-color", "#F51616");
        else
            lastSeenCell.Attributes.CssStyle.Add("background-color", "#10EB14");

        if (GetHealthStatus())
        {
            healthStateCell.InnerHtml = "Running";
            healthStateCell.Attributes.CssStyle.Add("background-color", "#10EB14");
        }
        else
        {
            healthStateCell.InnerText = "Error";
            healthStateCell.Attributes.CssStyle.Add("background-color", "#F51616");
        }
    }
    protected void FreezeTerminalBtn_Click(object sender, EventArgs e)
    {
        bool selected = false;
        for (int i = 0; i < TerminalsRep.Items.Count; i++)
        {
            if ((TerminalsRep.Items[i].Controls[1] as CheckBox).Checked)
            {
                if (DB.Instance.FreezeTerminal(Convert.ToInt32(allTerminals.Rows[i][0])))
                    rawHTMLSuccess += "<li>" + allTerminals.Rows[i][0] + " frozen successfully</li>";
                else
                    rawHTMLError += "<li>Database error, please contact database admin</li>";
                selected = true;
            }
        }
        if (!selected)
        {
            rawHTMLError += "<li>Please select at least one terminal</li>";
            return;
        }
        for (int i = 0; i < TerminalsRep.Items.Count; i++)
            (TerminalsRep.Items[i].Controls[1] as CheckBox).Checked = false;
        SelectAllTerminals.Checked = false;

        TerminalsRep.DataSource = DB.Instance.GetTerminals();
        TerminalsRep.DataBind();
    }
}

