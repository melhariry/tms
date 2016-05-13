using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

public partial class Terminals_Index : System.Web.UI.Page
{
    protected void Page_Load(object sender, EventArgs e)
    {
        PopulateGrid();
    }

    private void PopulateGrid()
    {
        TerminalsGrid.DataSource = (new DB()).GetTerminals();
        TerminalsGrid.DataBind();
    }
    protected void TerminalsGrid_DataBound(object sender, EventArgs e)
    {
        for (int i = 0; i < TerminalsGrid.Rows.Count; i++)
        {
            HyperLink terminalShow = new HyperLink();
            terminalShow.NavigateUrl = "./Edit.aspx?id=" + TerminalsGrid.Rows[i].Cells[0].Text;
            terminalShow.Text = TerminalsGrid.Rows[i].Cells[0].Text;
            TerminalsGrid.Rows[i].Cells[0].Controls.Add(terminalShow);
        }
    }
}