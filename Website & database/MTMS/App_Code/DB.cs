using System;
using System.Collections.Generic;
using System.Text;
using System.Data;
using System.Data.SqlClient;
using System.Configuration;

/// <summary>
/// Database class
/// </summary>

public sealed class DB
{
    SqlConnection Conn = new SqlConnection(ConfigurationManager.ConnectionStrings["DbConnectionString"].ToString());
    
    private static volatile DB instance = new DB();
    private static object syncRoot = new Object();
    private DB() { }
    public static DB Instance
    {
        get
        {
            if (instance == null)
            {
                lock (syncRoot)
                {
                    if (instance == null)
                        instance = new DB();
                }
            }
            return instance;
        }
    }

    public bool PosExists(string posSerialNumber, string posLastConnectionIp, out int posId)
    {
        try
        {
            SqlCommand cmd = new SqlCommand("PosExists", Conn);
            cmd.CommandType = CommandType.StoredProcedure;
            SqlDataAdapter adapter = new SqlDataAdapter(cmd);
            DataTable table = new DataTable();
            cmd.Parameters.AddWithValue("@PosSerialNumber", posSerialNumber);
            cmd.Parameters.AddWithValue("@LastConnectionIp", posLastConnectionIp);
            SqlParameter id = cmd.Parameters.Add("@RETURN_VALUE", SqlDbType.Int);
            id.Direction = ParameterDirection.ReturnValue;
            Conn.Open();
            adapter.Fill(table);
            Conn.Close();
            posId = Convert.ToInt32(id.Value);
            if (posId != 0)
                return true;
            else
                return false;
        }
        catch (Exception EX)
        {
            throw (EX);
        }
        finally
        {
            Conn.Close();
        }
    }

    public bool CreatePosRecord(string serialNumber, string vendor, string model, string ip, Int64 diskCapacity, Int64 ramSize, out int PosId)
    {
        try
        {
            SqlCommand cmd = new SqlCommand("CreatePosRecord", Conn);
            cmd.CommandType = CommandType.StoredProcedure;
            DataTable table = new DataTable();
            cmd.Parameters.AddWithValue("@SerialNumber", serialNumber);
            cmd.Parameters.AddWithValue("@Vendor", vendor);
            cmd.Parameters.AddWithValue("@Model", model);
            cmd.Parameters.AddWithValue("@LastConnectionIP", ip);
            cmd.Parameters.AddWithValue("@TotalDiskCapacity", diskCapacity);
            cmd.Parameters.AddWithValue("@TotalRamSize", ramSize);
            SqlParameter Id = cmd.Parameters.Add("@RETURN_VALUE", SqlDbType.Int);
            Id.Direction = ParameterDirection.ReturnValue;
            Conn.Open();
            cmd.ExecuteNonQuery();
            Conn.Close();
            PosId = Int32.Parse((Id.Value.ToString()));
            if (PosId != -1)
                return true;
            else
                return false;
        }
        catch (Exception EX)
        {
            throw (EX);
        }
        finally
        {
            Conn.Close();
        }
    }

    public bool SubmitHealthTestResults(string serialNumber, int crypto, int printer, int timer, int buzzer, int led, int rtc, Int64 usedDiskSize, Int64 usedRamSize)
    {
        try
        {
            SqlCommand cmd = new SqlCommand("SubmitHealthTestResults", Conn);
            cmd.CommandType = CommandType.StoredProcedure;
            DataTable table = new DataTable();
            cmd.Parameters.AddWithValue("@SerialNumber", serialNumber);
            cmd.Parameters.AddWithValue("@Crypto", crypto);
            cmd.Parameters.AddWithValue("@Printer", printer);
            cmd.Parameters.AddWithValue("@Timer", timer);
            cmd.Parameters.AddWithValue("@Buzzer", buzzer);
            cmd.Parameters.AddWithValue("@Led", led);
            cmd.Parameters.AddWithValue("@Rtc", rtc);
            cmd.Parameters.AddWithValue("@UsedDiskSize", usedDiskSize);
            cmd.Parameters.AddWithValue("@UsedRamSize", usedRamSize);
            SqlParameter rowCount = cmd.Parameters.Add("@RETURN_VALUE", SqlDbType.Int);
            rowCount.Direction = ParameterDirection.ReturnValue;
            Conn.Open();
            cmd.ExecuteNonQuery();
            Conn.Close();
            if (Int32.Parse(rowCount.Value.ToString()) == 1)
                return true;
            else
                return false;
        }
        catch (Exception EX)
        {
            throw (EX);
        }
        finally
        {
            Conn.Close();
        }
    }

    public DataTable GetTerminals()
    {
        try
        {
            SqlCommand cmd = new SqlCommand("GetTerminals", Conn);
            cmd.CommandType = CommandType.StoredProcedure;
            SqlDataAdapter adapter = new SqlDataAdapter(cmd);
            DataTable table = new DataTable();
            Conn.Open();
            adapter.Fill(table);
            Conn.Close();
            return table;
        }
        catch (Exception EX)
        {
            throw (EX);
        }
        finally
        {
            Conn.Close();
        }
    }

    public DataRow GetCommandToSend(int posId)
    {
        try
        {
            SqlCommand cmd = new SqlCommand("GetCommandToSend", Conn);
            cmd.CommandType = CommandType.StoredProcedure;
            SqlDataAdapter adapter = new SqlDataAdapter(cmd);
            DataTable table = new DataTable();
            cmd.Parameters.AddWithValue("@PosId", posId);
            Conn.Open();
            adapter.Fill(table);
            Conn.Close();
            DataRow row = null;
            if (table.Rows.Count > 0)
                row = table.Rows[0];
            return row;
        }
        catch (Exception EX)
        {
            throw (EX);
        }
        finally
        {
            Conn.Close();
        }
    }

    public bool UpdateCommandToSend(int id, int command, string updateFileParams, string updateAppParams, string deleteFileParams, string pushFileParams, string pullFileParams, bool isScheduled)
    {
        try
        {
            SqlCommand cmd = new SqlCommand("UpdateCommandToSend", Conn);
            cmd.CommandType = CommandType.StoredProcedure;

            cmd.Parameters.AddWithValue("@Id", id);
            cmd.Parameters.AddWithValue("@Command", command);
            cmd.Parameters.AddWithValue("@UpdateFileParams", updateFileParams);
            cmd.Parameters.AddWithValue("@UpdateAppParams", updateAppParams);
            cmd.Parameters.AddWithValue("@DeleteFileParams", deleteFileParams);
            cmd.Parameters.AddWithValue("@PushFileParams", pushFileParams);
            cmd.Parameters.AddWithValue("@PullFileParams", pullFileParams);
            cmd.Parameters.AddWithValue("@IsScheduled", isScheduled);
            SqlParameter rowCount = cmd.Parameters.Add("@RETURN_VALUE", SqlDbType.Int);
            rowCount.Direction = ParameterDirection.ReturnValue;
            Conn.Open();
            cmd.ExecuteNonQuery();
            Conn.Close();
            if (Convert.ToInt32(rowCount.Value) == 1)
                return true;
            else
                return false;
        }
        catch (Exception EX)
        {
            throw (EX);
        }
        finally
        {
            Conn.Close();
        }
    }

    public DataRow GetCommandParameters(string serialNumber)
    {
        try
        {
            SqlCommand cmd = new SqlCommand("GetCommandParameters", Conn);
            cmd.CommandType = CommandType.StoredProcedure;
            SqlDataAdapter adapter = new SqlDataAdapter(cmd);
            DataTable table = new DataTable();
            cmd.Parameters.AddWithValue("@SerialNumber", serialNumber);
            Conn.Open();
            adapter.Fill(table);
            Conn.Close();
            DataRow row = null;
            if (table.Rows.Count > 0)
                row = table.Rows[0];
            return row;
        }
        catch (Exception EX)
        {
            throw (EX);
        }
        finally
        {
            Conn.Close();
        }
    }

    public DataRow GetTerminalInfo(int posId)
    {
        try
        {
            SqlCommand cmd = new SqlCommand("GetTerminalInfo", Conn);
            cmd.CommandType = CommandType.StoredProcedure;
            SqlDataAdapter adapter = new SqlDataAdapter(cmd);
            DataTable table = new DataTable();
            cmd.Parameters.AddWithValue("@PosId", posId);
            Conn.Open();
            adapter.Fill(table);
            Conn.Close();
            DataRow row = null;
            if (table.Rows.Count > 0)
                row = table.Rows[0];
            return row;
        }
        catch (Exception EX)
        {
            throw (EX);
        }
        finally
        {
            Conn.Close();
        }
    }

    public DataTable GetTerminalFileList(int posId)
    {
        try
        {
            SqlCommand cmd = new SqlCommand("GetTerminalFileList", Conn);
            cmd.CommandType = CommandType.StoredProcedure;
            SqlDataAdapter adapter = new SqlDataAdapter(cmd);
            DataTable table = new DataTable();
            cmd.Parameters.AddWithValue("@PosId", posId);
            Conn.Open();
            adapter.Fill(table);
            Conn.Close();
            return table;
        }
        catch (Exception EX)
        {
            throw (EX);
        }
        finally
        {
            Conn.Close();
        }
    }

    public bool SubmitCommandResult(string serialNumber, string commandExecuted, string status, DateTime timestamp, string parameters)
    {
        try
        {
            SqlCommand cmd = new SqlCommand("SubmitCommandResult", Conn);
            cmd.CommandType = CommandType.StoredProcedure;
            DataTable table = new DataTable();
            cmd.Parameters.AddWithValue("@SerialNumber", serialNumber);
            cmd.Parameters.AddWithValue("@CommandExecuted", commandExecuted);
            cmd.Parameters.AddWithValue("@Status", status);
            cmd.Parameters.AddWithValue("@TimeStamp", timestamp);
            cmd.Parameters.AddWithValue("@Parameters", parameters);
            SqlParameter rowCount = cmd.Parameters.Add("@RETURN_VALUE", SqlDbType.Int);
            rowCount.Direction = ParameterDirection.ReturnValue;
            Conn.Open();
            cmd.ExecuteNonQuery();
            Conn.Close();
            if (Int32.Parse(rowCount.Value.ToString()) == 1)
                return true;
            else
                return false;
        }
        catch (Exception EX)
        {
            throw (EX);
        }
        finally
        {
            Conn.Close();
        }
    }

    public bool ClearCommandToSend(string serialNumber, bool serverClear = true)
    {
        try
        {
            SqlCommand cmd = new SqlCommand("ClearCommandToSend", Conn);
            cmd.CommandType = CommandType.StoredProcedure;
            DataTable table = new DataTable();
            cmd.Parameters.AddWithValue("@SerialNumber", serialNumber);
            cmd.Parameters.AddWithValue("@ServerClear", serverClear);
            SqlParameter rowCount = cmd.Parameters.Add("@RETURN_VALUE", SqlDbType.Int);
            rowCount.Direction = ParameterDirection.ReturnValue;
            Conn.Open();
            cmd.ExecuteNonQuery();
            Conn.Close();
            if (Int32.Parse(rowCount.Value.ToString()) > 0)
                return true;
            else
                return false;
        }
        catch (Exception EX)
        {
            throw (EX);
        }
        finally
        {
            Conn.Close();
        }
    }
    public bool DeletePosFileFromList(string serialNumber, string fileName, string parentFolder)
    {
        try
        {
            SqlCommand cmd = new SqlCommand("DeletePosFileFromList", Conn);
            cmd.CommandType = CommandType.StoredProcedure;
            DataTable table = new DataTable();
            cmd.Parameters.AddWithValue("@SerialNumber", serialNumber);
            cmd.Parameters.AddWithValue("@FileName", fileName);
            cmd.Parameters.AddWithValue("@ParentFolder", parentFolder);
            SqlParameter rowCount = cmd.Parameters.Add("@RETURN_VALUE", SqlDbType.Int);
            rowCount.Direction = ParameterDirection.ReturnValue;
            Conn.Open();
            cmd.ExecuteNonQuery();
            Conn.Close();
            if (Int32.Parse(rowCount.Value.ToString()) == 1)
                return true;
            else
                return false;
        }
        catch (Exception EX)
        {
            throw (EX);
        }
        finally
        {
            Conn.Close();
        }
    }

    public bool SubmitPosAppList(string serialNumber, string name, string version, string company)
    {
        try
        {
            SqlCommand cmd = new SqlCommand("SubmitPosAppList", Conn);
            cmd.CommandType = CommandType.StoredProcedure;
            DataTable table = new DataTable();
            cmd.Parameters.AddWithValue("@SerialNumber", serialNumber);
            cmd.Parameters.AddWithValue("@Name", name);
            cmd.Parameters.AddWithValue("@Version", version);
            cmd.Parameters.AddWithValue("@Company", company);
            SqlParameter rowCount = cmd.Parameters.Add("@RETURN_VALUE", SqlDbType.Int);
            rowCount.Direction = ParameterDirection.ReturnValue;
            Conn.Open();
            cmd.ExecuteNonQuery();
            Conn.Close();
            if (Int32.Parse(rowCount.Value.ToString()) > 0)
                return true;
            else
                return false;
        }
        catch (Exception EX)
        {
            throw (EX);
        }
        finally
        {
            Conn.Close();
        }
    }

    public DataRow GetTerminalsCount()
    {
        try
        {
            SqlCommand cmd = new SqlCommand("GetTerminalsCount", Conn);
            cmd.CommandType = CommandType.StoredProcedure;
            SqlDataAdapter adapter = new SqlDataAdapter(cmd);
            DataTable table = new DataTable();
            Conn.Open();
            adapter.Fill(table);
            Conn.Close();
            DataRow row = null;
            if (table.Rows.Count > 0)
                row = table.Rows[0];
            return row;
        }
        catch (Exception EX)
        {
            throw (EX);
        }
        finally
        {
            Conn.Close();
        }
    }
    
    public DataRow GetGroupsCount()
    {
        try
        {
            SqlCommand cmd = new SqlCommand("GetGroupsCount", Conn);
            cmd.CommandType = CommandType.StoredProcedure;
            SqlDataAdapter adapter = new SqlDataAdapter(cmd);
            DataTable table = new DataTable();
            Conn.Open();
            adapter.Fill(table);
            Conn.Close();
            DataRow row = null;
            if (table.Rows.Count > 0)
                row = table.Rows[0];
            return row;
        }
        catch (Exception EX)
        {
            throw (EX);
        }
        finally
        {
            Conn.Close();
        }
    }

    public DataTable GetPosGroups()
    {
        try
        {
            SqlCommand cmd = new SqlCommand("GetPosGroups", Conn);
            cmd.CommandType = CommandType.StoredProcedure;
            SqlDataAdapter adapter = new SqlDataAdapter(cmd);
            DataTable table = new DataTable();
            Conn.Open();
            adapter.Fill(table);
            Conn.Close();
            return table;
        }
        catch (Exception EX)
        {
            throw (EX);
        }
        finally
        {
            Conn.Close();
        }
    }

    public DataTable GetGroupTerminals(int groupId)
    {
        try
        {
            SqlCommand cmd = new SqlCommand("GetGroupTerminals", Conn);
            cmd.Parameters.AddWithValue("@GroupId", groupId);
            cmd.CommandType = CommandType.StoredProcedure;
            SqlDataAdapter adapter = new SqlDataAdapter(cmd);
            DataTable table = new DataTable();
            Conn.Open();
            adapter.Fill(table);
            Conn.Close();
            return table;
        }
        catch (Exception EX)
        {
            throw (EX);
        }
        finally
        {
            Conn.Close();
        }
    }

    public bool SubmitPosFileList(string serialNumber, string name, long size, string parentFolder)
    {
        try
        {
            SqlCommand cmd = new SqlCommand("SubmitPosFileList", Conn);
            cmd.CommandType = CommandType.StoredProcedure;
            DataTable table = new DataTable();
            cmd.Parameters.AddWithValue("@SerialNumber", serialNumber);
            cmd.Parameters.AddWithValue("@Name", name);
            cmd.Parameters.AddWithValue("@Size", size);
            cmd.Parameters.AddWithValue("@ParentFolder", parentFolder);
            SqlParameter rowCount = cmd.Parameters.Add("@RETURN_VALUE", SqlDbType.Int);
            rowCount.Direction = ParameterDirection.ReturnValue;
            Conn.Open();
            cmd.ExecuteNonQuery();
            Conn.Close();
            if (Int32.Parse(rowCount.Value.ToString()) > 0)
                return true;
            else
                return false;
        }
        catch (Exception EX)
        {
            throw (EX);
        }
        finally
        {
            Conn.Close();
        }
    }

    public Boolean CheckCredentials(string username, string password)
    {
        try
        {
            SqlCommand cmd = new SqlCommand("CheckCredentials", Conn);
            cmd.Parameters.AddWithValue("@Username", username);
            cmd.Parameters.AddWithValue("@Password", password);
            SqlParameter Success = cmd.Parameters.Add("@RETURN_VALUE", SqlDbType.Int);
            Success.Direction = ParameterDirection.ReturnValue;
            cmd.CommandType = CommandType.StoredProcedure;
            SqlDataAdapter adapter = new SqlDataAdapter(cmd);
            DataTable table = new DataTable();
            Conn.Open();
            adapter.Fill(table);
            Conn.Close();
            return Int32.Parse(Success.Value.ToString()) == 1;
        }
        catch (Exception EX)
        {
            throw (EX);
        }
        finally
        {
            Conn.Close();
        }
    }

    public DataTable GetPosCommandLog(int posId)
    {
        try
        {
            SqlCommand cmd = new SqlCommand("GetPosCommandLog", Conn);
            cmd.Parameters.AddWithValue("@PosId", posId);
            cmd.CommandType = CommandType.StoredProcedure;
            SqlDataAdapter adapter = new SqlDataAdapter(cmd);
            DataTable table = new DataTable();
            Conn.Open();
            adapter.Fill(table);
            Conn.Close();
            return table;
        }
        catch (Exception EX)
        {
            throw (EX);
        }
        finally
        {
            Conn.Close();
        }
    }

    public DataTable GetPosHealthTest(int posId)
    {
        try
        {
            SqlCommand cmd = new SqlCommand("GetPosHealthTest", Conn);
            cmd.Parameters.AddWithValue("@PosId", posId);
            cmd.CommandType = CommandType.StoredProcedure;
            SqlDataAdapter adapter = new SqlDataAdapter(cmd);
            DataTable table = new DataTable();
            Conn.Open();
            adapter.Fill(table);
            Conn.Close();
            return table;
        }
        catch (Exception EX)
        {
            throw (EX);
        }
        finally
        {
            Conn.Close();
        }
    }

    public DataTable GetPosAppList(int posId)
    {
        try
        {
            SqlCommand cmd = new SqlCommand("GetPosAppList", Conn);
            cmd.Parameters.AddWithValue("@PosId", posId);
            cmd.CommandType = CommandType.StoredProcedure;
            SqlDataAdapter adapter = new SqlDataAdapter(cmd);
            DataTable table = new DataTable();
            Conn.Open();
            adapter.Fill(table);
            Conn.Close();
            return table;
        }
        catch (Exception EX)
        {
            throw (EX);
        }
        finally
        {
            Conn.Close();
        }
    }

    public DataTable GetPosFileList(int posId)
    {
        try
        {
            SqlCommand cmd = new SqlCommand("GetPosFileList", Conn);
            cmd.Parameters.AddWithValue("@PosId", posId);
            cmd.CommandType = CommandType.StoredProcedure;
            SqlDataAdapter adapter = new SqlDataAdapter(cmd);
            DataTable table = new DataTable();
            Conn.Open();
            adapter.Fill(table);
            Conn.Close();
            return table;
        }
        catch (Exception EX)
        {
            throw (EX);
        }
        finally
        {
            Conn.Close();
        }
    }

    public bool CreatePosGroup(string name, string branch, string contactPerson, string contactNumber, out int GroupId)
    {
        try
        {
            SqlCommand cmd = new SqlCommand("CreatePosGroup", Conn);
            cmd.CommandType = CommandType.StoredProcedure;
            DataTable table = new DataTable();
            cmd.Parameters.AddWithValue("@Name", name);
            cmd.Parameters.AddWithValue("@Branch", branch);
            cmd.Parameters.AddWithValue("@ContactPerson", contactPerson);
            cmd.Parameters.AddWithValue("@ContactNumber", contactNumber);
            SqlParameter Id = cmd.Parameters.Add("@RETURN_VALUE", SqlDbType.Int);
            Id.Direction = ParameterDirection.ReturnValue;
            Conn.Open();
            cmd.ExecuteNonQuery();
            Conn.Close();
            GroupId = Int32.Parse((Id.Value.ToString()));
            if (GroupId != -1)
                return true;
            else
                return false;
        }
        catch (Exception EX)
        {
            throw (EX);
        }
        finally
        {
            Conn.Close();
        }
    }

    public bool DeletePosAppList(string serialNumber)
    {
        try
        {
            SqlCommand cmd = new SqlCommand("DeletePosAppList", Conn);
            cmd.CommandType = CommandType.StoredProcedure;
            DataTable table = new DataTable();
            cmd.Parameters.AddWithValue("@SerialNumber", serialNumber);
            SqlParameter rowCount = cmd.Parameters.Add("@RETURN_VALUE", SqlDbType.Int);
            rowCount.Direction = ParameterDirection.ReturnValue;
            Conn.Open();
            cmd.ExecuteNonQuery();
            Conn.Close();
            int count = Int32.Parse(rowCount.Value.ToString());
            if (count > 0 || count == -1)
                return true;
            else
                return false;
        }
        catch (Exception EX)
        {
            throw (EX);
        }
        finally
        {
            Conn.Close();
        }
    }

    public DataTable GetPosLastHealthTest(int posId)
    {
        try
        {
            SqlCommand cmd = new SqlCommand("GetPosLastHealthTest", Conn);
            cmd.Parameters.AddWithValue("@PosId", posId);
            cmd.CommandType = CommandType.StoredProcedure;
            SqlDataAdapter adapter = new SqlDataAdapter(cmd);
            DataTable table = new DataTable();
            Conn.Open();
            adapter.Fill(table);
            Conn.Close();
            return table;
        }
        catch (Exception EX)
        {
            throw (EX);
        }
        finally
        {
            Conn.Close();
        }
    }

    public bool UpdateGroupTerminalsGroup(int oldGroupId, int newGroupId)
    {
        try
        {
            SqlCommand cmd = new SqlCommand("UpdateGroupTerminalsGroup", Conn);
            cmd.CommandType = CommandType.StoredProcedure;
            cmd.Parameters.AddWithValue("@OldGroupId", oldGroupId);
            cmd.Parameters.AddWithValue("@NewGroupId", newGroupId);
            SqlParameter rowCount = cmd.Parameters.Add("@RETURN_VALUE", SqlDbType.Int);
            rowCount.Direction = ParameterDirection.ReturnValue;
            Conn.Open();
            cmd.ExecuteNonQuery();
            Conn.Close();
            if (Convert.ToInt32(rowCount.Value) == 1)
                return true;
            else
                return false;
        }
        catch (Exception EX)
        {
            throw (EX);
        }
        finally
        {
            Conn.Close();
        }
    }

    public bool UpdateTerminalGroup(int posId, int newGroupId)
    {
        try
        {
            SqlCommand cmd = new SqlCommand("UpdateTerminalGroup", Conn);
            cmd.CommandType = CommandType.StoredProcedure;
            cmd.Parameters.AddWithValue("@PosId", posId);
            cmd.Parameters.AddWithValue("@NewGroupId", newGroupId);
            SqlParameter rowCount = cmd.Parameters.Add("@RETURN_VALUE", SqlDbType.Int);
            rowCount.Direction = ParameterDirection.ReturnValue;
            Conn.Open();
            cmd.ExecuteNonQuery();
            Conn.Close();
            if (Convert.ToInt32(rowCount.Value) == 1)
                return true;
            else
                return false;
        }
        catch (Exception EX)
        {
            throw (EX);
        }
        finally
        {
            Conn.Close();
        }
    }
    public bool DeleteGroup(int groupId)
    {
        try
        {
            SqlCommand cmd = new SqlCommand("DeleteGroup", Conn);
            cmd.CommandType = CommandType.StoredProcedure;
            DataTable table = new DataTable();
            cmd.Parameters.AddWithValue("@GroupId", groupId);
            SqlParameter rowCount = cmd.Parameters.Add("@RETURN_VALUE", SqlDbType.Int);
            rowCount.Direction = ParameterDirection.ReturnValue;
            Conn.Open();
            cmd.ExecuteNonQuery();
            Conn.Close();
            if (Int32.Parse(rowCount.Value.ToString()) == 1)
                return true;
            else
                return false;
        }
        catch (Exception EX)
        {
            throw (EX);
        }
        finally
        {
            Conn.Close();
        }
    }

    public DataTable GetOuterTerminals(int groupId)
    {
        try
        {
            SqlCommand cmd = new SqlCommand("GetOuterTerminals", Conn);
            cmd.Parameters.AddWithValue("@GroupId", groupId);
            cmd.CommandType = CommandType.StoredProcedure;
            SqlDataAdapter adapter = new SqlDataAdapter(cmd);
            DataTable table = new DataTable();
            Conn.Open();
            adapter.Fill(table);
            Conn.Close();
            return table;
        }
        catch (Exception EX)
        {
            throw (EX);
        }
        finally
        {
            Conn.Close();
        }
    }

    public bool DeletePosFileList(string serialNumber)
    {
        try
        {
            SqlCommand cmd = new SqlCommand("DeletePosFileList", Conn);
            cmd.CommandType = CommandType.StoredProcedure;
            DataTable table = new DataTable();
            cmd.Parameters.AddWithValue("@SerialNumber", serialNumber);
            SqlParameter rowCount = cmd.Parameters.Add("@RETURN_VALUE", SqlDbType.Int);
            rowCount.Direction = ParameterDirection.ReturnValue;
            Conn.Open();
            cmd.ExecuteNonQuery();
            Conn.Close();
            int count = Int32.Parse(rowCount.Value.ToString());
            if (count > 0 || count == -1)
                return true;
            else
                return false;
        }
        catch (Exception EX)
        {
            throw (EX);
        }
        finally
        {
            Conn.Close();
        }
    }
    public DataRow GetGroupInfo(int groupId)
    {
        try
        {
            SqlCommand cmd = new SqlCommand("GetGroupInfo", Conn);
            cmd.CommandType = CommandType.StoredProcedure;
            SqlDataAdapter adapter = new SqlDataAdapter(cmd);
            DataTable table = new DataTable();
            cmd.Parameters.AddWithValue("@GroupId", groupId);
            Conn.Open();
            adapter.Fill(table);
            Conn.Close();
            DataRow row = null;
            if (table.Rows.Count > 0)
                row = table.Rows[0];
            return row;
        }
        catch (Exception EX)
        {
            throw (EX);
        }
        finally
        {
            Conn.Close();
        }
    }

    public int UpdateGroup(int groupId, string name, string branch, string contactPerson, string contactNumber)
    {
        try
        {
            SqlCommand cmd = new SqlCommand("UpdateGroup", Conn);
            cmd.CommandType = CommandType.StoredProcedure;
            DataTable table = new DataTable();
            cmd.Parameters.AddWithValue("@GroupId", groupId);
            cmd.Parameters.AddWithValue("@Name", name);
            cmd.Parameters.AddWithValue("@Branch", branch);
            cmd.Parameters.AddWithValue("@ContactPerson", contactPerson);
            cmd.Parameters.AddWithValue("@ContactNumber", contactNumber);
            SqlParameter Id = cmd.Parameters.Add("@RETURN_VALUE", SqlDbType.Int);
            Id.Direction = ParameterDirection.ReturnValue;
            Conn.Open();
            cmd.ExecuteNonQuery();
            Conn.Close();
            return Int32.Parse((Id.Value.ToString()));
        }
        catch (Exception EX)
        {
            throw (EX);
        }
        finally
        {
            Conn.Close();
        }
    }

    public bool FreezeGroup(int groupId)
    {
        try
        {
            SqlCommand cmd = new SqlCommand("FreezeGroup", Conn);
            cmd.CommandType = CommandType.StoredProcedure;
            DataTable table = new DataTable();
            cmd.Parameters.AddWithValue("@GroupId", groupId);
            SqlParameter rowCount = cmd.Parameters.Add("@RETURN_VALUE", SqlDbType.Int);
            rowCount.Direction = ParameterDirection.ReturnValue;
            Conn.Open();
            cmd.ExecuteNonQuery();
            Conn.Close();
            if (Int32.Parse(rowCount.Value.ToString()) > 0)
                return true;
            else
                return false;
        }
        catch (Exception EX)
        {
            throw (EX);
        }
        finally
        {
            Conn.Close();
        }
    }

    public bool FreezeTerminal(int posId)
    {
        try
        {
            SqlCommand cmd = new SqlCommand("FreezeTerminal", Conn);
            cmd.CommandType = CommandType.StoredProcedure;
            DataTable table = new DataTable();
            cmd.Parameters.AddWithValue("@PosId", posId);
            SqlParameter rowCount = cmd.Parameters.Add("@RETURN_VALUE", SqlDbType.Int);
            rowCount.Direction = ParameterDirection.ReturnValue;
            Conn.Open();
            cmd.ExecuteNonQuery();
            Conn.Close();
            if (Int32.Parse(rowCount.Value.ToString()) > 0)
                return true;
            else
                return false;
        }
        catch (Exception EX)
        {
            throw (EX);
        }
        finally
        {
            Conn.Close();
        }
    }
}
