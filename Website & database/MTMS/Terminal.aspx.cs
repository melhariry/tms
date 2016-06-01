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
    DataTable terminalHistory, terminalHealthTest, terminalAppList, terminalFileList, priFiles, pubFiles, mtmsFiles;
    DataRow commandToSend, terminalInfo;
    int posId;
    string rawHTMLError = string.Empty, rawHTMLSuccess = string.Empty;
    protected void Page_Load(object sender, EventArgs e)
    {
        posId = Convert.ToInt32(Context.Request.Params["id"]);
        terminalHistory = DB.Instance.GetPosCommandLog(posId);
        terminalHealthTest = DB.Instance.GetPosHealthTest(posId);
        terminalAppList = DB.Instance.GetPosAppList(posId);
        terminalFileList = DB.Instance.GetPosFileList(posId);
        commandToSend = DB.Instance.GetCommandToSend(posId);
        terminalInfo = DB.Instance.GetTerminalInfo(posId);
        mtmsFiles = terminalFileList.Select("ParentFolder='MTMS'").Length == 0 ? new DataTable() : terminalFileList.Select("ParentFolder='MTMS'").CopyToDataTable();
        pubFiles = terminalFileList.Select("ParentFolder='pub'").Length == 0 ? new DataTable() : terminalFileList.Select("ParentFolder='pub'").CopyToDataTable();
        priFiles = terminalFileList.Select("ParentFolder='pri'").Length == 0 ? new DataTable() : terminalFileList.Select("ParentFolder='pri'").CopyToDataTable();
        Schedule.Checked = Convert.ToBoolean(commandToSend["isScheduled"]);
        if (terminalInfo["Vendor"].ToString().Equals("Castles"))
            AppValidator.ValidationExpression="(.+\\.([Cc][Aa][Bb]))";
        else
            AppValidator.ValidationExpression = "(.+\\.([Aa][Pp][Kk]))";
        if (!IsPostBack)
        {
            if (Session["id"] == null)
                Response.Redirect("~/Login.aspx");
            MTMSFileListRep.DataSource = mtmsFiles;
            MTMSFileListRep.DataBind();
            pubFileListRep.DataSource = pubFiles;
            pubFileListRep.DataBind();
            priFileListRep.DataSource = priFiles;
            priFileListRep.DataBind();
            MTMSDeleteFileRep.DataSource = mtmsFiles;
            MTMSDeleteFileRep.DataBind();
            pubDeleteFileRep.DataSource = pubFiles;
            pubDeleteFileRep.DataBind();
            priDeleteFileRep.DataSource = priFiles;
            priDeleteFileRep.DataBind();
            MTMSPushFileRep.DataSource = mtmsFiles;
            MTMSPushFileRep.DataBind();
            pubPushFileRep.DataSource = pubFiles;
            pubPushFileRep.DataBind();
            priPushFileRep.DataSource = priFiles;
            priPushFileRep.DataBind();
        }
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
        Schedule.Checked = Convert.ToBoolean(commandToSend["isScheduled"]);
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
        double ramUsed, diskUsed;
        for (int i = 0; i < terminalHealthTest.Rows.Count; i++)
        {
            rawHTML += "<tr>";
            ramUsed = Convert.ToInt64(terminalHealthTest.Rows[i][6]) / Convert.ToInt64(terminalInfo["TotalRamSize"]);
            diskUsed = Convert.ToInt64(terminalHealthTest.Rows[i][7]) / Convert.ToInt64(terminalInfo["TotalDiskCapacity"]);
            for (int j = 0; j < 6; j++)
            {
                if (terminalHealthTest.Rows[i][j].ToString().Equals("0"))
                    rawHTML += "<td>Success</td>";
                else
                    rawHTML += "<td style=\"background-color:#F51616; color:white;\">Error</td>";
            }

            if (ramUsed > 0.9)
                rawHTML += "<td style=\"background-color:#F51616; color:white;\">" + terminalHealthTest.Rows[i][6].ToString() + "/" + terminalInfo["TotalRamSize"] + "</td>";
            else
                rawHTML += "<td>" + terminalHealthTest.Rows[i][6].ToString() + "/" + terminalInfo["TotalRamSize"] + "</td>";

            if (diskUsed > 0.9)
                rawHTML += "<td style=\"background-color:#F51616; color:white;\">" + terminalHealthTest.Rows[i][7].ToString() + "/" + terminalInfo["TotalDiskCapacity"] + "</td>";
            else
                rawHTML += "<td>" + terminalHealthTest.Rows[i][7].ToString() + "/" + terminalInfo["TotalDiskCapacity"] + "</td>";
            
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

    private string FormatDeleteParams()
    {
        string param = string.Empty;
        for (int i = 0; i < MTMSDeleteFileRep.Items.Count; i++)
            if ((MTMSDeleteFileRep.Items[i].Controls[1] as CheckBox).Checked)
                param += "pub/MTMS/" + mtmsFiles.Rows[i][0] + ";";

        for (int i = 0; i < pubDeleteFileRep.Items.Count; i++)
            if ((pubDeleteFileRep.Items[i].Controls[1] as CheckBox).Checked)
                param += "pub/" + pubFiles.Rows[i][0] + ";";

        for (int i = 0; i < priDeleteFileRep.Items.Count; i++)
            if ((priDeleteFileRep.Items[i].Controls[1] as CheckBox).Checked)
                param += "pri/" + priFiles.Rows[i][0] + ";";
        return param;
    }

    private string FormatPushParams()
    {
        string param = string.Empty;
        for (int i = 0; i < MTMSPushFileRep.Items.Count; i++)
            if ((MTMSPushFileRep.Items[i].Controls[1] as CheckBox).Checked)
                param += "pub/MTMS/" + mtmsFiles.Rows[i][0] + ";";

        for (int i = 0; i < pubPushFileRep.Items.Count; i++)
            if ((pubPushFileRep.Items[i].Controls[1] as CheckBox).Checked)
                param += "pub/" + pubFiles.Rows[i][0] + ";";

        for (int i = 0; i < priPushFileRep.Items.Count; i++)
            if ((priPushFileRep.Items[i].Controls[1] as CheckBox).Checked)
                param += "pri/" + priFiles.Rows[i][0] + ";";
        return param;
    }
    protected void TerminalSubmitBtn_Click(object sender, EventArgs e)
    {
        BitArray cmd = new BitArray(32, false);
        int ongoingCommand = 0, index = 0;
        string updateAppParams, pullFileParams, pushFileParams, deleteFileParams;
        string uploadSrcs = string.Empty, uploadFiles = string.Empty;
        string appPath = "Apps/" + terminalInfo["Vendor"].ToString() + "/";
        string filesPath = "Terminals/" + terminalInfo["Vendor"].ToString() + "/" + terminalInfo["SerialNumber"].ToString() + "/pub/MTMS/";
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
                if (update_app_src.HasFiles)
                {
                    foreach (HttpPostedFile uploadedSrc in update_app_src.PostedFiles)
                    {
                        if (!Methods.UploadToFtp(uploadedSrc.FileName, uploadedSrc.ContentLength, appPath, uploadedSrc.InputStream))
                            rawHTMLError += "<li>" + uploadedSrc.FileName + " cannot be uploaded</li>";
                        else
                        {
                            rawHTMLSuccess += "<li>" + uploadedSrc.FileName + " uploaded successfully</li>";
                            uploadSrcs += uploadedSrc.FileName + ";";
                        }
                    }
                }
                else
                    rawHTMLError += "<li>No or empty app source</li>";
            }
            if (PULL_FILE.Checked)
            {
                if (pull_file_src.HasFiles)
                {
                    foreach (HttpPostedFile uploadedFile in pull_file_src.PostedFiles)
                    {
                        if (!Methods.UploadToFtp(uploadedFile.FileName, uploadedFile.ContentLength, filesPath, uploadedFile.InputStream))
                            rawHTMLError += "<li>" + uploadedFile.FileName + " cannot be uploaded</li>";
                        else
                        {
                            uploadFiles += uploadedFile.FileName + ";";
                            rawHTMLSuccess += "<li>" + uploadedFile.FileName + " uploaded successfully</li>";
                        }
                    }
                }
                else
                    rawHTMLError += "<li>No or empty file source</li>";
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
                pushFileParams += FormatPushParams();

            if (PULL_FILE.Checked)
                pullFileParams += "pub/MTMS/" + pull_file_src.FileName + ";";

            if (UPDATE_APP.Checked)
                updateAppParams += uploadSrcs;

            if (DELETE_FILE.Checked)
                deleteFileParams += FormatDeleteParams();

            if (!DB.Instance.UpdateCommandToSend(Convert.ToInt32(commandToSend["Id"]), ongoingCommand, string.Empty, updateAppParams, deleteFileParams, pushFileParams, pullFileParams, Schedule.Checked))
                rawHTMLError += "<li>" + "Database error, please contact Database adminstrator</li>";
            else
                rawHTMLSuccess += "<li>Commands submitted successfully</li>";
            ClearCheckBoxes();
            commandToSend = DB.Instance.GetCommandToSend(posId);
        }
        catch (WebException ex)
        {
            rawHTMLError += "<li>" + ex.Message + "</li>";
        }
    }
    protected void TerminalClearBtn_Click(object sender, EventArgs e)
    {
        if (!DB.Instance.ClearCommandToSend(DB.Instance.GetTerminalInfo(posId)["SerialNumber"].ToString(), false))
            rawHTMLError += "<li>" + "Database error, please contact Database adminstrator</li>";
        else
            rawHTMLSuccess += "<li>" + "Commands cleared successfully</li>";
        commandToSend = DB.Instance.GetCommandToSend(posId);
        ClearCheckBoxes();
    }
}