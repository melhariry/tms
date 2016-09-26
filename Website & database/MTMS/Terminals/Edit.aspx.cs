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
<<<<<<< HEAD
        DataRow commandToSend = (new DB()).GetCommandToSend(Convert.ToInt32(HttpContext.Current.Request.Params["id"]));
=======
        DataRow commandToSend = DB.Instance.GetCommandToSend(Convert.ToInt32(HttpContext.Current.Request.Params["id"]));
>>>>>>> 0750e108d81bcc258ea5450c4233212a647e8222
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
<<<<<<< HEAD
=======

        //if (commandToSend != null)
        //{
        //    int index = 0;
        //    BitArray cmd = new BitArray(new int[] { Convert.ToInt32(commandToSend["Command"]) });
        //    foreach (AppParameters.Commands commandIndex in Enum.GetValues(typeof(AppParameters.Commands)))
        //    {
        //        if (index == 7)
        //            break;
        //        (TerminalForm.Controls[index++] as CheckBox).Checked = cmd.Get((int)(commandIndex));
        //    }
        //}
>>>>>>> 0750e108d81bcc258ea5450c4233212a647e8222
    }

    private void PopulateGrid()
    {
<<<<<<< HEAD
        FileListGrid.DataSource = (new DB()).GetTerminalFileList(Convert.ToInt32(HttpContext.Current.Request.Params["id"]));
=======
        FileListGrid.DataSource = DB.Instance.GetTerminalFileList(Convert.ToInt32(HttpContext.Current.Request.Params["id"]));
>>>>>>> 0750e108d81bcc258ea5450c4233212a647e8222
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
<<<<<<< HEAD
        if ((new DB()).UpdateCommandToSend(commandId, command[0]))
=======
        /*if (DB.Instance.UpdateCommandToSend(commandId, command[0]))
>>>>>>> 0750e108d81bcc258ea5450c4233212a647e8222
            HttpContext.Current.Response.Redirect("./Index.aspx?success");

        //TODO: Redirect with error mo7taram
        else
<<<<<<< HEAD
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
            request.Credentials = new NetworkCredential("anonymous", "ibrahim@mail.com");
            request.ContentLength = fileBytes.Length;
            request.KeepAlive = false;


            using (Stream requestStream = request.GetRequestStream())
            {
                requestStream.Write(fileBytes, 0, fileBytes.Length);
                requestStream.Close();
            }
            FtpWebResponse response = (FtpWebResponse)request.GetResponse();
=======
            HttpContext.Current.Response.Redirect("./Index.aspx?error=2");*/

    }
    protected void Upload_Click(object sender, EventArgs e)
    {
        DataRow terminalInfo = DB.Instance.GetTerminalInfo(Convert.ToInt32(HttpContext.Current.Request.Params["id"]));
        string ftpDirectory = "ftp://localhost/Terminals/" + terminalInfo["Vendor"] + "/" + terminalInfo["SerialNumber"] + "/";
        string fileName = Path.GetFileName(FileUpload1.FileName);
        byte[] buffer = new byte[4096];
        int bytesWritten = 0;
        int totalBytes = FileUpload1.PostedFile.ContentLength;
        try
        {
            FtpWebRequest request = (FtpWebRequest)WebRequest.Create(ftpDirectory + fileName);
            request.Method = WebRequestMethods.Ftp.UploadFile;
            request.Credentials = new NetworkCredential("MTMS_FTP", "1234");
            request.ContentLength = FileUpload1.PostedFile.ContentLength;
            request.KeepAlive = false;
            request.UseBinary = true;

            using (Stream fileStream = FileUpload1.PostedFile.InputStream)
            {
                using (Stream requestStream = request.GetRequestStream())
                {
                    while (totalBytes > 0)
                    {
                        bytesWritten = fileStream.Read(buffer, 0, buffer.Length);
                        requestStream.Write(buffer, 0, bytesWritten);
                        totalBytes -= bytesWritten;
                    }
                    requestStream.Close();
                }
                fileStream.Close();
            }
            FtpWebResponse response = (FtpWebResponse)request.GetResponse();
            FtpStatus.Text = response.StatusDescription;
>>>>>>> 0750e108d81bcc258ea5450c4233212a647e8222
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
<<<<<<< HEAD
=======
    protected void Download_Click(object sender, EventArgs e)
    {
        //TODO:: pick fileName from fileList and set proper local Path
        string fileName = "xx.mp3";
        string localPath = "D:\\University\\GP\\" + fileName;

        int bytesRead = 0;
        byte[] buffer = new byte[4096];
        try
        {
            DataRow terminalInfo = DB.Instance.GetTerminalInfo(Convert.ToInt32(HttpContext.Current.Request.Params["id"]));
            string ftpDirectory = "ftp://localhost/Terminals/" + terminalInfo["Vendor"] + "/" + terminalInfo["SerialNumber"] + "/";
            FtpWebRequest request = (FtpWebRequest)WebRequest.Create(ftpDirectory + fileName);
            request.Method = WebRequestMethods.Ftp.DownloadFile;
            request.Credentials = new NetworkCredential("MTMS_FTP", "1234");
            request.ContentLength = FileUpload1.PostedFile.ContentLength;
            request.KeepAlive = false;
            request.UseBinary = true;

            using (FileStream fileStream = new FileStream(localPath, FileMode.Create))
            {
                using (Stream requestStream = request.GetResponse().GetResponseStream())
                {
                    do
                    {
                        bytesRead = requestStream.Read(buffer, 0, buffer.Length);
                        fileStream.Write(buffer, 0, bytesRead);

                    } while (bytesRead != 0);
                    requestStream.Close();
                }
                fileStream.Close();
            }
            FtpWebResponse response = (FtpWebResponse)request.GetResponse();
            FtpStatus.Text = response.StatusDescription;
            response.Close();
        }
        catch (WebException)
        {
            //TODO: Redirect with error mo7taram
            HttpContext.Current.Response.Redirect("./Index.aspx?error=3");
        }
    }
>>>>>>> 0750e108d81bcc258ea5450c4233212a647e8222
}