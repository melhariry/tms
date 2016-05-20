<%@ Page Language="C#" AutoEventWireup="true" CodeFile="Index.aspx.cs" Inherits="Terminals_Index" %>

<!DOCTYPE html>

<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
    <title>POS Terminals</title>
</head>
<body>
    <h1>Welcome!</h1>
    <form id="ViewTerminals" runat="server">
        <asp:GridView ID="TerminalsGrid" runat="server" OnDataBound="TerminalsGrid_DataBound">
        </asp:GridView>
    </form>

</body>
</html>
