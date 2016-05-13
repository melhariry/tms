using System;
using System.Collections;
using System.Collections.Generic;
using System.Data;
using System.IO;
using System.Linq;
using System.Net;
using System.Text;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

public partial class Terminals_Show : System.Web.UI.Page
{
    int commandId;
    protected void Page_Load(object sender, EventArgs e)
    {
        PopulateGrid();
        DataRow commandToSend = (new DB()).GetCommandToSend(Convert.ToInt32(HttpContext.Current.Request.Params["id"]));
        if (commandToSend != null)
        {
            BitArray cmd = new BitArray(new int[] { Convert.ToInt32(commandToSend["Command"]) }) ;
            commandId = Convert.ToInt32(commandToSend["Id"]);
            foreach (AppParameters.Commands commandIndex in Enum.GetValues(typeof(AppParameters.Commands)))
            {
                CheckBox box = new CheckBox();
                box.Text = commandIndex.ToString();
                box.Checked = cmd.Get((int)(commandIndex));
                CommandsDiv.Controls.Add(box);
            }
        }
        //TODO: Redirect with error mo7taram
        else
            HttpContext.Current.Response.Redirect("./Index.aspx?error=2");
    }

    private void PopulateGrid()
    {
        FileListGrid.DataSource = (new DB()).GetTerminalFileList(Convert.ToInt32(HttpContext.Current.Request.Params["id"]));
        FileListGrid.DataBind();
    }

    protected void Submit_Click(object sender, EventArgs e)
    {
        BitArray cmd = new BitArray(32,false);
        int index = 0;
        foreach (Control ctrl in CommandsDiv.Controls)
            if (ctrl is CheckBox)
                cmd.Set(index++,(ctrl as CheckBox).Checked);

        int[] command = new int[1];
        cmd.CopyTo(command, 0);
        if ((new DB()).UpdateCommandToSend(commandId, command[0]))
            HttpContext.Current.Response.Redirect("./Index.aspx?success");

        //TODO: Redirect with error mo7taram
        else
            HttpContext.Current.Response.Redirect("./Index.aspx?error=2");

    }
    protected void Test_Click(object sender, EventArgs e)
    {
        DataRow terminalInfo = (new DB()).GetTerminalInfo(Convert.ToInt32(HttpContext.Current.Request.Params["id"]));
        string ftpDirectory = "ftp://localhost/Terminals/" + terminalInfo["Vendor"] + "/" + terminalInfo["SerialNumber"] + "/";
        byte[] fileBytes = null;

        try
        {
            string fileName = Path.GetFileName(FileUpload1.FileName);
            using (StreamReader fileStream = new StreamReader(FileUpload1.PostedFile.InputStream))
            {
                fileBytes = Encoding.UTF8.GetBytes(fileStream.ReadToEnd());
                fileStream.Close();
            }

            FtpWebRequest request = (FtpWebRequest)WebRequest.Create(ftpDirectory + fileName);
            request.Method = WebRequestMethods.Ftp.UploadFile;
            request.Credentials = new NetworkCredential("MTMS_FTP", "1234");
            request.ContentLength = fileBytes.Length;
            request.KeepAlive = false;


            using (Stream requestStream = request.GetRequestStream())
            {
                requestStream.Write(fileBytes, 0, fileBytes.Length);
                requestStream.Close();
            }
            FtpWebResponse response = (FtpWebResponse)request.GetResponse();
            response.Close();
            
        }
        catch (WebException)
        {
            //TODO: Redirect with error mo7taram
            HttpContext.Current.Response.Redirect("./Index.aspx?error=3");
        }
    }

    protected void FileListGrid_DataBound(object sender, EventArgs e)
    {
        for (int i = 0; i < FileListGrid.Rows.Count; i++)
        {
            CheckBox selectFile = new CheckBox();
            FileListGrid.Rows[i].Cells[0].Controls.Add(selectFile);
        }

    }
}