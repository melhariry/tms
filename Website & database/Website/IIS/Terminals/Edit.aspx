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
    <asp:Button ID="Submit" runat="server" Text="Submit" OnClick="Submit_Click" />
    </form>
</body>
</html>
