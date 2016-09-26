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
<<<<<<< HEAD
        TerminalsGrid.DataSource = (new DB()).GetTerminals();
=======
        TerminalsGrid.DataSource = DB.Instance.GetTerminals();
>>>>>>> 0750e108d81bcc258ea5450c4233212a647e8222
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