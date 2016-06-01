<%@ Page Language="C#" AutoEventWireup="true" CodeFile="Config.aspx.cs" Inherits="Config" %>

<!DOCTYPE html>

<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
	<meta name="viewport" content="width=device-width, initial-scale=1.0"/>
    <link rel="shortcut icon" type="image/x-icon" href="Resources/img/favicon.ico" />
	<link type="text/css" href="Resources/css/bootstrap.min.css" rel="stylesheet"/>
	<link type="text/css" href="Resources/css/bootstrap-responsive.min.css" rel="stylesheet"/>
	<link type="text/css" href="Resources/css/theme.css" rel="stylesheet"/>
	<link type="text/css" href="Resources/css/font-awesome.css" rel="stylesheet"/>
	<link type="text/css" href='http://fonts.googleapis.com/css?family=Open+Sans:400italic,600italic,400,600' rel='stylesheet'/>
    <title>MTMS - Config File</title>
</head>
<body>

    <div class="navbar navbar-fixed-top">
            <div class="navbar-inner">
                <div class="container">
                    <a class="btn btn-navbar" data-toggle="collapse" data-target=".navbar-inverse-collapse">
                        <i class="icon-reorder shaded"></i></a><a class="brand" href="index.aspx">MTMS </a>
                    <div class="nav-collapse collapse navbar-inverse-collapse">
                        <ul class="nav pull-right">
                            <li><a href="logout.aspx">Logout </a></li>
                        </ul>
                    </div>
                    <!-- /.nav-collapse -->
                </div>
            </div>
            <!-- /navbar-inner -->
        </div>
        <!-- /navbar -->
        <div class="wrapper">
            <div class="container">
                <div class="row">
                    <div class="span3">
                        <div class="sidebar">
                            <ul class="widget widget-menu unstyled">
                                <li><a href="groups.aspx"><i class="menu-icon icon-group"></i>Groups <b class="label green pull-right"> <%= Methods.getGroupsCount() %> </b></a></li>
                                <li><a href="terminals.aspx"><i class="menu-icon icon-hdd"></i>Terminals<b class="label orange pull-right"> <%= Methods.getTerminalsCount() %> </b></a></li>
                                <li><a href="config.aspx"><i class="menu-icon icon-hdd"></i>Generate Config File<b class="label orange pull-right"></b></a></li>
                            </ul>
                            <!--/.widget-nav-->
                        </div>
                        <!--/.sidebar-->
                    </div>
                    <!--/.span3-->
                    <form runat="server" class="span9">
                        <% PrintStatus(); %>
                        <div class="content">

                            <div class="module" id="groups_module">
                                <div class="module-head">
                                    <h3>Config File Params</h3>
                                </div>
                                <div class="module-body" runat="server">
                                    <h5>This form generates the configuration file for the TMS app and forces all the terminals to pull the generated file.</h5>
                                    <div class="control-group">
									    <div class="controls">
										    <asp:TextBox runat="server" id="FirstIpTxt" placeholder="First IP" CssClass="span3" />
										    <asp:RequiredFieldValidator runat="server" ErrorMessage="*" ControlToValidate="FirstIpTxt" ForeColor="Red" /> 
                                            <asp:RegularExpressionValidator ID="IPValidator" runat="server" ErrorMessage="Invalid IP address"
                                            ValidationExpression="([0-9]{1,3}\.|\*\.){3}([0-9]{1,3}|\*){1}"
                                            ControlToValidate="FirstIpTxt"
                                            ForeColor="Red" />
                                        </div>
                                        <div class="controls">
										    <asp:TextBox runat="server" id="SecondIpTxt" placeholder="Second IP" CssClass="span3" />
                                            <asp:RequiredFieldValidator runat="server" ErrorMessage="*" ControlToValidate="SecondIpTxt" ForeColor="Red"  />
                                            <asp:RegularExpressionValidator ID="IP2Validator" runat="server" ErrorMessage="Invalid IP address"
                                            ValidationExpression="([0-9]{1,3}\.|\*\.){3}([0-9]{1,3}|\*){1}"
                                            ControlToValidate="SecondIpTxt"
                                            ForeColor="Red" />
                                        </div>
                                        <div class="controls">
										    <asp:TextBox runat="server" id="IntervalTxt" TextMode="Number" placeholder="Time Interval" CssClass="span3" />
										    <asp:DropDownList runat="server" ID="TimeList" CssClass="span2"  />
                                            <asp:RequiredFieldValidator runat="server" ErrorMessage="*" ControlToValidate="IntervalTxt" ForeColor="Red"  />
                                         </div>
                                        <div class="controls">
										    <asp:TextBox runat="server" id="FTPUserTxt" placeholder="FTP Username" CssClass="span3" />
										    <asp:RequiredFieldValidator runat="server" ErrorMessage="*" ControlToValidate="FTPUserTxt" ForeColor="Red"  />
									    </div>
                                        <div class="controls">
										    <asp:TextBox runat="server" id="FTPPassTxt" TextMode="Password" placeholder="FTP Password" CssClass="span3" />
										    <asp:RequiredFieldValidator runat="server" ErrorMessage="*" ControlToValidate="FTPPassTxt" ForeColor="Red"  />
									    </div>
                                        <div class="controls">
										    <asp:TextBox runat="server" id="FTPPassConfirm" TextMode="Password" placeholder="Confirm FTP Password" CssClass="span3" />
										    <asp:RequiredFieldValidator runat="server" ErrorMessage="*" ControlToValidate="FTPPassConfirm" ForeColor="Red"  />
									        <asp:CompareValidator runat="server" id="cmpPass" controltovalidate="FTPPassTxt" controltocompare="FTPPassConfirm" errormessage="Passwords do not match." ForeColor="Red" />
                                        </div>
                                        <div class="align-right">
                                            <asp:Button ID="GenerateConfigBtn" runat="server" CssClass="btn btn-primary btn-large" Text="Generate" OnClick="GenerateConfigBtn_Click"></asp:Button>
                                        </div>
								        
							        </div>
                                </div>
                            </div>
                        </div>
                        <!--/.content-->
                    </form>
                    <!--/.span9-->
                </div>
            </div>
            <!--/.container-->
        </div>
        <!--/.wrapper-->
        <div class="footer">
            <div class="container">
                <b class="copyright">&copy; 2016 MTMS</b>
            </div>
        </div>

	<script src="Resources/js/jquery-1.9.1.min.js" type="text/javascript"></script>
	<script src="Resources/js/jquery-ui-1.10.1.custom.min.js" type="text/javascript"></script>
	<script src="Resources/js/bootstrap.min.js" type="text/javascript"></script>

</body>
</html>
