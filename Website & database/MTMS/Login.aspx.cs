using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

public partial class login : System.Web.UI.Page
{
    string rawHTMLError = string.Empty;
    protected void Page_Load(object sender, EventArgs e)
    {
        if (!IsPostBack && Session["id"] != null)
            Response.Redirect("~/Index.aspx");
    }

    protected void Login_Click(object sender, EventArgs e)
    {
        if (DB.Instance.CheckCredentials(inputEmail.Text, inputPassword.Text))
        {
            Session["id"] = inputEmail.Text;
            Response.Redirect("~/Index.aspx");
        }
        else
            rawHTMLError = "Invalid username or password";
    }

    protected void PrintStatus()
    {
        if (!rawHTMLError.Equals(string.Empty))
            Response.Write(Methods.ReturnHTMLStatus(rawHTMLError, "error"));
        rawHTMLError = string.Empty;
    }
}