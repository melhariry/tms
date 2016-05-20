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
        //FTP Server URL.
        string ftp = "ftp://localhost/MTMS_FTP/";

        //FTP Folder name. Leave blank if you want to upload to root folder.
        string ftpFolder = HttpContext.Current.Request.Params["id"] + "/";

        byte[] fileBytes = null;

        try
        {
            //Read the FileName and convert it to Byte array.
            string fileName = Path.GetFileName(FileUpload1.FileName);
            using (StreamReader fileStream = new StreamReader(FileUpload1.PostedFile.InputStream))
            {
                fileBytes = Encoding.UTF8.GetBytes(fileStream.ReadToEnd());
                fileStream.Close();
            }
            //Create FTP Request.
            FtpWebRequest request = (FtpWebRequest)WebRequest.Create(ftp + ftpFolder + fileName);
            request.Method = WebRequestMethods.Ftp.UploadFile;

            //Enter FTP Server credentials.
            request.Credentials = new NetworkCredential("anonymous", "ibrahim@mail.com");
            request.ContentLength = fileBytes.Length;


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

}