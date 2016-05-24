using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Collections;
using System.IO;
using System.Net;

public partial class Terminal : System.Web.UI.Page
{
    DataTable terminalHistory, terminalHealthTest, terminalAppList, terminalFileList;
    DataRow commandToSend;
    int posId;
    string rawHTMLError = string.Empty, rawHTMLSuccess = string.Empty;
    protected void Page_Load(object sender, EventArgs e)
    {
        if (!IsPostBack)
            if (Session["id"] == null)
                Response.Redirect("~/Login.aspx");
        posId = Convert.ToInt32(Context.Request.Params["id"]);
        terminalHistory = DB.Instance.GetPosCommandLog(posId);
        terminalHealthTest = DB.Instance.GetPosHealthTest(posId);
        terminalAppList = DB.Instance.GetPosAppList(posId);
        terminalFileList = DB.Instance.GetPosFileList(posId);
        commandToSend = DB.Instance.GetCommandToSend(posId);
    }

    private void ClearCheckBoxes()
    {
        foreach (Control ctrl in TerminalForm.Controls)
        {
            if (ctrl is CheckBox)
            {
                (ctrl as CheckBox).Checked = false;
            }
        }
    }

    protected void PrintStatus()
    {
        if (!rawHTMLError.Equals(string.Empty))
            Response.Write(Methods.ReturnHTMLStatus(rawHTMLError, "error"));
        if (!rawHTMLSuccess.Equals(string.Empty))
            Response.Write(Methods.ReturnHTMLStatus(rawHTMLSuccess, "success"));
        rawHTMLError = string.Empty;
        rawHTMLSuccess = string.Empty;
    }

    protected void PrintTerminalHistory()
    {
        string rawHTML = string.Empty;
        for (int i = 0; i < terminalHistory.Rows.Count; i++)
        {
            if (terminalHistory.Rows[i][1].ToString().Split(';').Length > 1)
            {
                string[] commandStatus = terminalHistory.Rows[i][1].ToString().Split(';');
                string[] commandParams = terminalHistory.Rows[i][3].ToString().Split(';');

                for (int j = 0; j < commandStatus.Length -1; j++)
                {
                    rawHTML += "<tr>";
                    rawHTML += "<td>" + terminalHistory.Rows[i][0] + "</td>";
                    rawHTML += "<td>" + (commandStatus[j].Equals("0") ? "Success":"Failed") + "</td>";
                    rawHTML += "<td>" + terminalHistory.Rows[i][2] + "</td>";
                    rawHTML += "<td>" + commandParams[j] + "</td>";
                    rawHTML += "</tr>";
                }
            }
            else
            {
                rawHTML += "<tr>";
                rawHTML += "<td>" + terminalHistory.Rows[i][0] + "</td>";
                rawHTML += "<td>" + (terminalHistory.Rows[i][1].ToString().Equals("0") ? "Success" : "Failed") + "</td>";
                rawHTML += "<td>" + terminalHistory.Rows[i][2] + "</td>";
                rawHTML += "<td>" + (terminalHistory.Rows[i][3].ToString().Equals(string.Empty) ? "N/A" : terminalHistory.Rows[i][3].ToString()) + "</td>";
                rawHTML += "</tr>";
            }
        }
        Response.Write(rawHTML);
    }
    protected void PrintTerminalHealthTest()
    {
        string rawHTML = string.Empty;
        for (int i = 0; i < terminalHealthTest.Rows.Count; i++)
        {
            rawHTML += "<tr>";
            for (int j = 0; j < 6; j++)
            {
                rawHTML += "<td>" + (terminalHealthTest.Rows[i][j].ToString().Equals("0") ? "Success" : "Error") + "</td>";
            }

            rawHTML += "<td>" + Convert.ToInt64(terminalHealthTest.Rows[i][6]) / 1024 +"</td>";
            rawHTML += "<td>" + Convert.ToInt64(terminalHealthTest.Rows[i][7]) / 1024 + "</td>";
            rawHTML += "<td>" + terminalHealthTest.Rows[i][8] + "</td>";
            rawHTML += "</tr>";
        }
        Response.Write(rawHTML);
    }

    protected void PrintTerminalAppList()
    {
        string rawHTML = string.Empty;
        for (int i = 0; i < terminalAppList.Rows.Count; i++)
        {
            rawHTML += "<tr>";
            for (int j = 0; j < 3; j++)
            {
                rawHTML += "<td>" + terminalAppList.Rows[i][j] + "</td>";
            }

            rawHTML += "</tr>";
        }
        Response.Write(rawHTML);
    }

    protected void PrintTerminalFileList()
    {
        string rawHTML = string.Empty;
        for (int i = 0; i < terminalFileList.Rows.Count; i++)
        {
            rawHTML += "<tr>";

            if (terminalFileList.Rows[i][2].ToString().Equals("MTMS"))
                rawHTML += "<td>" + "pub/MTMS/" + terminalFileList.Rows[i][0] + "</td>";
            else
                rawHTML += "<td>" + terminalFileList.Rows[i][2] + "/" + terminalFileList.Rows[i][0] + "</td>";

            rawHTML += "<td>" + terminalFileList.Rows[i][1] + "</td>";

            if (terminalFileList.Rows[i][2].ToString().Equals("pri"))
                rawHTML += "<td>Private</td>";
            else
                rawHTML += "<td>Public</td>";

            rawHTML += "</tr>";
        }
        Response.Write(rawHTML);
    }

    private string formatString(string param)
    {
        if (param.Equals(string.Empty))
            return "N/A";
        string formatParam = param.Remove(param.Length-1);
        return formatParam.Replace(";", ", ");
    }
    protected void PrintCommandToSend()
    {
        if (commandToSend != null)
        {
            string rawHTML = string.Empty, formatParam = string.Empty;
            BitArray cmd = new BitArray(new int[] { Convert.ToInt32(commandToSend["Command"]) });
            foreach (AppParameters.Commands commandIndex in Enum.GetValues(typeof(AppParameters.Commands)))
            {
                if (cmd.Get((int)(commandIndex)))
                {
                    rawHTML += "<tr>";
                    rawHTML += "<td>" + commandIndex.ToString() + "</td>";
                    
                    switch (commandIndex)
                    {
                        case AppParameters.Commands.DELETE_FILE:
                            rawHTML += "<td>" + formatString(commandToSend["DeleteFileParameters"].ToString()) + "</td>";
                            break;
                        case AppParameters.Commands.PUSH_FILE:
                            rawHTML += "<td>" + formatString(commandToSend["PushFileParameters"].ToString()) + "</td>";
                            break;
                        case AppParameters.Commands.PULL_FILE:
                            rawHTML += "<td>" + formatString(commandToSend["PullFileParameters"].ToString()) + "</td>";
                            break;
                        case AppParameters.Commands.UPDATE_APP:
                            rawHTML += "<td>" + formatString(commandToSend["UpdateAppParameters"].ToString()) + "</td>";
                            break;
                        default:
                            rawHTML += "<td>N/A</td>";
                            break;
                    }
                    rawHTML += "</tr>";
                }
            }
            Response.Write(rawHTML);
        }
    }
    protected void TerminalSubmitBtn_Click(object sender, EventArgs e)
    {
        //TODO: Handle delete files, modify exception handling
        BitArray cmd = new BitArray(32, false);
        int ongoingCommand = 0, index = 0;
        string updateAppParams, pullFileParams, pushFileParams, deleteFileParams, serNumber = string.Empty, vendor = string.Empty;
        DataRow terminalInfo = DB.Instance.GetTerminalInfo(posId);
        if (terminalInfo != null)
        {
            serNumber = terminalInfo["SerialNumber"].ToString();
            vendor = terminalInfo["Vendor"].ToString();
        }
        string appPath = "Apps/" + vendor + "/", filesPath = "Terminals/" + vendor + "/" + serNumber + "/pub/MTMS/";
        try
        {
            foreach (Control ctrl in TerminalForm.Controls)
            {
                if (index == 7)
                    break;
                if (ctrl is CheckBox)
                {
                    cmd.Set((int)Enum.Parse(typeof(AppParameters.Commands), ctrl.ID), (ctrl as CheckBox).Checked);
                    index++;
                }
            }
            int[] newCommand = new int[1];
            cmd.CopyTo(newCommand, 0);

            if (UPDATE_APP.Checked)
            {
                if (update_app_src.HasFile)
                    if (!Methods.UploadToFtp(Path.GetFileName(update_app_src.FileName), update_app_src.PostedFile.ContentLength, appPath, update_app_src.PostedFile.InputStream))
                        rawHTMLError += "<li>" + Path.GetFileName(update_app_src.FileName) + " cannot be uploaded</li>";
                    else
                        rawHTMLSuccess += "<li>" + Path.GetFileName(update_app_src.FileName) + " uploaded successfully</li>";
            }
            if (PULL_FILE.Checked)
            {
                if (pull_file_src.HasFile)
                    if (!Methods.UploadToFtp(Path.GetFileName(pull_file_src.FileName), pull_file_src.PostedFile.ContentLength, filesPath, pull_file_src.PostedFile.InputStream))
                        rawHTMLError += "<li>" + Path.GetFileName(pull_file_src.FileName) + " cannot be uploaded</li>";
                    else
                        rawHTMLSuccess += "<li>" + Path.GetFileName(pull_file_src.FileName) + " uploaded successfully</li>";
            }

            if (Convert.ToInt32(commandToSend["Command"]) != 0)
                ongoingCommand = newCommand[0] | Convert.ToInt32(commandToSend["Command"]);
            else
                ongoingCommand = newCommand[0];

            updateAppParams = commandToSend["UpdateAppParameters"].ToString();
            pullFileParams = commandToSend["PullFileParameters"].ToString();
            pushFileParams = commandToSend["PushFileParameters"].ToString();
            deleteFileParams = commandToSend["DeleteFileParameters"].ToString();
            if (PUSH_FILE.Checked)
            {
                //TODO:: Check with both terminals file names and exact paths
                if (hotlist_file.Checked)
                    pushFileParams += "pub/hotlist.txt;";
                if (tms_conf.Checked)
                    pushFileParams += "pub/MTMS/tms_configuration.txt;";
                if (trsc_conf.Checked)
                    pushFileParams += "pub/trsc_configuration.txt;";
            }
            if (PULL_FILE.Checked)
                pullFileParams += pull_file_src.FileName + ";";

            if (UPDATE_APP.Checked)
                updateAppParams += update_app_src.FileName + ";";


            if (!DB.Instance.UpdateCommandToSend(Convert.ToInt32(commandToSend["Id"]), ongoingCommand, string.Empty, updateAppParams, deleteFileParams, pushFileParams, pullFileParams))
                rawHTMLError += "<li>" + "Database error, please contact Database adminstrator</li>";
            else
                rawHTMLSuccess += "<li>" + "Commands submitted successfully</li>";
            ClearCheckBoxes();
            commandToSend = DB.Instance.GetCommandToSend(posId);
        }
        catch (WebException ex)
        {
            Response.StatusCode = 500;
            Response.StatusDescription = ex.Message;
            Response.Write(ex.Message);
        }
    }
}