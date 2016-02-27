﻿using System;
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
    
    public bool PosExists(string posSerialNumber)
    {
        try
        {
            SqlCommand cmd = new SqlCommand("PosExists", Conn);
            cmd.CommandType = CommandType.StoredProcedure;
            SqlDataAdapter adapter = new SqlDataAdapter(cmd);
            DataTable table = new DataTable();
            cmd.Parameters.AddWithValue("@PosSerialNumber", posSerialNumber);
            Conn.Open();
            adapter.Fill(table);
            Conn.Close();
            if (table.Rows.Count == 1)
                return true;
            else
                return false;
        }
        catch (Exception EX)
        {
            throw (EX);
        }
    }

    public bool CreatePosRecord(string serialNumber, string vendor, string model, decimal lat, decimal lng, Int64 diskCapacity, Int64 ramSize)
    {
        try
        {
            SqlCommand cmd = new SqlCommand("CreatePosRecord", Conn);
            cmd.CommandType = CommandType.StoredProcedure;
            DataTable table = new DataTable();
            cmd.Parameters.AddWithValue("@SerialNumber", serialNumber);
            cmd.Parameters.AddWithValue("@Vendor", vendor);
            cmd.Parameters.AddWithValue("@Model", model);
            cmd.Parameters.AddWithValue("@LocationLat", lat);
            cmd.Parameters.AddWithValue("@LocationLng", lng);
            cmd.Parameters.AddWithValue("@TotalDiskCapacity", diskCapacity);
            cmd.Parameters.AddWithValue("@TotalRamSize", ramSize);
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

}
