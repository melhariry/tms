<%@ Page Language="C#" AutoEventWireup="true" CodeFile="Terminal.aspx.cs" Inherits="Terminal" %>

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
    <title>MTMS - Terminal <%= Context.Request.Params["id"].ToString() %></title>
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
                            </ul>
                            <!--/.widget-nav-->
                        </div>
                        <!--/.sidebar-->
                    </div>
                    <!--/.span3-->
                    <form runat="server" id="TerminalForm" class="span9">
                        <div class="btn-controls">
                            <div class="btn-box-row row-fluid">
                                <h1 class="btn-box big span12">
                                    <p class="text-muted">Terminal</p><b> <%= Context.Request.Params["id"].ToString() %> </b>
                                </h1>
                            </div>
                            
                        </div>
                        <% PrintStatus(); %>
                        <div class="content">
                            <div class="module">
                                    <div class="module-head">
                                        <h3>
                                            History</h3>
                                    </div>
                                    <div class="module-body table">
                                        <table id="history" border="0" class="datatable-1 table table-bordered table-striped  display dataTable"  aria-describedby="DataTables_Table_0_info" style="width: 100%;">
								            <thead>
									        <tr>
                                                <th rowspan="1" colspan="1">Command</th>
                                                <th rowspan="1" colspan="1">Status</th>
                                                <th rowspan="1" colspan="1">Timestamp</th>
                                                <th rowspan="1" colspan="1">Parameters</th>
									        </tr>
								        </thead>
								            <tbody role="alert" aria-live="polite" aria-relevant="all">
                                                <% PrintTerminalHistory(); %>
								            </tbody>
                                        </table>
							        </div>
                                </div>

                                <div class="module">
                                    <div class="module-head">
                                        <h3>
                                            Test Health History</h3>
                                    </div>
                                    <div class="module-body table">
                                        <table id="testHealth" border="0" class="datatable-1 table table-bordered table-striped  display dataTable"  aria-describedby="DataTables_Table_0_info" style="width: 100%;">
								            <thead>
									        <tr>
                                                <th rowspan="1" colspan="1">Crypto</th>
                                                <th rowspan="1" colspan="1">Printer</th>
                                                <th rowspan="1" colspan="1">Timer</th>
                                                <th rowspan="1" colspan="1">Buzzer</th>
                                                <th rowspan="1" colspan="1">Led</th>
                                                <th rowspan="1" colspan="1">Rtc</th>
                                                <th rowspan="1" colspan="1">RamUsed(KB)</th>
                                                <th rowspan="1" colspan="1">DiskUsed(KB)</th>
                                                <th rowspan="1" colspan="1">Timestamp</th>
									        </tr>
								        </thead>
								            <tbody role="alert" aria-live="polite" aria-relevant="all">
                                                <% PrintTerminalHealthTest(); %>
								            </tbody>
                                        </table>
							        </div>
                                </div>

                                <div class="module">
                                    <div class="module-head">
                                        <h3>
                                            App List</h3>
                                    </div>
                                    <div class="module-body table">
                                        <table id="appList" border="0" class="datatable-1 table table-bordered table-striped  display dataTable"  aria-describedby="DataTables_Table_0_info" style="width: 100%;">
								            <thead>
									        <tr>
                                                <th rowspan="1" colspan="1">Name</th>
                                                <th rowspan="1" colspan="1">Version</th>
                                                <th rowspan="1" colspan="1">Company</th>
									        </tr>
								        </thead>
								            <tbody role="alert" aria-live="polite" aria-relevant="all">
                                                <% PrintTerminalAppList(); %>
								            </tbody>
                                        </table>
							        </div>
                                </div>

                                <div class="module">
                                    <div class="module-head">
                                        <h3>
                                            File List</h3>
                                    </div>
                                    <div class="module-body">
                                        <h5>Select files you want to delete</h5>
                                        <div class="acidjs-css3-treeview">
	                                    <ul>
		                                    <li>
			                                    <input type="checkbox" id="node-0" checked="checked" /><label><input type="checkbox" /><span></span></label><label class="folder" for="node-0">Root</label>
			                                    <ul>
				                                    <li>
					                                    <input type="checkbox" id="node-0-0" checked="checked" /><label><input type="checkbox" /><span></span></label><label class="folder" for="node-0-0">Public</label>
					                                    <ul>
						                                    <li>
							                                    <input type="checkbox" id="node-0-0-0" checked="checked" /><label><input type="checkbox" /><span></span></label><label class="folder" for="node-0-0-0">MTMS</label>
							                                    <ul>
                                                                    <asp:Repeater runat="server" ID="MTMSListRep">
                                                                        <ItemTemplate>
								                                    <li>
									                                    <label><asp:CheckBox runat="server" /><span></span></label><label class="file" ><%# Eval("Name") %> (<%# Eval("SizeInBytes") %> Bytes)</label>
								                                    </li>
                                                                        </ItemTemplate>
                                                                    </asp:Repeater>
							                                    </ul>
						                                    </li>
                                                            <asp:Repeater runat="server" ID="pubListRep">
                                                                <ItemTemplate>
								                            <li>
									                            <label><asp:CheckBox runat="server" /><span></span></label><label class="file" ><%# Eval("Name") %> (<%# Eval("SizeInBytes") %> Bytes)</label>
								                            </li>
                                                                </ItemTemplate>
                                                            </asp:Repeater>
					                                    </ul>
				                                    </li>
				                                    <li>
					                                    <input type="checkbox" id="node-0-1" checked="checked"/><label><input type="checkbox" /><span></span></label><label class="folder" for="node-0-1">Private</label>
					                                    <ul>
						                                    <asp:Repeater runat="server" ID="priListRep">
                                                                <ItemTemplate>
								                            <li>
									                            <label><asp:CheckBox runat="server" /><span></span></label><label class="file" ><%# Eval("Name") %> (<%# Eval("SizeInBytes") %> Bytes)</label>
								                            </li>
                                                                </ItemTemplate>
                                                            </asp:Repeater>
					                                    </ul>
				                                    </li>
			                                    </ul>
		                                    </li>
	                                    </ul>
                                    </div>
                                    </div>
                                </div>
                                <div class="module">
                                    <div class="module-head">
                                        <h3>
                                            Command To Be Sent</h3>
                                    </div>
                                    <div class="module-body table">
                                        <table id="commandToSend" border="0" class="datatable-1 table table-bordered table-striped  display dataTable"  aria-describedby="DataTables_Table_0_info" style="width: 100%;">
								            <thead>
									        <tr>
                                                <th rowspan="1" colspan="1">Command</th>
                                                <th rowspan="1" colspan="1">Parameters</th>
									        </tr>
								        </thead>
								            <tbody role="alert" aria-live="polite" aria-relevant="all">
                                                <% PrintCommandToSend(); %>
								            </tbody>
                                        </table>
							        </div>
                                </div>
                            </div>
                        <div class="module">
                            <div class="module-head">
                                    <h3>
                                        Ongoing Command</h3>
                            </div>
                        </div>
                        <div class="btn-controls">
                            
							<div class="btn-box-row row-fluid">
								<a class="btn-box big span3">
                                    <asp:CheckBox id="TEST_HEALTH" runat="server" />
									<i class="icon-adjust"></i>
									<b>Test Health</b>
								</a>
								<a class="btn-box big span3">
                                    <asp:CheckBox id="LIST_FILES" runat="server" />
									<i class="icon-briefcase"></i>
									<b>List Files</b>
								</a>
								<a class="btn-box big span3">
                                    <asp:CheckBox id="LIST_APPS" runat="server" />
									<i class="icon-gift"></i>
									<b>List Apps</b>
								</a>
                                <a class="btn-box big span3">
                                    <asp:CheckBox id="DELETE_FILE" runat="server" />
									<i class="icon-gift"></i>
									<b>Delete File</b>
								</a>
							</div>

							<div class="btn-box-row row-fluid">
							    <a class="btn-box big span4">
                                    <asp:CheckBox id="PUSH_FILE" runat="server" />
									<i class="icon-adjust"></i>
									<b>Push File</b>
								</a>
								<a class="btn-box big span4">
                                    <asp:CheckBox id="PULL_FILE" runat="server" />
									<i class="icon-briefcase"></i>
									<b>Pull File</b>
								</a>
								<a class="btn-box big span4">
                                    <asp:CheckBox id="UPDATE_APP" runat="server" />
									<i class="icon-gift"></i>
									<b>Update App</b>
								</a>
							</div>
                        </div>

                        <div class="module">
                            <div class="module-head">
                                <h3>Inputs</h3>
                            </div>
                            <div class="module-body" id="inputs_div">
                                <div id='update_app_params' hidden="hidden">
                                    <h4>Update App parameters</h4>
                                    <label>App Source:</label>
                                    <asp:FileUpload id='update_app_src' runat="server" /><br/>
                                    <hr/>
                                </div>
                                <div id='pull_file_params' hidden="hidden">
                                    <h4>Pull File Parameters</h4>
                                    <label>File Source:</label>
                                    <asp:FileUpload id='pull_file_src' runat="server" />
                                    <hr/>
                                </div>
                                <div id='push_file_params' hidden="hidden">
                                    <h4>Push File Parameters</h4>
                                    <div class="module-head">
                                        <h3>
                                            <asp:CheckBox id="hotlist_file" runat="server" />
                                            Hotlist File</h3>
                                    </div>
                                    <div class="module-head">
                                        <h3>
                                            <asp:CheckBox id="tms_conf" runat="server"/>
                                            TMS App conf file</h3>
                                    </div>
                                    <div class="module-head">
                                        <h3>
                                            <asp:CheckBox id="trsc_conf" runat="server"/>
                                            Transaction App conf file</h3>
                                    </div>
                                    <hr/>
                                </div>
                                <div class="align-right">
                                    <asp:CheckBox id="Schedule" runat="server" Text="Schedule this command"/>
                                    <asp:Button ID="TerminalSubmitBtn" runat="server" CssClass="btn btn-primary btn-large" Text="Submit" OnClick="TerminalSubmitBtn_Click"></asp:Button>
                                    <asp:Button ID="TerminalClearBtn" runat="server" CssClass="btn btn-large" Text="Clear" OnClick="TerminalClearBtn_Click"></asp:Button>
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
    <script src="Resources/js/jquery.dataTables.js" type="text/javascript"></script>
    <script src="Resources/js/forms_builder.js" type="text/javascript"></script>
    <script>
        $(document).ready(function () {
            $('#history, #testHealth, #appList, #fileList, #commandToSend').DataTable();
            $('.dataTables_paginate').addClass("btn-group datatable-pagination");
            $('.dataTables_paginate > a').wrapInner('<span />');
            $('.dataTables_paginate > a:first-child').append('<i class="icon-chevron-left shaded"></i>');
            $('.dataTables_paginate > a:last-child').append('<i class="icon-chevron-right shaded"></i>');
        });
    </script>

</body>
</html>
