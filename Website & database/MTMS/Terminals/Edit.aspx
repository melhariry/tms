<%@ Page Language="C#" AutoEventWireup="true" CodeFile="Edit.aspx.cs" Inherits="Terminals_Show" %>

<!DOCTYPE html>

<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
    <title></title>
</head>
<body>
    <h1>Commands</h1>
    <form id="CommandsForm" runat="server">
    <div id="CommandsDiv" runat="server">
    </div>
    <h2>File List</h2>
    <asp:GridView ID="FileListGrid" runat="server" OnDataBound="FileListGrid_DataBound">
        <Columns>
            <asp:BoundField HeaderText="Select" />
        </Columns>
    </asp:GridView>
    <asp:Button ID="Submit" runat="server" Text="Submit" OnClick="Submit_Click" />
    <asp:FileUpload ID="FileUpload1" runat="server" />
    <asp:Button ID="Upload" runat="server" Text="Upload" OnClick="Upload_Click" />
    <br />
    <asp:Button ID="Download" runat="server" Text="Download" OnClick="Download_Click" />
    <asp:Label ID="FtpStatus"  runat="server"/>
    </form>
</body>
</html>
