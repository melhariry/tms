using System;
using System.Collections.Generic;
using System.Text;
using System.Data;
using System.Data.SqlClient;
using System.Configuration;

/// <summary>
/// Database class
/// </summary>

public class DB
{
    SqlConnection Conn = new SqlConnection(ConfigurationManager.ConnectionStrings["DbConnectionString"].ToString());
    
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
            if ( PosId != -1)
                return true;
            else
                return false;
        }
        catch (Exception EX)
        {
            throw (EX);
        }
    }

    public bool SubmitHealthTestResults(string serialNumber, int crypto, int printer, int timer, int buzzer, int led, int rtc, int memory, Int64 usedDiskSize, Int64 usedRamSize)
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
            cmd.Parameters.AddWithValue("@Memory", memory);
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
    }

    public bool UpdateCommandToSend(int id, int command)
    {
        try
        {
            SqlCommand cmd = new SqlCommand("UpdateCommandToSend", Conn);
            cmd.CommandType = CommandType.StoredProcedure;

            cmd.Parameters.AddWithValue("@Id", id);
            cmd.Parameters.AddWithValue("@Command", command);
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
    }
}
